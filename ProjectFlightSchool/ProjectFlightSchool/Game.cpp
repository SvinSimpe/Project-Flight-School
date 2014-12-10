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
	cin.ignore();
	if ( choice == 0 )
	{
		if ( Server::GetInstance()->Initialize( port ) )
		{
			if ( Server::GetInstance()->Connect() )
			{
				mServerThread = std::thread( &Server::Run, Server::GetInstance() );
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
	
	mClient = new Client();
	mNetworkThread = std::thread( &Game::NetworkInit, this );

	return S_OK;
}

void Game::Release()
{
	delete mStateMachine;

	mServerThread.join();
	mNetworkThread.join();
	delete mClient;
	Server::GetInstance()->Release();
}

Game::Game()
{
	mStateMachine	= nullptr;
	mClient			= nullptr;
	mNetworkThread	= std::thread();
	mServerThread	= std::thread();
}

Game::~Game()
{
}
