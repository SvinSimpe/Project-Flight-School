#include "Client.h"

Client* Client::mInstance = nullptr;


Client::Client() : Network()
{
	mID						= (UINT)-1;
	mTeamID					= (UINT)-1;
	mSocketManager			= nullptr;
	mNEF					= nullptr;
	mRemoteIDs				= std::list<UINT>();
	mActive					= false;

	mLowerBodyPos			= XMFLOAT3( 0.0f, 0.0f, 0.0f );
	mVelocity				= XMFLOAT3( 0.0f, 0.0f, 0.0f );
	mUpperBodyDirection		= XMFLOAT3( 0.0f, 0.0f, 0.0f );
}

Client::~Client()
{
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Start of eventlistening functions

void Client::LocalJoined( IEventPtr eventPtr )
{
	if( eventPtr->GetEventType() == Event_Local_Joined::GUID )
	{
		std::shared_ptr<Event_Local_Joined> data = std::static_pointer_cast<Event_Local_Joined>( eventPtr );
		mID = data->ID();
		mTeamID = data->TeamID();

		std::cout << "My ID is: " << mID << " and I just joined team " << mTeamID << std::endl;
		mActive = true;
	}
}

void Client::RemoteJoined( IEventPtr eventPtr )
{
	if( eventPtr->GetEventType() == Event_Remote_Joined::GUID )
	{
		std::shared_ptr<Event_Remote_Joined> data = std::static_pointer_cast<Event_Remote_Joined>( eventPtr );
		UINT id = data->ID();
		UINT teamID = data->TeamID();
		mRemoteIDs.push_back( id );

		std::cout << "Remote with ID: " << id << " joined team: " << teamID << ". There are now " << mRemoteIDs.size() << " remotes online." << std::endl;
	}
}

void Client::RemoteLeft( IEventPtr eventPtr )
{
	if( eventPtr->GetEventType() == Event_Remote_Left::GUID )
	{
		std::shared_ptr<Event_Remote_Left> data = std::static_pointer_cast<Event_Remote_Left>( eventPtr );
		UINT id = data->ID();
		mRemoteIDs.remove( id );

		std::cout << "Remote with ID: " << id << " left. There are now " << mRemoteIDs.size() << " remotes online." << std::endl;
	}
}

void Client::RemoteUpdate( IEventPtr eventPtr )
{
	if( eventPtr->GetEventType() == Event_Remote_Update::GUID )
	{
		std::shared_ptr<Event_Remote_Update> data = std::static_pointer_cast<Event_Remote_Update>( eventPtr );
		UINT id = data->ID();
		XMFLOAT3 pos = data->LowerBodyPos();
		XMFLOAT3 vel = data->Velocity();
		XMFLOAT3 dir = data->UpperBodyDirection();

		std::cout << "Remote with ID: " << id << " updated." << std::endl;
	}
}

void Client::RemoteDied( IEventPtr eventPtr )
{
	if( eventPtr->GetEventType() == Event_Remote_Died::GUID )
	{
		std::shared_ptr<Event_Remote_Died> data = std::static_pointer_cast<Event_Remote_Died>( eventPtr );
		UINT id = data->ID();
		UINT killerID = data->KillerID();

		std::cout << "Remote with ID: " << id << " was killed by: " << killerID << std::endl;
	}
}

void Client::RemoteDamaged( IEventPtr eventPtr )
{
	if( eventPtr->GetEventType() == Event_Remote_Damaged::GUID )
	{
		std::shared_ptr<Event_Remote_Damaged> data = std::static_pointer_cast<Event_Remote_Damaged>( eventPtr );
		UINT id = data->ID();
		UINT projectileID = data->ProjectileID();

		std::cout << "Remote with ID: " << id << " was shot by bullet with ID: " << projectileID << std::endl;
	}
}

void Client::RemoteSpawned( IEventPtr eventPtr )
{
	if( eventPtr->GetEventType() == Event_Remote_Spawned::GUID )
	{
		std::shared_ptr<Event_Remote_Spawned> data = std::static_pointer_cast<Event_Remote_Spawned>( eventPtr );
		UINT id = data->ID();

		std::cout << "Remote with ID: " << id << " just spawned." << std::endl;
	}
}

void Client::RemoteFiredProjectile( IEventPtr eventPtr )
{
	if( eventPtr->GetEventType() == Event_Remote_Fired_Projectile::GUID )
	{
		std::shared_ptr<Event_Remote_Fired_Projectile> data = std::static_pointer_cast<Event_Remote_Fired_Projectile>( eventPtr );
		UINT id = data->ID();
		UINT projectileID = data->ProjectileID();
		XMFLOAT3 pos = data->BodyPos();
		XMFLOAT3 dir = data->Direction();

		std::cout << "Remote with ID: " << id << " just fired bullet: " << projectileID << " X-Pos: " << pos.x << " X-dir: " << dir.x << std::endl;
	}
}

void Client::RemoteUpdateHP( IEventPtr eventPtr )
{
	if( eventPtr->GetEventType() == Event_Remote_Update_HP::GUID )
	{
		std::shared_ptr<Event_Remote_Update_HP> data = std::static_pointer_cast<Event_Remote_Update_HP>( eventPtr );
		UINT id = data->ID();
		float hp = data->HP();

		if( hp < 0.0f )
		{
			std::cout << "Remote with ID: " << id << " lost " << (-1.0f)*hp << " HP." << std::endl;
		}
		else if( hp > 0.0f )
		{
			std::cout << "Remote with ID: " << id << " gained " << hp << " HP." << std::endl;
		}
		else
		{
			std::cout << "Remote with ID: " << id << " updated it's HP for no reason." << std::endl;
		}
	}
}

// End of eventlistening functions
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void Client::StartUp( IEventPtr eventPtr )
{
	if( eventPtr->GetEventType() == Event_Start_Client::GUID )
	{
		std::shared_ptr<Event_Start_Client> data = std::static_pointer_cast<Event_Start_Client>( eventPtr );
		std::string IP	= data->IP();
		UINT port		= data->Port();
		if( !Connect( IP, port ) )
			std::cout << "Failed to connect client!" << std::endl;
	}
}

/* Registers all the events that should be listened to from the server. */

bool Client::Connect( std::string ip, UINT port )
{
	mSocketManager = new ClientSocketManager();
	if( !mSocketManager->Connect( ip, port ) )
	{
		return false;
	}
	std::cout << "Client connected to server on IP: " << ip << ", port: " << port << std::endl;

	mNEF = new NetworkEventForwarder();
	mNEF->Initialize( 0, mSocketManager ); // Always sends to socket 0, the server's socketID
	return true;
}

Client* Client::GetInstance()
{
	if( !mInstance )
		mInstance = new Client();
	return mInstance;
}

void Client::SendEvent( IEventPtr eventPtr )
{
	mNEF->ForwardEvent( eventPtr );
}

void Client::Update( float deltaTime )
{
	if( mActive )
	{
		//IEventPtr E1( PFS_NEW Event_Client_Update( mID, mLowerBodyPos, mVelocity, mUpperBodyDirection ) );
		//IEventPtr E1( PFS_NEW Event_Client_Died( mID, 0 ) );
		//IEventPtr E1( PFS_NEW Event_Client_Spawned( mID ) );

		for( float i = -1.0f; i <= 1.0f; i += 1.0f )
		{
			IEventPtr E1( PFS_NEW Event_Client_Update_HP( mID, i ) );
			SendEvent( E1 );
		}

		//for( int i = 0; i < 10; i++ )
		//{
		//	//IEventPtr E1( PFS_NEW Event_Client_Damaged( mID, i ) );
		//	IEventPtr E1( PFS_NEW Event_Client_Fired_Projectile( mID, i, XMFLOAT3((float)i, 0.0f, 0.0f), XMFLOAT3((float)i, 0.0f, 0.0f ) ) );
		//	SendEvent( E1 );
		//}
		//SendEvent( E1 );
	}
}

void Client::DoSelect( int pauseMicroSecs, bool handleInput )
{
	mSocketManager->DoSelect( pauseMicroSecs, handleInput );
}

bool Client::Initialize()
{
	EF::REGISTER_EVENT( Event_Client_Joined );
	EF::REGISTER_EVENT( Event_Client_Left );
	EF::REGISTER_EVENT( Event_Local_Joined );
	EF::REGISTER_EVENT( Event_Remote_Joined );
	EF::REGISTER_EVENT( Event_Remote_Left );
	EF::REGISTER_EVENT( Event_Client_Update );
	EF::REGISTER_EVENT( Event_Remote_Update );
	EF::REGISTER_EVENT( Event_Change_State );
	EF::REGISTER_EVENT( Event_Start_Server );
	EF::REGISTER_EVENT( Event_Start_Client );
	EF::REGISTER_EVENT( Event_Game_Started );
	EF::REGISTER_EVENT( Event_Game_Ended );
	EF::REGISTER_EVENT( Event_Client_Died );
	EF::REGISTER_EVENT( Event_Remote_Died );
	EF::REGISTER_EVENT( Event_Client_Damaged );
	EF::REGISTER_EVENT( Event_Remote_Damaged );
	EF::REGISTER_EVENT( Event_Client_Spawned );
	EF::REGISTER_EVENT( Event_Remote_Spawned );
	EF::REGISTER_EVENT( Event_Client_Fired_Projectile );
	EF::REGISTER_EVENT( Event_Remote_Fired_Projectile );
	EF::REGISTER_EVENT( Event_Client_Update_HP );
	EF::REGISTER_EVENT( Event_Remote_Update_HP );

	EventManager::GetInstance()->AddListener( &Client::LocalJoined, this, Event_Local_Joined::GUID );
	EventManager::GetInstance()->AddListener( &Client::RemoteJoined, this, Event_Remote_Joined::GUID );
	EventManager::GetInstance()->AddListener( &Client::RemoteLeft, this, Event_Remote_Left::GUID );
	EventManager::GetInstance()->AddListener( &Client::RemoteUpdate, this, Event_Remote_Update::GUID );
	EventManager::GetInstance()->AddListener( &Client::RemoteDied, this, Event_Remote_Died::GUID );
	EventManager::GetInstance()->AddListener( &Client::RemoteDamaged, this, Event_Remote_Damaged::GUID );
	EventManager::GetInstance()->AddListener( &Client::RemoteSpawned, this, Event_Remote_Spawned::GUID );
	EventManager::GetInstance()->AddListener( &Client::RemoteFiredProjectile, this, Event_Remote_Fired_Projectile::GUID );
	EventManager::GetInstance()->AddListener( &Client::RemoteUpdateHP, this, Event_Remote_Update_HP::GUID );

	EventManager::GetInstance()->AddListener( &Client::StartUp, this, Event_Start_Client::GUID );
	return true;
}

void Client::Release()
{
	if( mSocketManager )
		mSocketManager->Release();
	SAFE_DELETE( mSocketManager );
	SAFE_DELETE( mNEF );
	mRemoteIDs.clear();
	SAFE_DELETE( mInstance );
}