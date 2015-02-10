#include "Connection.h"

#define EXIT_ASSERT PFS_ASSERT(0);

/////////////////////////////////////////////////////////////////
// NetSocket functions

bool NetSocket::Connect( UINT ip, UINT port, bool forceCoalesce )
{
	struct sockaddr_in sa;
	int x = 1;

	if( ( mSocket = socket( AF_INET, SOCK_STREAM, IPPROTO_TCP ) ) == INVALID_SOCKET )
	{
		return false;
	}

	if( !forceCoalesce )
	{
		setsockopt( mSocket, IPPROTO_TCP, TCP_NODELAY, (char*)&x, sizeof(x) );
	}

	sa.sin_family		= AF_INET;
	sa.sin_addr.s_addr	= htonl( ip );
	sa.sin_port			= htons( port );

	if( connect( mSocket, (sockaddr*)&sa, sizeof(sa)))
	{
		closesocket( mSocket );
		mSocket = INVALID_SOCKET;
		return false;
	}
	return true;
}

void NetSocket::SetBlocking( bool blocking )
{
#ifdef WIN32
	unsigned long val = blocking ? 0 : 1;
	ioctlsocket( mSocket, FIONBIO, &val );
#else
	int val = fcntl( mSocket, F_GETFL, 0 );
	if( blocking )
		val &= ~(O_NONBLOCK);
	else
		val |= O_NONBLOCK;
	fctnl( mSocket, F_SETFL, val );
#endif
}

void NetSocket::Send( std::shared_ptr<IPacket> pkt, bool clearTimeOut )
{
	if( clearTimeOut )
		mTimeOut = 0;
	mOutList.push_back( pkt );
}

int NetSocket::HasOutput()
{
	return !mOutList.empty();
}

void NetSocket::HandleOutput()
{
	int fSent = 0;
	do
	{
		PFS_ASSERT( !mOutList.empty() );
		PacketList::iterator i			= mOutList.begin();
		std::shared_ptr<IPacket> pkt	= *i;
		const char* buf					= pkt->GetData();
		int len							= static_cast<int>( pkt->GetSize() );

		int rc = send( mSocket, buf + mSendOfs, len - mSendOfs, 0 );
		if( rc > 0 )
		{
			mSocketManager->AddToOutbound( rc );
			mSendOfs	+= rc;
			fSent		= 1;
		}
		else if( WSAGetLastError() != WSAEWOULDBLOCK )
		{
			HandleException();
			fSent = 0;
		}
		else
		{
			fSent = 0;
		}

		if( mSendOfs == pkt->GetSize() )
		{
			mOutList.pop_front();
			mSendOfs = 0;
		}
		
	} while( fSent && !mOutList.empty() );
}

