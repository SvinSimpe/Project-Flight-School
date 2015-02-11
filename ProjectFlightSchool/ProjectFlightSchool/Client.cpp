#include "Client.h"

Client* Client::mInstance = nullptr;

Client::Client() : Network()
{
	mSocketManager			= nullptr;
	mNEF					= nullptr;
	mEventList				= std::list<IEventPtr>();
	mActive					= false;
}

Client::~Client()
{
//<<<<<<< HEAD
//	if ( newEvent->GetEventType() == Event_Player_Update::GUID )
//	{
//		std::shared_ptr<Event_Player_Update> data = std::static_pointer_cast<Event_Player_Update>( newEvent );
//		if ( mServerSocket != INVALID_SOCKET )
//		{
//			EvPlayerUpdate msg;
//			msg.id						= mID;
//			msg.lowerBodyPosition		= data->LowerBodyPos();
//			msg.velocity				= data->Velocity();
//			msg.upperBodyDirection		= data->UpperBodyDirection();
//			msg.isAlive					= data->IsAlive();
//			msg.playerName				= data->Name();
//			
//			if ( mServerSocket != INVALID_SOCKET )
//			{
//				mConn->SendPkg( mServerSocket, 0, Net_Event::EV_PLAYER_UPDATE, msg );
//			}
//		}
//	}
//	else if ( newEvent->GetEventType() == Event_Player_Died::GUID )
//	{
//		std::shared_ptr<Event_Player_Died> data = std::static_pointer_cast<Event_Player_Died>( newEvent );
//		if ( mServerSocket != INVALID_SOCKET )
//		{
//			EvKilled msg;
//			msg.ID			= data->ID();
//			msg.killerID	= data->KillerID();
//
//			if ( mServerSocket != INVALID_SOCKET )
//			{
//				mConn->SendPkg( mServerSocket, 0, Net_Event::EV_PLAYER_DIED, msg );
//			}
//		}
//	}
//	else if ( newEvent->GetEventType() == Event_Player_Damaged::GUID )
//	{
//		std::shared_ptr<Event_Player_Damaged> data = std::static_pointer_cast<Event_Player_Damaged>( newEvent );
//		if ( mServerSocket != INVALID_SOCKET )
//		{
//			EvPlayerID msg;
//			msg.ID = data->ID();
//			msg.projectileID = data->ProjectileID();
//
//			if ( mServerSocket != INVALID_SOCKET )
//			{
//				mConn->SendPkg( mServerSocket, 0, Net_Event::EV_PLAYER_DAMAGED, msg );
//			}
//		}
//	}
//	else if ( newEvent->GetEventType() == Event_Player_Spawned::GUID )
//	{
//		std::shared_ptr<Event_Player_Spawned> data = std::static_pointer_cast<Event_Player_Spawned>( newEvent );
//		if ( mServerSocket != INVALID_SOCKET )
//		{
//			EvPlayerID msg;
//			msg.ID = data->ID();
//
//			if ( mServerSocket != INVALID_SOCKET )
//			{
//				mConn->SendPkg( mServerSocket, 0, Net_Event::EV_PLAYER_SPAWNED, msg );
//			}
//		}
//	}
//	else if ( newEvent->GetEventType() == Event_Projectile_Fired::GUID )
//	{
//		std::shared_ptr<Event_Projectile_Fired> data = std::static_pointer_cast<Event_Projectile_Fired>( newEvent );
//		if ( mServerSocket != INVALID_SOCKET )
//		{
//			EvProjectileFired msg;
//			msg.ID			= mID;
//			msg.position	= data->BodyPos();
//			msg.direction	= data->Direction();
//			msg.speed		= data->Speed();
//			msg.range		= data->Range();
//
//			if ( mServerSocket != INVALID_SOCKET )
//			{
//				mConn->SendPkg( mServerSocket, 0, Net_Event::EV_PROJECTILE_FIRED, msg );
//			}
//		}
//	}
//	else if ( newEvent->GetEventType() == Event_Player_Update_HP::GUID )
//	{
//		std::shared_ptr<Event_Player_Update_HP> data = std::static_pointer_cast<Event_Player_Update_HP>( newEvent );
//		if ( mServerSocket != INVALID_SOCKET )
//		{
//			EvPlayerID msg;
//			msg.ID	= mID;
//			msg.HP	= (unsigned int)data->HP();
//
//			if ( mServerSocket != INVALID_SOCKET )
//			{
//				mConn->SendPkg( mServerSocket, 0, Net_Event::EV_UPDATE_HP, msg );
//			}
//		}
//	}
//	else if ( newEvent->GetEventType() == Event_Player_Melee_Hit::GUID )
//	{
//		std::shared_ptr<Event_Player_Melee_Hit> data = std::static_pointer_cast<Event_Player_Melee_Hit>( newEvent );
//		if ( mServerSocket != INVALID_SOCKET )
//		{
//			EvMeleeHit msg;
//			msg.ID			= data->ID();
//			msg.damage		= data->Damage();
//			msg.knockBack	= data->KnockBack();
//			msg.direction	= data->Direction();
//			
//
//			if ( mServerSocket != INVALID_SOCKET )
//			{
//				mConn->SendPkg( mServerSocket, 0, Net_Event::EV_MELEE_HIT, msg );
//			}
//		}
//	}
//	else if ( newEvent->GetEventType() == Event_Player_Attack::GUID )
//	{
//		std::shared_ptr<Event_Player_Attack> data = std::static_pointer_cast<Event_Player_Attack>( newEvent );
//		if ( mServerSocket != INVALID_SOCKET )
//		{
//			EvPlayerAttack msg;
//			msg.ID			= mID;
//			msg.armID		= data->ArmID();
//			msg.animation	= data->Animation();		
//
//			if ( mServerSocket != INVALID_SOCKET )
//			{
//				mConn->SendPkg( mServerSocket, 0, Net_Event::EV_PLAYER_ATTACK, msg );
//			}
//		}
//	}
//	else if ( newEvent->GetEventType() == Event_Projectile_Damage_Enemy::GUID )
//	{
//		std::shared_ptr<Event_Projectile_Damage_Enemy> data = std::static_pointer_cast<Event_Projectile_Damage_Enemy>( newEvent );
//		if ( mServerSocket != INVALID_SOCKET )
//		{
//			EvEnemyProjectileDamage msg;
//			msg.shooterID		= data->Shooter();	
//			msg.projectileID	= data->Projectile();
//			msg.enemyID			= data->Enemy();
//			msg.damage			= data->Damage();
//
//			if ( mServerSocket != INVALID_SOCKET )
//			{
//				mConn->SendPkg( mServerSocket, 0, Net_Event::EV_ENEMY_PROJECTILE_DAMGAE, msg );
//			}
//		}
//	}
//	else if ( newEvent->GetEventType() == Event_Enemy_Melee_Hit::GUID )
//	{
//		std::shared_ptr<Event_Enemy_Melee_Hit> data = std::static_pointer_cast<Event_Enemy_Melee_Hit>( newEvent );
//		if ( mServerSocket != INVALID_SOCKET )
//		{
//			EvEnemyMeleeHit msg;
//			msg.ID			= data->Enemy();
//			msg.damage		= data->Damage();
//			msg.knockBack	= data->KnockBack();
//			msg.direction	= data->Direction();
//			msg.stun		= data->Stun();
//			
//
//			if ( mServerSocket != INVALID_SOCKET )
//			{
//				mConn->SendPkg( mServerSocket, 0, Net_Event::EV_ENEMY_MELEE_HIT, msg );
//			}
//		}
//	}
//	else if ( newEvent->GetEventType() == Event_Player_Down::GUID )
//	{
//		std::shared_ptr<Event_Player_Down> data = std::static_pointer_cast<Event_Player_Down>( newEvent );
//		if ( mServerSocket != INVALID_SOCKET )
//		{
//			EvPlayerID msg;
//			msg.ID = data->Player();
//=======
}

