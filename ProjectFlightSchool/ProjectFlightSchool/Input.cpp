#include "Input.h"
#include <strsafe.h>
#pragma region Public functions

RAWINPUT* Input::ReadMessage(LPARAM lParam)
{
	UINT dwSize;

	GetRawInputData( (HRAWINPUT)lParam, RID_INPUT, nullptr, &dwSize, sizeof( RAWINPUTHEADER ) );

	LPBYTE lpb = new BYTE[dwSize];
	if( lpb == NULL )
	{
		return nullptr;
	}

	if( GetRawInputData( (HRAWINPUT)lParam, RID_INPUT, lpb, &dwSize, sizeof( RAWINPUTHEADER ) ) != dwSize )
	{
		OutputDebugString( TEXT( "GetRawInputData does not return the correct size!\n" ) );
	}

	RAWINPUT* raw = (RAWINPUT*)lpb;
	TCHAR szTempOutput[256];
	
	if (raw->header.dwType == RIM_TYPEKEYBOARD) 
    {
			StringCchPrintf(szTempOutput, STRSAFE_MAX_CCH, TEXT(" Kbd: make=%04x Flags:%04x Reserved:%04x ExtraInformation:%08x, msg=%04x VK=%04x \n"), 
            raw->data.keyboard.MakeCode, 
            raw->data.keyboard.Flags, 
            raw->data.keyboard.Reserved, 
            raw->data.keyboard.ExtraInformation, 
            raw->data.keyboard.Message, 
            raw->data.keyboard.VKey);

        OutputDebugString(szTempOutput);
    }
    else if (raw->header.dwType == RIM_TYPEMOUSE) 
    {
			StringCchPrintf(szTempOutput, STRSAFE_MAX_CCH, TEXT("Mouse: usFlags=%04x ulButtons=%04x usButtonFlags=%04x usButtonData=%04x ulRawButtons=%04x lLastX=%04x lLastY=%04x ulExtraInformation=%04x\r\n"), 
            raw->data.mouse.usFlags, 
            raw->data.mouse.ulButtons, 
            raw->data.mouse.usButtonFlags, 
            raw->data.mouse.usButtonData, 
            raw->data.mouse.ulRawButtons, 
            raw->data.mouse.lLastX, 
            raw->data.mouse.lLastY, 
            raw->data.mouse.ulExtraInformation);

        OutputDebugString(szTempOutput);
    } 

    delete[] lpb; 
    return 0;
}

HRESULT	Input::Initialize()
{
	UINT cbSize, nrOfActiveDevices;
	PRAWINPUTDEVICELIST prids;
	DWORD errorMsg;

	if( GetRawInputDeviceList( NULL, &nrOfActiveDevices, sizeof( RAWINPUTDEVICELIST ) ) != 0 )
	{
		errorMsg = GetLastError();
		return S_FALSE;
	}
	
	prids = (PRAWINPUTDEVICELIST)malloc( sizeof( RAWINPUTDEVICELIST ) * nrOfActiveDevices );
	if( prids == NULL ) 
	{
		errorMsg = GetLastError();
		return S_FALSE;
	}

	if( GetRawInputDeviceList( prids, &nrOfActiveDevices, sizeof( RAWINPUTDEVICELIST ) ) == (UINT)-1 )
	{
		errorMsg = GetLastError();
		return S_FALSE;
	}

	RID_DEVICE_INFO rdi;
	rdi.cbSize = sizeof( RID_DEVICE_INFO );
	cbSize = rdi.cbSize;
	//for( UINT i = 0; i < nrOfActiveDevices; i++ )
	//{
	//	if( GetRawInputDeviceInfo( prids[i].hDevice, RIDI_DEVICEINFO, &rdi, &cbSize ) < 0 ) 
	//	{
	//		errorMsg = GetLastError();
	//		return S_FALSE;
	//	}
	//	else
	//	{
	//		switch( rdi.dwType )
	//		{
	//			case RIM_TYPEMOUSE:
	//				mRid[i].usUsagePage	= 1;
	//				mRid[i].usUsage		= 2;
	//				mRid[i].dwFlags		= 0;
	//				mRid[i].hwndTarget		= NULL;
	//				break;
	//			case RIM_TYPEKEYBOARD:
	//				mRid[i].usUsagePage	= 1;
	//				mRid[i].usUsage		= 6;
	//				mRid[i].dwFlags		= 0;
	//				mRid[i].hwndTarget		= NULL;
	//				break;
	//			case RIM_TYPEHID:
	//				//TBD
	//				break;
	//		}
	//	}
	//}

		mRid[0].usUsagePage	= 1;
		mRid[0].usUsage		= 2;
		mRid[0].dwFlags		= 0;
		mRid[0].hwndTarget		= NULL;

		mRid[1].usUsagePage	= 1;
		mRid[1].usUsage		= 6;
		mRid[1].dwFlags		= 0;
		mRid[1].hwndTarget		= NULL;
	
	if( RegisterRawInputDevices( mRid, 2, sizeof( mRid[0] )) == false )
	{
		errorMsg = GetLastError();
		return S_FALSE;
	}

	free( prids );
	return S_OK;
}

void Input::Release()
{
	
}

#pragma endregion