#include "Client.h"
#include <iostream>

void Client::HandleLocalJoin( IEventPtr evtPtr )
{
	if( evtPtr->GetEventType() == Event_Local_Client_Joined::GUID )
	{
		std::shared_ptr<Event_Local_Client_Joined> data = std::static_pointer_cast<Event_Local_Client_Joined>( evtPtr );
		mID = data->ID();
		std::cout << "My ID is: " << mID << std::endl;
		mActive = true;
	}
}

void Client::HandleRemoteList( IEventPtr evtPtr )
{
	if( evtPtr->GetEventType() == Event_Remote_Client_List::GUID )
	{
		mRemoteIDs.clear();
		std::shared_ptr<Event_Remote_Client_List> data = std::static_pointer_cast<Event_Remote_Client_List>( evtPtr );
		mRemoteIDs = data->RemoteIDs();
		std::cout << "Number of other players online: " << mRemoteIDs.size() << std::endl;
	}
}

// Code for adding events that should be listened to by the client
void Client::InitEventListening()
{
	EventManager::GetInstance()->AddListener( &Client::HandleLocalJoin, this, Event_Local_Client_Joined::GUID );
	EventManager::GetInstance()->AddListener( &Client::HandleRemoteList, this, Event_Remote_Client_List::GUID );
}

// Initializes all the eventlisteners for events that needs to be forwarded to the server
void Client::InitForwardingEvents()
{
	// Code for adding events that should be forwarded to the network by the client here
}

void Client::Update( float deltaTime )
{
	// Nothing here yet!
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
	std::cout << "Client connected to server on IP: " << mIP << ", port: " << mPort << std::endl;
	mNEF = new NetworkEventForwarder();
	mNEF->Initialize( 0, *mSocketManager );
	InitForwardingEvents();
	InitEventListening();
	return true;
}

void Client::Release()
{
	mSocketManager->Release();
	SAFE_DELETE( mSocketManager );
	SAFE_DELETE( mNEF );
}

Client::Client() : Network()
{
	mSocketManager	= nullptr;
	mNEF			= nullptr;
	mActive			= false;
	mID				= -1;
	mRemoteIDs		= std::vector<UINT>();
}

Client::~Client()
{
}