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
	

	return S_OK;
}

void Game::Release()
{
	delete mStateMachine;
}

Game::Game()
{
	mStateMachine	= nullptr;
}

Game::~Game()
{
}
