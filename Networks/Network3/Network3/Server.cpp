#include "Server.h"

void Server::ClientJoined( IEventPtr eventPtr )
{
	if( eventPtr->GetEventType() == Event_Client_Joined::GUID )
	{
		std::shared_ptr<Event_Client_Joined> data = std::static_pointer_cast<Event_Client_Joined>( eventPtr );
		UINT id = data->ID();
		mForwardMap[id].Initialize( id, mSocketManager );

		std::cout << "Client with ID: " << id << " joined. There are now " << mForwardMap.size() << " clients online." << std::endl;

		IEventPtr E1( PFS_NEW Event_Local_Joined( id ) );
		mForwardMap[id].ForwardEvent( E1 );
	}
}

void Server::ClientLeft( IEventPtr eventPtr )
{
	if( eventPtr->GetEventType() == Event_Client_Left::GUID )
	{
		std::shared_ptr<Event_Client_Left> data = std::static_pointer_cast<Event_Client_Left>( eventPtr );
		UINT id = data->ID();
		mForwardMap.erase( id );
		std::cout << "Client with ID: " << id << " left. There are now " << mForwardMap.size() << " client(s) online." << std::endl;
	}
}

/* This will register all the events that only the server will listen to,
	such as clients joining/disconnecting. */
void Server::InitEventListening()
{
	EventManager::GetInstance()->AddListener( &Server::ClientJoined, this, Event_Client_Joined::GUID );
	EventManager::GetInstance()->AddListener( &Server::ClientLeft, this, Event_Client_Left::GUID );
}

/* This will register all the events that needs to be updated to all the clients.
	These events will be local server events such as updating enemies and such. */
void Server::RemoveEventForwarding( NetworkEventForwarder* nef )
{

}

void Server::Update( float deltaTime )
{
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

	InitEventListening();
	return true;
}

void Server::Release()
{
	mSocketManager->Release();
	SAFE_DELETE( mSocketManager );
	mForwardMap.clear();
}

Server::Server() : Network()
{
	mSocketManager = nullptr;
	mForwardMap = std::map<UINT, NetworkEventForwarder>();
}

Server::~Server()
{
}