bool NetSocket::HandleInput()
{
	bool pktReceived = false;
	u_long packetSize = 0;
	SetBlocking( false );
	int rc = recv( mSocket, mRecvBuf + mRecvBegin + mRecvOfs, RECV_BUFFER_SIZE - ( mRecvBegin + mRecvOfs ), 0 );
	//printf( "Incoming %6d bytes. Begin %6d offset %4d\n", rc, mRecvBegin, mRecvOfs );

	if( rc == -1 )
	{
		return false;
	}
	if( rc == SOCKET_ERROR )
	{
		printf( "recv() failed with error: %d\n", WSAGetLastError() );
		mDeleteFlag = 1;
		return false;
	}

	const int hdrSize = sizeof( u_long );
	UINT newData = mRecvOfs + rc;
	int processedData = 0;

	while( newData > hdrSize )
	{
		packetSize = *(reinterpret_cast<u_long*>( mRecvBuf + mRecvBegin ) );
		packetSize = ntohl( packetSize );

		if( mBinaryProtocol )
		{
			if( newData < packetSize )
				break;

			if( packetSize > MAX_PACKET_SIZE )
			{
				HandleException();
				return false;
			}

			if( newData >= packetSize )
			{
				std::shared_ptr<BinaryPacket> pkt( PFS_NEW BinaryPacket( &mRecvBuf[mRecvBegin + hdrSize], packetSize - hdrSize ) );
				mInList.push_back( pkt );
				pktReceived = true;
				processedData += packetSize;
				newData -= packetSize;
				mRecvBegin += packetSize;
			}
		}
		else
		{
			char* cr = static_cast<char*>( memchr( &mRecvBuf[mRecvBegin], 0x0a, rc ) );
			if( cr )
			{
				*(cr + 1) = 0;
				std::shared_ptr<TextPacket> pkt( PFS_NEW TextPacket( &mRecvBuf[mRecvBegin] ) );
				mInList.push_back( pkt );
				packetSize = cr - &mRecvBuf[mRecvBegin];
				pktReceived = true;

				processedData += packetSize;
				newData -= packetSize;
				mRecvBegin += packetSize;
			}
		}
	}

	mSocketManager->AddToInbound( rc );
	mRecvOfs = newData;

	if( pktReceived )
	{
		if( mRecvOfs == 0 )
		{
			mRecvBegin = 0;
		}
		else if( mRecvBegin + mRecvOfs + MAX_PACKET_SIZE > RECV_BUFFER_SIZE )
		{
			int leftOver = mRecvOfs;
			memcpy( mRecvBuf, &mRecvBuf[mRecvBegin], mRecvOfs );
			mRecvBegin = 0;
		}
	}
	return pktReceived;
}

void NetSocket::HandleException()
{
	mDeleteFlag |= 1;
}

void NetSocket::TimeOut()
{
	mTimeOut = 0;
}

void NetSocket::SetTimeOut( UINT ms )
{
	mTimeOut = timeGetTime() + ms;
}

UINT NetSocket::GetIPAddress()
{
	return mIPAddr;
}

NetSocket::NetSocket( SocketManager* socketManager )
{
	mSocketManager  = socketManager;
	mSocket			= INVALID_SOCKET;
	mDeleteFlag		= 0;
	mSendOfs		= 0;
	mTimeOut		= 0;
	mRecvOfs		= 0;
	mRecvBegin		= 0;
	mInternal		= 0;
	mBinaryProtocol = 1;
}

NetSocket::NetSocket( SocketManager* socketManager, SOCKET socket, UINT ip )
{
	mSocketManager	= socketManager;
	mDeleteFlag		= 0;
	mSendOfs		= 0;
	mTimeOut		= 0;
	mBinaryProtocol = 1;
	mRecvOfs		= 0;
	mRecvBegin		= 0;
	mInternal		= 0;
	mTimeCreated	= timeGetTime();
	mSocket			= socket;
	mIPAddr			= ip;
	mInternal		= mSocketManager->IsInternal(mIPAddr);
	setsockopt( mSocket, SOL_SOCKET, SO_DONTLINGER, nullptr, 0 );
}

NetSocket::~NetSocket()
{
	if( mSocket != INVALID_SOCKET )
	{
		closesocket( mSocket );
		mSocket = INVALID_SOCKET;
	}
}

// End of NetSocket functions
/////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////
// NetListenSocket functions

void NetListenSocket::InitScan( int portNum_min, int portNum_max )
{
	sockaddr_in sa;
	int x = 1;

	if( ( mSocket = socket( AF_INET, SOCK_STREAM, IPPROTO_TCP ) ) == INVALID_SOCKET )
	{
		EXIT_ASSERT
			exit( 1 );
	}
	if( setsockopt( mSocket, SOL_SOCKET, SO_REUSEADDR, (char*)&x, sizeof( x ) ) == SOCKET_ERROR )
	{
		closesocket( mSocket );
		mSocket = INVALID_SOCKET;
		EXIT_ASSERT
			exit( 1 );
	}

	memset( &sa, 0, sizeof( sa ) );
	sa.sin_family = AF_INET;
	int portNum;
	for( portNum = portNum_min; portNum < portNum_max; portNum++ )
	{
		sa.sin_port = htons( portNum );
		if( bind(mSocket, (sockaddr*)&sa, sizeof( sa ))  != SOCKET_ERROR )
		{
			break;
		}
	}

	if( portNum == portNum_max )
	{
		closesocket( mSocket );
		mSocket = INVALID_SOCKET;
		EXIT_ASSERT
			exit( 1 );
	}

	SetBlocking( false );

	if( listen( mSocket, 8 ) == SOCKET_ERROR )
	{
		closesocket( mSocket );
		mSocket = INVALID_SOCKET;
		EXIT_ASSERT
			exit( 1 );
	}

	gPort = portNum;
}

