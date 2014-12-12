#include "Input.h"
#include <strsafe.h>

#pragma region Public functions

bool Input::IsKeyDown( UINT flag )
{
	if( ( flag & RI_KEY_BREAK ) != 0 )
	{
		return false;
	}

	return true;
}

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


void Input::Update( LPARAM lParam )
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
				//mCurrentFrame[KEYS::KEYS_MOUSE_LEFT] = true;
			}

			if(raw->data.mouse.usFlags == MOUSE_MOVE_RELATIVE)
			{
				static POINT pt;
				GetCursorPos(&pt);
				mCurrentMousePos.x = pt.x - raw->data.mouse.lLastX;
				mCurrentMousePos.y = pt.y - raw->data.mouse.lLastY;

				POINT position;
				GetCursorPos( &position );
				ScreenToClient( mHwnd, &position );

				float mouseXCoord = (float)position.x;
				float mouseYCoord = (float)position.y;

				Graphics::GetInstance()->SetNDCSpaceCoordinates( mouseXCoord, mouseYCoord );
				float viewSpaceZ	= 0.0f; 

				mCurrentNDCMousePos = XMVectorSet( mouseXCoord, mouseYCoord, 0.0f, 1.0f );				
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
				case VK_SPACE:	
					mCurrentFrame[KEYS::KEYS_SPACE] = IsKeyDown( raw->data.keyboard.Flags );
					break;
			}
			break;
		case RIM_TYPEHID:
			//TBD for xbox controller
			break;
	}
}

HRESULT	Input::Initialize( UINT screenWidth, UINT screenHeight, HWND hWnd )
{
	mScreenWidth	= screenWidth;
	mScreenHeight	= screenHeight;
	mHwnd			= hWnd;

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

	mCurrentMousePos.x = (long)( screenWidth * 0.5 );
	mCurrentMousePos.y	= (long)( screenHeight * 0.5 );

	return S_OK;
}

void Input::Release()
{
	
}

#pragma endregion