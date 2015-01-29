#include <stdio.h>
#include <errno.h>
#include "Network.h"
#include "Events.h"
#include "EventManager.h"
#include <string>

#pragma comment(lib, "Ws2_32")

#define EXIT_ASSERT assert(0);

const char* BinaryPacket::gType = "BinaryPacket";
const char* TextPacket::gType = "TextPacket";

BaseSocketManager *gpSocketManager = nullptr;


NetSocket::NetSocket()
{
	mSock = INVALID_SOCKET;
	mDeleteFlag = 0;
	mSendOfs = 0;
	mTimeOut = 0;

	mRecvOfs = mRecvBegin = 0;
	mInternal = 0;
	mBinaryProtocol = 1;
}

NetSocket::NetSocket( SOCKET newSock, unsigned int hostIP )
{
	mDeleteFlag = 0;
	mSendOfs = 0;
	mTimeOut = 0;

	mBinaryProtocol = 1;
	mRecvOfs = mRecvBegin = 0;
	mInternal = 0;

	mTimeCreated = timeGetTime();

	mSock = newSock;
	mIPaddr = hostIP;

	mInternal = gpSocketManager->IsInternal( mIPaddr );

	setsockopt( mSock, SOL_SOCKET, SO_DONTLINGER, NULL, 0 );

}

NetSocket::~NetSocket()
{
	if( mSock != INVALID_SOCKET )
	{
		closesocket( mSock );
		mSock = INVALID_SOCKET;
	}
}

bool NetSocket::Connect( unsigned int ip, unsigned int port, bool forceCoalesce )
{
	struct sockaddr_in sa;
	int x = 1;

	if( (mSock = socket( PF_INET, SOCK_STREAM, 0 )) == INVALID_SOCKET )
	{
		return false;
	}

	if( !forceCoalesce )
	{
		setsockopt( mSock, IPPROTO_TCP, TCP_NODELAY, (char *)&x, sizeof(x) );
	}

	sa.sin_family = AF_INET;
	sa.sin_addr.s_addr = htonl( ip );
	sa.sin_port = htons( port );

	if( connect( mSock, (struct sockaddr *)&sa, sizeof(sa) ) )
	{
		closesocket( mSock );
		mSock = INVALID_SOCKET;
		return false;
	}
	return true;
}

void NetSocket::Send( std::shared_ptr<IPacket> pkt, bool clearTimeOut )
{
	if( clearTimeOut )
	{
		mTimeOut = 0;
	}
	mOutList.push_back( pkt );
}

void NetSocket::SetBlocking( bool blocking )
{
	#ifdef WIN32
			unsigned long val = blocking ? 0 : 1;
			ioctlsocket( mSock, FIONBIO, &val );
	#else
			int val = fcntl( mSock, F_GETFL, 0 );
			if( blocking )
				val &= ~(O_NONBLOCK);
			else
				val |= O_NONBLOCK;
			fcntl( mSock, F_SETFL, val );
	#endif
}

void NetSocket::VHandleOutput()
{
	int fSent = 0;
	do
	{
		assert( !mOutList.empty() );
		PacketList::iterator i = mOutList.begin();

		std::shared_ptr<IPacket> pkt = *i;
		const char *buf = pkt->VGetData();
		int len = static_cast<int>(pkt->VGetSize());

		int rc = send( mSock, buf + mSendOfs, len - mSendOfs, 0 );
		if( rc > 0 )
		{
			gpSocketManager->AddToOutbound( rc );
			mSendOfs += rc;
			fSent = 1;
		}
		else if ( WSAGetLastError() != WSAEWOULDBLOCK )
		{
			HandleException();
			fSent = 0;
		}
		else
		{
			fSent = 0;
		}
		if( mSendOfs == pkt->VGetSize() )
		{
			mOutList.pop_front();
			mSendOfs = 0;
		}

	} while ( fSent && !mOutList.empty() );
}

