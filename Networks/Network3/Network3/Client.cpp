#include "Client.h"

void Client::LocalJoin( IEventPtr eventPtr )
{
	if( eventPtr->GetEventType() == Event_Local_Joined::GUID )
	{
		std::shared_ptr<Event_Local_Joined> data = std::static_pointer_cast<Event_Local_Joined>( eventPtr );
		mID = data->ID();

		std::cout << "My ID is: " << mID << std::endl;
		mActive = true;
	}
}

void Client::RemoteJoined( IEventPtr eventPtr )
{
	if( eventPtr->GetEventType() == Event_Remote_Joined::GUID )
	{
		std::shared_ptr<Event_Remote_Joined> data = std::static_pointer_cast<Event_Remote_Joined>( eventPtr );
		UINT id = data->ID();
		mRemoteIDs.push_back( id );

		std::cout << "Remote with ID: " << id << " joined. There are now " << mRemoteIDs.size() << " remotes online." << std::endl;
	}
}

void Client::RemoteLeft( IEventPtr eventPtr )
{
	if( eventPtr->GetEventType() == Event_Remote_Left::GUID )
	{
		std::shared_ptr<Event_Remote_Left> data = std::static_pointer_cast<Event_Remote_Left>( eventPtr );
		UINT id = data->ID();
		mRemoteIDs.remove( id );

		std::cout << "Remote with ID: " << id << " left. There are now " << mRemoteIDs.size() << " remotes online." << std::endl;
	}
}

/* Registers all the events that should be listened to from the server. */
void Client::InitEventListening()
{
	EventManager::GetInstance()->AddListener( &Client::LocalJoin, this, Event_Local_Joined::GUID );
	EventManager::GetInstance()->AddListener( &Client::RemoteJoined, this, Event_Remote_Joined::GUID );
	EventManager::GetInstance()->AddListener( &Client::RemoteLeft, this, Event_Remote_Left::GUID );
}

/* Registers all the events that should be sent to the server. */
void Client::InitForwardingEvents()
{
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
		return false;
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
	mRemoteIDs.clear();
}

Client::Client() : Network()
{
	mSocketManager	= nullptr;
	mIP				= "";
	mNEF			= nullptr;
	mID				= 0;
	mRemoteIDs		= std::list<UINT>();
	mActive			= false;
}

Client::~Client()
{
}