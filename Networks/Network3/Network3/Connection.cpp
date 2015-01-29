#include "Connection.h"

#define EXIT_ASSERT PFS_ASSERT(0);

/////////////////////////////////////////////////////////////////
// NetSocket functions
bool NetSocket::Connect( UINT ip, UINT port, bool forceCoalesce )
{
	struct sockaddr_in sa;
	int x = 1;

	if( ( mSocket = socket( PF_INET, SOCK_STREAM, 0 ) ) == INVALID_SOCKET )
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
	return false;
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
			//gSocketManager->AddToOutbound( rc );
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

void NetSocket::HandleInput()
{
	bool pktReceived = false;
	u_long packetSize = 0;
	int rc = recv( mSocket, mRecvBuf + mRecvBegin + mRecvOfs, RECV_BUFFER_SIZE - ( mRecvBegin + mRecvOfs ), 0 );
	
	printf( "Incoming %6d bytes. Begin %6d offset %4d\n", rc, mRecvBegin, mRecvOfs );

	if( rc == 0 )
	{
		return;
	}
	if( rc == SOCKET_ERROR )
	{
		mDeleteFlag = 1;
		return;
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
				return;
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

	//gSocketManager->AddToInbound( rc );
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

NetSocket::NetSocket()
{
	mSocket			= INVALID_SOCKET;
	mDeleteFlag		= 0;
	mSendOfs		= 0;
	mTimeOut		= 0;
	mRecvOfs		= 0;
	mRecvBegin		= 0;
	mInternal		= 0;
	mBinaryProtocol = 1;
}

NetSocket::NetSocket( SOCKET socket, UINT ip )
{
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
	mInternal		= //gSocketManager->IsInternal(mIPAddr);
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

	if( ( mSocket = socket( PF_INET, SOCK_STREAM, IPPROTO_TCP ) ) == INVALID_SOCKET )
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
	sockaddr_in sa;
	int value = 1;

	if( ( mSocket = socket( PF_INET, SOCK_STREAM, 0 ) )  == INVALID_SOCKET )
	{
		PFS_ASSERT( "NetListenSocket Error: Init failed to create socket handle" );
	}
	if( setsockopt( mSocket, SOL_SOCKET, SO_REUSEADDR, (char*)&value, sizeof( value ) ) == SOCKET_ERROR )
	{
		perror( "NetListenSocket::Initialize::setsockopt" );
		closesocket( mSocket );
		mSocket = INVALID_SOCKET;
		PFS_ASSERT( "NetListenSocket Error : Initialize failed to set socket options ");
	}
}

NetListenSocket::NetListenSocket()
{
}

NetListenSocket::NetListenSocket( int portNum )
{
	gPort = 0;
	Initialize( portNum );
}

// End of NetListenSocket functions
/////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////
// ServerListenSocket functions

void ServerListenSocket::HandleInput()
{
	UINT ipAddr;
	SOCKET newSocket = AcceptConnection( &ipAddr );

	int value = 1;
	setsockopt( newSocket, SOL_SOCKET, SO_DONTLINGER, (char*)value, sizeof( value ) );
	if( newSocket != INVALID_SOCKET )
	{
		RemoteEventSocket* socket = PFS_NEW RemoteEventSocket( newSocket, ipAddr );
		int sockID = gSocketManager->AddSocket( socket );
		int ipAddress = gSocketManager->GetIPAddress( sockID );
        //std::shared_ptr<EvtData_Remote_Client> pEvent(GCC_NEW EvtData_Remote_Client(sockId, ipAddress));
        //IEventManager::Get()->VQueueEvent(pEvent);
	}
}

ServerListenSocket::ServerListenSocket( int portNum )
{
	Initialize( portNum );
}

// End of ServerListenSocket functions
/////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////
// RemoteEventSocket functions
void RemoteEventSocket::CreateEvent( std::istringstream &in )
{
}

void RemoteEventSocket::HandleInput()
{
	NetSocket::HandleInput();

	while( !mInList.empty() )
	{
		std::shared_ptr<IPacket> packet = *mInList.begin();
		mInList.pop_front();

		if( !strcmp( packet->GetType(), BinaryPacket::gType ) ) // Checks the type of the packet here
		{
			// HERE I AM YO
		}
	}
}

RemoteEventSocket::RemoteEventSocket( SOCKET newSocket, UINT ip )
	: NetSocket( newSocket, ip )
{
}

RemoteEventSocket::RemoteEventSocket()
{
}

// End of RemoteEventSocket functions
/////////////////////////////////////////////////////////////////