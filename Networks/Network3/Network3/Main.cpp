#include <string>
#include <iostream>
#include "Server.h"
#include "Client.h"

int StartServerAndClient();
int StartServerOrClient();

int main()
{
#if defined(DEBUG) | defined(_DEBUG)
	_CrtSetDbgFlag( _CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF );
	//_CrtSetBreakAlloc(142); // Break at specific memory allocation point
#endif
	// REGISTER_EVENT should only be run once for each event
	EF::REGISTER_EVENT( Event_Client_Joined );
	EF::REGISTER_EVENT( Event_Client_Left );
	EF::REGISTER_EVENT( Event_Local_Joined );
	EF::REGISTER_EVENT( Event_Remote_Joined );
	EF::REGISTER_EVENT( Event_Remote_Left );
	EF::REGISTER_EVENT( Event_Local_Update );
	EF::REGISTER_EVENT( Event_Remote_Update );

	std::string answer = "";
	std::cout << "Would you like to" << std::endl;
	std::cout << "(1) start a server AND client connection?" << std::endl;
	std::cout << "(2) start a server OR client connection?" << std::endl;

	std::getline( std::cin, answer );
	if( answer == "1" )
	{
		return StartServerAndClient();
	}
	else if( answer == "2" )
	{
		return StartServerOrClient();
	}
	else
	{
		EventManager::GetInstance()->Release();
		OutputDebugStringA( "Wrong input, douchebag." );
		return 3;
	}

	return 0;
}

int StartServerAndClient()
{
	std::string ip	= "127.0.0.1";
	UINT port		= 27015;

	Server* server = nullptr;
	Client* client = nullptr;

	server = new Server();
	if( !server->Initialize(port) )
	{
		if( server )
			server->Release();
		SAFE_DELETE( server );
		EventManager::GetInstance()->Release();
		return 1;
	}

	client = new Client();
	if( !client->Initialize( ip, port ) )
	{
		if( client )
			client->Release();
		SAFE_DELETE( client );

		if( server )
			server->Release();
		SAFE_DELETE( server );

		EventManager::GetInstance()->Release();
		return 2;
	}

	int frameCount = 0;
	while( !GetAsyncKeyState( VK_ESCAPE ) )
	{
		frameCount++;
		client->DoSelect( 0 );
		server->DoSelect( 0 );

		if( frameCount > 10000 )
		{
			client->Update( 0.0f );
			server->Update( 0.0f );
			frameCount = 0;
		}

		EventManager::GetInstance()->Update();
	}

	if( client )
		client->Release();
	SAFE_DELETE( client );

	if( server )
		server->Release();
	SAFE_DELETE( server );
	EventManager::GetInstance()->Release();
	return 0;
}
int StartServerOrClient()
{
	std::string answer = "";
	std::cout << "Would you like to start a (S)erver or a (C)lient? ";
	std::getline( std::cin, answer );

	std::string ip	= "127.0.0.1";
	UINT port		= 27015;

	Network* network = nullptr;
	if( answer == "S" || answer == "s" )
	{
		network = new Server();
		if( !network->Initialize(port) )
		{
			if( network )
				network->Release();
			SAFE_DELETE( network );
			EventManager::GetInstance()->Release();
			return 1;
		}
	}
	else if( answer == "C" || answer == "c" )
	{
		network = new Client();
		if( !dynamic_cast<Client*>(network)->Initialize( ip, port ) )
		{
			if( network )
				network->Release();
			SAFE_DELETE( network );
			EventManager::GetInstance()->Release();
			return 2;
		}
	}
	else
	{
		OutputDebugStringA( "Incorrect input. Idiot.\n" );
		return 3;
	}


	int frameCount = 0;
	while( !GetAsyncKeyState( VK_ESCAPE ) )
	{
		network->DoSelect( 0 );
		frameCount++;
		if( frameCount > 10000 )
		{
			network->Update( 0.0f );
			frameCount = 0;
		}
		EventManager::GetInstance()->Update();
	}

	if( network )
		network->Release();
	SAFE_DELETE( network );
	EventManager::GetInstance()->Release();
	return 0;
}