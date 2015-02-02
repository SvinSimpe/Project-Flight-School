#include "Server.h"
#include "Client.h"
#include <string>
#include <iostream>

int main()
{
#if defined(DEBUG) | defined(_DEBUG)
	_CrtSetDbgFlag( _CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF );
	// 143, 145, 146
	//_CrtSetBreakAlloc(143); // Break at specific memory allocation point
#endif
	REGISTER_EVENT( Event_Client );
	REGISTER_EVENT( Event_Text );

	std::string answer = "";
	std::cout << "Would you like to start a (S)erver or a (C)lient? ";
	std::getline( std::cin, answer );

	std::string ip	= "127.0.0.1";
	UINT port		= 27015;

	bool serverOn = false;

	Server* server = nullptr;
	if( answer == "S" || answer == "s" )
	{
		server = new Server();
		if( !server->Initialize( port ) )
			OutputDebugStringA( "FUCK!" );
		std::cout << "Server up and running." << std::endl;
		serverOn = true;
	}

	answer = "";
	std::cout << "Enter IP: (leave empty for localhost) ";
	std::getline( std::cin, answer );

	if( !(answer == "") )
	{
		ip = answer;
	}
	std:: cout << "Attempting to connect to server with IP: " << ip << std::endl;

	Client* client = new Client();
	if( !client->Initialize( ip, port ) )
		OutputDebugStringA( "FUCK!" );

	std::cout << "Client connected to server." << std::endl;

	while( !GetAsyncKeyState( VK_ESCAPE ) )
	{
		if( serverOn )
		{
			gSocketManager = server->mSocketManager;
			gSocketManager->DoSelect( 0 );
		}
		gSocketManager = client->mSocketManager;
		gSocketManager->DoSelect( 0 );
		EventManager::GetInstance()->Update();
	}

	EventManager::GetInstance()->Release();
	server->Release();
	SAFE_DELETE( server );
	client->Release();
	SAFE_DELETE( client );
	return 0;
}