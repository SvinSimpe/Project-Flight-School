#include "Server.h"
#include <iostream>

void Server::Broadcast( NetSocket* exception, Packet msg )
{

}

void Server::Send( NetSocket* receiver, Packet pkt )
{

}

void Server::HandleEvents( IEventPtr evtPtr )
{
	if( evtPtr->GetEventType() == Event_Client_Status_Update::GUID )
	{
		mSocketList.clear();
		std::shared_ptr<Event_Client_Status_Update> data = std::static_pointer_cast<Event_Client_Status_Update>( evtPtr );
		mSocketList = data->SocketList();
		std::cout << "Amount of connected clients: " << mSocketList.size() << std::endl;
	}
}

void Server::InitEventListening()
{
	// Code for adding events that should be listened to by the server

	EventManager::GetInstance()->AddListener( &Server::HandleEvents, this, Event_Client_Status_Update::GUID );
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
	mSocketList.clear();
}

Server::Server() : Network()
{
	mSocketManager	= nullptr;
	mSocketList		= std::vector<int>();
}

Server::~Server()
{
}