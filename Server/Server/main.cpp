#include "Server.h"
#include "Client.h"
#include <thread>
#include <crtdbg.h>
#include <io.h>
#include <stdio.h>
#include <fcntl.h>

int main()
{
#if defined(DEBUG) | defined(_DEBUG)
	_CrtSetDbgFlag( _CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF );
	//_CrtSetBreakAlloc(142); // Break at specific memory allocation point

	// Below code is used to open a separate console window, only used in debug
	AllocConsole();
	HANDLE handle_out = GetStdHandle(STD_OUTPUT_HANDLE);
	int hCrt = _open_osfhandle((long)handle_out, _O_TEXT);
	FILE* hf_out = _fdopen(hCrt, "w");
	setvbuf(hf_out, nullptr, _IONBF, 1);
	*stdout = *hf_out;

	HANDLE handle_in = GetStdHandle(STD_INPUT_HANDLE);
	hCrt = _open_osfhandle((long)handle_in, _O_TEXT);
	FILE* hf_int = _fdopen(hCrt, "r");
	setvbuf(hf_int, nullptr, _IONBF, 128);
	*stdin = *hf_int;
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