#include "Graphics.h"

Graphics::Graphics()
{
	mHWnd			= 0;
	mScreenWidth	= 0;
	mScreenHeight	= 0;				

	mSwapChain		= nullptr;
	mDevice			= nullptr;
	mDeviceContext	= nullptr;

	mRenderTargetView	= nullptr;
	mDepthStencilView	= nullptr;

	mAssetManager		= nullptr;
}

Graphics::~Graphics()
{

}

//Load a static 3d asset to the AssetManager.
HRESULT Graphics::LoadStatic3dAsset( char* fileName, UINT &assetId )
{
	return mAssetManager->LoadStatic3dAsset( mDevice, fileName, assetId );
}

//Render a static 3d asset given the assetId
void Graphics::RenderStatic3dAsset( UINT assetId )
{
	mDeviceContext->OMSetRenderTargets( 1, &mRenderTargetView, mDepthStencilView );
	mDeviceContext->RSSetViewports( 1, &mStandardView );

	mDeviceContext->IASetPrimitiveTopology( D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP );

	UINT32 vertexSize				= sizeof( Vertex );
	UINT32 offset					= 0;

	ID3D11Buffer* buffersToSet[]	= { mAssetManager->mAssetContainer[assetId]->mVertexBuffer };
	mDeviceContext->IASetVertexBuffers( 0, 1, buffersToSet, &vertexSize, &offset );

	mDeviceContext->IASetInputLayout( mEffect->GetInputLayout() );

	mDeviceContext->VSSetShader( mEffect->GetVertexShader(), nullptr, 0 );
	mDeviceContext->HSSetShader( nullptr, nullptr, 0 );
	mDeviceContext->DSSetShader( nullptr, nullptr, 0 );
	mDeviceContext->GSSetShader( nullptr, nullptr, 0 );
	mDeviceContext->PSSetShader( mEffect->GetPixelShader(), nullptr, 0 );

	mDeviceContext->Draw( 4, 0 );

}

//Clear canvas and prepare for rendering.
void Graphics::BeginScene()
{
	static float clearColor[4] = { 0.0f, 0.0f, 0.0f, 1.0f };
	mDeviceContext->ClearRenderTargetView( mRenderTargetView, clearColor );
	mDeviceContext->ClearDepthStencilView( mDepthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0 );
}

//Finalize rendering.
void Graphics::EndScene()
{
	mSwapChain->Present( 0, 0 );
}

//Singleton for the Graphics dll.
Graphics* Graphics::GetInstance()
{
	static Graphics instance;
	return &instance;
}

