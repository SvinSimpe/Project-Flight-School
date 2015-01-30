#include "Game.h"

///////////////////////////////////////////////////////////////////////////////
//									PRIVATE
///////////////////////////////////////////////////////////////////////////////

void Game::ServerInit( std::string port )
{
	if( !mServerIsActive )
	{
		mServer = new Server();
		if ( mServer->Initialize( port ) && mServer->Connect() )
		{
			mServerThread	= std::thread( &Server::Run, mServer );
			mServerIsActive	= true;
		}
		else
		{
			IEventPtr E1( new Event_Connection_Failed( "EVENT: Unable to start server." ) );
			EventManager::GetInstance()->QueueEvent( E1 );
			mServer->Release();
			SAFE_DELETE( mServer );
		}
	}
	ClientInit( "localhost", port );
}

void Game::ClientInit( std::string ip, std::string port )
{
	mClient = new Client();
	if ( mClient->Initialize( ip, port ) &&  mClient->Connect() )
	{
		IEventPtr E1( new Event_Change_State( PLAY_STATE ) );
		EventManager::GetInstance()->QueueEvent( E1 );
		mClientThread = std::thread( &Client::Run, mClient );
	}
	else if( mServerIsActive )
	{
		IEventPtr E1( new Event_Connection_Failed( "EVENT: Unable to start client even though server is running." ) );
		EventManager::GetInstance()->QueueEvent( E1 );

		mServer->Release();
		SAFE_DELETE( mServer );
		mServerThread.join();
		mServerIsActive = false;

		mClient->Release();
		SAFE_DELETE( mClient );
	}
	else
	{
		IEventPtr E1( new Event_Connection_Failed( "EVENT: Unable to start client." ) );
		EventManager::GetInstance()->QueueEvent( E1 );
		mClient->Release();
		SAFE_DELETE( mClient );
		if( mClientThread.joinable() )
			mClientThread.join();
	}
}

void Game::EventListener( IEventPtr newEvent )
{
	if( newEvent->GetEventType() == Event_Start_Server::GUID )
	{
		std::shared_ptr<Event_Start_Server> data = std::static_pointer_cast<Event_Start_Server>( newEvent );
		std::string s_port	= data->Port();

		ServerInit( s_port );
	}
	else if( newEvent->GetEventType() == Event_Start_Client::GUID )
	{
		std::shared_ptr<Event_Start_Client> data = std::static_pointer_cast<Event_Start_Client>( newEvent );
		std::string s_ip	= data->IP();
		std::string s_port	= data->Port();

		ClientInit( s_ip, s_port );
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

	// Update enemies on server
	if( mServerIsActive )
	{
		mServer->Update( deltaTime );
	}

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
	mServerIsActive = false;

	OutputDebugString( L"----- Game Initialization Complete. -----" );

	return S_OK;
}

void Game::Release()
{
	if ( mServerIsActive && mServer )
		mServer->Release();
	SAFE_DELETE( mServer );
	if ( mServerThread.joinable() )
	{
		mServerThread.join();
	}

	if( mClient )
		mClient->Release();
	SAFE_DELETE( mClient );
	if ( mClientThread.joinable() )
	{
		mClientThread.join();
	}

	mStateMachine->Release();
	SAFE_DELETE( mStateMachine );
}

Game::Game()
{
	mStateMachine		= nullptr;
	mClientThread		= std::thread();
	mServerThread		= std::thread();
	mClient				= nullptr;
	mServer				= nullptr;
}

Game::~Game()
{
	printf("Destructor for %s\n", __FILE__);
}
