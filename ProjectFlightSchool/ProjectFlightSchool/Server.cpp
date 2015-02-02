#include "Server.h"

///////////////////////////////////////////////////////////////////////////////
//									PRIVATE
///////////////////////////////////////////////////////////////////////////////
void Server::EventListener( IEventPtr newEvent )
{
	if ( newEvent->GetEventType() == Event_Game_Started::GUID )
	{
		mInGame		= true;
	}
	else if ( newEvent->GetEventType() == Event_Game_Ended::GUID )
	{
		mInGame		= false;
	}
	else if( newEvent->GetEventType() == Event_Set_Enemy_State::GUID )
	{
		std::shared_ptr<Event_Set_Enemy_State> data = std::static_pointer_cast<Event_Set_Enemy_State>( newEvent );
		EvSetEnemyState state;
		state.ID	= data->ID();
		state.state	= data->State();
		for ( auto& socket : mClientSockets )
		{
			if ( socket.s != INVALID_SOCKET )
			{
				mConn->SendPkg( socket.s, 0, Net_Event::EV_SET_ENEMY_STATE, state );
			}
		}
	}
}

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
		int flag = 1;
		mResult = setsockopt( s,            /* socket affected */
			IPPROTO_TCP,     /* set option at TCP level */
			TCP_NODELAY,     /* name of option */
			(char*)&flag,  /* the cast is historical cruft */
			sizeof(int)) ;    /* length of option value */
		if ( mResult != 0 )
		{
			printf( "setsockopt failed with error: %d\n", WSAGetLastError() );
			shutdown( s, SD_SEND );
			closesocket( s );
			WSACleanup();
			return false;
		}

		Clientinfo newClient	= { s, mNextTeamDelegation };
		mNextTeamDelegation		= ( mNrOfTeams - 1 ) - ( mNextTeamDelegation / ( mNrOfTeams - 1 ) );

		EvInitialize toJoining;
		for ( auto& socket : mClientSockets )
		{
			if( socket.s != INVALID_SOCKET )
			{
				toJoining.ID	= socket.s;
				toJoining.team	= socket.team;
				mConn->SendPkg( s, 0, Net_Event::EV_PLAYER_JOINED, toJoining ); // Sends the ID of the already existing clients to the joining client
				Sleep( 10 );
			}
		}
		Sleep( 10 );

		mClientSockets.push_back( newClient );
		EvInitialize msg;
		msg.ID		= (unsigned int)s;
		msg.team	= newClient.team;
		mConn->SendPkg( s, -1, Net_Event::YOUR_ID, msg );
	}
	printf( "%d connected.\n", s );

	return true;
}

