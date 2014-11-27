#include "Input.h"
#include <strsafe.h>
#pragma region Public functions

RAWINPUT* Input::ReadMessage(LPARAM lParam)
{
	UINT dwSize;
	GetRawInputData( (HRAWINPUT)lParam, RID_INPUT, nullptr, &dwSize, sizeof( RAWINPUTHEADER ) );

	if( lpb == NULL )
	{
		return nullptr;
	}

	if( GetRawInputData( (HRAWINPUT)lParam, RID_INPUT, lpb, &dwSize, sizeof( RAWINPUTHEADER ) ) > BUFFERSIZE )
	{
		OutputDebugStringA( "GetRawInputData returns a too big of a buffer size!\n" );
		return nullptr;
	}

	RAWINPUT* raw = (RAWINPUT*)lpb;
	
    return raw;
}

HRESULT	Input::Initialize()
{
	UINT errorMsg;

	//Mouse
	mRid[0].usUsagePage	= 1;			//Set flag for generic desktop control
	mRid[0].usUsage		= 2;			//Set this RAWINPUTDEVICE usage for mouse
	mRid[0].dwFlags		= 0;			//Interpretation of information from usUsagePage and usUsage set to default; sending rawinput from device to registered application.
	mRid[0].hwndTarget	= NULL;			//Set to NULL so the current window will be the focus

	//Keyboard
	mRid[1].usUsagePage	= 1;
	mRid[1].usUsage		= 6;			//Set this RAWINPUTDEVICE usage for keyboard
	mRid[1].dwFlags		= 0;
	mRid[1].hwndTarget	= NULL;
	
	if( RegisterRawInputDevices( mRid, NROFDEVICES, sizeof( mRid[0] )) == false )
	{
		errorMsg = GetLastError();
		return S_FALSE;
	}

	return S_OK;
}

void Input::Release()
{
	
}

#pragma endregion