#include "Game.h"

///////////////////////////////////////////////////////////////////////////////
//									PRIVATE
///////////////////////////////////////////////////////////////////////////////

void Game::ServerInit( std::string port )
{
	mServer = new Server();
	if ( mServer->Initialize( port ) )
	{
		if ( mServer->Connect() )
		{
			mServerThread	= std::thread( &Server::Run, mServer );
			mServerIsActive	= true;
		}
	}

	ClientInit( "localhost", port );
}

void Game::ClientInit( std::string ip, std::string port )
{
	if ( mClient->Initialize( ip, port ) )
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
		std::shared_ptr<Event_Start_Server> data = std::static_pointer_cast<Event_Start_Server>( newEvent );

		std::string s_port	= data->Port();

		const char* c_port	 = s_port.c_str();

		mNetworkThread		= std::thread( &Game::ServerInit, this, s_port );
	}
	else if( newEvent->GetEventType() == Event_Start_Client::GUID )
	{
		std::shared_ptr<Event_Start_Client> data = std::static_pointer_cast<Event_Start_Client>( newEvent );

		std::string s_ip	= data->IP();
		std::string s_port	= data->Port();

		const char* c_ip	= s_ip.c_str();
		const char* c_port	= s_port.c_str();

		mNetworkThread		= std::thread( &Game::ClientInit, this, s_ip, s_port );
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
