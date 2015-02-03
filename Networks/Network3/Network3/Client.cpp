#include "Client.h"

ClientSocketManager* Client::mSocketManager = nullptr;

void Client::InitEventListening()
{
	// Code for adding events that should be listened to by the client
}

void Client::InitForwardingEvents()
{
	// Code for adding events that should be forwarded to the network by the client here
	EventManager::GetInstance()->AddListener( &NetworkEventForwarder::ForwardEvent, mNEF, Event_Text::GUID );
}

bool Client::Initialize( std::string ip, unsigned int port )
{
	mSocketManager = new ClientSocketManager();
	mNEF = new NetworkEventForwarder( 0, mSocketManager );
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
	SAFE_RELEASE( mSocketManager );
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