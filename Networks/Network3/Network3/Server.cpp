#include "Server.h"
#include <iostream>

void Server::BroadcastPkt( Packet pkt, UINT exception )
{
	for( auto& socket : mSocketList )
	{
		if( socket != exception )
		{
			mSocketManager->Send( socket, pkt );
		}
	}
}

void Server::BroadcastEvent( IEventPtr eventPtr, UINT exception )
{
	for( auto& socket : mSocketList )
	{
		if( socket != exception )
		{
			mNEF->ForwardEvent( socket, eventPtr );
		}
	}
}

void Server::SendPkt( UINT receiver, Packet pkt )
{
	mSocketManager->Send( receiver, pkt );
}

void Server::SendEvent( UINT receiver, IEventPtr eventPtr )
{
	mNEF->ForwardEvent( receiver, eventPtr );
}

void Server::HandleEvents( IEventPtr eventPtr )
{
	if( eventPtr->GetEventType() == Event_Client_Status_Update::GUID )
	{
		mSocketList.clear();
		std::shared_ptr<Event_Client_Status_Update> data = std::static_pointer_cast<Event_Client_Status_Update>( eventPtr );
		mSocketList = data->SocketList();
		std::cout << "Amount of connected clients: " << mSocketList.size() << std::endl;
	
		IEventPtr E1 = std::shared_ptr<Event_Remote_Client_List>( PFS_NEW Event_Remote_Client_List( mSocketList.size(), mSocketList ) );
		BroadcastEvent( E1 );
	}
}

void Server::InitEventListening()
{
	// Code for adding events that should be listened to by the server
	EventManager::GetInstance()->AddListener( &Server::HandleEvents, this, Event_Client_Status_Update::GUID );
}

void Server::Update( float deltaTime )
{

}

void Server::DoSelect( int pauseMicroSecs, bool handleInput )
{
	gSocketManager = mSocketManager;
	gSocketManager->DoSelect( pauseMicroSecs, handleInput );
}

bool Server::Initialize( UINT port )
{
	Network::Initialize( port );
	mSocketManager = new SocketManager();
	if( !mSocketManager->Initialize() )
	{
		OutputDebugStringA( "Failed to initialize server.\n" );
		return false;
	}
	mSocketManager->AddSocket( new ServerListenSocket( mPort ) );
	std::cout << "Server started on port: " << mPort << std::endl;

	mNEF = new NetworkEventForwarder();
	mNEF->Initialize( 0, *mSocketManager );

	InitEventListening();
	return true;
}

void Server::Release()
{
	mSocketManager->Release();
	SAFE_DELETE( mSocketManager );
	mSocketList.clear();
	SAFE_DELETE( mNEF );
}

Server::Server() : Network()
{
	mSocketManager	= nullptr;
	mSocketList		= std::vector<UINT>();
}

Server::~Server()
{
}