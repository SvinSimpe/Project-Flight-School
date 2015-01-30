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
	//_CrtSetBreakAlloc(832854); // Break at specific memory allocation point
#endif

	std::string answer = "";
	std::cout << "Would you like to start a (S)erver or a (C)lient? ";
	std::getline( std::cin, answer );

	std::string ip	= "127.0.0.1";
	UINT port		= 27015;

	bool serverOn = false;

	if( answer == "S" || answer == "s" )
	{
		gSocketManager = new SocketManager();
		if( ! (serverOn = gSocketManager->Initialize() ) )
		{
			std::cout << "Failed to initialize server." << std::endl;
			getchar();
			Cleanup( gSocketManager );
			return 1;
		}
		gSocketManager->AddSocket( new ServerListenSocket( port ) );
		std::cout << "Server up and running." << std::endl;
	}
	else
	{
		std::cout << "Fuck you!" << std::endl;
	}

	answer = "";
	std::cout << "Enter IP: (leave empty for localhost)" << std::endl;
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
		Cleanup( client );
		Cleanup( gSocketManager );
		return 2;
	}
	std::cout << "Client connected to server." << std::endl;

	while( !GetAsyncKeyState( VK_ESCAPE ) )
	{
		if( serverOn )
			gSocketManager->DoSelect( 0 );
		client->DoSelect( 0 );
	}

	Cleanup( client );
	Cleanup( gSocketManager );
	return 0;
}