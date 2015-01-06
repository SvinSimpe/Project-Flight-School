#include "Server.h"

///////////////////////////////////////////////////////////////////////////////
//									PRIVATE
///////////////////////////////////////////////////////////////////////////////

bool Server::AcceptConnection()
{
	SOCKET s = accept( mListenSocket, nullptr, nullptr );
	if ( s == INVALID_SOCKET )
	{
		printf( "accept failed with error: %d\n", WSAGetLastError() );
		return false;
	}
	else
	{
		EvPlayerConnection toJoining;
		for ( auto& socket : mClientSockets )
		{
			if(socket != INVALID_SOCKET)
			{
				toJoining.ID = (unsigned int)socket;
				mConn->SendPkg( s, 0, Net_Event::EV_PLAYER_JOINED, toJoining ); // Sends the ID of the already existing clients to the joining client
				Sleep(10);
			}
		}
		Sleep(10);

		int flag	= 1;
        mResult		= setsockopt( s,            /* socket affected */
                                IPPROTO_TCP,     /* set option at TCP level */
                                TCP_NODELAY,     /* name of option */
                                (char*) &flag,  /* the cast is historical cruft */
                                sizeof(int) );    /* length of option value */
		if(mResult != 0)
		{
			printf( "setsockopt failed with error: %d\n", WSAGetLastError() );
			shutdown( s, SD_SEND );
			closesocket( s );
			WSACleanup();
			return false;
		}

		mClientSockets.push_back( s );
		EvPlayerConnection msg;
		msg.ID = (unsigned int)s;
		mConn->SendPkg( s, -1, Net_Event::YOUR_ID, s );
	}
	printf( "%d connected.\n", s );

	return true;
}

bool Server::ReceiveLoop( int index )
{
	Package<void*>* p = new Package<void*>[DEFAULT_BUFLEN];
	while ( mClientSockets.at(index) != INVALID_SOCKET )
	{
		SOCKET savedSocket = mClientSockets.at(index); // Used in case of disconnect
		if ( mConn->ReceivePkg( mClientSockets.at(index), *p ) )
		{
			if ( p->head.eventType != Net_Event::ERROR_EVENT )
			{
				HandlePkg( mClientSockets.at(index), p );
			}
		}
		if( mClientSockets.at(index) == INVALID_SOCKET )
		{
			DisconnectClient( savedSocket );
		}
	}

	if ( p )
	{
		delete[] p;
	}
	return true;
}

void Server::DisconnectClient( SOCKET s )
{
	EvPlayerConnection msg;
	msg.ID = s;
	for( auto& to : mClientSockets )
	{
		mConn->SendPkg( to, 0, Net_Event::EV_PLAYER_LEFT, msg );
	}
}

///////////////////////////////////////////////////////////////////////////////
//									PUBLIC
///////////////////////////////////////////////////////////////////////////////

Server* Server::GetInstance()
{
	static Server instance;
	return &instance;
}

bool Server::Connect()
{
	mListenSocket = socket( mAddrResult->ai_family, mAddrResult->ai_socktype, mAddrResult->ai_protocol );
	if ( mListenSocket == INVALID_SOCKET )
	{
		printf( "socket failed with error: %d\n", WSAGetLastError() );
		freeaddrinfo( mAddrResult );
		WSACleanup();
		return false;
	}

	mResult = bind( mListenSocket, mAddrResult->ai_addr, (int)mAddrResult->ai_addrlen );
	if ( mResult != 0 )
	{
		printf( "bind failed with error: %d\n", WSAGetLastError() );
		freeaddrinfo( mAddrResult );
		closesocket( mListenSocket );
		WSACleanup();
		return false;
	}
	freeaddrinfo( mAddrResult );

	mResult = listen( mListenSocket, SOMAXCONN );
	if ( mResult == SOCKET_ERROR )
	{
		printf( "listen failed with error: %d\n", WSAGetLastError() );
		closesocket( mListenSocket );
		WSACleanup();
		return false;
	}
	printf( "Server up and running.\n" );

	return true;
}

bool Server::Run()
{
	while ( true )
	{
		if ( AcceptConnection() )
		{
			printf( "Number of connected clients: %d.\n", mClientSockets.size() );
			mListenThreads.push_back( std::thread( &Server::ReceiveLoop, this, mClientSockets.size() - 1 ) );
		}
		else
		{
			break;
		}
	}
	return true;
}

bool Server::Initialize( const char* port )
{
	WSADATA WSAData = WSADATA();
	mResult			= WSAStartup( MAKEWORD( 2, 2 ), &WSAData );
	if ( mResult != 0 )
	{
		printf( "WSAStartup failed with error: %d\n", mResult );
		return false;
	}

	addrinfo hints;
	ZeroMemory( &hints, sizeof( hints ) );
	hints.ai_family		= AF_INET;
	hints.ai_socktype	= SOCK_STREAM;
	hints.ai_protocol	= 0;
	hints.ai_flags		= AI_PASSIVE;

	mResult				= getaddrinfo( nullptr, port, &hints, &mAddrResult );
	if ( mResult != 0 )
	{
		printf( "getaddrinfo failed with error: %d\n", mResult );
		WSACleanup();
		return false;
	}

	mConn = new Connection();
	mConn->Initialize();

	return true;
}

void Server::Release()
{
	for ( auto& t : mListenThreads )
	{
		t.join();
	}
	mListenThreads.clear();

	for( auto& s : mClientSockets )
	{
		mConn->DisconnectSocket( s ); 
	}
	mClientSockets.clear();
	WSACleanup();
	mConn->Release();

	if ( mConn )
	{
		delete mConn;
	}
}

Server::Server()
{
	mResult			= 0;
	mAddrResult		= nullptr;
	mListenSocket	= INVALID_SOCKET;
	mClientSockets	= std::vector<SOCKET>( 0 );
	mConn			= nullptr;
	mListenThreads	= std::vector<std::thread>( 0 );
}

Server::~Server()
{
}