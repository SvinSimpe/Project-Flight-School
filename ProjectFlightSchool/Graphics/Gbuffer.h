#ifndef GBUFFER_H
#define GBUFFER_H

#include <d3d11.h>

#define SAFE_RELEASE( x )					\
	if( x )									\
		( x )->Release(), ( x ) = nullptr;

struct Gbuffer
{
	private:
	protected:
	public:
		ID3D11RenderTargetView*		mRenderTargetView;
		ID3D11ShaderResourceView*	mShaderResourceView;

	private:
	protected:
	public:
		HRESULT Initialize( ID3D11Device* device, UINT width, UINT height )
		{
			////////////////////////////////////////////////////////
			// Initialize RenderTargetView and ShaderResourceView
			////////////////////////////////////////////////////////

			HRESULT hr = S_OK;

			ID3D11Texture2D* targetTex;

			D3D11_TEXTURE2D_DESC td;
			ZeroMemory( &td, sizeof( td ) );

			td.Width			= width;
			td.Height			= height;
			td.BindFlags		= D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
			td.Usage			= D3D11_USAGE_DEFAULT;
			td.MipLevels		= 1;
			td.ArraySize		= 1;
			td.SampleDesc.Count	= 1;
			td.Format			= DXGI_FORMAT_R16G16B16A16_FLOAT;

			
			hr = device->CreateTexture2D( &td, nullptr, &targetTex );

			D3D11_RENDER_TARGET_VIEW_DESC rtd;
			ZeroMemory( &rtd, sizeof( rtd ) );

			rtd.Format			= td.Format;
			rtd.ViewDimension	= D3D11_RTV_DIMENSION_TEXTURE2D;

			hr = device->CreateRenderTargetView( targetTex, &rtd, &mRenderTargetView );
			
			hr = device->CreateShaderResourceView( targetTex, nullptr, &mShaderResourceView );

			SAFE_RELEASE( targetTex );

			return hr;
		}

		void Release()
		{
			SAFE_RELEASE( mRenderTargetView );
			SAFE_RELEASE( mShaderResourceView );
		}

		RTPack()
		{	
			mRenderTargetView	= nullptr;
			mShaderResourceView = nullptr;
		}

		~RTPack()
		{
		}
};

#endif