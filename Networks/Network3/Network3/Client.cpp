#include "Client.h"

ClientSocketManager* Client::mSocketManager = nullptr;

void Client::DoSelect( int pauseMicroSecs, bool handleInput )
{
	gSocketManager->DoSelect( pauseMicroSecs, handleInput );
}

void Client::Initialize( std::string ip, unsigned int port )
{
	mSocketManager = PFS_NEW ClientSocketManager();
	mNEF = PFS_NEW NetworkEventForwarder( 0, mSocketManager );
	if( !mSocketManager->Connect( ip, port ) )
	{
		Release();
		return;
	}
}

void Client::Release()
{
	mSocketManager->Release();
	SAFE_DELETE( mSocketManager );
	SAFE_DELETE( mNEF );
}

Client::Client()
{
	mNEF = nullptr;
}

Client::~Client()
{
}