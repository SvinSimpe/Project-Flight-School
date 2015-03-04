#include "Game.h"

///////////////////////////////////////////////////////////////////////////////
//									PRIVATE
///////////////////////////////////////////////////////////////////////////////

void Game::ResetGame( IEventPtr eventPtr )
{
	if( eventPtr->GetEventType() == Event_Reset_Game::GUID )
	{
		IEventPtr E1( new Event_Shutdown_Client() );
		EventManager::GetInstance()->TriggerEvent( E1 );

		if( mServer )
		{
			mServer->BroadcastEvent( E1 );
			mServer->Reset();
		}
	}
}

void Game::StartPlayState( IEventPtr eventPtr )
{
	if( eventPtr->GetEventType() == Event_Connect_Client_Success::GUID )
	{
		IEventPtr E1( new Event_Change_State( LOBBY_STATE ) );
		EventManager::GetInstance()->TriggerEvent( E1 );
	}
}

void Game::StartServer( IEventPtr eventPtr )
{
	if( eventPtr->GetEventType() == Event_Game_Start_Server::GUID )
	{
		std::shared_ptr<Event_Game_Start_Server> data = std::static_pointer_cast<Event_Game_Start_Server>( eventPtr );
		if( mServer )
		{
			IEventPtr reset( new Event_Reset_Game() );
			EventManager::GetInstance()->TriggerEvent( reset );
		}
		else
		{
			mServer = new Server();
			mServer->Initialize();
		}

		IEventPtr E1( new Event_Start_Server( data->Port(), data->MaxPlayers() ) );
		EventManager::GetInstance()->QueueEvent( E1 );
	}
}

///////////////////////////////////////////////////////////////////////////////
//									PUBLIC
///////////////////////////////////////////////////////////////////////////////

HRESULT Game::Update( float deltaTime )
{
	Client::GetInstance()->Update( deltaTime );
	if( mServer )
		mServer->Update( deltaTime );

	Client::GetInstance()->DoSelect( 0 );
	if( mServer )
		mServer->DoSelect( 0 );
	EventManager::GetInstance()->Update();

	Client::GetInstance()->DoSelect( 0 );
	if( mServer )
		mServer->DoSelect( 0 );
	RenderManager::GetInstance()->Update( deltaTime );
	
	Client::GetInstance()->DoSelect( 0 );
	if( mServer )
		mServer->DoSelect( 0 );
	mStateMachine->Update( deltaTime );

	Client::GetInstance()->DoSelect( 0 );
	if( mServer )
		mServer->DoSelect( 0 );
	Client::GetInstance()->DoSelect( 0 );
	return S_OK;
}

HRESULT Game::Render( float deltaTime )
{
	mStateMachine->Render( deltaTime );

	return S_OK;
}

HRESULT Game::Initialize()
{
	mStateMachine	= new StateMachine();
	mStateMachine->Initialize();

	OutputDebugString( L"----- Game Initialization Complete. -----\n" );

	EventManager::GetInstance()->AddListener( &Game::ResetGame, this, Event_Reset_Game::GUID );
	EventManager::GetInstance()->AddListener( &Game::StartPlayState, this, Event_Connect_Client_Success::GUID );
	EventManager::GetInstance()->AddListener( &Game::StartServer, this, Event_Game_Start_Server::GUID );

	return S_OK;
}

void Game::Release()
{
	IEventPtr E1( new Event_Reset_Game() );
	EventManager::GetInstance()->TriggerEvent( E1 );

	mStateMachine->Release();
	SAFE_DELETE( mStateMachine );
	if( mServer )
		mServer->Release();
	SAFE_DELETE( mServer );
}

Game::Game()
{
	mStateMachine	= nullptr;
	mServer			= nullptr;
}

Game::~Game()
{
}
