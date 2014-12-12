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
	mCbufferPerFrame	= nullptr;

	mAssetManager		= nullptr;
}

Graphics::~Graphics()
{

}

//Loads a texture from file, the filename can be expressed as a string put with L prefix e.g L"Hello World", texture and SRV are both optional, size = Maximum size of buffer.
HRESULT Graphics::LoadTextureFromFile ( wchar_t* fileName, ID3D11Resource** texture, ID3D11ShaderResourceView** srv, size_t size )
{
	HRESULT hr = S_OK;
	hr = CreateWICTextureFromFile(mDevice, mDeviceContext, fileName, texture, srv, size );
	return hr;
}
//Map buffer
HRESULT Graphics::MapBuffer( ID3D11Buffer* buffer, void* data, int size )
{
	D3D11_MAPPED_SUBRESOURCE mapRes;
	mDeviceContext->Map( buffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mapRes );
	memcpy( mapRes.pData, data, size );
	mDeviceContext->Unmap( buffer, 0 );

	return S_OK;
}

//Load a static 3d asset to the AssetManager.
HRESULT Graphics::LoadStatic3dAsset( char* fileName, AssetID &assetId )
{
	return mAssetManager->LoadStatic3dAsset( mDevice, fileName, assetId );
}

HRESULT Graphics::LoadAnimated3dAsset( char* fileName, AssetID skeletonId, AssetID &assetId )
{
	return mAssetManager->LoadAnimated3dAsset( mDevice, fileName, skeletonId, assetId );
}

HRESULT Graphics::LoadSkeletonAsset( std::string filePath, std::string fileName, AssetID &assetId )
{
	return mAssetManager->LoadSkeletonAsset( filePath, fileName, assetId );
}

HRESULT Graphics::LoadAnimationAsset( std::string filePath, std::string fileName, AssetID &assetId )
{
	return mAssetManager->LoadAnimationAsset( filePath, fileName, assetId );
}

//Render a static 3d asset given the assetId
void Graphics::RenderStatic3dAsset( AssetID assetId )
{
	mDeviceContext->IASetPrimitiveTopology( D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST );

	UINT32 vertexSize				= sizeof( StaticVertex );
	UINT32 offset					= 0;
	ID3D11Buffer* buffersToSet[]	= { ( (Static3dAsset*)mAssetManager->mAssetContainer[assetId] )->mVertexBuffer };
	mDeviceContext->IASetVertexBuffers( 0, 1, buffersToSet, &vertexSize, &offset );

	mDeviceContext->IASetInputLayout( mStaticEffect->GetInputLayout() );

	mDeviceContext->VSSetShader( mStaticEffect->GetVertexShader(), nullptr, 0 );
	mDeviceContext->HSSetShader( nullptr, nullptr, 0 );
	mDeviceContext->DSSetShader( nullptr, nullptr, 0 );
	mDeviceContext->GSSetShader( nullptr, nullptr, 0 );
	mDeviceContext->PSSetShader( mStaticEffect->GetPixelShader(), nullptr, 0 );

	//Map CbufferPerObject
	CbufferPerObject data;
	data.worldMatrix = DirectX::XMMatrixIdentity();
	MapBuffer( mCbufferPerObject, &data, sizeof( CbufferPerObject ) );

	mDeviceContext->VSSetConstantBuffers( 1, 1, &mCbufferPerObject );

	mDeviceContext->Draw( ( (Static3dAsset*)mAssetManager->mAssetContainer[assetId] )->mVertexCount, 0 );
}

void Graphics::RenderStatic3dAsset( AssetID assetId, float x, float y, float z )
{
	mDeviceContext->IASetPrimitiveTopology( D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST );

	UINT32 vertexSize				= sizeof( StaticVertex );
	UINT32 offset					= 0;
	ID3D11Buffer* buffersToSet[]	= { ( (Static3dAsset*)mAssetManager->mAssetContainer[assetId] )->mVertexBuffer };
	mDeviceContext->IASetVertexBuffers( 0, 1, buffersToSet, &vertexSize, &offset );

	mDeviceContext->IASetInputLayout( mStaticEffect->GetInputLayout() );

	mDeviceContext->VSSetShader( mStaticEffect->GetVertexShader(), nullptr, 0 );
	mDeviceContext->HSSetShader( nullptr, nullptr, 0 );
	mDeviceContext->DSSetShader( nullptr, nullptr, 0 );
	mDeviceContext->GSSetShader( nullptr, nullptr, 0 );
	mDeviceContext->PSSetShader( mStaticEffect->GetPixelShader(), nullptr, 0 );

	//Map CbufferPerObject
	CbufferPerObject data;
	data.worldMatrix = DirectX::XMMatrixTranspose( DirectX::XMMatrixTranslation( x, y, z ) );
	MapBuffer( mCbufferPerObject, &data, sizeof( CbufferPerObject ) );

	mDeviceContext->VSSetConstantBuffers( 1, 1, &mCbufferPerObject );

	mDeviceContext->Draw( ( (Static3dAsset*)mAssetManager->mAssetContainer[assetId] )->mVertexCount, 0 );
}

