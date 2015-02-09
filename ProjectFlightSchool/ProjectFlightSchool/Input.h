#ifndef INPUT_H
#define INPUT_H

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif
#include <Windows.h>

#define VC_EXTRALEAN
#include <vector>
#include "KeyDefines.h"
#include <DirectXMath.h>
#include "Graphics.h"

const UINT NUMBER_OF_DEVICES	= 2;	//The amount of devices to be registered; mouse and keyboard currently, added to the array in written order.
const UINT BUFFER_SIZE			= 40;	//Fixed buffer size for interpretating RAWINPUT messages, set to the size of the mouse input since it is the biggest of the 2 devices we're handling.

class Input
{
	private:
		std::vector<bool>	mCurrentFrame;
		std::vector<bool>	mLastFrame;
	protected:
	public:
		RAWINPUTDEVICE		mRid[NUMBER_OF_DEVICES];
		LPBYTE				mLpb[BUFFER_SIZE];

		POINT				mCurrentMousePos;
		DirectX::XMVECTOR	mCurrentNDCMousePos;
		UINT				mScreenWidth;
		UINT				mScreenHeight;

		HWND				mHwnd;

	private:
		void	KeyChecker( KEYS key, UINT flag );
		//Stoping the compiler from generating methods for copying the object
				Input( Input const& );		//Not implemented
		void	operator=( Input const& );	//Not implemented
				//Private constructor
				Input() {};

	protected:
	public:
		//Returns true if the key is currently being pressed down, otherwise false. Do not call this, call Update( ... ).
		bool		IsKeyDown( KEYS key );
		bool		IsKeyPressed( KEYS key );
		void		LostFocus();
		//Do not call this, call Update( ... ).
		RAWINPUT*	ReadMessage( LPARAM lParam );
		void		RawRead( LPARAM lParam );
		void		Update();
		HRESULT		Initialize( UINT screenWidth, UINT screenHeight, HWND hWnd );
		void		Release();

		static Input* GetInstance()
		{
			static Input instance;
			return &instance;
		}
		~Input(){};
};

#endif