void NetSocket::VHandleInput()
{
	bool bPktRecieved = false;
	u_long packetSize = 0;
	int rc = recv( mSock, mRecvBuf + mRecvBegin + mRecvOfs, RECV_BUFFER_SIZE - ( mRecvBegin + mRecvOfs ), 0 );

	char metrics[1024];
	sprintf_s( metrics, 1024, "Incoming: %6d bytes. begin %6d Offset %4d\n", rc, mRecvBegin, mRecvOfs );

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
	unsigned int newData = mRecvOfs + rc;
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
				std::shared_ptr<BinaryPacket> pkt( new BinaryPacket( &mRecvBuf[mRecvBegin + hdrSize], packetSize - hdrSize ) );
				mInList.push_back( pkt );
				bPktRecieved = true;
				processedData += packetSize;
				newData -= packetSize;
				mRecvBegin += packetSize;
			}
		}
	}

	gpSocketManager->AddToInbound( rc );
	mRecvOfs = newData;
	
	if( bPktRecieved )
	{
		if( mRecvOfs == 0 )
		{
			mRecvBegin = 0;
		}
		else if( mRecvBegin + mRecvOfs + MAX_PACKET_SIZE > RECV_BUFFER_SIZE )
		{
			int leftover = mRecvOfs;
			memcpy( mRecvBuf, &mRecvBuf[mRecvBegin], mRecvOfs );
			mRecvBegin = 0;
		}
	}
}

NetListenSocket::NetListenSocket( int portnum )
{
	port = 0;
	Init( portnum );
}

void NetListenSocket::Init( int portnum )
{
	struct sockaddr_in sa;
	int value = 1;

	if( (mSock = socket( PF_INET, SOCK_STREAM, 0 ) ) == INVALID_SOCKET )
	{
		assert("NetListSocket Error: Init failed to create socket handle");
	}
	if( setsockopt( mSock, SOL_SOCKET, SO_REUSEADDR, (char *)&value, sizeof(value) ) == SOCKET_ERROR )
	{
		perror( "NetListenSocket::Init: setsockopt" );
		closesocket( mSock );
		mSock = INVALID_SOCKET;
		assert( "NetListSocket error: Init failed to set socket options" );

	}

	memset( &sa, 0, sizeof(sa) );
	sa.sin_family = AF_INET;
	sa.sin_addr.s_addr = ADDR_ANY;
	sa.sin_port = htons( portnum );

	if( bind( mSock, (struct sockaddr *)&sa, sizeof(sa ) == SOCKET_ERROR ) )
	{
		perror( "NetListenSocket::Init: bind" );
		closesocket( mSock );
		mSock = INVALID_SOCKET;
	}
	
	SetBlocking( false );

	if( listen( mSock, 256 ) == SOCKET_ERROR )
	{
		closesocket( mSock );
		mSock = INVALID_SOCKET;
		assert( "NetListenSocket Error: Init failed to listen" );
	}
	port = portnum;
}

void NetListenSocket::InitScan( int portnum_min, int portnum_max )
{
	struct sockaddr_in sa;
	int portnum, x = 1;

	if( ( mSock = socket( PF_INET, SOCK_STREAM, IPPROTO_TCP ) ) == INVALID_SOCKET )
	{
		EXIT_ASSERT
			exit(1);
	}
	if( setsockopt( mSock, SOL_SOCKET, SO_REUSEADDR, (char *)&x, sizeof(x) ) == SOCKET_ERROR )
	{
		closesocket( mSock );
		mSock = INVALID_SOCKET;
		EXIT_ASSERT
			exit(1);
	}

	memset( &sa, 0, sizeof(sa) );
	sa.sin_family = AF_INET;
	for( portnum = portnum_min; portnum < portnum_max; portnum++ )
	{
		sa.sin_port = htons( portnum );

		if( bind( mSock, (struct sockaddr *)&sa, sizeof(sa) ) != SOCKET_ERROR )
		{
			break;
		}
	}
	if( portnum == portnum_max )
	{
		closesocket( mSock );
		mSock = INVALID_SOCKET;
		EXIT_ASSERT
			exit(1);
	}

	SetBlocking( false );

	if( listen( mSock, 8 ) == SOCKET_ERROR )
	{
		closesocket( mSock );
		mSock = INVALID_SOCKET;
		EXIT_ASSERT
			exit(1);
	}

	port = portnum;
}

SOCKET NetListenSocket::AcceptConnection( unsigned int *pAddr )
{
	SOCKET newSock;
	struct sockaddr_in sock;
	int size = sizeof(sock);

	if( (newSock = accept( mSock, (struct sockaddr *)&sock, &size ) ) == INVALID_SOCKET )
	{
		return INVALID_SOCKET;
	}
	if( getpeername( newSock, (struct sockaddr *)&sock, &size ) == SOCKET_ERROR )
	{
		closesocket( newSock );
		return INVALID_SOCKET;
	}
	*pAddr = ntohl( sock.sin_addr.s_addr );
	return newSock;
}