void Graphics::RenderStatic3dAsset( AssetID assetId, DirectX::XMFLOAT3 position, DirectX::XMFLOAT3 rotation )
{
	mDeviceContext->IASetPrimitiveTopology( D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST );

	UINT32 vertexSize				= sizeof( StaticVertex );
	UINT32 offset					= 0;
	ID3D11Buffer* buffersToSet[]	= { ( (Static3dAsset*)mAssetManager->mAssetContainer[assetId] )->mVertexBuffer };
	mDeviceContext->IASetVertexBuffers( 0, 1, buffersToSet, &vertexSize, &offset );

	mDeviceContext->IASetInputLayout( mStaticEffect->GetInputLayout() );

	mDeviceContext->VSSetShader( mStaticEffect->GetVertexShader(), nullptr, 0 );
	mDeviceContext->HSSetShader( nullptr, nullptr, 0 );
	mDeviceContext->DSSetShader( nullptr, nullptr, 0 );
	mDeviceContext->GSSetShader( nullptr, nullptr, 0 );
	mDeviceContext->PSSetShader( mStaticEffect->GetPixelShader(), nullptr, 0 );

	//Map CbufferPerObject
	CbufferPerObject data;
	data.worldMatrix = DirectX::XMMatrixTranspose( 
		DirectX::XMMatrixRotationRollPitchYaw( rotation.x, rotation.y, rotation.z ) *										
		DirectX::XMMatrixTranslation( position.x, position.y, position.z ) );
	MapBuffer( mCbufferPerObject, &data, sizeof( CbufferPerObject ) );

	mDeviceContext->VSSetConstantBuffers( 1, 1, &mCbufferPerObject );

	mDeviceContext->Draw( ( (Static3dAsset*)mAssetManager->mAssetContainer[assetId] )->mVertexCount, 0 );
}

void Graphics::RenderStatic3dAsset( AssetID assetId, XMFLOAT4X4* world )
{
	mDeviceContext->IASetPrimitiveTopology( D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST );

	UINT32 vertexSize				= sizeof( StaticVertex );
	UINT32 offset					= 0;
	ID3D11Buffer* buffersToSet[]	= { ( (Static3dAsset*)mAssetManager->mAssetContainer[assetId] )->mVertexBuffer };
	mDeviceContext->IASetVertexBuffers( 0, 1, buffersToSet, &vertexSize, &offset );

	mDeviceContext->IASetInputLayout( mStaticEffect->GetInputLayout() );

	mDeviceContext->VSSetShader( mStaticEffect->GetVertexShader(), nullptr, 0 );
	mDeviceContext->HSSetShader( nullptr, nullptr, 0 );
	mDeviceContext->DSSetShader( nullptr, nullptr, 0 );
	mDeviceContext->GSSetShader( nullptr, nullptr, 0 );
	mDeviceContext->PSSetShader( mStaticEffect->GetPixelShader(), nullptr, 0 );

	//Map CbufferPerObject
	CbufferPerObject data;
	data.worldMatrix = DirectX::XMMatrixTranspose( DirectX::XMLoadFloat4x4( world ) );
	MapBuffer( mCbufferPerObject, &data, sizeof( CbufferPerObject ) );

	mDeviceContext->VSSetConstantBuffers( 1, 1, &mCbufferPerObject );

	mDeviceContext->Draw( ( (Static3dAsset*)mAssetManager->mAssetContainer[assetId] )->mVertexCount, 0 );
}

