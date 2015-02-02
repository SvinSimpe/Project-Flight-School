#include "Server.h"

SocketManager* Server::mSocketManager = nullptr;

void Server::InitForwardingEvents()
{
	// Code for adding events that should be forwarded to the network by the server
	EventManager::GetInstance()->AddListener( &NetworkEventForwarder::ForwardEvent, mNEF, Event_Text::GUID );
}

bool Server::Initialize( unsigned int port )
{
	mSocketManager = PFS_NEW SocketManager();
	mNEF = PFS_NEW NetworkEventForwarder( 0, mSocketManager );
	if( !mSocketManager->Initialize() )
	{
		OutputDebugStringA( "Server failed to initialize." );
		Release();
		return false;
	}
	mSocketManager->AddSocket( new ServerListenSocket( port ) );
	InitForwardingEvents();
	return true;
}

void Server::Release()
{
	mSocketManager->Release();
	SAFE_DELETE( mSocketManager );
	SAFE_DELETE( mNEF );
}

Server::Server()
{
	mNEF = nullptr;
}

Server::~Server()
{
}