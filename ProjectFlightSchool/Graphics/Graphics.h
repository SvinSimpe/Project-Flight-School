#ifndef GRAPHICS_H
#define GRAPHICS_H

#pragma comment( lib, "d3d11.lib" )
//#pragma comment( lib, "d3dcompiler.lib" )

#include <d3d11.h>
//#include <d3dcompiler.h>

#define SAFE_RELEASE( x ) if( x ) { ( x )->Release(); ( x ) = nullptr; }

#ifdef COMPILE_LIBRARY
	#define LIBRARY_EXPORT __declspec( dllexport )
#else
	#define LIBRARY_EXPORT __declspec( dllimport )
#endif

class LIBRARY_EXPORT Graphics
{
	//Class members
	private:
		HWND		mHWnd;
		UINT		mScreenWidth;
		UINT		mScreenHeight;				

		IDXGISwapChain*			mSwapChain;
		ID3D11Device*			mDevice;
		ID3D11DeviceContext*	mDeviceContext;

		ID3D11RenderTargetView*	mRenderTargetView;
		ID3D11DepthStencilView*	mDepthStencilView;

	protected:
	public:

	//Class functions
	private:
		Graphics();
		virtual	~Graphics();

	protected:
	public:
		void	BeginScene();
		void	EndScene();
		static	Graphics* GetInstance();
		HRESULT Initialize( HWND hWnd, UINT screenWidth, UINT screenHeight );
		void	Release();
};
#endif