#include "Server.h"

void Server::HandleEvents( IEventPtr evtPtr )
{
	if( evtPtr->GetEventType() == Event_Text::GUID )
	{
		std::shared_ptr<Event_Text> data = std::static_pointer_cast<Event_Text>( evtPtr );
		int socketID = data->Socket();
		std::string text = data->Text();
		std::cout << socketID << " says: " << text << std::endl;
	}
}

void Server::InitEventListening()
{
	// Code for adding events that should be listened to by the server
	EventManager::GetInstance()->AddListener( &Server::HandleEvents, this, Event_Text::GUID );
}

void Server::Update( float deltaTime )
{

}

void Server::DoSelect( int pauseMicroSecs, bool handleInput )
{
	gSocketManager = mSocketManager;
	gSocketManager->DoSelect( pauseMicroSecs, handleInput );
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
	mSocketManager->AddSocket( new ServerListenSocket( mPort ) );
	std::cout << "Server started on port: " << mPort << std::endl;

	InitEventListening();
	return true;
}

void Server::Release()
{
	mSocketManager->Release();
	SAFE_DELETE( mSocketManager );
}

Server::Server() : Network()
{
	mSocketManager = nullptr;
}

Server::~Server()
{
}