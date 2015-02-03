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
	//_CrtSetBreakAlloc(142); // Break at specific memory allocation point
#endif
	EF::REGISTER_EVENT( Event_Client_Joined );
	EF::REGISTER_EVENT( Event_Text );

	std::string answer = "";
	std::cout << "Would you like to start a (S)erver or a (C)lient? ";
	std::getline( std::cin, answer );

	std::string ip	= "127.0.0.1";
	UINT port		= 27015;

	bool serverOn = false;

	Server* server		= nullptr;
	Client* client		= nullptr;
	if( answer == "S" || answer == "s" )
	{
		server = new Server();
		if( !server->Initialize(port) )
		{
			return 1;
		}
		serverOn = true;
	}
	client = new Client();
	if( !client->Initialize( ip, port ) )
	{
		return 2;
	}

	int frameCount = 0;
	while( !GetAsyncKeyState( VK_ESCAPE ) )
	{
		frameCount++;
		if( serverOn )
		{
			server->DoSelect( 0 );
		}
		client->DoSelect( 0 );
		if( frameCount > 2000 )
		{
			if( serverOn )
			{
				server->Update( 0.0f );
			}
			client->Update( 0.0f );
			frameCount = 0;
		}
		EventManager::GetInstance()->Update();
	}

	EventManager::GetInstance()->Release();
	if( serverOn )
	{
		if( server )
			server->Release();
		SAFE_DELETE( server );
	}
	if( client )
		client->Release();
	SAFE_DELETE( client );
	gSocketManager = nullptr;
	return 0;
}