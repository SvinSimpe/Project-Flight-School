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

void Game::ReachEvents( IEventPtr eventPtr )
{
	if( eventPtr->GetEventType() == Event_Server_Reach_Client::GUID )
	{
		//OutputDebugStringA("Server reached the client.\n");
	}
	else if( eventPtr->GetEventType() == Event_Client_Reach_Server::GUID )
	{
		//OutputDebugStringA("Client reached the server.\n");
	}
}

///////////////////////////////////////////////////////////////////////////////
//									PUBLIC
///////////////////////////////////////////////////////////////////////////////

HRESULT Game::Update( float deltaTime )
{
	Client::GetInstance()->Update( deltaTime );
	mServer->Update( deltaTime );

	Client::GetInstance()->DoSelect( 0 );
	mServer->DoSelect( 0 );
	EventManager::GetInstance()->Update();

	Client::GetInstance()->DoSelect( 0 );
	mServer->DoSelect( 0 );
	RenderManager::GetInstance()->Update( deltaTime );
	
	Client::GetInstance()->DoSelect( 0 );
	mServer->DoSelect( 0 );
	mStateMachine->Update( deltaTime );

	Client::GetInstance()->DoSelect( 0 );
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
	mServer = new Server();
	mServer->Initialize();

	OutputDebugString( L"----- Game Initialization Complete. -----\n" );

	EventManager::GetInstance()->AddListener( &Game::ResetGame, this, Event_Reset_Game::GUID );
	EventManager::GetInstance()->AddListener( &Game::StartPlayState, this, Event_Connect_Client_Success::GUID );
	EventManager::GetInstance()->AddListener( &Game::ReachEvents, this, Event_Server_Reach_Client::GUID );
	EventManager::GetInstance()->AddListener( &Game::ReachEvents, this, Event_Client_Reach_Server::GUID );

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
	mStateMachine		= nullptr;
}

Game::~Game()
{
}
