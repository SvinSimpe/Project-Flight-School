#include "Input.h"
#include <strsafe.h>

void Input::KeyChecker( KEYS key, UINT flag )
{
	if( ( flag & RI_KEY_MAKE ) == 0 )
		mCurrentFrame[key] = true;
	if( ( flag & RI_KEY_BREAK ) == 1 )
		mCurrentFrame[key] = false;
}

#pragma region Public functions

bool Input::IsKeyDown( KEYS key )
{
	return mCurrentFrame[key];
}

bool Input::IsKeyPressed( KEYS key )
{
	return mCurrentFrame[key] && !mLastFrame[key];
}

void Input::LostFocus()
{
	for( int i = 0; i < KEYS::KEYS_AMOUNT; i++ )
	{
		mLastFrame[i]		= false;
		mCurrentFrame[i]	= false;
	}
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

void Input::RawRead( LPARAM lParam )
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
			if( raw->data.mouse.ulButtons & RI_MOUSE_LEFT_BUTTON_DOWN )
			{
				mCurrentFrame[KEYS::KEYS_MOUSE_LEFT] = true;
			}
			else if( raw->data.mouse.ulButtons & RI_MOUSE_LEFT_BUTTON_UP )
			{
				mCurrentFrame[KEYS::KEYS_MOUSE_LEFT] = false;
			}

			if( raw->data.mouse.ulButtons & RI_MOUSE_RIGHT_BUTTON_DOWN )
			{
				mCurrentFrame[KEYS::KEYS_MOUSE_RIGHT] = true;
			}
			else if( raw->data.mouse.ulButtons & RI_MOUSE_RIGHT_BUTTON_UP )
			{
				mCurrentFrame[KEYS::KEYS_MOUSE_RIGHT] = false;
			}

			if(raw->data.mouse.usFlags == MOUSE_MOVE_RELATIVE)
			{
				static POINT pt;
				GetCursorPos( &pt );
				ScreenToClient( mHwnd, &pt );

				mCurrentMousePos.x = pt.x - raw->data.mouse.lLastX;
				mCurrentMousePos.y = pt.y - raw->data.mouse.lLastY;

				POINT position;
				GetCursorPos( &position );
				ScreenToClient( mHwnd, &position );

				float mouseXCoord = (float)position.x;
				float mouseYCoord = (float)position.y;

				Graphics::GetInstance()->SetNDCSpaceCoordinates( mouseXCoord, mouseYCoord );
				float viewSpaceZ	= 0.0f; 

				mCurrentNDCMousePos = DirectX::XMVectorSet( mouseXCoord, mouseYCoord, 0.0f, 1.0f );				
			}

			break;
		case RIM_TYPEKEYBOARD:
			switch( raw->data.keyboard.VKey )
			{
				case 0x30: // 0
					KeyChecker( KEYS::KEYS_0, raw->data.keyboard.Flags );
					break;
				case 0x31: // 1
					KeyChecker( KEYS::KEYS_1, raw->data.keyboard.Flags );
					break;
				case 0x32: // 2
					KeyChecker( KEYS::KEYS_2, raw->data.keyboard.Flags );
					break;
				case 0x33: // 3 
					KeyChecker( KEYS::KEYS_3, raw->data.keyboard.Flags );
					break;
				case 0x34: // 4
					KeyChecker( KEYS::KEYS_4, raw->data.keyboard.Flags );
					break;
				case 0x35: // 5
					KeyChecker( KEYS::KEYS_5, raw->data.keyboard.Flags );
					break;
				case 0x36: // 6
					KeyChecker( KEYS::KEYS_6, raw->data.keyboard.Flags );
					break;
				case 0x37: // 7
					KeyChecker( KEYS::KEYS_7, raw->data.keyboard.Flags );
					break;
				case 0x38: // 8
					KeyChecker( KEYS::KEYS_8, raw->data.keyboard.Flags );
					break;
				case 0x39: // 9
					KeyChecker( KEYS::KEYS_9, raw->data.keyboard.Flags );
					break;

				case 0x41:		//A
					KeyChecker( KEYS::KEYS_A, raw->data.keyboard.Flags );
					break;
				case 0x42:		//B
					KeyChecker( KEYS::KEYS_B, raw->data.keyboard.Flags );
					break;
				case 0x43:		//C
					KeyChecker( KEYS::KEYS_C, raw->data.keyboard.Flags );
					break;
				case 0x44:		//D
					KeyChecker( KEYS::KEYS_D, raw->data.keyboard.Flags );
					break;
				case 0x45:		//E
					KeyChecker( KEYS::KEYS_E, raw->data.keyboard.Flags );
					break;
				case 0x46:		//F
					KeyChecker( KEYS::KEYS_F, raw->data.keyboard.Flags );
					break;
				case 0x47:		//G
					KeyChecker( KEYS::KEYS_G, raw->data.keyboard.Flags );
					break;
				case 0x48:		//H
					KeyChecker( KEYS::KEYS_H, raw->data.keyboard.Flags );
					break;
				case 0x49:		//I
					KeyChecker( KEYS::KEYS_I, raw->data.keyboard.Flags );
					break;
				case 0x4A:		//J
					KeyChecker( KEYS::KEYS_J, raw->data.keyboard.Flags );
					break;
				case 0x4B:		//K
					KeyChecker( KEYS::KEYS_K, raw->data.keyboard.Flags );
					break;
				case 0x4C:		//L
					KeyChecker( KEYS::KEYS_L, raw->data.keyboard.Flags );
					break;
				case 0x4D:		//M
					KeyChecker( KEYS::KEYS_M, raw->data.keyboard.Flags );
					break;
				case 0x4E:		//N
					KeyChecker( KEYS::KEYS_N, raw->data.keyboard.Flags );
					break;
				case 0x4F:		//O
					KeyChecker( KEYS::KEYS_O, raw->data.keyboard.Flags );
					break;
				case 0x50:		//P
					KeyChecker( KEYS::KEYS_P, raw->data.keyboard.Flags );
					break;
				case 0x51:		//Q
					KeyChecker( KEYS::KEYS_Q, raw->data.keyboard.Flags );
					break;
				case 0x52:		//R
					KeyChecker( KEYS::KEYS_R, raw->data.keyboard.Flags );
					break;
				case 0x53:		//S
					KeyChecker( KEYS::KEYS_S, raw->data.keyboard.Flags );
					break;
				case 0x54:		//T
					KeyChecker( KEYS::KEYS_T, raw->data.keyboard.Flags );
					break;
				case 0x55:		//U
					KeyChecker( KEYS::KEYS_U, raw->data.keyboard.Flags );
					break;
				case 0x56:		//V
					KeyChecker( KEYS::KEYS_V, raw->data.keyboard.Flags );
					break;
				case 0x57:		//W
					KeyChecker( KEYS::KEYS_W, raw->data.keyboard.Flags );
					break;
				case 0x58:		//X
					KeyChecker( KEYS::KEYS_X, raw->data.keyboard.Flags );
					break;
				case 0x59:		//Y
					KeyChecker( KEYS::KEYS_Y, raw->data.keyboard.Flags );
					break;
				case 0x5A:		//Z
					KeyChecker( KEYS::KEYS_Z, raw->data.keyboard.Flags );
					break;

				case VK_SPACE:	
					KeyChecker( KEYS::KEYS_SPACE, raw->data.keyboard.Flags );
					break;
				case VK_BACK:
					KeyChecker( KEYS::KEYS_BACKSPACE, raw->data.keyboard.Flags );
					break;
				case VK_OEM_PERIOD:
					KeyChecker( KEYS::KEYS_PERIOD, raw->data.keyboard.Flags );
					break;
				case VK_RETURN:
					KeyChecker( KEYS::KEYS_ENTER, raw->data.keyboard.Flags );
					break;
				case VK_CONTROL:
					KeyChecker( KEYS::KEYS_RCTRL, raw->data.keyboard.Flags );
					break;
				case VK_LSHIFT:
					KeyChecker( KEYS::KEYS_LSHIFT, raw->data.keyboard.Flags );
					break;
				case VK_UP:
					KeyChecker( KEYS::KEYS_UP, raw->data.keyboard.Flags );
					break;
				case VK_DOWN:
					KeyChecker( KEYS::KEYS_DOWN, raw->data.keyboard.Flags );
					break;
				case VK_LEFT:
					KeyChecker( KEYS::KEYS_LEFT, raw->data.keyboard.Flags );
					break;
				case VK_RIGHT:
					KeyChecker( KEYS::KEYS_RIGHT, raw->data.keyboard.Flags );
					break;
			}
			break;
		case RIM_TYPEHID:
			//TBD for xbox controller
			break;
	}
}

void Input::Update()
{
	for( int i = 0; i < KEYS::KEYS_AMOUNT; i++ )
		mLastFrame[i] = mCurrentFrame[i];
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
	mRid[1].dwFlags		= RIDEV_NOLEGACY;
	mRid[1].hwndTarget	= NULL;
	
	if( RegisterRawInputDevices( mRid, NUMBER_OF_DEVICES, sizeof( mRid[0] )) == false )
	{
		errorMsg = GetLastError();
		return S_FALSE;
	}

	mCurrentFrame.resize( KEYS::KEYS_AMOUNT );
	mLastFrame.resize( KEYS::KEYS_AMOUNT );

	for( int i = 0; i < KEYS::KEYS_AMOUNT; i++ )
	{
		mCurrentFrame[i]	= false;
		mLastFrame[i]		= false;
	}

	mCurrentMousePos.x = (long)( screenWidth * 0.5 );
	mCurrentMousePos.y	= (long)( screenHeight * 0.5 );

	return S_OK;
}

void Input::Release()
{
	mCurrentFrame.clear();
}

#pragma endregion