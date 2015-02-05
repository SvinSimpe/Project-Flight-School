#include <string>
#include <iostream>
#include "Server.h"
#include "Client.h"

int StartServerAndClient( std::string ip, UINT port );
int StartServerOrClient( std::string ip, UINT port );

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
	EF::REGISTER_EVENT( Event_Change_State );
	EF::REGISTER_EVENT( Event_Start_Server );
	EF::REGISTER_EVENT( Event_Start_Client );
	EF::REGISTER_EVENT( Event_Game_Started );
	EF::REGISTER_EVENT( Event_Game_Ended );

	std::string ip	= "127.0.0.1";
	UINT port		= 27015;

	//return StartServerAndClient( ip, port );
	return StartServerOrClient( ip, port );

	return 0;
}

int StartServerAndClient( std::string ip, UINT port )
{
	Server* server = nullptr;
	Client* client = nullptr;

	std::string answer = "";
	std::cout << "Would you like to start a (S)erver? ";
	std::getline( std::cin, answer );

	bool serverOn = false;
	if( answer == "S" || answer == "s" )
	{
		server = new Server();
		IEventPtr E1( PFS_NEW Event_Start_Server( port ) );
		EventManager::GetInstance()->TriggerEvent( E1 );
		serverOn = true;
	}

	client = new Client();
	IEventPtr E2( PFS_NEW Event_Start_Client( ip, port ) );
	EventManager::GetInstance()->TriggerEvent( E2 );

	int frameCount = 0;
	while( !GetAsyncKeyState( VK_ESCAPE ) )
	{
		frameCount++;
		client->DoSelect( 0 );
		if( serverOn )
			server->DoSelect( 0 );

		if( frameCount > 10000 )
		{
			client->Update( 0.0f );
			if( serverOn )
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

int StartServerOrClient( std::string ip, UINT port )
{
	std::string answer = "";
	std::cout << "Would you like to start a (S)erver or a (C)lient? ";
	std::getline( std::cin, answer );

	Network* network = nullptr;
	if( answer == "S" || answer == "s" )
	{
		network = new Server();
		IEventPtr E1( PFS_NEW Event_Start_Server( port ) );
		EventManager::GetInstance()->TriggerEvent( E1 );
	}
	else if( answer == "C" || answer == "c" )
	{
		network = new Client();
		IEventPtr E2( PFS_NEW Event_Start_Client( ip, port ) );
		EventManager::GetInstance()->TriggerEvent( E2 );
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