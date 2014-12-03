#include "Input.h"
#include <strsafe.h>
#pragma region Public functions

#pragma region Private

bool Input::IsKeyDown( UINT flag )
{
	if( ( flag & RI_KEY_BREAK ) != 0 )
	{
		return false;
	}

	return true;
}

#pragma endregion 
RAWINPUT* Input::ReadMessage( LPARAM lParam )
{
	UINT dwSize;
	GetRawInputData( (HRAWINPUT)lParam, RID_INPUT, nullptr, &dwSize, sizeof( RAWINPUTHEADER ) );

	if( mLpb == NULL )
	{
		return nullptr;
	}

	if( GetRawInputData( (HRAWINPUT)lParam, RID_INPUT, mLpb, &dwSize, sizeof( RAWINPUTHEADER ) ) > BUFFER_SIZE )
	{
		OutputDebugStringA( "GetRawInputData returns a too big of a buffer size!\n" );
		return nullptr;
	}

	RAWINPUT* raw = (RAWINPUT*)mLpb;
	
    return raw;
}

void Input::InterpetrateRawInput( LPARAM lParam )
{
	RAWINPUT* raw;
	raw = ReadMessage( lParam );

	if( raw == nullptr )
	{
		UINT errorMsg;
		errorMsg = GetLastError();
		OutputDebugStringA( "Error when reading message. \n" );
		return;
	}
	
	switch( raw->header.dwType )
	{
		case RIM_TYPEMOUSE:
			if( raw->data.mouse.ulButtons & RI_MOUSE_BUTTON_1_DOWN )
			{
				OutputDebugStringA( "Mouse1 down \n" );
			}
			break;
		case RIM_TYPEKEYBOARD:
			switch( raw->data.keyboard.VKey )
			{
				case 0x57:		//W
					mCurrentFrame[KEYS::KEYS_W] = IsKeyDown( raw->data.keyboard.Flags );
					break;
				case 0x41:		//A
					mCurrentFrame[KEYS::KEYS_A] = IsKeyDown( raw->data.keyboard.Flags );
					break;
				case 0x53:		//S
					mCurrentFrame[KEYS::KEYS_S] = IsKeyDown( raw->data.keyboard.Flags );
					break;
				case 0x44:		//D
					mCurrentFrame[KEYS::KEYS_D] = IsKeyDown( raw->data.keyboard.Flags );
					break;
			}
			break;
		case RIM_TYPEHID:
			//TBD for xbox controller
			break;
	}
	RI_KEY_E0;
}

std::vector<bool>& Input::Update( LPARAM lParam )
{
	InterpetrateRawInput( lParam );

	//mCurrentFrame[KEYS_W] = false;
	return mCurrentFrame;
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
	
	if( RegisterRawInputDevices( mRid, NUMBER_OF_DEVICES, sizeof( mRid[0] )) == false )
	{
		errorMsg = GetLastError();
		return S_FALSE;
	}

	mCurrentFrame.resize( KEYS::KEYS_AMOUNT );

	for( int i = 0; i < KEYS::KEYS_AMOUNT; i++ )
	{
		mCurrentFrame[i] = false;
	}

	return S_OK;
}

void Input::Release()
{
	
}

#pragma endregion