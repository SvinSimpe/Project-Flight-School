#include "Server.h"

void Server::HandleEvents( IEventPtr evtPtr )
{
	if( evtPtr->GetEventType() == Event_Text::GUID )
	{
		std::shared_ptr<Event_Text> data = std::static_pointer_cast<Event_Text>( evtPtr );
		int socketID = data->SocketID();
		std::string text = data->Text();
		std::cout << socketID << " says: " << text << std::endl;
	}
	else if( evtPtr->GetEventType() == Event_Client_Amount_Update::GUID )
	{
		std::shared_ptr<Event_Client_Amount_Update> data = std::static_pointer_cast<Event_Client_Amount_Update>( evtPtr );
		mSocketIDs = data->SocketIDs();

		std::cout << "Number of connected clients: " << mSocketIDs.size() << std::endl;
		//mActive = true;
	}
}

void Server::InitEventListening()
{
	EventManager::GetInstance()->AddListener( &Server::HandleEvents, this, Event_Text::GUID );
	EventManager::GetInstance()->AddListener( &Server::HandleEvents, this, Event_Client_Amount_Update::GUID );
}

// Idea: The server has a list of network event forwarders that takes care of one socket each
// in order to distribute events between clients.
// This list is updated with the Event_Client_Amount_Update.

void Server::InitEventForwarding()
{
	EventManager::GetInstance()->AddListener( &NetworkEventForwarder::ForwardEvent, &mNEF, Event_Client_Joined::GUID );
	EventManager::GetInstance()->AddListener( &NetworkEventForwarder::ForwardEvent, &mNEF, Event_Text::GUID );
}

void Server::Update( float deltaTime )
{
	if( mActive )
	{
		IEventPtr E1( PFS_NEW Event_Text( 0, "Hello_Client!" ) );
		EventManager::GetInstance()->QueueEvent( E1 );
	}
}

void Server::DoSelect( int pauseMicroSecs, bool handleInput )
{
	mSocketManager->DoSelect( pauseMicroSecs, handleInput );
}

bool Server::Initialize( UINT port )
{
	Network::Initialize( port );
	mSocketManager = new SocketManager();
	if( !mSocketManager->Initialize() )
	{
		OutputDebugStringA( "Failed to initialize server.\n" );
		return false;
	}
	mSocketManager->AddSocket( new ServerListenSocket( mSocketManager, mPort ) );
	std::cout << "Server started on port: " << mPort << std::endl;

	mNEF.Initialize( 1, mSocketManager );

	InitEventListening();
	InitEventForwarding();
	return true;
}

void Server::Release()
{
	mSocketManager->Release();
	SAFE_DELETE( mSocketManager );
	mSocketIDs.clear();
}

Server::Server() : Network()
{
	mSocketManager = nullptr;
	mSocketIDs = std::list<UINT>();
	mActive = false;
}

Server::~Server()
{
}