#include "Network.h"
#include <string>
#include <iostream>

int main()
{
	printf("Enter host name: (Empty to start server):\n");
	std::string host;
	std::getline( std::cin, host );

	if( host.empty() )
	{
		g_pSocketManager = new BaseSocketManager;
		g_pSocketManager->Init();
		g_pSocketManager->AddSocket( new GameServerListenSocket( 27015 ) );
		printf("Server is running....\n");
	}
	else
	{
		ClientSocketManager* pclient = new ClientSocketManager( host, 27015 );
		if(! pclient->Connect() )
		{
			printf("client connection failed.\n");
			return 1;
		}
		g_pSocketManager = pclient;		
		printf("Connected!\n");
	}
	while(!GetAsyncKeyState(VK_ESCAPE))
	{
		g_pSocketManager->DoSelect(0);
	}

	g_pSocketManager->Shutdown();
	return 0;
}