void Graphics::RenderAnimated3dAsset( AssetID modelAssetId, AssetID animationAssetId, float &animationTime )
{
	Animated3dAsset*	model		= (Animated3dAsset*)mAssetManager->mAssetContainer[modelAssetId];
	Skeleton*			skeleton	= &( (SkeletonAsset*)mAssetManager->mAssetContainer[model->mSkeletonId] )->mSkeleton;
	AnimationData*		animation	= &( (AnimationAsset*)mAssetManager->mAssetContainer[animationAssetId] )->mAnimationData;

	int	framesJumped	 = 0;
	bool animationLooped = false;

	if( animationTime > (float)animation->AnimLength / 60.0f )
	{
		animationTime	-= ( (float)animation->AnimLength / 60.0f - 1.0f / 60.0f );
		animationLooped  = true;
	}

	float calcTime = animationTime * 60.0f;

	while( calcTime > 1.0f )
	{
		calcTime -= 1.0f;
		framesJumped++;
	}

	//std::cout << framesJumped << std::endl;
	for( int i = 0; i < (int)skeleton->joints.size(); i++ )
	{
		int					lastFrame = 1;
		DirectX::XMFLOAT4X4	previousMatrix = skeleton->joints.at(i).originalMatrix;


		for( int j = 0; j < (int)animation->joints.at(i).keys.size(); j++ )
		{
			//If no keyframes exist apply static pose
			if( animation->joints.at(i).keys.size() == 1 && animation->joints.at(i).keys.at(j) == 0 )
			{
				if( skeleton->joints.at(i).parentIndex == -1 )
				{
					model->mCurrentBoneTransforms[i] = animation->joints.at(i).matricies.at( 0 );
				}
				else
				{
					DirectX::XMMATRIX child		= DirectX::XMLoadFloat4x4( &animation->joints.at(i).matricies.at( 0 ) );
					DirectX::XMMATRIX parent	= DirectX::XMLoadFloat4x4( &model->mCurrentBoneTransforms[animation->joints.at(i).parentIndex] );

					DirectX::XMStoreFloat4x4( &model->mCurrentBoneTransforms[i], child * parent );
				}
			}
			//Find next keyframe and interpolate previousMatrix with next matrix in animation based on key.
			else
			{
				if( animationLooped )
					for( int frames = 0; frames < animation->AnimLength; frames++ )
					{
						if( animation->joints.at(i).keys.at(j) == frames )
						{
							previousMatrix	= animation->joints.at(i).matricies.at(j);
							//lastFrame		= frames;
						}
					}
				else
					for( int frames = 0; frames < framesJumped; frames++ )
					{
						if( animation->joints.at(i).keys.at(j) == frames )
						{
							previousMatrix	= animation->joints.at(i).matricies.at(j);
							lastFrame		= frames;
						}
					}

				if( animation->joints.at(i).keys.at(j) == framesJumped )
				{
					previousMatrix	= animation->joints.at(i).matricies.at(j);
					lastFrame		= framesJumped;

					DirectX::XMMATRIX child		= DirectX::XMLoadFloat4x4( &previousMatrix );
					DirectX::XMMATRIX parent	= animation->joints.at(i).parentIndex == -1 ? DirectX::XMMatrixIdentity() :
													DirectX::XMLoadFloat4x4( &model->mCurrentBoneTransforms[animation->joints.at(i).parentIndex] );

					DirectX::XMStoreFloat4x4( &model->mCurrentBoneTransforms[i], child * parent );
					break;
				}
				else if( animation->joints.at(i).keys.at(j) > framesJumped )
				{
					float interpolation					=	(float)( animationTime * 60.0f - lastFrame ) /
															(float)( animation->joints.at(i).keys.at(j) - lastFrame );

					DirectX::XMMATRIX targetMatrix		= DirectX::XMLoadFloat4x4( &animation->joints.at(i).matricies.at(j) );
					DirectX::XMMATRIX child				= DirectX::XMLoadFloat4x4( &previousMatrix );
					
					DirectX::XMVECTOR targetComp[3];
					DirectX::XMVECTOR childComp[3];

					DirectX::XMMatrixDecompose( &targetComp[0], &targetComp[1], &targetComp[2], targetMatrix );
					DirectX::XMMatrixDecompose( &childComp[0], &childComp[1], &childComp[2], child );

					child = DirectX::XMMatrixAffineTransformation(	DirectX::XMVectorLerp( childComp[0], targetComp[0], interpolation ),
																	DirectX::XMVectorZero(),
																	DirectX::XMQuaternionSlerp( childComp[1], targetComp[1], interpolation ),
																	DirectX::XMVectorLerp( childComp[2], targetComp[2], interpolation ) );			

					DirectX::XMMATRIX parent	= animation->joints.at(i).parentIndex == -1 ? DirectX::XMMatrixIdentity() :
													DirectX::XMLoadFloat4x4( &model->mCurrentBoneTransforms[animation->joints.at(i).parentIndex] );

					DirectX::XMStoreFloat4x4( &model->mCurrentBoneTransforms[i], child * parent );
					break;
				}
				else
				{
					DirectX::XMMATRIX child		= DirectX::XMLoadFloat4x4( &previousMatrix );
					DirectX::XMMATRIX parent	= animation->joints.at(i).parentIndex == -1 ? DirectX::XMMatrixIdentity() :
													DirectX::XMLoadFloat4x4( &model->mCurrentBoneTransforms[animation->joints.at(i).parentIndex] );

					DirectX::XMStoreFloat4x4( &model->mCurrentBoneTransforms[i], child * parent );
				}
			}
		}
	}


	//////////////////////////////////////////////////////////////////
	//						RENDER CALL
	//////////////////////////////////////////////////////////////////

	mDeviceContext->IASetPrimitiveTopology( D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST );

	UINT32 vertexSize				= sizeof( AnimatedVertex );
	UINT32 offset					= 0;
	ID3D11Buffer* buffersToSet[]	= { model->mVertexBuffer };
	mDeviceContext->IASetVertexBuffers( 0, 1, buffersToSet, &vertexSize, &offset );

	mDeviceContext->IASetInputLayout( mAnimatedEffect->GetInputLayout() );

	mDeviceContext->VSSetShader( mAnimatedEffect->GetVertexShader(), nullptr, 0 );
	mDeviceContext->HSSetShader( nullptr, nullptr, 0 );
	mDeviceContext->DSSetShader( nullptr, nullptr, 0 );
	mDeviceContext->GSSetShader( nullptr, nullptr, 0 );
	mDeviceContext->PSSetShader( mAnimatedEffect->GetPixelShader(), nullptr, 0 );

	//Map CbufferPerObject
	CbufferPerObjectAnimated data;
	data.worldMatrix = DirectX::XMMatrixIdentity();
	for( int i = 0; i < NUM_SUPPORTED_JOINTS; i++ )
		data.boneTransforms[i] = DirectX::XMMatrixIdentity();
	for( int i = 1; i < skeleton->nrOfJoints; i++ )
		data.boneTransforms[i] = DirectX::XMMatrixTranspose( DirectX::XMLoadFloat4x4( &model->mCurrentBoneTransforms[i] ) );
	MapBuffer( mCbufferPerObjectAnimated, &data, sizeof( CbufferPerObjectAnimated ) );

	mDeviceContext->VSSetConstantBuffers( 1, 1, &mCbufferPerObjectAnimated );

	mDeviceContext->Draw( model->mVertexCount, 0 );
}

