#include "Client.h"

void Client::HandleEvents( IEventPtr evtPtr )
{
	if( evtPtr->GetEventType() == Event_Client_Joined::GUID )
	{
		std::shared_ptr<Event_Client_Joined> data = std::static_pointer_cast<Event_Client_Joined>( evtPtr );
		int hostID = data->HostID();
		int socketID = data->SocketID();
		std::cout << hostID << ", " << socketID << std::endl;
	}
}

void Client::InitEventListening()
{
	// Code for adding events that should be listened to by the client
	EventManager::GetInstance()->AddListener( &Client::HandleEvents, this, Event_Client_Joined::GUID );
}

void Client::InitForwardingEvents()
{
	// Code for adding events that should be forwarded to the network by the client here
	EventManager::GetInstance()->AddListener( &NetworkEventForwarder::ForwardEvent, mNEF, Event_Text::GUID );
}

void Client::DoSelect( int pauseMicroSecs, bool handleInput )
{
	gSocketManager = mSocketManager;
	gSocketManager->DoSelect( pauseMicroSecs, handleInput );
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
	mNEF = new NetworkEventForwarder();
	mNEF->Initialize( 0, mSocketManager );
	return true;
}

void Client::Release()
{
	Network::Release();
	mSocketManager->Release();
	SAFE_DELETE( mSocketManager );
}

Client::Client() : Network()
{
	mSocketManager = nullptr;
}

Client::~Client()
{
}