SOCKET NetListenSocket::AcceptConnection( UINT* addr )
{
	SOCKET newSocket;
	sockaddr_in sock;
	int size = sizeof( sock );

	if( ( newSocket = accept( mSocket, (sockaddr*)&sock, &size ) ) == INVALID_SOCKET )
	{
		return INVALID_SOCKET;
	}

	if( getpeername( newSocket, (sockaddr*)&sock, &size ) == SOCKET_ERROR )
	{
		closesocket( newSocket );
		return INVALID_SOCKET;
	}

	*addr = ntohl( sock.sin_addr.s_addr );
	return newSocket;
}

void NetListenSocket::Initialize( int portNum )
{
	if( ( mSocket = socket( AF_INET, SOCK_STREAM, IPPROTO_TCP ) )  == INVALID_SOCKET )
	{
		PFS_ASSERT( "NetListenSocket Error: Init failed to create socket handle" );
	}

	int value = 1;
	if( setsockopt( mSocket, IPPROTO_TCP, TCP_NODELAY, (char*)&value, sizeof( value ) ) == SOCKET_ERROR )
	{
		perror( "NetListenSocket::Initialize::setsockopt" );
		closesocket( mSocket );
		mSocket = INVALID_SOCKET;
		PFS_ASSERT( "NetListenSocket Error : Initialize failed to set socket options ");
	}

	sockaddr_in sa;
	memset( &sa, 0, sizeof( sa ) );
	sa.sin_family		= AF_INET;
	sa.sin_addr.s_addr	= ADDR_ANY;
	sa.sin_port = htons( portNum );

	if( bind( mSocket, (sockaddr*)&sa, sizeof( sa ) ) == SOCKET_ERROR )
	{
		perror( "NetListenSocket::Initialize::bind" );
		closesocket( mSocket );
		mSocket = INVALID_SOCKET;
		PFS_ASSERT( "NetListenSocket error: Initialize failed to bind." );
	}

	SetBlocking( false );
	if( listen( mSocket, 256 ) == SOCKET_ERROR )
	{
		closesocket( mSocket );
		mSocket = INVALID_SOCKET;
		PFS_ASSERT( "NetListenSocket error: Initialize failed to listen." );
	}

	gPort = portNum;
}

NetListenSocket::NetListenSocket( SocketManager* socketManager )
	: NetSocket( socketManager )
{
}

NetListenSocket::NetListenSocket( SocketManager* socketManager, int portNum )
	: NetSocket( socketManager )
{
	gPort = 0;
	mSocketManager = socketManager;
	Initialize( portNum );
}

// End of NetListenSocket functions
/////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////
// ServerListenSocket functions

bool ServerListenSocket::HandleInput()
{
	UINT ipAddr;
	SOCKET newSocket = AcceptConnection( &ipAddr );

	int value = 1;
	setsockopt( newSocket, SOL_SOCKET, SO_DONTLINGER, (char*)value, sizeof( value ) );
	if( newSocket != INVALID_SOCKET )
	{
		RemoteEventSocket* socket = PFS_NEW RemoteEventSocket( mSocketManager, newSocket, ipAddr );
		int sockID = mSocketManager->AddSocket( socket );
		int ipAddress = socket->GetIPAddress();

		//printf( "Client with sockID: %d connected from %d.\n", sockID, ipAddr );

		IEventPtr E1( PFS_NEW Event_Client_Joined( sockID ) );
		EventManager::GetInstance()->QueueEvent( E1 );
	}
	return false;
}