//Initialize graphics interfaces.
HRESULT Graphics::Initialize( HWND hWnd, UINT screenWidth, UINT screenHeight )
{
	mHWnd			= hWnd;
	mScreenWidth	= screenWidth;
	mScreenHeight	= screenHeight;

	/////////////////////////
	// CREATE SWAPCHAIN
	/////////////////////////
	D3D_DRIVER_TYPE driverTypes[] = {	D3D_DRIVER_TYPE_HARDWARE,
										D3D_DRIVER_TYPE_WARP,
										D3D_DRIVER_TYPE_REFERENCE };

	DXGI_SWAP_CHAIN_DESC swapChainDesc;
	ZeroMemory( &swapChainDesc, sizeof( swapChainDesc ) );

	swapChainDesc.BufferCount							= 1;
	swapChainDesc.BufferDesc.Width						= screenWidth;
	swapChainDesc.BufferDesc.Height						= screenHeight;
	swapChainDesc.BufferDesc.Format						= DXGI_FORMAT_R8G8B8A8_UNORM;
	swapChainDesc.BufferDesc.RefreshRate.Numerator		= 60;
	swapChainDesc.BufferDesc.RefreshRate.Denominator	= 1;
	swapChainDesc.BufferUsage							= DXGI_USAGE_RENDER_TARGET_OUTPUT;
	swapChainDesc.OutputWindow							= hWnd;
	swapChainDesc.SampleDesc.Count						= 1;
	swapChainDesc.SampleDesc.Quality					= 0;
	swapChainDesc.Windowed								= true;

	D3D_FEATURE_LEVEL featureLevelsToTry[] = { D3D_FEATURE_LEVEL_11_0 };
	D3D_FEATURE_LEVEL initiatedFeatureLevel;

	HRESULT hr = E_FAIL;
	for(	UINT driverTypeIndex = 0;
			driverTypeIndex < ARRAYSIZE( driverTypes ) && FAILED( hr );
			driverTypeIndex++ )
		hr = D3D11CreateDeviceAndSwapChain(
			nullptr,
			driverTypes[driverTypeIndex],
			nullptr,
			D3D11_CREATE_DEVICE_DEBUG,
			featureLevelsToTry,
			ARRAYSIZE( featureLevelsToTry ),
			D3D11_SDK_VERSION,
			&swapChainDesc,
			&mSwapChain,
			&mDevice,
			&initiatedFeatureLevel,
			&mDeviceContext );

	if( FAILED( hr ) )
		return hr;
	
	/////////////////////////////////////////////
	// CREATE RENDERTARGETVIEW & BackBufferUAV
	/////////////////////////////////////////////

	ID3D11Texture2D* backBuffer;
	if( SUCCEEDED( hr = mSwapChain->GetBuffer( 0, __uuidof( ID3D11Texture2D ), (void**)&backBuffer ) ) )
	{
		hr = mDevice->CreateRenderTargetView( backBuffer, nullptr, &mRenderTargetView );
		SAFE_RELEASE( backBuffer );
	}

	if( FAILED( hr ) )
		return hr;

	//////////////////////////////
	// CREATE DEPTHSTENCILVIEW
	//////////////////////////////
	D3D11_TEXTURE2D_DESC	textureDesc;
	ID3D11Texture2D*		depthStencil;

	textureDesc.Width				= screenWidth;
	textureDesc.Height				= screenHeight;
	textureDesc.MipLevels			= 1;
	textureDesc.ArraySize			= 1;
	textureDesc.Format				= DXGI_FORMAT_D32_FLOAT;
	textureDesc.SampleDesc.Count		= 1;
	textureDesc.SampleDesc.Quality	= 0;
	textureDesc.Usage				= D3D11_USAGE_DEFAULT;
	textureDesc.BindFlags			= D3D11_BIND_DEPTH_STENCIL;
	textureDesc.CPUAccessFlags		= 0;
	textureDesc.MiscFlags			= 0;

	if( FAILED( hr = mDevice->CreateTexture2D( &textureDesc, nullptr, &depthStencil ) ) )
		return hr;

	D3D11_DEPTH_STENCIL_VIEW_DESC dDSV;
	ZeroMemory( &dDSV, sizeof( dDSV ) );
	dDSV.Format				= textureDesc.Format;
	dDSV.ViewDimension		= D3D11_DSV_DIMENSION_TEXTURE2D;
	dDSV.Texture2D.MipSlice	= 0;

	if( FAILED( hr = mDevice->CreateDepthStencilView( depthStencil, &dDSV, &mDepthStencilView ) ) )
		return hr;

	SAFE_RELEASE( depthStencil );

	//////////////////////////////
	// CREATE VIEWPORT
	//////////////////////////////
	mStandardView.Height	= (float)screenHeight;
	mStandardView.Width		= (float)screenWidth;
	mStandardView.MaxDepth	= 1.0f;
	mStandardView.MinDepth	= 0.0f;
	mStandardView.TopLeftX	= 0.0f;
	mStandardView.TopLeftY	= 0.0f;

	mAssetManager = new AssetManager;
	mAssetManager->Initialize( mDevice );

	mEffect	= new Effect;

	EffectInfo effectInfo;
	ZeroMemory( &effectInfo, sizeof( EffectInfo ) );
	effectInfo.fileName					= "../Content/Effects/Placeholder.hlsl";
	effectInfo.isVertexShaderIncluded	= true;
	effectInfo.isPixelShaderIncluded	= true;

	hr = mEffect->Intialize( mDevice, &effectInfo );

	return hr;
}

//Release all the stuff.
void Graphics::Release()
{
	SAFE_RELEASE( mSwapChain );
	SAFE_RELEASE( mDevice );
	SAFE_RELEASE( mDeviceContext );
	SAFE_RELEASE( mRenderTargetView );
	SAFE_RELEASE( mDepthStencilView );

	mAssetManager->Release();
	mEffect->Release();

	SAFE_DELETE( mAssetManager );
	SAFE_DELETE( mEffect );
}