#include "Game.h"

///////////////////////////////////////////////////////////////////////////////
//									PRIVATE
///////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
//									PUBLIC
///////////////////////////////////////////////////////////////////////////////

HRESULT Game::Update( float deltaTime )
{
	mStateMachine->Update( deltaTime );
	EventManager::GetInstance()->Update();
	RenderManager::GetInstance()->Update( deltaTime );

	mServer->Update( deltaTime );
	mServer->DoSelect( 0 );

	Client::GetInstance()->Update( deltaTime );
	Client::GetInstance()->DoSelect( 0 );


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

	return S_OK;
}

void Game::Release()
{
	mStateMachine->Release();
	SAFE_DELETE( mStateMachine );
	mServer->Release();
	SAFE_DELETE( mServer );
}

Game::Game()
{
	mStateMachine		= nullptr;
}

Game::~Game()
{
	printf("Destructor for %s\n", __FILE__);
}
