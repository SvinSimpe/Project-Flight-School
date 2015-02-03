#include <string>
#include <iostream>
#include "Server.h"
#include "Client.h"

void HandleEvents( IEventPtr evtPtr )
{
	if( evtPtr->GetEventType() == Event_Client_Joined::GUID )
	{
		std::shared_ptr<Event_Client_Joined> data = std::static_pointer_cast<Event_Client_Joined>( evtPtr );
		int hostID = data->HostID();
		int socketID = data->SocketID();

		std::cout << hostID << ", " << socketID << std::endl;
	}
}

int main()
{
#if defined(DEBUG) | defined(_DEBUG)
	_CrtSetDbgFlag( _CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF );
	// 149
	//_CrtSetBreakAlloc(149); // Break at specific memory allocation point
#endif
	REGISTER_EVENT( Event_Client_Joined );
	REGISTER_EVENT( Event_Text );

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
		OutputDebugStringA( "No correct input. Idiot.\n" );
		return 3;
	}

	while( !GetAsyncKeyState( VK_ESCAPE ) )
	{
		network->DoSelect( 0 );
		EventManager::GetInstance()->Update();
	}

	if( network )
		network->Release();
	SAFE_DELETE( network );
	gSocketManager = nullptr;
	EventManager::GetInstance()->Release();
	/////////////////////////////////////////////////////////////////////////////////
	//NetworkEventForwarder* nef = nullptr;
	//ClientSocketManager* client = nullptr;
	//if( answer == "S" || answer == "s" )
	//{
	//	gSocketManager = new SocketManager();
	//	if( !gSocketManager->Initialize() )
	//	{
	//		OutputDebugStringA( "Server couldn't initialize.\n" );
	//		return 1;
	//	}
	//	gSocketManager->AddSocket( new ServerListenSocket( port ) );
	//	std::cout << "Server up and running." << std::endl;
	//	serverOn = true;
	//}
	//else if( answer == "C" || answer == "c" )
	//{
	//	client = new ClientSocketManager();
	//	if( !client->Connect( ip, port ) )
	//	{
	//		OutputDebugStringA( "Client couldn't connect.\n" );
	//		return 2;
	//	}
	//	gSocketManager = client;
	//	nef = new NetworkEventForwarder();
	//	nef->Initialize( 0, gSocketManager );
	//	EventManager::GetInstance()->AddListener( &NetworkEventForwarder::ForwardEvent, nef, Event_Client_Joined::GUID );
	//	std::cout << "Client connected to server." << std::endl;
	//}
	//else
	//{
	//	printf( "Things messed up.\n" );
	//	return 3;
	//}

	//while( !GetAsyncKeyState( VK_ESCAPE ) )
	//{
	//	gSocketManager->DoSelect( 0 );
	//	EventManager::GetInstance()->Update();
	//}

	//EventManager::GetInstance()->Release();
	//gSocketManager->Release();
	//SAFE_DELETE( gSocketManager );
	//SAFE_DELETE( nef );
	return 0;
}