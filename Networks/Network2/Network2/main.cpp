#include "Network.h"
#include "Events.h"
#include "EventManagerImpl.h"
#include <string>
#include <iostream>

template <class I, class F>
EventListenerDelegate MakeDelegate( I instance, F function )
{
	EventListenerDelegate delegate = std::bind( instance, function, std::placeholders::_1 );
	return delegate;
}

int main()
{
#if defined(DEBUG) | defined(_DEBUG)
	_CrtSetDbgFlag( _CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF );
	_CrtSetBreakAlloc(0); // Break at specific memory allocation point
#endif
	//REGISTER_EVENT( EvtData_Send_Text );

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
		if( !pclient->Connect() )
		{
			printf("client connection failed.\n");
			return 1;
		}
		g_pSocketManager = pclient;		
		printf("Connected!\n");

		NetworkEventForwarder* forwarder = new NetworkEventForwarder(0);
		EventManager::Get()->VAddListener( MakeDelegate( forwarder, &NetworkEventForwarder::ForwardEvent ), EvtData_Send_Text::sk_EventType );

		EvtData_Send_Text* hello = GCC_NEW EvtData_Send_Text("Hello world!");
		forwarder->ForwardEvent( hello->VCopy() );

		EventManager::Get()->VRemoveListener( MakeDelegate( forwarder, &NetworkEventForwarder::ForwardEvent ), EvtData_Send_Text::sk_EventType );
	}

	while(!GetAsyncKeyState(VK_ESCAPE))
	{
		g_pSocketManager->DoSelect(0);
	}

	g_pSocketManager->Shutdown();
	return 0;
}