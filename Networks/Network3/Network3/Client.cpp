#include "Client.h"

void Client::HandleEvents( IEventPtr evtPtr )
{
	if( evtPtr->GetEventType() == Event_Client_Joined::GUID )
	{
		std::shared_ptr<Event_Client_Joined> data = std::static_pointer_cast<Event_Client_Joined>( evtPtr );
		int hostID = data->HostID();
		mID = data->SocketID();

		std::cout << "My ID is: " << mID << std::endl;
	}
}

// Code for adding events that should be listened to by the client
void Client::InitEventListening()
{
	EventManager::GetInstance()->AddListener( &Client::HandleEvents, this, Event_Client_Joined::GUID );
}

// Initializes all the eventlisteners for events that needs to be forwarded to the server
void Client::InitForwardingEvents()
{
	// Code for adding events that should be forwarded to the network by the client here
	EventManager::GetInstance()->AddListener( &NetworkEventForwarder::ForwardEvent, mNEF, Event_Text::GUID );
}

void Client::Update( float deltaTime )
{

}

void Client::DoSelect( int pauseMicroSecs, bool handleInput )
{
	mSocketManager->DoSelect( pauseMicroSecs, handleInput );
}

bool Client::Initialize( std::string ip, unsigned int port )
{
	Network::Initialize( port );
	mIP = ip;
	mSocketManager = new ClientSocketManager();
	if( !mSocketManager->Connect( mIP, mPort ) )
	{
		OutputDebugStringA( "Client failed to connect to server.\n" );
	}
	std::cout << "Client connected to server on IP: " << mIP << ", port: " << mPort << std::endl;
	mNEF = new NetworkEventForwarder();
	mNEF->Initialize( 0, mSocketManager ); // Always sends to socket 0, the server's socketID
	InitForwardingEvents();
	InitEventListening();
	return true;
}

void Client::Release()
{
	if( mSocketManager )
		mSocketManager->Release();
	SAFE_DELETE( mSocketManager );
	SAFE_DELETE( mNEF );
}

Client::Client() : Network()
{
	mSocketManager	= nullptr;
	mIP				= "";
	mNEF			= nullptr;
	mID				= 0;
	mActive			= false;
}

Client::~Client()
{
}