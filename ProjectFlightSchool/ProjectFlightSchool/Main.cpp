#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif

#include "System.h"
#include <crtdbg.h>

// Below includes are used for the console window
#include <stdio.h>
#include <io.h>
#include <fcntl.h>
#include <vld.h>

int WINAPI wWinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR lpCmdLine, int nCmdShow )
{
#if defined(DEBUG) | defined(_DEBUG)

	_CrtSetDbgFlag( _CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF );
	// 9376964 9301170 9143178 9143172 9143171 9143170
 	// _CrtSetBreakAlloc(9376964); // Break at specific memory allocation point
#endif
	// Console window code
	//if( AllocConsole() )
	//{
	//	HANDLE handle_out = GetStdHandle( STD_OUTPUT_HANDLE );
	//	int hCrt = _open_osfhandle( (long)handle_out, _O_TEXT );
	//	FILE* hf_out = _fdopen( hCrt, "w" );
	//	setvbuf( hf_out, nullptr, _IONBF, 1 );
	//	*stdout = *hf_out;
	//
	//	HANDLE handle_in = GetStdHandle( STD_INPUT_HANDLE );
	//	hCrt = _open_osfhandle( (long)handle_in, _O_TEXT );
	//	FILE* hf_int = _fdopen( hCrt, "r" );
	//	setvbuf( hf_int, nullptr, _IONBF, 128 );
	//	*stdin = *hf_int;
	//}
	// End of console window code

	int localReturn = 0;
	char buf[1024];
	DWORD bufSize = 1024;
	GetComputerNameA( buf, &bufSize );

	if( strcmp( buf, "G426-PC" ) == 0 )
		MessageBox( NULL, L"Martin sluda spela!", L"Error", MB_OK );

	System* system = new System();
	system->Initialize( hInstance, nCmdShow );
	localReturn = system->Run();

	system->Release();
	SAFE_DELETE(system);

	return localReturn;
}