void Client::StartUp( IEventPtr eventPtr )
{
	if( eventPtr->GetEventType() == Event_Start_Client::GUID )
	{
		std::shared_ptr<Event_Start_Client> data = std::static_pointer_cast<Event_Start_Client>( eventPtr );
		std::string IP	= data->IP();
		UINT port		= data->Port();
		if( !Connect( IP, port ) )
			std::cout << "Failed to connect client!" << std::endl;
		else
			mActive = true;
	}
}

void Client::SendEvent( IEventPtr eventPtr )
{
	mNEF->ForwardEvent( eventPtr );
}

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

void Client::QueueEvent( IEventPtr ptr )
{
	mEventList.push_front( IEventPtr( ptr ) );
}

void Client::PopEvent()
{
	mEventList.pop_back();
}

void Client::Update( float deltaTime )
{
	if( mActive )
	{
		while( !mEventList.empty() )
		{
			SendEvent( mEventList.back() );
			PopEvent();
		}
	}
}

void Client::DoSelect( int pauseMicroSecs, bool handleInput )
{
	if( mActive )
	{
		mSocketManager->DoSelect( pauseMicroSecs, handleInput );
	}
}

bool Client::Initialize()
{
	// REGISTER_EVENT should only be run once for each event
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
	EF::REGISTER_EVENT( Event_Server_Create_Enemy );
	EF::REGISTER_EVENT( Event_Server_Enemies_Created );
	EF::REGISTER_EVENT( Event_Client_Melee_Hit );
	EF::REGISTER_EVENT( Event_Remote_Melee_Hit );
	EF::REGISTER_EVENT( Event_Client_Attack );
	EF::REGISTER_EVENT( Event_Remote_Attack );
	EF::REGISTER_EVENT( Event_Server_Sync_Spawn );
	EF::REGISTER_EVENT( Event_Add_Point_Light );
	EF::REGISTER_EVENT( Event_Remove_Point_Light );
	EF::REGISTER_EVENT( Event_Server_Update_Enemy );

	EF::REGISTER_EVENT( Event_Server_Sync_Enemy_State ); //
	EF::REGISTER_EVENT( Event_Set_Enemy_State ); //
	EF::REGISTER_EVENT( Event_Remote_Set_Enemy_State ); // 

	EF::REGISTER_EVENT( Event_Client_Projectile_Damage_Enemy ); // 
	EF::REGISTER_EVENT( Event_Server_Enemy_Died ); // 
	EF::REGISTER_EVENT( Event_Server_Enemy_Attack_Player ); //
	
	EF::REGISTER_EVENT( Event_Client_Down );
	EF::REGISTER_EVENT( Event_Remote_Down );
	EF::REGISTER_EVENT( Event_Client_Up );
	EF::REGISTER_EVENT( Event_Remote_Up );
	EF::REGISTER_EVENT( Event_Client_Attempt_Revive );
	EF::REGISTER_EVENT( Event_Remote_Attempt_Revive );
	EF::REGISTER_EVENT( Event_Initialize_Success );
	EF::REGISTER_EVENT( Event_Initialize_Fail );
	EF::REGISTER_EVENT( Event_Load_Level );
	EF::REGISTER_EVENT( Event_Create_Player_Name );

	EF::REGISTER_EVENT( Event_Client_Enemy_Attack );
	EF::REGISTER_EVENT( Event_Remote_Enemy_Attack );

	EventManager::GetInstance()->AddListener( &Client::StartUp, this, Event_Start_Client::GUID );
	return true;
}

void Client::Release()
{
	if( mSocketManager )
		mSocketManager->Release();
	SAFE_DELETE( mSocketManager );
	SAFE_DELETE( mNEF );
	mEventList.clear();
	SAFE_DELETE( mInstance );
}