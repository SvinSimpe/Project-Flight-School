#include "Game.h"

///////////////////////////////////////////////////////////////////////////////
//									PRIVATE
///////////////////////////////////////////////////////////////////////////////


void Game::EventListener( IEventPtr newEvent )
{
}

///////////////////////////////////////////////////////////////////////////////
//									PUBLIC
///////////////////////////////////////////////////////////////////////////////

HRESULT Game::Update( float deltaTime )
{
	mStateMachine->Update( deltaTime );
	EventManager::GetInstance()->Update();
	RenderManager::GetInstance()->Update( deltaTime );


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

	EventManager::GetInstance()->AddListener( &Game::EventListener, this, Event_Start_Server::GUID );
	EventManager::GetInstance()->AddListener( &Game::EventListener, this, Event_Start_Client::GUID );

	OutputDebugString( L"----- Game Initialization Complete. -----" );

	return S_OK;
}

void Game::Release()
{
	mStateMachine->Release();
	SAFE_DELETE( mStateMachine );
}

Game::Game()
{
	mStateMachine		= nullptr;
}

Game::~Game()
{
	printf("Destructor for %s\n", __FILE__);
}
