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
					mCurrentFrame[KEYS::KEYS_0] = IsKeyDown( raw->data.keyboard.Flags );
					break;
				case 0x31: // 1
					mCurrentFrame[KEYS::KEYS_1] = IsKeyDown( raw->data.keyboard.Flags );
					break;
				case 0x32: // 2
					mCurrentFrame[KEYS::KEYS_2] = IsKeyDown( raw->data.keyboard.Flags );
					break;
				case 0x33: // 3 
					mCurrentFrame[KEYS::KEYS_3] = IsKeyDown( raw->data.keyboard.Flags );
					break;
				case 0x34: // 4
					mCurrentFrame[KEYS::KEYS_4] = IsKeyDown( raw->data.keyboard.Flags );
					break;
				case 0x35: // 5
					mCurrentFrame[KEYS::KEYS_5] = IsKeyDown( raw->data.keyboard.Flags );
					break;
				case 0x36: // 6
					mCurrentFrame[KEYS::KEYS_6] = IsKeyDown( raw->data.keyboard.Flags );
					break;
				case 0x37: // 7
					mCurrentFrame[KEYS::KEYS_7] = IsKeyDown( raw->data.keyboard.Flags );
					break;
				case 0x38: // 8
					mCurrentFrame[KEYS::KEYS_8] = IsKeyDown( raw->data.keyboard.Flags );
					break;
				case 0x39: // 9
					mCurrentFrame[KEYS::KEYS_9] = IsKeyDown( raw->data.keyboard.Flags );
					break;

				case 0x41:		//A
					mCurrentFrame[KEYS::KEYS_A] = IsKeyDown( raw->data.keyboard.Flags );
					break;
				case 0x42:		//B
					mCurrentFrame[KEYS::KEYS_B] = IsKeyDown( raw->data.keyboard.Flags );
					break;
				case 0x43:		//C
					mCurrentFrame[KEYS::KEYS_C] = IsKeyDown( raw->data.keyboard.Flags );
					break;
				case 0x44:		//D
					mCurrentFrame[KEYS::KEYS_D] = IsKeyDown( raw->data.keyboard.Flags );
					break;
				case 0x45:		//E
					mCurrentFrame[KEYS::KEYS_E] = IsKeyDown( raw->data.keyboard.Flags );
					break;
				case 0x46:		//F
					mCurrentFrame[KEYS::KEYS_F] = IsKeyDown( raw->data.keyboard.Flags );
					break;
				case 0x47:		//G
					mCurrentFrame[KEYS::KEYS_G] = IsKeyDown( raw->data.keyboard.Flags );
					break;
				case 0x48:		//H
					mCurrentFrame[KEYS::KEYS_H] = IsKeyDown( raw->data.keyboard.Flags );
					break;
				case 0x49:		//I
					mCurrentFrame[KEYS::KEYS_I] = IsKeyDown( raw->data.keyboard.Flags );
					break;
				case 0x4A:		//J
					mCurrentFrame[KEYS::KEYS_J] = IsKeyDown( raw->data.keyboard.Flags );
					break;
				case 0x4B:		//K
					mCurrentFrame[KEYS::KEYS_K] = IsKeyDown( raw->data.keyboard.Flags );
					break;
				case 0x4C:		//L
					mCurrentFrame[KEYS::KEYS_L] = IsKeyDown( raw->data.keyboard.Flags );
					break;
				case 0x4D:		//M
					mCurrentFrame[KEYS::KEYS_M] = IsKeyDown( raw->data.keyboard.Flags );
					break;
				case 0x4E:		//N
					mCurrentFrame[KEYS::KEYS_N] = IsKeyDown( raw->data.keyboard.Flags );
					break;
				case 0x4F:		//O
					mCurrentFrame[KEYS::KEYS_O] = IsKeyDown( raw->data.keyboard.Flags );
					break;
				case 0x50:		//P
					mCurrentFrame[KEYS::KEYS_P] = IsKeyDown( raw->data.keyboard.Flags );
					break;
				case 0x51:		//Q
					mCurrentFrame[KEYS::KEYS_Q] = IsKeyDown( raw->data.keyboard.Flags );
					break;
				case 0x52:		//R
					mCurrentFrame[KEYS::KEYS_R] = IsKeyDown( raw->data.keyboard.Flags );
					break;
				case 0x53:		//S
					mCurrentFrame[KEYS::KEYS_S] = IsKeyDown( raw->data.keyboard.Flags );
					break;
				case 0x54:		//T
					mCurrentFrame[KEYS::KEYS_T] = IsKeyDown( raw->data.keyboard.Flags );
					break;
				case 0x55:		//U
					mCurrentFrame[KEYS::KEYS_U] = IsKeyDown( raw->data.keyboard.Flags );
					break;
				case 0x56:		//V
					mCurrentFrame[KEYS::KEYS_V] = IsKeyDown( raw->data.keyboard.Flags );
					break;
				case 0x57:		//W
					mCurrentFrame[KEYS::KEYS_W] = IsKeyDown( raw->data.keyboard.Flags );
					break;
				case 0x58:		//X
					mCurrentFrame[KEYS::KEYS_X] = IsKeyDown( raw->data.keyboard.Flags );
					break;
				case 0x59:		//Y
					mCurrentFrame[KEYS::KEYS_Y] = IsKeyDown( raw->data.keyboard.Flags );
					break;
				case 0x5A:		//Z
					mCurrentFrame[KEYS::KEYS_Z] = IsKeyDown( raw->data.keyboard.Flags );
					break;

				case VK_SPACE:	
					mCurrentFrame[KEYS::KEYS_SPACE] = IsKeyDown( raw->data.keyboard.Flags );
					break;
				case VK_BACK:
					mCurrentFrame[KEYS::KEYS_BACKSPACE] = IsKeyDown( raw->data.keyboard.Flags );
					break;
				case VK_OEM_PERIOD:
					mCurrentFrame[KEYS::KEYS_PERIOD] = IsKeyDown( raw->data.keyboard.Flags );
					break;
				case VK_CONTROL:
					mCurrentFrame[KEYS::KEYS_RCTRL] = IsKeyDown( raw->data.keyboard.Flags );
					break;
				case VK_UP:
					mCurrentFrame[KEYS::KEYS_UP] = IsKeyDown( raw->data.keyboard.Flags );
					break;
				case VK_DOWN:
					mCurrentFrame[KEYS::KEYS_DOWN] = IsKeyDown( raw->data.keyboard.Flags );
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
	mCurrentFrame.clear();
}

#pragma endregion