bool Server::ReceiveLoop( int index )
{
	Package<void*>* p = new Package<void*>[DEFAULT_BUFLEN];
	while ( mClientSockets.at(index).s != INVALID_SOCKET )
	{
		SOCKET savedSocket = mClientSockets.at(index).s; // Used in case of disconnect
		if ( mConn->ReceivePkg( mClientSockets.at(index).s, *p ) )
		{
			if ( p->head.eventType != Net_Event::ERROR_EVENT )
			{
				HandlePkg( mClientSockets.at(index).s, p );
			}
		}
		if( mClientSockets.at(index).s == INVALID_SOCKET )
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
	EvPlayerID msg;
	msg.ID = s;
	for( auto& to : mClientSockets )
	{
		mConn->SendPkg( to.s, 0, Net_Event::EV_PLAYER_LEFT, msg );
	}
}

XMFLOAT3 Server::GetNextSpawn()
{
	return mSpawners[mNrOfEnemiesSpawned++ % MAX_NR_OF_ENEMY_SPAWNERS]->GetSpawnPosition();
}

void Server::AggroCheck()
{
	for ( size_t i = 0; i < MAX_NR_OF_ENEMIES; i++ )
	{
		if( mEnemies[i]->IsAlive() )
		{
			for ( size_t j = 0; j < mNrOfPlayers; j++ )
			{
				// The players agg circle
				mAggCircle->center = mPlayers[j].Position;
				mAggCircle->radius = 0.5f;

				if( mEnemies[i]->GetAttentionCircle()->Intersect( mAggCircle ) )
				{
					if( mEnemies[i]->GetAttackCircle()->Intersect( mAggCircle ) )
					{
						mEnemies[i]->SetState( Attack );
					}
					else
					{
						mEnemies[i]->SetState( HuntPlayer );
						mEnemies[i]->SetHuntedPlayer( mPlayers[j].Position );
					}
				}
				else
				{
					mEnemies[i]->SetState( Idle );
				}
			}
		}
	}
}

///////////////////////////////////////////////////////////////////////////////
//									PUBLIC
///////////////////////////////////////////////////////////////////////////////

HRESULT Server::Update( float deltaTime )
{
	if( mInGame && mSafeUpdate )//&& mFrameCount++ > 2 )
	{
		EvUpdateEnemyPosition enemy;
		for ( size_t i = 0; i < MAX_NR_OF_ENEMIES; i++ )
		{
			if( mEnemies[i]->IsAlive() )
			{
				mEnemies[i]->Update( deltaTime );

				enemy.ID			= mEnemies[i]->GetID();
				enemy.position		= mEnemies[i]->GetPosition();
				enemy.direction		= mEnemies[i]->GetDirection();

				for ( auto& socket : mClientSockets )
				{
					if ( socket.s != INVALID_SOCKET && mEnemyListSynced )
					{
						mConn->SendPkg( socket.s, 0, Net_Event::EV_ENEMY_UPDATE_POSITION, enemy ); // Update all enemy positions to all players
						//Sleep( 1 );
					}
				}
			}
			else
			{
				mEnemies[i]->Spawn( GetNextSpawn() );
			}
		}

		//if( mSafeUpdate )
			AggroCheck();

		mFrameCount = 0;
	}

	//mFrameCount++;

	return S_OK;
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
		mConn->DisconnectSocket( mListenSocket );
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

bool Server::Initialize( std::string port )
{
	mNextTeamDelegation = 0;
	mNrOfTeams			= 2;

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

	mResult				= getaddrinfo( nullptr, port.c_str(), &hints, &mAddrResult );
	if ( mResult != 0 )
	{
		printf( "getaddrinfo failed with error: %d\n", mResult );
		WSACleanup();
		return false;
	}

	mConn = new Connection();
	mConn->Initialize();

	// Enemies & Spawners
	srand( time( NULL ) );
	mSpawners	= new EnemySpawn*[MAX_NR_OF_ENEMY_SPAWNERS];
	for ( size_t i = 0; i < MAX_NR_OF_ENEMY_SPAWNERS; i++ )
	{
		// Map size values
		int negX, negY, posX, posY;
		negX = rand() % 50;
		negY = rand() % 50;
		posX = rand() % 50;
		posY = rand() % 50;
		mSpawners[i] = new EnemySpawn();
		mSpawners[i]->Initilaize( i );
		mSpawners[i]->SetPosition( XMFLOAT3( (float)(posX - negX), 0.0f, (float)(negY - posY) ) );
	}

	mEnemies	= new Enemy*[MAX_NR_OF_ENEMIES];
	for ( size_t i = 0; i < MAX_NR_OF_ENEMIES; i++ )
	{
		mEnemies[i] = new Enemy();
		mEnemies[i]->Initialize( i );
		mEnemies[i]->Spawn( GetNextSpawn() );
		
		//mConn->SendPkg( mServerSocket, 0, Net_Event::EV_PLAYER_UPDATE, msg );

		//IEventPtr E1( new Event_Enemy_Created( mEnemies[i]->GetID(), mEnemies[i]->GetPosition(), MAX_NR_OF_ENEMIES) );
		//EventManager::GetInstance()->QueueEvent( E1 );
	}

	mEnemyListSynced	= false;
	mSafeUpdate			= false;
	mAggCircle			= new BoundingCircle();

	EventManager::GetInstance()->AddListener( &Server::EventListener, this, Event_Game_Started::GUID );
	EventManager::GetInstance()->AddListener( &Server::EventListener, this, Event_Game_Ended::GUID );
	EventManager::GetInstance()->AddListener( &Server::EventListener, this, Event_Set_Enemy_State::GUID );


	IEventPtr E1( new Event_Server_Initialized() );
	EventManager::GetInstance()->QueueEvent( E1 );

	return true;
}

void Server::Release()
{
	for( auto& s : mClientSockets )
	{
		mConn->DisconnectSocket( s.s ); 
	}
	for ( auto& t : mListenThreads )
	{
		if( t.joinable() )
			t.join();
	}
	mListenThreads.clear();
	mClientSockets.clear();
	WSACleanup();
	mConn->DisconnectSocket( mListenSocket );

	mConn->Release();
	if( mConn )
		delete mConn;

	// Enemies
	for ( size_t i = 0; i < MAX_NR_OF_ENEMIES; i++ )
		SAFE_DELETE( mEnemies[i] );

	delete [] mEnemies;

	for ( size_t i = 0; i < MAX_NR_OF_ENEMY_SPAWNERS; i++ )
		SAFE_DELETE( mSpawners[i] );

	delete [] mSpawners;

	SAFE_DELETE( mAggCircle );
}

Server::Server()
{
	mResult			= 0;
	mAddrResult		= nullptr;
	mListenSocket	= INVALID_SOCKET;
	mClientSockets	= std::vector<Clientinfo>( 0 );
	mConn			= nullptr;
	mListenThreads	= std::vector<std::thread>( 0 );
	mNrOfProjectilesFired	= 1;
	
	// Enemies
	mInGame				= false;
	mEnemies			= nullptr;
	mSpawners			= nullptr;
	mNrOfEnemiesSpawned	= 0;
	mFrameCount			= 0;

	mAggCircle			= nullptr;

	mEnemyListSynced	= false;
	mSafeUpdate			= false;
	mNrOfPlayers		= 0;
}

Server::~Server()
{
}