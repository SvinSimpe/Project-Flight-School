#include "Server.h"

SocketManager* Server::mSocketManager = nullptr;

void Server::DoSelect( int pauseMicroSecs, bool handleInput )
{
	gSocketManager->DoSelect( pauseMicroSecs, handleInput );
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