#include "Game.h"

///////////////////////////////////////////////////////////////////////////////
//									PRIVATE
///////////////////////////////////////////////////////////////////////////////

void Game::NetworkInit()
{
	const char* port	= DEFAULT_PORT;
	const char* ip		= DEFAULT_IP;
	int choice			= 0;

	std::cin >> choice;
	std::cin.ignore();
	if ( choice == 0 )
	{
		if ( Server::GetInstance()->Initialize( port ) )
		{
			if ( Server::GetInstance()->Connect() )
			{
				mServerThread	= std::thread( &Server::Run, Server::GetInstance() );
				mServerIsActive	= true;
			}
		}
	}

	if ( mClient->Initialize( port, ip ) )
	{
		if ( mClient->Connect() )
		{
			mClient->Run();
		}
	}
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
	
	mClient			= new Client();
	mNetworkThread	= std::thread( &Game::NetworkInit, this );
	mServerIsActive = false;

	return S_OK;
}

void Game::Release()
{
	SAFE_DELETE( mStateMachine );
	mClient->Release();
	SAFE_DELETE(mClient);
	if ( mServerThread.joinable() )
	{
		mServerThread.join();
	}
	if ( mNetworkThread.joinable() )
	{
		mNetworkThread.join();
	}
	if ( mServerIsActive )
	{
		Server::GetInstance()->Release();
	}
}

Game::Game()
{
	mStateMachine	= nullptr;
	mNetworkThread	= std::thread();
	mServerThread	= std::thread();
	mClient			= nullptr;
}

Game::~Game()
{
}
