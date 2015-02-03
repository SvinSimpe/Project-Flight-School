#include "Server.h"

SocketManager* Server::mSocketManager = nullptr;

void Server::InitEventListening()
{
	// Code for adding events that should be listened to by the server
}

void Server::InitForwardingEvents()
{
	// Code for adding events that should be forwarded to the network by the server
}

bool Server::Initialize( unsigned int port )
{
	mSocketManager = new SocketManager();
	mNEF = new NetworkEventForwarder( 0, mSocketManager );
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
	SAFE_RELEASE( mSocketManager );
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