ServerListenSocket::ServerListenSocket( SocketManager* socketManager, int portNum )
	: NetListenSocket( socketManager )
{
	Initialize( portNum );
}

// End of ServerListenSocket functions
/////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////
// RemoteEventSocket functions

void RemoteEventSocket::CreateEvent( std::istringstream &in )
{
	EventType eventType;
	in >> eventType;

	IEventPtr E1( EF::CREATE_EVENT( eventType ) );
	if( E1 )
	{
		E1->Deserialize( in );
		if( !EventManager::GetInstance()->QueueEvent( E1 ) )
			printf( "Failed to queue event with ID: %d\n", eventType );
	}
	else
	{
		printf("ERROR Unknown event type from remote: 0x%d\n", eventType);
	}
}

bool RemoteEventSocket::HandleInput()
{
	while( NetSocket::HandleInput() );

	while( !mInList.empty() )
	{
		std::shared_ptr<IPacket> packet = *mInList.begin();
		mInList.pop_front();

		if( packet->GetType() == BinaryPacket::GUID ) // Checks the type of the packet here
		{
			const char* buf = packet->GetData();
			int size = static_cast<int>( packet->GetSize() );

			std::istringstream in( buf + sizeof( u_long ), (size - sizeof( u_long ) ) );

			int type;
			in >> type;
			switch( type ) // This is where we will put the input logic to the client
			{
			case NetMsg_Event:
				{
					CreateEvent( in );
				}
				break;
			default:
				{
					std::cout << "Unknown message type." << std::endl;
				}
			}
		}
		else if( packet->GetType() == BinaryPacket::GUID )
		{
			printf( "Network: %s\n", packet->GetData()+sizeof(u_long) );
		}
	}
	return false;
}

RemoteEventSocket::RemoteEventSocket( SocketManager* socketManager, SOCKET newSocket, UINT ip )
	: NetSocket( socketManager, newSocket, ip )
{
}

RemoteEventSocket::RemoteEventSocket( SocketManager* socketManager )
	: NetSocket( socketManager )
{
}

// End of RemoteEventSocket functions
/////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////
// SocketManager functions

NetSocket* SocketManager::FindSocket( UINT sockID )
{
	SocketIDMap::iterator i = mSocketMap.find( sockID );
	if( i == mSocketMap.end() )
	{
		return nullptr;
	}
	return i->second;
}

void SocketManager::PrintError()
{
	int realError = WSAGetLastError();
	char* reason;

	switch( realError )
	{
	case WSANOTINITIALISED:
		reason = "A successful WSAStartup must occur before using this API.";
		break;
	case WSAEFAULT:
		reason = "The Windows Sockets implmentation was unable to allocate the needed resources for its internal operations, or the readfds, writefds, exceptfds, or timeval parameters are not part of the user address space.";
		break;
	case WSAENETDOWN:
		reason = "The network subsystem has failed.";
		break;
	case WSAEINVAL:
		reason = "The timeout value is not valid, or all three descriptor parameters were nullptr.";
		break;
	case WSAEINTR:
		reason = "The (blocking) call was canceled via WSACancelBlockingCall.";
		break;
	case WSAEINPROGRESS:
		reason = "A blocking Windows Sockets 1.1 call is in progress, or the service provider is still processing a callback function.";
		break;
	case WSAENOTSOCK:
		reason = "One of the descriptor sets contains an entry which is not a socket.";
		break;
	default:
		reason = "Unknown";
	}

	printf( "SOCKET error: %s\n", reason );
}

int SocketManager::AddSocket( NetSocket* socket )
{
	socket->mID = mNextSocketID;
	mSocketMap[mNextSocketID] = socket;
	++mNextSocketID;

	mSocketList.push_front( socket );
	if( mSocketList.size() > mMaxOpenSockets )
	{
		++mMaxOpenSockets;
	}

	return socket->mID;
}

