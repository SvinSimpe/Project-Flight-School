#include "Server.h"
#include "Client.h"
#include <string>
#include <iostream>

int main()
{
#if defined(DEBUG) | defined(_DEBUG)
	_CrtSetDbgFlag( _CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF );
	// 149 150 151 152 153 154
	//_CrtSetBreakAlloc(161); // Break at specific memory allocation point
#endif
	REGISTER_EVENT( Event_Client_Joined );
	REGISTER_EVENT( Event_Text );

	std::string answer = "";
	std::cout << "Would you like to start a (S)erver or a (C)lient? ";
	std::getline( std::cin, answer );

	std::string ip	= "127.0.0.1";
	UINT port		= 27015;

	bool serverOn = false;

	Server* server = nullptr;
	Client* client = nullptr;

	if( answer == "S" || answer == "s" )
	{
		server = new Server();
		if( !server->Initialize( port ) )
		{
			OutputDebugStringA( "FUCK!" );
			return 1;
		}
		std::cout << "Server up and running." << std::endl;
		serverOn = true;
		gSocketManager = server->mSocketManager;
	}
	else if( answer == "C" || answer == "c")
	{
		answer = "";
		std::cout << "Enter IP: (leave empty for localhost) ";
		std::getline( std::cin, answer );

		if( !(answer == "") )
		{
			ip = answer;
		}
		std:: cout << "Attempting to connect to server with IP: " << ip << std::endl;

		client = new Client();
		if( !client->Initialize( ip, port ) )
		{
			OutputDebugStringA( "FUCK!" );
			return 2;
		}
		std::cout << "Client connected to server." << std::endl;
		gSocketManager = client->mSocketManager;
	}
	else
	{
		OutputDebugStringA( "FUCK!" );
		return 3;
	}

	while( !GetAsyncKeyState( VK_ESCAPE ) )
	{
		gSocketManager->DoSelect( 0 );
		EventManager::GetInstance()->Update();
	}

	EventManager::GetInstance()->Release();
	SAFE_RELEASE( client );
	SAFE_DELETE( client );
	SAFE_RELEASE( server );
	SAFE_DELETE( server );
	SAFE_RELEASE( gSocketManager );
	SAFE_DELETE( gSocketManager );
	return 0;
}