Camera* Graphics::GetCamera()
{
	return mCamera;
}

void Graphics::SetNDCSpaceCoordinates( float &mousePositionX, float &mousePositionY )
{
	//Calculate mouse position in NDC space
	mousePositionX	= ( ( 2.0f *  mousePositionX ) / mScreenWidth  - 1.0f );
	mousePositionY	= ( ( 2.0f * -mousePositionY ) / mScreenHeight + 1.0f );
}

void Graphics::SetInverseViewMatrix( XMMATRIX &inverseViewMatrix )
{
	inverseViewMatrix = mCamera->GetInverseViewMatrix();
}

void Graphics::SetInverseProjectionMatrix( XMMATRIX &projectionViewMatrix )
{
	projectionViewMatrix = mCamera->GetInverseProjectionMatrix();
}

void Graphics::SetEyePosition( XMFLOAT3 &eyePosition )
{
	mCamera->SetEyePosition( eyePosition );
}

void Graphics::SetFocus( XMFLOAT3 &focusPoint )
{
	mCamera->SetFocus( focusPoint );
}

//Clear canvas and prepare for rendering.
void Graphics::BeginScene()
{
	mCamera->Update();

	static float clearColor[4] = { 0.3f, 0.3f, 0.3f, 1.0f };
	mDeviceContext->ClearRenderTargetView( mRenderTargetView, clearColor );
	mDeviceContext->ClearDepthStencilView( mDepthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0 );

	mDeviceContext->OMSetRenderTargets( 1, &mRenderTargetView, mDepthStencilView );
	mDeviceContext->RSSetViewports( 1, &mStandardView );

	//Map CbufferPerFrame
	CbufferPerFrame data;
	data.viewMatrix			= mCamera->GetViewMatrix();
	data.projectionMatrix	= mCamera->GetProjMatrix();
	MapBuffer( mCbufferPerFrame, &data, sizeof( CbufferPerFrame ) );

	mDeviceContext->VSSetConstantBuffers( 0, 1, &mCbufferPerFrame );
}