void SocketManager::RemoveSocket( NetSocket* socket )
{
	UINT id = (UINT)socket->mID;
	mSocketList.remove( socket );
	mSocketMap.erase( socket->mID );
	SAFE_DELETE( socket );

	printf("Removing socket %d.\n", id);

	IEventPtr E1( PFS_NEW Event_Client_Left( id ) );
	EventManager::GetInstance()->QueueEvent( E1 );

	if( id == 0 )
	{
		IEventPtr E2( PFS_NEW Event_Shutdown_Client() );
		EventManager::GetInstance()->QueueEvent( E2 );
	}
}

UINT SocketManager::GetHostByName( const std::string &hostName )
{
	// Retreives the IP details and puts it into the lHostEnt structure
	hostent* hostEnt = gethostbyname( hostName.c_str() );
	sockaddr_in sa; // placeholder for the IP address

	if( hostEnt == nullptr )
	{
		printf("Error happened in GetHostByName()");
		return 0;
	}
	memcpy( &sa.sin_addr, hostEnt->h_addr, hostEnt->h_length );
	return ntohl( sa.sin_addr.s_addr );
}

const char* SocketManager::GetHostByAddr( UINT ip )
{
	static char host[256];

	int netIP = htonl( ip );
	hostent* hostEnt = gethostbyaddr( (const char*)&netIP, 4, PF_INET );

	if( hostEnt )
	{
		strcpy_s( host, 256, hostEnt->h_name );
		return host;
	}

	return nullptr;
}

UINT SocketManager::GetIPAddress( UINT sockID )
{
	NetSocket* socket = FindSocket( sockID );
	if( socket )
		return socket->GetIPAddress();
	else
		return 0;
}

void SocketManager::SetSubnet( UINT subnet, UINT subnetMask )
{
	mSubnet		= subnet;
	mSubnetMask = subnetMask;
}

bool SocketManager::IsInternal( UINT ipAddr )
{
	if( !mSubnetMask )
		return false;

	if( ( ipAddr & mSubnetMask ) == mSubnet )
		return false;

	return true;
}

bool SocketManager::Send( UINT sockID, std::shared_ptr<IPacket> packet )
{
	NetSocket* socket = FindSocket( sockID );

	if( !socket )
		return false;
	socket->Send( packet );
	return true;
}

void SocketManager::AddToOutbound( int rc )
{
	mOutbound += rc;
}

void SocketManager::AddToInbound( int rc )
{
	mInbound += rc;
}

void SocketManager::DoSelect( int pauseMicroSecs, bool handleInput )
{
	timeval tv;
	tv.tv_sec = 0;
	tv.tv_usec = pauseMicroSecs; // 100 microseconds is 0.1 milliseconds, or 0.0001 seconds! THE MORE YOU KNOW!

	fd_set input_set; 
	fd_set output_set;
	fd_set exception_set;

	FD_ZERO( &input_set );
	FD_ZERO( &output_set );
	FD_ZERO( &exception_set );
	int maxDesc = 0;

	// select setup
	for( auto& it : mSocketList )
	{
		NetSocket* socket = it;
		if( socket->mDeleteFlag & 1 || socket->mSocket == INVALID_SOCKET )
			continue;

		if( handleInput )
			FD_SET( socket->mSocket, &input_set );

		FD_SET( socket->mSocket, &exception_set );

		if( socket->HasOutput() )
			FD_SET( socket->mSocket, &output_set );
		if( (int)socket->mSocket > maxDesc )
			maxDesc = (int)socket->mSocket;
	}

	// do the actual select, duration passed as tv, use nullptr to block until event
	int selectReturn = 0;
	selectReturn = select( maxDesc + 1, &input_set, &output_set, &exception_set, &tv );
	if( selectReturn == SOCKET_ERROR )
	{
		PrintError();
		return;
	}

	// handle input, output, and exceptions
	if( selectReturn )
	{
		for( auto& it : mSocketList )
		{
			NetSocket* socket = it;

			if( ( socket->mDeleteFlag & 1 ) || socket->mSocket == INVALID_SOCKET )
				continue;

			if( FD_ISSET( socket->mSocket, &exception_set ) )
				socket->HandleException();

			if( !( socket->mDeleteFlag & 1 ) && FD_ISSET( socket->mSocket, &output_set ) )
				socket->HandleOutput();

			if( handleInput && !(socket->mDeleteFlag & 1 ) && FD_ISSET( socket->mSocket, &input_set ) )
				socket->HandleInput();
		}
	}
	UINT timeNow = timeGetTime();

	// handle deletion of any socket
	SocketList::iterator i = mSocketList.begin();
	while( i != mSocketList.end() )
	{
		NetSocket* socket = *i;
		if( socket->mTimeOut )
		{
			if( socket->mTimeOut < timeNow )
				socket->TimeOut();
		}

		if( socket->mDeleteFlag & 1 )
		{
			switch( socket->mDeleteFlag )
			{
			case 1:
				RemoveSocket( socket );
				i = mSocketList.begin();
				continue;
			case 3:
				socket->mDeleteFlag = 2;
				if( socket->mSocket != INVALID_SOCKET )
				{
					closesocket( socket->mSocket );
					socket->mSocket = INVALID_SOCKET;
				}
				break;
			}
		}
		++i;
	}
}

