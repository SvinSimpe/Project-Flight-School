#include "Client.h"

bool Client::ReceiveLoop()
{
	Package<void*>* p = new Package<void*>[DEFAULT_BUFLEN];
	while ( mServerSocket != INVALID_SOCKET )
	{
		if ( mConn->ReceivePkg( mServerSocket, *p ) )
		{
			if ( p->head.eventType != Net_Event::ERROR_EVENT )
			{
				HandlePkg( p );
			}
		}
	}
	if (p)
	{
		delete[] p;
	}
	return true;
}

void Client::PlayerMoved( IEventPtr newEvent )
{
	if ( newEvent->GetEventType() == Event_Player_Moved::GUID )
	{
		std::shared_ptr<Event_Player_Moved> data = std::static_pointer_cast<Event_Player_Moved>( newEvent );
		if ( mServerSocket != INVALID_SOCKET )
		{
			EvPlayerMoved msg;
			msg.lowerBody	= data->LowerBodyPos();
			msg.upperBody	= data->UpperBodyPos();
			msg.direction	= data->Direction();

			if ( mServerSocket != INVALID_SOCKET )
			{
				mConn->SendPkg( mServerSocket, 0, Net_Event::EV_PLAYER_MOVED, msg );
			}
		}
	}
}

bool Client::Connect()
{
	for ( addrinfo* ptr = mAddrResult; ptr != nullptr; ptr = ptr->ai_next )
	{
		mServerSocket = socket( ptr->ai_family, ptr->ai_socktype, ptr->ai_protocol );
		if ( mServerSocket == INVALID_SOCKET )
		{
			printf( "socket failed with error: %d\n", WSAGetLastError() );
			WSACleanup();
			return false;
		}

		mResult = connect( mServerSocket, ptr->ai_addr, (int)ptr->ai_addrlen );
		if ( mResult == SOCKET_ERROR )
		{
			closesocket( mServerSocket );
			mServerSocket = INVALID_SOCKET;
			continue;
		}
		break;
	}
	freeaddrinfo( mAddrResult );

	if ( mServerSocket == INVALID_SOCKET )
	{
		printf( "Unable to connect to server.\n" );
		closesocket( mServerSocket );
		WSACleanup();
		return false;
	}

	printf( "Connected to: %d\n", mServerSocket );
	mConn->SendPkg( mServerSocket, 0, Net_Event::EV_PLAYER_JOINED, 0 ); // The client "announces" itself to the server, and by extension, the other clients

	return true;
}

bool Client::Run()
{
	EventManager::GetInstance()->AddListener( &Client::PlayerMoved, this, Event_Player_Moved::GUID );

	std::thread listen( &Client::ReceiveLoop, this );

	listen.join();

	return true;
}

bool Client::Initialize( const char* port, const char* ip )
{
	WSADATA WSAData = WSADATA();
	mResult			= WSAStartup( MAKEWORD( 2, 2 ), &WSAData );
	if ( mResult != 0 )
	{
		printf( "WSAStartup failed with error: %d\n", mResult );
		return false;
	}

	addrinfo hints = { 0 };
	ZeroMemory( &hints, sizeof( hints ) );
	hints.ai_family		= AF_INET;
	hints.ai_socktype	= SOCK_STREAM;
	hints.ai_protocol	= IPPROTO_TCP;

	mResult = getaddrinfo( ip, port, &hints, &mAddrResult );
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

void Client::Release()
{
	WSACleanup();

	mConn->Release();
	if ( mConn )
		delete mConn;
}

Client::Client()
{
	mResult			= 0;
	mAddrResult		= nullptr;
	mServerSocket	= INVALID_SOCKET;
	mConn			= nullptr;
}

Client::~Client()
{
}