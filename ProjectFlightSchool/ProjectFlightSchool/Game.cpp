#include "Game.h"

///////////////////////////////////////////////////////////////////////////////
//									PRIVATE
///////////////////////////////////////////////////////////////////////////////

void Game::ServerInit()
{
	const char* port	= DEFAULT_PORT;
	
	mServer = new Server();
	if ( mServer->Initialize( port ) )
	{
		if ( mServer->Connect() )
		{
			mServerThread	= std::thread( &Server::Run, mServer );
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
	EventManager::GetInstance()->AddListener( &Game::EventListener, this, Event_Start_Server::GUID );
	EventManager::GetInstance()->AddListener( &Game::EventListener, this, Event_Start_Client::GUID );
	mServerIsActive = false;

	return S_OK;
}

void Game::Release()
{
	EventManager::GetInstance()->Release();

	mClient->Release();
	SAFE_DELETE( mClient );

	if ( mServerIsActive )
		mServer->Release();
	SAFE_DELETE( mServer );

	if ( mServerThread.joinable() )
	{
		mServerThread.join();
	}
	if ( mNetworkThread.joinable() )
	{
		mNetworkThread.join();
	}

	mStateMachine->Release();
	SAFE_DELETE( mStateMachine );
}

Game::Game()
{
	mStateMachine	= nullptr;
	mNetworkThread	= std::thread();
	mServerThread	= std::thread();
	mClient			= nullptr;
	mServer			= nullptr;
}

Game::~Game()
{
}
