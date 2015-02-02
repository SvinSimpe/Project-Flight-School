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
	// 186, 185, 184, 183, 182, 181, 180
	//_CrtSetBreakAlloc(186); // Break at specific memory allocation point
#endif

	std::string answer = "";
	std::cout << "Would you like to start a (S)erver or a (C)lient? ";
	std::getline( std::cin, answer );

	std::string ip	= "localhost";
	UINT port		= 27015;

	bool serverOn = false;
	SocketManager* server = nullptr;

	if( !REGISTER_EVENT( Event_Client ) )
		std::cout << "Failed to register event!" << std::endl;

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
		std::cout << "Server up and running." << std::endl;
	}
	else
	{
		std::cout << "Fuck you!" << std::endl;
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
	std::cout << "Client connected to server." << std::endl;

	NetworkEventForwarder* nef = PFS_NEW NetworkEventForwarder( 0, client );
	EventManager::GetInstance()->AddListener( &NetworkEventForwarder::ForwardEvent, nef, Event_Client::GUID );

	while( !GetAsyncKeyState( VK_ESCAPE ) )
	{
		if( serverOn )
		{
			gSocketManager = server;
			gSocketManager->DoSelect( 0 );
		}
		gSocketManager = client;
		gSocketManager->DoSelect( 0 );
	}

	EventManager::GetInstance()->RemoveListener( &NetworkEventForwarder::ForwardEvent, nef, Event_Client::GUID );
	SAFE_DELETE( nef );
	EventManager::GetInstance()->Release();
	Cleanup( client );
	Cleanup( server );
	return 0;
}