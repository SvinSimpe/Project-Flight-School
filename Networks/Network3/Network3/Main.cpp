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
	std::string answer = "";
	std::cout << "Would you like to start a (S)erver or a (C)lient? ";
	std::getline( std::cin, answer );

	std::string ip	= "127.0.0.1";
	UINT port		= 27015;

	bool serverOn = false;

	Network* network = nullptr;
	if( answer == "S" || answer == "s" )
	{
		network = new Server();
		if( !network->Initialize(port) )
		{
			return 1;
		}
	}
	else if( answer == "C" || answer == "c" )
	{
		network = new Client();
		if( !dynamic_cast<Client*>(network)->Initialize( ip, port ) )
		{
			return 2;
		}
	}
	else
	{
		OutputDebugStringA( "Incorrect input. Idiot.\n" );
		return 3;
	}

	EF::REGISTER_EVENT( Event_Client_Joined );
	EF::REGISTER_EVENT( Event_Client_Left );
	EF::REGISTER_EVENT( Event_Text );

	int frameCount = 0;
	while( !GetAsyncKeyState( VK_ESCAPE ) )
	{
		network->DoSelect( 0 );
		frameCount++;
		if( frameCount > 10000 )
		{
			network->Update( 0.0f );
		}
		EventManager::GetInstance()->Update();
	}

	if( network )
		network->Release();
	SAFE_DELETE( network );
	EventManager::GetInstance()->Release();
	return 0;
}