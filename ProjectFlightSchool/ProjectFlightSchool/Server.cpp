#include "Server.h"

Server* Server::GetInstance()
{
	static Server instance;
	return &instance;
}

bool Server::AcceptConnection()
{
	SOCKET s	= INVALID_SOCKET;
	s			= accept( mListenSocket, nullptr, nullptr );
	if ( s == INVALID_SOCKET )
	{
		printf( "accept failed with error: %d\n", WSAGetLastError() );
		return false;
	}
	else
	{
		mClientSockets.push_back( s );
	}
	printf( "%d connected.\n", s );

	return true;
}

bool Server::ReceiveLoop( int index )
{
	while ( mClientSockets.at( index ) != INVALID_SOCKET )
	{
		Package<Position> p;

		if ( mConn->ReceiveMsg( mClientSockets.at(index), p ) )
		{
			Position r = p.body.content;
			printf("%d sent: %d, %d, %d\n", mClientSockets.at(index), r.x, r.y, r.z);
			//HandleMsg( mClientSockets.at(index), m.msg );
			//printf( "%d sent: %s\n", mClientSockets.at(index), m.msg );
		}
	}
	return true;
}

void Server::HandleMsg( SOCKET &socket, char* msg )
{
	if ( strcmp( msg, "Quit" ) == 0 )
	{
		printf("%d disconnected.\n", socket);
		mConn->SendMsg( socket, msg );
		mConn->DisconnectSocket( socket );

		bool allClosed = true;
		for ( auto& s : mClientSockets )
		{
			if ( s != INVALID_SOCKET )
			{
				allClosed = false;
			}
		}
		if ( allClosed )
		{
			mConn->DisconnectSocket( mListenSocket );
		}
	}
	else if ( strcmp( msg, "Shutdown" ) == 0 )
	{
		for ( auto& s : mClientSockets )
		{
			if ( s != INVALID_SOCKET )
			{
				mConn->SendMsg( s, "Quit" );
			}
		}

		for ( auto& s : mClientSockets )
		{
			if ( s != INVALID_SOCKET )
			{
				mConn->DisconnectSocket( s );
			}
		}
		mConn->DisconnectSocket( mListenSocket );
	}
	else
	{
		for ( auto& s : mClientSockets )
		{
			if ( s != INVALID_SOCKET )
			{
				if ( s != socket )
				{
					std::string sMsg = std::to_string( socket ) + " says: " + msg;
					msg = (char*)sMsg.c_str();
					mConn->SendMsg( s, msg );
				}
				else
				{
					Message msg;
					msg.msg = "Hello client!";
					mConn->SendMsg( socket, msg );
				}
			}
		}
	}
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
			mListenThreads.push_back( std::thread( &Server::ReceiveLoop, this, mClientSockets.size() - 1) );
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
	WSADATA WSAData;

	mResult = WSAStartup( MAKEWORD( 2, 2 ), &WSAData );
	if ( mResult != 0 )
	{
		printf( "WSAStartup failed with error: %d\n", mResult );
		return false;
	}

	addrinfo hints;
	ZeroMemory( &hints, sizeof( hints ) );
	hints.ai_family		= AF_INET;
	hints.ai_socktype	= SOCK_STREAM;
	hints.ai_protocol	= IPPROTO_TCP;
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

	mClientSockets.clear();
	WSACleanup();
	mConn->Release();

	if ( mConn )
		delete mConn;
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