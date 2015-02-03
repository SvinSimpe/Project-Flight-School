#include "Server.h"

void Server::InitEventListening()
{
	// Code for adding events that should be listened to by the server
}

void Server::InitForwardingEvents()
{
	// Code for adding events that should be forwarded to the network by the server
	EventManager::GetInstance()->AddListener( &NetworkEventForwarder::ForwardEvent, mNEF, Event_Client_Joined::GUID );
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
	return true;
}

void Server::Release()
{
	Network::Release();
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