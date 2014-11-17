#include "System.h"
#include <crtdbg.h>

int WINAPI wWinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR lpCmdLine, int nCmdShow )
{
	int localReturn = 0;

	_CrtSetDbgFlag( _CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF );
	
	System* system = new System();
	system->Initialize( hInstance, nCmdShow );
	localReturn = system->Run();
	system->Release();

	delete system;

	return localReturn;
}