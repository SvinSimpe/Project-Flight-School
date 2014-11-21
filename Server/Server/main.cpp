#include "Server.h"
#include "Client.h"
#include <thread>
#include <crtdbg.h>

int main()
{
#if defined(DEBUG) | defined(_DEBUG)
	_CrtSetDbgFlag( _CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF );
	//_CrtSetBreakAlloc(142); // Break at specific memory allocation point
#endif

	const char* port = DEFAULT_PORT;
	const char* ip = DEFAULT_IP;

	std::string choice = "";
	std::getline( std::cin, choice );
	if ( choice == "s" )
	{
		ServerObj->Initialize( port );
		ServerObj->Connect();
		ServerObj->Update();
		ServerObj->Release();
	}
	else
	{
		Client c;
		c.Initialize( ip, port );
		c.Connect();
		c.Run();
		c.Release();
	}

	system( "pause" );
	return 0;
}