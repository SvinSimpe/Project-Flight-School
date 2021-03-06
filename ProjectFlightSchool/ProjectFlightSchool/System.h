#ifndef SYSTEM_H
#define SYSTEM_H

#include "Input.h"
#include "Graphics.h"
#include "Game.h"
#include "Player.h"
#include "RenderManager.h"

#define DELTA_TIME_CAP 0.2f

struct InitializeFile
{
	bool fullscreen;
};

class System
{
	//Class members
	private:
		HINSTANCE	mHInstance;
		HWND		mHWnd;
		UINT		mScreenWidth;
		UINT		mScreenHeight;
		bool		mFullscreen;
		Game*		mGame;
		Timer*		mTimer;

	protected:
	public:

	//Class functions
	private:
		void					EventListener( IEventPtr newEvent );
		static LRESULT CALLBACK WndProc( HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam );
		HRESULT					Update( float deltaTime );
		HRESULT					Render( float deltaTime );

	protected:
	public:
		int		Run();
		HRESULT Initialize( HINSTANCE hInstance, int nCmdShow );
		void	Release();
				System();
		virtual ~System();
};
#endif