bool SocketManager::Initialize()
{
	if( !(WSAStartup( 0x0202, &mWsaData) == 0) )
		return false;

	return true;
}

void SocketManager::Release()
{
	SocketList eraseList;
	for( auto& it : mSocketList )
	{
		eraseList.push_front( it );
	}
	for( auto& it : eraseList )
	{
		RemoveSocket( it );
	}
	mSocketList.clear();
	mSocketMap.clear();

	WSACleanup();
}

SocketManager::SocketManager()
{
	mInbound = 0;
	mOutbound = 0;
	mMaxOpenSockets = 0;
	mSubnetMask = 0;
	mSubnet = 0xffffffff;
	mNextSocketID = 0;
	ZeroMemory( &mWsaData, sizeof( WSADATA ) );
}

SocketManager::~SocketManager()
{
}

// End of SocketManager functions
/////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////
// Start of ClientSocketManager functions

bool ClientSocketManager::Connect( const std::string &hostName, UINT port )
{
	mHostName	= hostName;
	mPort		= port;

	if( !SocketManager::Initialize() )
		return false;

	RemoteEventSocket* socket = PFS_NEW RemoteEventSocket( this );

	if( !socket->Connect( GetHostByName( mHostName ), mPort ) )
	{
		SAFE_DELETE( socket );
		return false;
	}
	AddSocket( socket );
	return true;
}

ClientSocketManager::ClientSocketManager() 
	: SocketManager()
{
	mHostName	= "";
	mPort		= 0;
}

// End of ClientSocketManager functions
/////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////
// Start of NetworkEventForwarder functions

void NetworkEventForwarder::ForwardEvent( IEventPtr eventPtr )
{
	std::ostringstream out;

	out << static_cast<int>( RemoteEventSocket::NetMsg_Event ) << " ";
	out << eventPtr->GetEventType() << " ";
	eventPtr->Serialize( out );
	out << "\r\n";

	std::shared_ptr<BinaryPacket> msg(PFS_NEW BinaryPacket( out.str().c_str(), (u_long)out.str().length()));

	mSocketManager->Send( mSocketID, msg );
}

void NetworkEventForwarder::Initialize( UINT socketID, SocketManager* sm )
{
	mSocketID		= socketID;
	mSocketManager	= sm;
}

NetworkEventForwarder::NetworkEventForwarder()
{
	mSocketID		= 0;
	mSocketManager	= nullptr;
}

NetworkEventForwarder::~NetworkEventForwarder( )
{
}

// End of NetworkEventForwarder functions
/////////////////////////////////////////////////////////////////