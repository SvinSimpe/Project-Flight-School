#include <string>
#include <iostream>
#include "Server.h"
#include "Client.h"

int main()
{
#if defined(DEBUG) | defined(_DEBUG)
	_CrtSetDbgFlag( _CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF );
	//_CrtSetBreakAlloc(142); // Break at specific memory allocation point
#endif
	EF::REGISTER_EVENT( Event_Local_Client_Joined );
	EF::REGISTER_EVENT( Event_Remote_Client_List );
	EF::REGISTER_EVENT( Event_Client_Status_Update );

	std::string answer = "";
	std::cout << "Would you like to start a (S)erver or a (C)lient? ";
	std::getline( std::cin, answer );

	std::string ip	= "127.0.0.1";
	UINT port		= 1337;

	bool serverOn = false;

	Network* network = nullptr;
	if( answer == "S" || answer == "s" )
	{
		network = new Server();
		if( !network->Initialize(port) )
		{
			return 1;
		}
		serverOn = true;
	}
	else if( answer == "C" || answer == "c" )
	{
		network = new Client();
		if( !dynamic_cast<Client*>(network)->Initialize( ip, port ) )
		{
			return 2;
		}
	}

	int frameCount = 0;
	while( !GetAsyncKeyState( VK_ESCAPE ) )
	{
		frameCount++;
		network->DoSelect( 0 );
		if( frameCount > 2000 )
		{
			network->Update( 0.0f );
			frameCount = 0;
		}
		EventManager::GetInstance()->Update();
	}

	EventManager::GetInstance()->Release();
	if( network )
		network->Release();
	SAFE_DELETE( network );
	gSocketManager = nullptr;
	return 0;
}