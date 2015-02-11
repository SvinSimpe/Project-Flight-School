#include "Game.h"

///////////////////////////////////////////////////////////////////////////////
//									PRIVATE
///////////////////////////////////////////////////////////////////////////////

void Game::ResetGame( IEventPtr eventPtr )
{
	if( eventPtr->GetEventType() == Event_Reset_Game::GUID )
	{
		IEventPtr E1( new Event_Shutdown_Client() );
		EventManager::GetInstance()->QueueEvent( E1 );

		if( mServer )
		{
			mServer->BroadcastEvent( E1 );
			mServer->Release();
		}
	}
}

void Game::StartPlayState( IEventPtr eventPtr )
{
	if( eventPtr->GetEventType() == Event_Connect_Client_Success::GUID )
	{
		IEventPtr E1( new Event_Change_State( PLAY_STATE ) );
		EventManager::GetInstance()->QueueEvent( E1 );
	}
}

///////////////////////////////////////////////////////////////////////////////
//									PUBLIC
///////////////////////////////////////////////////////////////////////////////

HRESULT Game::Update( float deltaTime )
{
	EventManager::GetInstance()->Update();
	RenderManager::GetInstance()->Update( deltaTime );

	Client::GetInstance()->Update( deltaTime );
	Client::GetInstance()->DoSelect( 0 );

	mServer->Update( deltaTime );
	mServer->DoSelect( 0 );

	mStateMachine->Update( deltaTime );

	return S_OK;
}

HRESULT Game::Render()
{
	mStateMachine->Render();

	return S_OK;
}

HRESULT Game::Initialize()
{
	mStateMachine	= new StateMachine();
	mStateMachine->Initialize();
	mServer = new Server();
	mServer->Initialize();

	OutputDebugString( L"----- Game Initialization Complete. -----" );

	EventManager::GetInstance()->AddListener( &Game::ResetGame, this, Event_Reset_Game::GUID );
	EventManager::GetInstance()->AddListener( &Game::StartPlayState, this, Event_Connect_Client_Success::GUID );

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
