#include "Client.h"

bool Client::Connect()
{
	addrinfo* ptr = nullptr;
	for (ptr = mAddrResult; ptr != nullptr; ptr = ptr->ai_next)
	{
		mSocket = socket( ptr->ai_family, ptr->ai_socktype, ptr->ai_protocol );
		if ( mSocket == INVALID_SOCKET )
		{
			printf( "socket failed with error: %d\n", WSAGetLastError() );
			WSACleanup();
			return false;
		}

		mResult = connect( mSocket, ptr->ai_addr, (int)ptr->ai_addrlen );
		if ( mResult == SOCKET_ERROR )
		{
			closesocket( mSocket );
			mSocket = INVALID_SOCKET;
			continue;
		}
		break;
	}
	freeaddrinfo( mAddrResult );

	if ( mSocket == INVALID_SOCKET )
	{
		printf( "Unable to connect to server.\n" );
		closesocket( mSocket );
		WSACleanup();
		return false;
	}

	printf( "Connected to: %d\n", mSocket );

	return true;
}

bool Client::Run()
{
	std::thread write( &Client::MsgLoop, this );
	std::thread listen( &Client::RecvLoop, this );

	write.join();
	listen.join();

	return true;
}

bool Client::MsgLoop()
{
	bool result = false;
	while ( mSocket != INVALID_SOCKET )
	{
		std::string msg = "";
		std::getline( std::cin, msg );
		result = mConn->SendMsg( mSocket, (char*)msg.c_str() );
	};

	return true;
}

bool Client::RecvLoop()
{
	do
	{
		char* msg = mConn->RecvMsg( mSocket );
		if ( msg )
		{
			HandleMsg( msg );
			delete msg;
		}
	} while ( mSocket != INVALID_SOCKET );
	return true;
}

bool Client::HandleMsg( char* msg )
{
	if ( strcmp( msg, "Quit" ) == 0 )
	{
		mConn->DisconnectSocket( mSocket );
		mConn->PrintMsg( "Connection lost." );
	}
	else
	{
		mConn->PrintMsg( msg );
	}
	return true;
}

bool Client::Initialize( const char* ip, const char* port )
{
	WSADATA WSAData;

	mResult = WSAStartup( MAKEWORD( 2, 2 ), &WSAData );
	if (mResult != 0)
	{
		printf( "WSAStartup failed with error: %d\n", mResult );
		return false;
	}

	addrinfo hints = { 0 };
	ZeroMemory(&hints, sizeof( hints ));
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_protocol = IPPROTO_TCP;

	mResult = getaddrinfo( ip, port, &hints, &mAddrResult );
	if (mResult != 0)
	{
		printf( "getaddrinfo failed with error: %d\n", mResult );
		WSACleanup();
		return false;
	}

	mConn = new Connection();
	mConn->Initialize();

	return true;
}


void Client::Release()
{
	WSACleanup();

	mConn->Release();
	if (mConn)
		delete mConn;
}

Client::Client()
{
	mResult		= 0;
	mAddrResult = nullptr;
	mSocket		= INVALID_SOCKET;
	mConn		= nullptr;
}


Client::~Client()
{
}
