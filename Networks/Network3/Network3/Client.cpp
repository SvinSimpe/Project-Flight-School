#include "Client.h"

ClientSocketManager* Client::mSocketManager = nullptr;

void Client::InitForwardingEvents()
{
	// Code for adding events that should be forwarded to the network by the client here
}

bool Client::Initialize( std::string ip, unsigned int port )
{
	mSocketManager = PFS_NEW ClientSocketManager();
	mNEF = PFS_NEW NetworkEventForwarder( 0, mSocketManager );
	if( !mSocketManager->Connect( ip, port ) )
	{
		OutputDebugStringA( "Client failed to initialize." );
		Release();
		return false;
	}
	InitForwardingEvents();
	return true;
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