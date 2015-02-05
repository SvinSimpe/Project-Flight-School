#include "Client.h"

Client* Client::mInstance = nullptr;

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Start of eventlistening functions

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

void Client::RemoteUpdate( IEventPtr eventPtr )
{
	if( eventPtr->GetEventType() == Event_Remote_Update::GUID )
	{
		std::shared_ptr<Event_Remote_Update> data = std::static_pointer_cast<Event_Remote_Update>( eventPtr );
		UINT id = data->ID();
		XMFLOAT3 pos = data->LowerBodyPos();
		XMFLOAT3 vel = data->Velocity();
		XMFLOAT3 dir = data->UpperBodyDirection();

		std::cout << "Remote with ID: " << id << " updated." << std::endl;
	}
}

void Client::RemoteDied( IEventPtr eventPtr )
{
	if( eventPtr->GetEventType() == Event_Remote_Died::GUID )
	{
		std::shared_ptr<Event_Remote_Died> data = std::static_pointer_cast<Event_Remote_Died>( eventPtr );
		UINT id = data->ID();
		UINT killerID = data->KillerID();

		std::cout << "Remote with ID: " << id << " was killed by: " << killerID << std::endl;
	}
}

// End of eventlistening functions
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void Client::StartUp( IEventPtr eventPtr )
{
	if( eventPtr->GetEventType() == Event_Start_Client::GUID )
	{
		std::shared_ptr<Event_Start_Client> data = std::static_pointer_cast<Event_Start_Client>( eventPtr );
		std::string IP	= data->IP();
		UINT port		= data->Port();
		if( !Connect( IP, port ) )
			std::cout << "Failed to connect client!" << std::endl;
	}
}

/* Registers all the events that should be listened to from the server. */
void Client::InitEventListening()
{
	EventManager::GetInstance()->AddListener( &Client::LocalJoin, this, Event_Local_Joined::GUID );
	EventManager::GetInstance()->AddListener( &Client::RemoteJoined, this, Event_Remote_Joined::GUID );
	EventManager::GetInstance()->AddListener( &Client::RemoteLeft, this, Event_Remote_Left::GUID );
	EventManager::GetInstance()->AddListener( &Client::RemoteUpdate, this, Event_Remote_Update::GUID );
	EventManager::GetInstance()->AddListener( &Client::RemoteDied, this, Event_Remote_Died::GUID );
}

bool Client::Connect( std::string ip, UINT port )
{
	mSocketManager = new ClientSocketManager();
	if( !mSocketManager->Connect( ip, port ) )
	{
		return false;
	}
	std::cout << "Client connected to server on IP: " << ip << ", port: " << port << std::endl;

	mNEF = new NetworkEventForwarder();
	mNEF->Initialize( 0, mSocketManager ); // Always sends to socket 0, the server's socketID
	return true;
}

bool Client::Initialize()
{
	InitEventListening();
	return true;
}

Client* Client::GetInstance()
{
	if( !mInstance )
		mInstance = new Client();
	return mInstance;
}

void Client::SendEvent( IEventPtr eventPtr )
{
	mNEF->ForwardEvent( eventPtr );
}

void Client::Update( float deltaTime )
{
	if( mActive )
	{
		//IEventPtr E1( PFS_NEW Event_Local_Update( mID, mLowerBodyPos, mVelocity, mUpperBodyDirection ) );
		IEventPtr E1( PFS_NEW Event_Local_Died( mID, 0 ) );

		SendEvent( E1 );
	}
}

void Client::DoSelect( int pauseMicroSecs, bool handleInput )
{
	mSocketManager->DoSelect( pauseMicroSecs, handleInput );
}

void Client::Release()
{
	if( mSocketManager )
		mSocketManager->Release();
	SAFE_DELETE( mSocketManager );
	SAFE_DELETE( mNEF );
	mRemoteIDs.clear();
	SAFE_DELETE( mInstance );
}

Client::Client() : Network()
{
	mID						= (UINT)-1;
	mSocketManager			= nullptr;
	mNEF					= nullptr;
	mRemoteIDs				= std::list<UINT>();
	mActive					= false;

	mLowerBodyPos			= XMFLOAT3( 0.0f, 0.0f, 0.0f );
	mVelocity				= XMFLOAT3( 0.0f, 0.0f, 0.0f );
	mUpperBodyDirection		= XMFLOAT3( 0.0f, 0.0f, 0.0f );

	EventManager::GetInstance()->AddListener( &Client::StartUp, this, Event_Start_Client::GUID );
}

Client::~Client()
{
}