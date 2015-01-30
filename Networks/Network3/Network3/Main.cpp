#include "Connection.h"
#include <string>
#include <iostream>

void Cleanup()
{
	gSocketManager->Release();
	SAFE_DELETE( gSocketManager );
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

	if( answer == "S" || answer == "s" )
	{
		gSocketManager = new SocketManager();
		if( !gSocketManager->Initialize() )
		{
			std::cout << "Failed to initialize server." << std::endl;
			getchar();
			Cleanup();
			return 1;
		}
		gSocketManager->AddSocket( new ServerListenSocket( port ) );
		std::cout << "Server up and running." << std::endl;
	}
	else if( answer == "C" || answer == "c" )
	{
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
			Cleanup();
			return 2;
		}
		gSocketManager = client;
		std::cout << "Client connected to server." << std::endl;
	}
	else
	{
		std::cout << "Fuck you!" << std::endl;
		getchar();
		return 3;
	}

	while( !GetAsyncKeyState( VK_ESCAPE ) )
	{
		gSocketManager->DoSelect(0);
	}
	Cleanup();
	return 0;
}