//Finalize rendering.
void Graphics::EndScene()
{
	mSwapChain->Present( 1, 0 );
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
	_CrtSetDbgFlag( _CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF );

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
	textureDesc.SampleDesc.Count	= 1;
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

	///////////////////////////////
	// CREATE CBUFFERPERFRAME
	///////////////////////////////
	D3D11_BUFFER_DESC bufferDesc;
	ZeroMemory( &bufferDesc, sizeof( bufferDesc ) );
	bufferDesc.BindFlags		= D3D11_BIND_CONSTANT_BUFFER;
	bufferDesc.ByteWidth		= sizeof( CbufferPerFrame );
	bufferDesc.CPUAccessFlags	= D3D11_CPU_ACCESS_WRITE;
	bufferDesc.Usage			= D3D11_USAGE_DYNAMIC;

	hr = mDevice->CreateBuffer( &bufferDesc, nullptr, &mCbufferPerFrame );

	///////////////////////////////
	// CREATE CBUFFERPEROBJECT
	///////////////////////////////
	bufferDesc.ByteWidth				= sizeof( CbufferPerObject );

	hr = mDevice->CreateBuffer( &bufferDesc, nullptr, &mCbufferPerObject );

	///////////////////////////////////////
	// CREATE CBUFFERPEROBJECTANIMATED
	///////////////////////////////////////
	bufferDesc.ByteWidth				= sizeof( CbufferPerObjectAnimated );

	hr = mDevice->CreateBuffer( &bufferDesc, nullptr, &mCbufferPerObjectAnimated );

	//AssetManager
	mAssetManager = new AssetManager;
	mAssetManager->Initialize( mDevice );

	//Effect
	mStaticEffect	= new Effect;

	EffectInfo effectInfo;
	ZeroMemory( &effectInfo, sizeof( EffectInfo ) );
	effectInfo.fileName					= "../Content/Effects/Placeholder.hlsl";
	effectInfo.vertexType				= STATIC_VERTEX_TYPE;
	effectInfo.isVertexShaderIncluded	= true;
	effectInfo.isPixelShaderIncluded	= true;

	hr = mStaticEffect->Intialize( mDevice, &effectInfo );

	mAnimatedEffect	= new Effect;
	effectInfo.fileName		= "../Content/Effects/PlaceholderAni.hlsl";
	effectInfo.vertexType	= ANIMATED_VERTEX_TYPE;

	hr = mAnimatedEffect->Intialize( mDevice, &effectInfo );
	
	//Camera
	mCamera = new Camera;

	CameraInfo cameraInfo;
	ZeroMemory( &cameraInfo, sizeof( cameraInfo ) );
	cameraInfo.eyePos		= DirectX::XMFLOAT4( 0.0f, 27.0f, -22.25f, 0.0f );
	cameraInfo.focusPoint	= DirectX::XMFLOAT4( 0.0f, 0.0f, 0.0f, 1.0f );
	cameraInfo.up			= DirectX::XMFLOAT4( 0.0f, 1.0f, 0.0f, 1.0f );
	cameraInfo.width		= (float)screenWidth;
	cameraInfo.height		= (float)screenHeight;
	cameraInfo.foVY			= 0.75f;
	cameraInfo.nearZ		= 0.1f;
	cameraInfo.farZ			= 1000.0f;

	hr = mCamera->Initialize( &cameraInfo );

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
	SAFE_RELEASE( mCbufferPerFrame );
	SAFE_RELEASE( mCbufferPerObject );

	mAssetManager->Release();
	mStaticEffect->Release();
	mAnimatedEffect->Release();
	mCamera->Release();

	SAFE_DELETE( mAssetManager );
	SAFE_DELETE( mStaticEffect );
	SAFE_DELETE( mAnimatedEffect );
	SAFE_DELETE( mCamera );

}