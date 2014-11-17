#include "Server.h"
#include "Client.h"
#include <thread>
#include <crtdbg.h>

#define DEFAULT_PORT "27015"

void RunServer(const char* port)
{
	Server::GetInstance()->Initialize(port);
	Server::GetInstance()->AcceptConnection();
	Server::GetInstance()->MessageLoop();
	Server::GetInstance()->CloseConnection();
	Server::GetInstance()->Release();
}

void RunClient(const char* port)
{
	Client c;
	c.Initialize(port);
	c.MessageLoop();
	c.Release();
}

int main(){
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

	std::string port = "";
	printf("Enter port: ");
	//std::getline(std::cin, port);
	port = DEFAULT_PORT;
	const char* c_port = port.c_str();

	std::thread server(RunServer, c_port);
	std::thread client(RunClient, c_port);

	server.join();
	client.join();

	system("pause");
	return 0;
}