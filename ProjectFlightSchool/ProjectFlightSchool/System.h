#ifndef SYSTEM_H
#define SYSTEM_H

#include <Windows.h>

class System
{
	//Class members
	private:
		HINSTANCE	mHInstance;
		HWND		mHWnd;
		UINT		mScreenWidth;
		UINT		mScreenHeight;

	protected:
	public:

	//Class functions
	private:
		static LRESULT CALLBACK WndProc( HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam );
		HRESULT					Update( float deltaTime );
		HRESULT					Render();

	protected:
	public:
		int		Run();
		HRESULT Initialize( HINSTANCE hInstance, int nCmdShow );
		void	Release();
				System();
				virtual ~System(); //why is this so fucked up man?
};
#endif