BaseSocketManager::BaseSocketManager()
{
	mInbound = 0;
	mOutbound = 0;
	mMaxOpenSockets = 0;
	mSubnetMask = 0;
	mSubnet = 0xffffffff;
	mNextSocketId = 0;

	gpSocketManager = this;
	ZeroMemory( &mWsaData, sizeof(WSADATA) );
}

bool BaseSocketManager::Init()
{
	if( WSAStartup( 0x0202, &mWsaData ) == 0 )
	{
		return true;
	}
	else
	{
		printf("WSAStartup failure!\n");
		return false;
	}
}

void BaseSocketManager::Shutdown()
{
	while( !mSockList.empty() )
	{
		delete *mSockList.begin();
		mSockList.pop_front();
	}

	WSACleanup();
}

int BaseSocketManager::AddSocket( NetSocket *socket )
{
	socket->mID = mNextSocketId;
	mSockMap[mNextSocketId] = socket;
	++mNextSocketId;

	mSockList.push_front( socket );
	if( mSockList.size() > mMaxOpenSockets )
	{
		++mMaxOpenSockets;
	}
	return socket->mID;
}

void BaseSocketManager::RemoveSocket( NetSocket *socket )
{
	mSockList.remove( socket );
	mSockMap.erase( socket->mID );
	if( socket )
	{
		delete  socket;
	}
}

NetSocket *BaseSocketManager::FindSocket( int sockID )
{
	SocketIdMap::iterator i = mSockMap.find( sockID );
	if( i == mSockMap.end() )
	{
		return nullptr;
	}
	return i->second;
}

int BaseSocketManager::GetIpAddress( int sockID )
{
	NetSocket *socket = FindSocket( sockID );
	if( socket )
	{
		return socket->GetIpAddress();
	}
	else
	{
		return 0;
	}
}

bool BaseSocketManager::Send( int sockID, std::shared_ptr<IPacket> packet )
{
	NetSocket *sock = FindSocket( sockID );
	if( !sock )
	{
		return false;
	}
	sock->Send( packet );
	return true;
}

void BaseSocketManager::DoSelect( int pauseMicroSecs, bool handleInput )
{
	timeval tv;
	tv.tv_sec = 0;
	tv.tv_usec = pauseMicroSecs;

	fd_set inp_set, out_set, exc_set;
	int maxdesc;

	FD_ZERO( &inp_set );
	FD_ZERO( &out_set );
	FD_ZERO( &exc_set );

	maxdesc = 0;

	for( SocketList::iterator i = mSockList.begin(); i != mSockList.end(); ++i )
	{
		NetSocket *pSock = *i;
		if( ( pSock->mDeleteFlag & 1 ) || pSock->mSock == INVALID_SOCKET )
		{
			continue;
		}
		if( handleInput )
		{
			FD_SET( pSock->mSock, &inp_set );
		}
		FD_SET( pSock->mSock, &exc_set );

		if( pSock->VHasOutput() )
		{
			FD_SET( pSock->mSock, &out_set );
		}
		if( (int)pSock->mSock > maxdesc )
		{
			maxdesc = (int)pSock->mSock;
		}
	}

	int selRet = 0;

	selRet = select( maxdesc + 1, &inp_set, &out_set, &exc_set, &tv );
	if( selRet == SOCKET_ERROR )
	{
		return;
	}

	if( selRet )
	{
		for( SocketList::iterator i = mSockList.begin(); i != mSockList.end(); ++i )
		{
			NetSocket *pSock = *i;

			if( ( pSock->mDeleteFlag & 1 ) || pSock->mSock == INVALID_SOCKET )
			{
				continue;
			}
			if( FD_ISSET( pSock->mSock, &exc_set ) )
			{
				pSock->HandleException();
			}
			if( !(pSock->mDeleteFlag & 1 ) && FD_ISSET( pSock->mSock, &out_set ) )
			{
				pSock->VHandleOutput();
			}
			if( handleInput && !(pSock->mDeleteFlag & 1 ) && FD_ISSET( pSock->mSock, &inp_set ) )
			{
				pSock->VHandleInput();
			}
		}
	}

	unsigned int timeNow = timeGetTime();

	SocketList::iterator i = mSockList.begin();
	while( i != mSockList.end() )
	{
		NetSocket *pSock = *i;
		if( pSock->mTimeOut )
		{
			if( pSock->mTimeOut < timeNow )
			{
				pSock->VTimeOut();
			}
		}
		if( pSock->mDeleteFlag & 1 )
		{
			switch( pSock->mDeleteFlag )
			{
				case 1:
					gpSocketManager->RemoveSocket( pSock );
					break;
				case 3:
					pSock->mDeleteFlag = 2;
					if( pSock->mSock != INVALID_SOCKET )
					{
						closesocket( pSock->mSock );
						pSock->mSock = INVALID_SOCKET;
					}
			}
		}
		++i;
	}
}

