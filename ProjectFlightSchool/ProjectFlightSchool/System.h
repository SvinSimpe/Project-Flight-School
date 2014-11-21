#ifndef SYSTEM_H
#define SYSTEM_H

#define WIN32_LEAN_AND_MEAN
#define VC_EXTRALEAN
#include <Windows.h>
#include "Graphics.h"

class System
{
	//Class members
	private:
		HINSTANCE	mHInstance;
		HWND		mHWnd;
		UINT		mScreenWidth;
		UINT		mScreenHeight;

		UINT		mAssetId;

		std::vector<RAWINPUTDEVICE> mRid;

	protected:
	public:

	//Class functions
	private:
		static LRESULT CALLBACK WndProc( HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam );
		HRESULT					Update( float deltaTime );
		HRESULT					Render();
		UINT					DetecAndRegisterDevices();

	protected:
	public:
		int		Run();
		HRESULT Initialize( HINSTANCE hInstance, int nCmdShow );
		void	Release();
				System();
		virtual ~System();
};
#endif
