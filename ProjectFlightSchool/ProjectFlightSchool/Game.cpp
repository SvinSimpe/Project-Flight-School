#include "Game.h"

///////////////////////////////////////////////////////////////////////////////
//									PRIVATE
///////////////////////////////////////////////////////////////////////////////

void Game::ServerInit()
{
	const char* port	= DEFAULT_PORT;

	if ( Server::GetInstance()->Initialize( port ) )
	{
		if ( Server::GetInstance()->Connect() )
		{
			mServerThread	= std::thread( &Server::Run, Server::GetInstance() );
			mServerIsActive	= true;
		}
	}

	ClientInit();
}

void Game::ClientInit()
{
	const char* port	= DEFAULT_PORT;
	const char* ip		= DEFAULT_IP;

	if ( mClient->Initialize( port, ip ) )
	{
		if ( mClient->Connect() )
		{
			mClient->Run();
		}
	}
}

void Game::EventListener( IEventPtr newEvent )
{
	if( newEvent->GetEventType() == Event_Start_Server::GUID )
	{
		mNetworkThread	= std::thread( &Game::ServerInit, this );
	}
	else if( newEvent->GetEventType() == Event_Start_Client::GUID )
	{
		mNetworkThread	= std::thread( &Game::ClientInit, this );
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
	
	mClient			= new Client();
	EventManager::GetInstance()->AddListener( &Game::EventListener, this, Event_Start_Server::GUID );
	EventManager::GetInstance()->AddListener( &Game::EventListener, this, Event_Start_Client::GUID );
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
