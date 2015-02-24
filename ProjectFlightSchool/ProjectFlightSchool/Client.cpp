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
}

void Client::StartUp( IEventPtr eventPtr )
{
	if( eventPtr->GetEventType() == Event_Start_Client::GUID )
	{
		std::shared_ptr<Event_Start_Client> data = std::static_pointer_cast<Event_Start_Client>( eventPtr );
		std::string IP		= data->IP();
		std::string port	= data->Port();

		std::stringstream sstr;
		sstr << port << " ";
		UINT iPort;
		sstr >> iPort;

		if( Connect( IP, iPort ) )
		{
			mActive = true;
			IEventPtr E1( new Event_Connect_Client_Success() );
			EventManager::GetInstance()->QueueEvent( E1 );
		}
		else
		{
			IEventPtr E1( new Event_Connect_Client_Fail( "Client failed at connecting to server!" ) );
			EventManager::GetInstance()->QueueEvent( E1 );
			if( mSocketManager )
				mSocketManager->Release();
			SAFE_DELETE( mSocketManager );
			SAFE_DELETE( mNEF );
			mEventList.clear();
			mActive = false;
		}
	}
}

void Client::SendEvent( IEventPtr eventPtr )
{
	mNEF->ForwardEvent( eventPtr );
	//mEventList.push_front( eventPtr );
}

bool Client::Connect( std::string ip, UINT port )
{
	mSocketManager = new ClientSocketManager();
	if( !mSocketManager->Connect( ip, port ) )
	{
		return false;
	}

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

void Client::Shutdown( IEventPtr eventPtr )
{
	if( eventPtr->GetEventType() == Event_Shutdown_Client::GUID && mActive)
	{
		if( mSocketManager )
			mSocketManager->Release();
		SAFE_DELETE( mSocketManager );
		SAFE_DELETE( mNEF );
		mEventList.clear();
		mActive = false;

		IEventPtr E1( new Event_Change_State( 0 ) ); // The state definitions should be moved to some global place!
		EventManager::GetInstance()->QueueEvent( E1 );
	}
}

void Client::DoSelect( int pauseMicroSecs, bool handleInput )
{
	if( this && mActive )
		mSocketManager->DoSelect( pauseMicroSecs, handleInput );
}

void Client::Update( float deltaTime )
{
	if( this && mActive )
	{
		while( !mEventList.empty() )
		{
			mNEF->ForwardEvent( mEventList.back() );
			mEventList.pop_back();
		}
		mEventList.clear();
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
	EF::REGISTER_EVENT( Event_Load_Level );
	EF::REGISTER_EVENT( Event_Create_Player_Name );

	EF::REGISTER_EVENT( Event_Client_Enemy_Attack );
	EF::REGISTER_EVENT( Event_Remote_Enemy_Attack );

	EF::REGISTER_EVENT( Event_Shutdown_Server );
	EF::REGISTER_EVENT( Event_Shutdown_Client );
	EF::REGISTER_EVENT( Event_Reset_Game );

	EF::REGISTER_EVENT( Event_Connect_Server_Success );
	EF::REGISTER_EVENT( Event_Connect_Client_Success );
	EF::REGISTER_EVENT( Event_Connect_Server_Fail );
	EF::REGISTER_EVENT( Event_Connect_Client_Fail );
	EF::REGISTER_EVENT( Event_Server_Spawn_Ship );
	EF::REGISTER_EVENT( Event_Server_Change_Buff_State );

	EF::REGISTER_EVENT( Event_Client_Sync_Energy_Cell );
	EF::REGISTER_EVENT( Event_Server_Sync_Energy_Cell );

	EF::REGISTER_EVENT( Event_Client_Update_Ship );
	EF::REGISTER_EVENT( Event_Server_Update_Ship );
	EF::REGISTER_EVENT( Event_Client_Change_Ship_Levels );
	EF::REGISTER_EVENT( Event_Server_Change_Ship_Levels );
	EF::REGISTER_EVENT( Event_Client_Win );
	EF::REGISTER_EVENT( Event_Remote_Win );
	EF::REGISTER_EVENT( Event_Server_XP );
	EF::REGISTER_EVENT( Event_Server_Update_Turret );
	EF::REGISTER_EVENT( Event_Turret_Fired_Projectile );
	EF::REGISTER_EVENT( Event_Server_Turret_Fired_Projectile );
	EF::REGISTER_EVENT( Event_Client_Initialize_LobbyPlayer );
	EF::REGISTER_EVENT( Event_Server_Initialize_LobbyPlayer );
	EF::REGISTER_EVENT( Event_Client_Lobby_Finished );
	EF::REGISTER_EVENT( Event_Server_Lobby_Finished );
	EF::REGISTER_EVENT( Event_Client_Switch_Team );
	EF::REGISTER_EVENT( Event_Server_Switch_Team );

	EventManager::GetInstance()->AddListener( &Client::StartUp, this, Event_Start_Client::GUID );
	EventManager::GetInstance()->AddListener( &Client::Shutdown, this, Event_Shutdown_Client::GUID );

	return true;
}

void Client::Release()
{
	if( mSocketManager )
		mSocketManager->Release();
	SAFE_DELETE( mSocketManager );
	SAFE_DELETE( mNEF );
	mEventList.clear();
	mActive = false;
	SAFE_DELETE( mInstance );
}