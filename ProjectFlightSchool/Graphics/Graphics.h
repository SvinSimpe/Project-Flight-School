#ifndef GRAPHICS_H
#define GRAPHICS_H

#pragma comment( lib, "d3d11.lib" )
#pragma comment( lib, "d3dcompiler.lib" )

#include "AssetManager.h"
#include "Effect.h"
#include "CbufferPerFrame.h"
#include "CbufferPerObject.h"
#include "Camera.h"
#include "WICTextureLoader.h"

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
		D3D11_VIEWPORT			mStandardView;
		ID3D11Buffer*			mCbufferPerFrame;
		ID3D11Buffer*			mCbufferPerObject;

		AssetManager*			mAssetManager;
		Effect*					mEffect;
		Camera*					mCamera;

	protected:
	public:

	//Class functions
	private:
		Graphics();
		virtual	~Graphics();

		HRESULT MapBuffer( ID3D11Buffer* buffer, void* data, int size );

	protected:
	public:
		HRESULT LoadTextureFromFile ( wchar_t* fileName, ID3D11Resource** texture, ID3D11ShaderResourceView** srv, size_t size = 0 );
		HRESULT LoadStatic3dAsset( char* fileName, UINT &assetId );

		void RenderStatic3dAsset( UINT assetId );
		void RenderStatic3dAsset( UINT assetId, float x, float y, float z );
		void RenderStatic3dAsset( UINT assetId, DirectX::XMFLOAT3 position, DirectX::XMFLOAT3 rotation );

		void	BeginScene();
		void	EndScene();
		static	Graphics* GetInstance();
		HRESULT Initialize( HWND hWnd, UINT screenWidth, UINT screenHeight );
		void	Release();
};
#endif