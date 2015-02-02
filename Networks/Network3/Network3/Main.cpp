#include "Connection.h"
#include "Events.h"
#include <string>
#include <iostream>

void Cleanup( SocketManager* socketManager )
{
	socketManager->Release();
	SAFE_DELETE( socketManager );
}

int main()
{
#if defined(DEBUG) | defined(_DEBUG)
	_CrtSetDbgFlag( _CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF );
	// 143, 145, 146
	//_CrtSetBreakAlloc(143); // Break at specific memory allocation point
#endif

	std::string answer = "";
	std::cout << "Would you like to start a (S)erver or a (C)lient? ";
	std::getline( std::cin, answer );

	std::string ip	= "localhost";
	UINT port		= 27015;

	bool serverOn = false;
	SocketManager* server = nullptr;
	NetworkEventForwarder* nef = nullptr;
	NetworkEventForwarder* nef2 = nullptr;

	if( !REGISTER_EVENT( Event_Client ) )
		std::cout << "Failed when registering event with ID: " << Event_Client::GUID << std::endl;
	if( !REGISTER_EVENT( Event_Text ) )
		std::cout << "Failed when registering event with ID: " << Event_Text::GUID << std::endl;

	if( answer == "S" || answer == "s" )
	{
		server = new SocketManager();
		if( ! (serverOn = server->Initialize() ) )
		{
			std::cout << "Failed to initialize server." << std::endl;
			getchar();
			Cleanup( server );
			return 1;
		}
		server->AddSocket( new ServerListenSocket( port ) );
		nef = PFS_NEW NetworkEventForwarder( 0, server );
		std::cout << "Server up and running." << std::endl;
	}

	answer = "";
	std::cout << "Enter IP: (leave empty for localhost) ";
	std::getline( std::cin, answer );

	if( !(answer == "") )
	{
		ip = answer;
	}
	std:: cout << "Attempting to connect to server with IP: " << ip << std::endl;
	ClientSocketManager* client = new ClientSocketManager();
	if( !client->Connect( ip, port ) )
	{
		std::cout << "Failed to connect to server." << std::endl;
		getchar();
		Cleanup( server );
		Cleanup( client );
		return 2;
	}

	EventManager::GetInstance()->AddListener( &NetworkEventForwarder::ForwardEvent, nef, Event_Client::GUID );
	EventManager::GetInstance()->AddListener( &NetworkEventForwarder::ForwardEvent, nef, Event_Text::GUID );
	std::cout << "Client connected to server." << std::endl;

	while( !GetAsyncKeyState( VK_ESCAPE ) )
	{
		if( serverOn )
		{
			gSocketManager = server;
			gSocketManager->DoSelect( 0 );
		}
		gSocketManager = client;
		gSocketManager->DoSelect( 0 );
		EventManager::GetInstance()->Update();
	}

	EventManager::GetInstance()->RemoveListener( &NetworkEventForwarder::ForwardEvent, nef, Event_Client::GUID );
	EventManager::GetInstance()->RemoveListener( &NetworkEventForwarder::ForwardEvent, nef, Event_Text::GUID );

	SAFE_DELETE( nef );
	EventManager::GetInstance()->Release();
	Cleanup( client );
	Cleanup( server );
	return 0;
}