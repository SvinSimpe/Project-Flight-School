#include "Server.h"

void Server::ClientJoined( IEventPtr eventPtr )
{
	if( eventPtr->GetEventType() == Event_Client_Joined::GUID )
	{
		std::shared_ptr<Event_Client_Joined> data = std::static_pointer_cast<Event_Client_Joined>( eventPtr );
		UINT id = data->ID();

		mClientMap[id].Initialize( id, mSocketManager );

		std::cout << "Client with ID: " << id << " joined. There are now " << mClientMap.size() << " clients online." << std::endl;

		IEventPtr E1( PFS_NEW Event_Remote_Joined( id ) ); // Sends the incoming ID to the existing remotes
		BroadcastEvent( E1, id );

		IEventPtr E2( PFS_NEW Event_Local_Joined( id ) );
		SendEvent( E2, id );

		for( auto& remote : mClientMap )
		{
			if( remote.first != id )
			{
				IEventPtr E3( PFS_NEW Event_Remote_Joined( remote.first ) ); // The key of the map is the ID of the remote
				SendEvent( E3, id );
			}
		}
	}
}

void Server::ClientLeft( IEventPtr eventPtr )
{
	if( eventPtr->GetEventType() == Event_Client_Left::GUID )
	{
		std::shared_ptr<Event_Client_Left> data = std::static_pointer_cast<Event_Client_Left>( eventPtr );
		UINT id = data->ID();
		mClientMap.erase( id );
		std::cout << "Client with ID: " << id << " left. There are now " << mClientMap.size() << " client(s) online." << std::endl;

		IEventPtr E1( PFS_NEW Event_Remote_Left( id ) );
		BroadcastEvent( E1 );
	}
}

void Server::LocalUpdate( IEventPtr eventPtr )
{
	if( eventPtr->GetEventType() == Event_Local_Update::GUID )
	{
		std::shared_ptr<Event_Local_Update> data = std::static_pointer_cast<Event_Local_Update>( eventPtr );
		UINT id = data->ID();
		XMFLOAT3 pos = data->LowerBodyPos();
		XMFLOAT3 vel = data->Velocity();
		XMFLOAT3 dir = data->UpperBodyDirection();

		std::cout << "Client with ID: " << id << " just updated." << std::endl;

		IEventPtr E1( PFS_NEW Event_Remote_Update( id, pos, vel, dir ) );
		BroadcastEvent( E1, id );
	}
}

void Server::StartUp( IEventPtr eventPtr )
{
	if( eventPtr->GetEventType() == Event_Start_Server::GUID )
	{
		std::shared_ptr<Event_Start_Server> data = std::static_pointer_cast<Event_Start_Server>( eventPtr );
		UINT port = data->Port();
		if( !Initialize( port ) )
			std::cout << "Failed to start server!" << std::endl;
	}
}

void Server::BroadcastEvent( IEventPtr eventPtr, UINT exception )
{
	for( auto& to : mClientMap )
	{
		if( to.first != exception )
		{
			to.second.ForwardEvent( eventPtr );
		}
	}
}

void Server::SendEvent( IEventPtr eventPtr, UINT to )
{
	mClientMap[to].ForwardEvent( eventPtr );
}

/* This will register all the events that only the server will listen to,
	such as clients joining/disconnecting. */
void Server::InitEventListening()
{
	EventManager::GetInstance()->AddListener( &Server::ClientJoined, this, Event_Client_Joined::GUID );
	EventManager::GetInstance()->AddListener( &Server::ClientLeft, this, Event_Client_Left::GUID );
	EventManager::GetInstance()->AddListener( &Server::LocalUpdate, this, Event_Local_Update::GUID );
}

bool Server::Initialize( UINT port )
{
	Network::Initialize( port );
	mSocketManager = new SocketManager();
	if( !mSocketManager->Initialize() )
	{
		return false;
	}
	mSocketManager->AddSocket( new ServerListenSocket( mSocketManager, mPort ) );
	std::cout << "Server started on port: " << mPort << std::endl;

	InitEventListening();
	return true;
}

void Server::Update( float deltaTime )
{
}

void Server::DoSelect( int pauseMicroSecs, bool handleInput )
{
	mSocketManager->DoSelect( pauseMicroSecs, handleInput );
}

void Server::Release()
{
	mSocketManager->Release();
	SAFE_DELETE( mSocketManager );
	mClientMap.clear();
}

Server::Server() : Network()
{
	mSocketManager	= nullptr;
	mClientMap		= std::map<UINT, NetworkEventForwarder>();

	EventManager::GetInstance()->AddListener( &Server::StartUp, this, Event_Start_Server::GUID );
}

Server::~Server()
{
}