bool BaseSocketManager::IsInternal( unsigned int ipaddr )
{
	if( !mSubnetMask )
	{
		return false;
	}
	if( ( ipaddr & mSubnetMask ) == mSubnet )
	{
		return false;
	}
	return true;
}

unsigned int BaseSocketManager::GetHostByName( const std::string &hostName )
{
	struct hostent *pHostEnt = gethostbyname( hostName.c_str() );
	struct sockaddr_in tmpSockAddr;

	if( pHostEnt == nullptr )
	{
		assert( "Error occured" );
		return 0;
	}

	memcpy( &tmpSockAddr.sin_addr, pHostEnt, pHostEnt->h_length );

	return ntohl( tmpSockAddr.sin_addr.s_addr );
}

const char * BaseSocketManager::GetHostByAddr( unsigned int ip )
{
	static char host[256];

	int netip = htonl( ip );
	struct hostent *lpHostEnt = gethostbyaddr( (const char *)&netip, 4, PF_INET );

	if( lpHostEnt )
	{
		strcpy_s( host, 256, lpHostEnt->h_name );
		return host;
	}

	return nullptr;
}


bool ClientSocketManager::Connect()
{
	if( !BaseSocketManager::Init() )
	{
		return false;
	}

	RemoteEventSocket *pSocket = new RemoteEventSocket;

	if( !pSocket->Connect( GetHostByName( mHostName ), mPort ) )
	{
		if( pSocket )
		{
			delete pSocket;
			return false;
		}
	}
	AddSocket( pSocket );
	return true;
}

void GameServerListenSocket::VHandleInput()
{
	unsigned int theipaddr;
	SOCKET newSock = AcceptConnection( &theipaddr );

	int value = 1;
	setsockopt( newSock, SOL_SOCKET, SO_DONTLINGER, (char *)&value, sizeof(value) );

	if( newSock != INVALID_SOCKET )
	{
		RemoteEventSocket *sock = new RemoteEventSocket( newSock, theipaddr );
		int sockID = gpSocketManager->AddSocket( sock );
		int ipAddress = gpSocketManager->GetIpAddress( sockID );
	}
}

void RemoteEventSocket::VHandleInput()
{
	NetSocket::VHandleInput();

	while( !mInList.empty() )
	{
		std::shared_ptr<IPacket> packet = *mInList.begin();
		mInList.pop_front();
		if( !strcmp( packet->VGetType(), BinaryPacket::gType ) )
		{
			const char *buf = packet->VGetData();
			int size = static_cast<int>( packet->VGetSize() );

			std::istrstream in( buf + sizeof(u_long), (size - sizeof(u_long) ) );

			int type;
			in >> type;
			switch( type )
			{
			case NetMsg_Event:
				CreateEvent( in );
				break;
			default:
				printf("Unknown message type");
			}
		}
		else
		{
			printf( "Unknown packet sizeof: %d", packet->VGetData() + sizeof(u_long) );
		}
	}
}

void RemoteEventSocket::CreateEventW( std::istrstream &in )
{
	EventType eventType;
	in >> eventType;

	IEventDataPtr pEvent(CREATE_EVENT(eventType));
	if( pEvent )
	{
		pEvent->VDeserialize(in);
		IEventManager::Get()->VQueueEvent( pEvent );
	}
	else
	{
		printf("ERROR Unknown event type from remote.\n");
	}
}

void NetworkEventForwarder::ForwardEvent( IEventDataPtr pEventData )
{
	std::ostrstream out;

	out << static_cast<int>(RemoteEventSocket::NetMsg_Event) << " ";
	out << pEventData->VGetEventType() << " ";
	pEventData->VSerialize( out );
	out << "\r\n";

	shared_ptr<BinaryPacket> eventMsg( new BinaryPacket( out.rdbuf()->str(), (u_long)out.pcount() ) );

	gpSocketManager->Send( mSockID, eventMsg );
}