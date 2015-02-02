#include "Server.h"

SocketManager* Server::mSocketManager = nullptr;

void Server::InitForwardingEvents()
{
	// Code for adding events that should be forwarded to the network by the server
}

void Server::Initialize( unsigned int port )
{
	mSocketManager = PFS_NEW SocketManager();
	mNEF = PFS_NEW NetworkEventForwarder( 0, mSocketManager );
	if( !mSocketManager->Initialize() )
	{
		Release();
		return;
	}
	mSocketManager->AddSocket( new ServerListenSocket( port ) );
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