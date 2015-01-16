#include "Client.h"

bool Client::ReceiveLoop()
{
	Package<void*>* p = new Package<void*>[DEFAULT_BUFLEN]; // This will only be called once per Client instance
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
	if ( p )
	{
		delete[] p;
	}
	return true;
}

void Client::EventListener( IEventPtr newEvent )
{
	if ( newEvent->GetEventType() == Event_Player_Update::GUID )
	{
		std::shared_ptr<Event_Player_Update> data = std::static_pointer_cast<Event_Player_Update>( newEvent );
		if ( mServerSocket != INVALID_SOCKET )
		{
			EvPlayerUpdate msg;
			msg.id						= mID;
			msg.lowerBodyPosition		= data->LowerBodyPos();
			msg.lowerBodyDirection		= data->LowerBodyDirection();
			msg.lowerBodyAnimation		= data->LowerBodyAnimation();
			msg.lowerBodyAnimationTime	= data->LowerBodyAnimationTime();
			msg.upperBodyPosition		= data->UpperBodyPos();
			msg.upperBodyDirection		= data->UpperBodyDirection();

			if ( mServerSocket != INVALID_SOCKET )
			{
				mConn->SendPkg( mServerSocket, 0, Net_Event::EV_PLAYER_UPDATE, msg );
			}
		}
	}
	else if ( newEvent->GetEventType() == Event_Player_Died::GUID )
	{
		std::shared_ptr<Event_Player_Died> data = std::static_pointer_cast<Event_Player_Died>( newEvent );
		if ( mServerSocket != INVALID_SOCKET )
		{
			EvPlayerID msg;
			msg.ID = data->ID();

			if ( mServerSocket != INVALID_SOCKET )
			{
				mConn->SendPkg( mServerSocket, 0, Net_Event::EV_PLAYER_DIED, msg );
			}
		}
	}
	else if ( newEvent->GetEventType() == Event_Player_Damaged::GUID )
	{
		std::shared_ptr<Event_Player_Damaged> data = std::static_pointer_cast<Event_Player_Damaged>( newEvent );
		if ( mServerSocket != INVALID_SOCKET )
		{
			EvPlayerID msg;
			msg.ID = data->ID();
			msg.projectileID = data->ProjectileID();

			if ( mServerSocket != INVALID_SOCKET )
			{
				mConn->SendPkg( mServerSocket, 0, Net_Event::EV_PLAYER_DAMAGED, msg );
			}
		}
	}
	else if ( newEvent->GetEventType() == Event_Player_Spawned::GUID )
	{
		std::shared_ptr<Event_Player_Spawned> data = std::static_pointer_cast<Event_Player_Spawned>( newEvent );
		if ( mServerSocket != INVALID_SOCKET )
		{
			EvPlayerID msg;
			msg.ID = data->ID();

			if ( mServerSocket != INVALID_SOCKET )
			{
				mConn->SendPkg( mServerSocket, 0, Net_Event::EV_PLAYER_SPAWNED, msg );
			}
		}
	}
	else if ( newEvent->GetEventType() == Event_Projectile_Fired::GUID )
	{
		std::shared_ptr<Event_Projectile_Fired> data = std::static_pointer_cast<Event_Projectile_Fired>( newEvent );
		if ( mServerSocket != INVALID_SOCKET )
		{
			EvProjectileFired msg;
			msg.ID			= mID;
			msg.position	= data->BodyPos();
			msg.direction	= data->Direction();

			if ( mServerSocket != INVALID_SOCKET )
			{
				mConn->SendPkg( mServerSocket, 0, Net_Event::EV_PROJECTILE_FIRED, msg );
			}
		}
	}
	else if ( newEvent->GetEventType() == Event_Player_Update_HP::GUID )
	{
		std::shared_ptr<Event_Player_Update_HP> data = std::static_pointer_cast<Event_Player_Update_HP>( newEvent );
		if ( mServerSocket != INVALID_SOCKET )
		{
			EvPlayerID msg;
			msg.ID			= mID;
			msg.HP	= data->HP();

			if ( mServerSocket != INVALID_SOCKET )
			{
				mConn->SendPkg( mServerSocket, 0, Net_Event::EV_UPDATE_HP, msg );
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

	int flag = 1;
    int mResult = setsockopt( mServerSocket,            /* socket affected */
                                 IPPROTO_TCP,     /* set option at TCP level */
                                 TCP_NODELAY,     /* name of option */
                                 (char *) &flag,  /* the cast is historical
                                                         cruft */
                                 sizeof(int) );    /* length of option value */

	if( mResult != 0 )
	{
		printf( "setsockopt failed with error %d.\n", WSAGetLastError() );
		mConn->DisconnectSocket( mServerSocket );
		WSACleanup();
		return false;
	}

	printf( "Connected to: %d\n", mServerSocket );

	return true;
}

bool Client::Run()
{
	EventManager::GetInstance()->AddListener( &Client::EventListener, this, Event_Player_Update::GUID );
	EventManager::GetInstance()->AddListener( &Client::EventListener, this, Event_Player_Died::GUID );
	EventManager::GetInstance()->AddListener( &Client::EventListener, this, Event_Player_Damaged::GUID );
	EventManager::GetInstance()->AddListener( &Client::EventListener, this, Event_Player_Spawned::GUID );
	EventManager::GetInstance()->AddListener( &Client::EventListener, this, Event_Projectile_Fired::GUID );
	EventManager::GetInstance()->AddListener( &Client::EventListener, this, Event_Player_Update_HP::GUID );
	std::thread listen( &Client::ReceiveLoop, this );

	mConn->SendPkg( mServerSocket, 0, Net_Event::EV_PLAYER_JOINED, 0 ); // The client "announces" itself to the server, and by extension, the other clients

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
	mConn->DisconnectSocket( mServerSocket );

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