#include "Graphics.h"

Graphics::Graphics()
{
	mHWnd			= 0;
	mScreenWidth	= 0;
	mScreenHeight	= 0;
	mFullscreen		= false;

	mSwapChain		= nullptr;
	mDevice			= nullptr;
	mDeviceContext	= nullptr;

	mRenderTargetView				= nullptr;
	mDepthStencilView				= nullptr;
	mLightStructuredBuffer			= nullptr;

	mAssetManager				= nullptr;
	mIsDeveloperCameraActive	= false;

	for( int i = 0; i < BUFFERS_AMOUNT; i++ )
		mBuffers[i] = nullptr;

	for( int i = 0; i < BLEND_STATES_AMOUNT; i++ )
		mBlendStates[i] = nullptr;

	for( int i = 0; i < DEPTHSTENCILS_AMOUNT; i++ )
		mDepthStencils[i] = nullptr;

	for( int i = 0; i < RASTERIZER_STATES_AMOUNT; i++ )
		mRasterizerState[i] = nullptr;

	for( int i = 0; i < EFFECTS_AMOUNT; i++ )
		mEffects[i] = nullptr;

	for( int i = 0; i < SAMPLERS_AMOUNT; i++ )
		mSamplerStates[i] = nullptr;

	for( int i = 0; i < CAMERAS_AMOUNT; i++ )
		mCamera[i] = nullptr;



	mNumPointLights				= 0;
}

Graphics::~Graphics()
{

}

HRESULT Graphics::InitializeDepthStencilStates()
{
	HRESULT hr = S_OK;
	/////////////////////////////////////
	// CREATE DEPTHSTENCILSTATE DISABLE
	/////////////////////////////////////
	D3D11_DEPTH_STENCIL_DESC depthStencilDesc;
	ZeroMemory( &depthStencilDesc, sizeof(depthStencilDesc) );

	depthStencilDesc.DepthEnable					= false;
	depthStencilDesc.DepthWriteMask					= D3D11_DEPTH_WRITE_MASK_ALL;
	depthStencilDesc.DepthFunc						= D3D11_COMPARISON_LESS;
	depthStencilDesc.StencilEnable					= false;
	depthStencilDesc.StencilReadMask				= 0xFF;
	depthStencilDesc.StencilWriteMask				= 0xFF;
	depthStencilDesc.FrontFace.StencilFailOp		= D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.FrontFace.StencilDepthFailOp	= D3D11_STENCIL_OP_INCR;
	depthStencilDesc.FrontFace.StencilPassOp		= D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.FrontFace.StencilFunc			= D3D11_COMPARISON_ALWAYS;
	depthStencilDesc.BackFace.StencilFailOp			= D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.BackFace.StencilDepthFailOp	= D3D11_STENCIL_OP_DECR;
	depthStencilDesc.BackFace.StencilPassOp			= D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.BackFace.StencilFunc			= D3D11_COMPARISON_ALWAYS;

	if ( FAILED( hr = mDevice->CreateDepthStencilState( &depthStencilDesc, &mDepthStencils[DEPTHSTENCILS_DISABLED] ) ) )
		return hr;

	////////////////////////////////////
	// CREATE DEPTHSTENCILSTATE ENABLE
	////////////////////////////////////
	depthStencilDesc.DepthEnable					= true;

	if ( FAILED( hr =mDevice->CreateDepthStencilState( &depthStencilDesc, &mDepthStencils[DEPTHSTENCILS_ENABLED] ) ) )
		return hr;

	////////////////////////////////////
	// CREATE DEPTHSTENCILSTATE MASK_DISABLED
	////////////////////////////////////
	depthStencilDesc.DepthWriteMask					= D3D11_DEPTH_WRITE_MASK_ZERO;

	if ( FAILED( hr = mDevice->CreateDepthStencilState( &depthStencilDesc, &mDepthStencils[DEPTHSTENCILS_MASK_DISABLED] ) ) )
		return hr;

	return hr;
}

HRESULT Graphics::InitializeSamplerStates()
{
	HRESULT hr = S_OK;
	//////////////////////////////
	// CREATE POINT SAMPLER
	//////////////////////////////
	D3D11_SAMPLER_DESC samplerDesc;
	ZeroMemory( &samplerDesc, sizeof( samplerDesc ) );
	samplerDesc.Filter			= D3D11_FILTER_MIN_MAG_MIP_POINT;
	samplerDesc.AddressU		= D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressV		= D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressW		= D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.ComparisonFunc	= D3D11_COMPARISON_ALWAYS;
	samplerDesc.MaxAnisotropy	= 1;
	samplerDesc.MaxLOD			= D3D11_FLOAT32_MAX;
	samplerDesc.MinLOD			= 0.0f;
	samplerDesc.MipLODBias		= 0.0f;

	if( FAILED( hr = mDevice->CreateSamplerState( &samplerDesc, &mSamplerStates[SAMPLERS_POINT] ) ) )
		return hr;

	//////////////////////////////
	// CREATE LINEAR SAMPLER
	//////////////////////////////
	samplerDesc.Filter			= D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	
	return hr = mDevice->CreateSamplerState( &samplerDesc, &mSamplerStates[SAMPLERS_LINEAR] );
}

HRESULT Graphics::InitializeBuffers()
{
	HRESULT hr = S_OK;

	///////////////////////////////
	// CREATE CBUFFERPERFRAME
	///////////////////////////////
	D3D11_BUFFER_DESC bufferDesc;
	ZeroMemory( &bufferDesc, sizeof( bufferDesc ) );
	bufferDesc.BindFlags		= D3D11_BIND_CONSTANT_BUFFER;
	bufferDesc.ByteWidth		= sizeof( CbufferPerFrame );
	bufferDesc.CPUAccessFlags	= D3D11_CPU_ACCESS_WRITE;
	bufferDesc.Usage			= D3D11_USAGE_DYNAMIC;

	if( FAILED( hr = mDevice->CreateBuffer( &bufferDesc, nullptr, &mBuffers[BUFFERS_CBUFFER_PER_FRAME] ) ) )
		return hr;

	//////////////////////////////////
	// CREATE CBUFFERPERFRAME SHADOW
	//////////////////////////////////

	bufferDesc.ByteWidth= sizeof( CbufferPerFrameShadow );

	if( FAILED( hr = mDevice->CreateBuffer( &bufferDesc, nullptr, &mBuffers[BUFFERS_CBUFFER_PER_FRAME_SHADOW] ) ) )
		return hr;

	///////////////////////////////
	// CREATE CBUFFERPEROBJECT
	///////////////////////////////
	bufferDesc.ByteWidth = sizeof( CbufferPerObject );

	if( FAILED( hr = mDevice->CreateBuffer( &bufferDesc, nullptr, &mBuffers[BUFFERS_CBUFFER_PER_OBJECT] ) ) )
		return hr;

	///////////////////////////////////////
	// CREATE CBUFFERPEROBJECT2D
	///////////////////////////////////////
	bufferDesc.ByteWidth = sizeof( CbufferPerObject2D );

	if( FAILED( hr = mDevice->CreateBuffer( &bufferDesc, nullptr, &mBuffers[BUFFERS_CBUFFER_PER_OBJECT_2D] ) ) )
		return hr;

	///////////////////////////////////////
	// CREATE CBUFFERPEROBJECTANIMATED
	///////////////////////////////////////
	bufferDesc.ByteWidth = sizeof( CbufferPerObjectAnimated );

	hr = mDevice->CreateBuffer( &bufferDesc, nullptr, &mBuffers[BUFFERS_CBUFFER_PER_OBJECT_ANIMATED] );

	//InstancedAnimatedData cbuffer 
	D3D11_BUFFER_DESC bufferInstancedDesc;
	ZeroMemory( &bufferInstancedDesc, sizeof( bufferInstancedDesc ) );
	bufferInstancedDesc.BindFlags			= D3D11_BIND_CONSTANT_BUFFER;
	bufferInstancedDesc.CPUAccessFlags		= D3D11_CPU_ACCESS_WRITE;
	bufferInstancedDesc.Usage				= D3D11_USAGE_DYNAMIC;
	bufferInstancedDesc.ByteWidth			= sizeof( CbufferPerObjectAnimated ) * MAX_ANIM_INSTANCE_BATCH;

	if( FAILED( hr = mDevice->CreateBuffer( &bufferInstancedDesc, nullptr, &mBuffers[BUFFERS_CBUFFER_PER_INSTANCED_ANIMATED] ) ) )
		return hr;

	//InstancedObject buffer
	bufferInstancedDesc.BindFlags			= D3D11_BIND_VERTEX_BUFFER;
	bufferInstancedDesc.CPUAccessFlags		= D3D11_CPU_ACCESS_WRITE;
	bufferInstancedDesc.Usage				= D3D11_USAGE_DYNAMIC;
	bufferInstancedDesc.ByteWidth			= sizeof( StaticInstance ) * MAX_STATIC3D_INSTANCE_BATCH;
	
	if( FAILED( hr = mDevice->CreateBuffer( &bufferInstancedDesc, nullptr, &mBuffers[BUFFERS_STATIC3D_PER_INSTANCED_OBJECT] ) ) )
		return hr;

	//Billboard buffer instanced
	bufferInstancedDesc.ByteWidth = sizeof( Vertex12 ) * MAX_BILLBOARD_BATCH;

	if( FAILED( hr = mDevice->CreateBuffer( &bufferInstancedDesc, nullptr, &mBuffers[BUFFERS_BILLBOARD] ) ) )
		return hr;

	//Single vertex buffer used for billboarding
	bufferInstancedDesc.BindFlags			= D3D11_BIND_VERTEX_BUFFER;
	bufferInstancedDesc.CPUAccessFlags		= 0;
	bufferInstancedDesc.Usage				= D3D11_USAGE_DEFAULT;
	bufferInstancedDesc.ByteWidth			= sizeof( Vertex12 );

	Vertex12 gv;
	gv.position[0] = 0.0f;
	gv.position[1] = 0.0f;
	gv.position[2] = 0.0f;

	D3D11_SUBRESOURCE_DATA test;
	test.pSysMem = &gv;

	if( FAILED( hr = mDevice->CreateBuffer( &bufferInstancedDesc, &test, &mBuffers[BUFFERS_SINGLE_VERTEX] ) ) )
		return hr;

	//Billboard buffer instanced
	bufferInstancedDesc.ByteWidth = sizeof( ParticleVertex16 ) * MAX_BILLBOARD_BATCH;

	//Single vertex buffer used for billboarding
	bufferInstancedDesc.BindFlags			= D3D11_BIND_VERTEX_BUFFER;
	bufferInstancedDesc.CPUAccessFlags		= D3D11_CPU_ACCESS_WRITE;
	bufferInstancedDesc.Usage				= D3D11_USAGE_DYNAMIC;

	if( FAILED( hr = mDevice->CreateBuffer( &bufferInstancedDesc, nullptr, &mBuffers[BUFFERS_PARTICLE] ) ) )
		return hr;

	//Single vertex buffer used for nodeGrid
	ZeroMemory( &bufferInstancedDesc, sizeof( bufferInstancedDesc ) );
	bufferInstancedDesc.BindFlags		= D3D11_BIND_VERTEX_BUFFER;
	bufferInstancedDesc.CPUAccessFlags	= D3D11_CPU_ACCESS_WRITE;
	bufferInstancedDesc.Usage			= D3D11_USAGE_DYNAMIC;
	bufferInstancedDesc.ByteWidth		= sizeof( StaticVertex ) * MAX_SINGLE_STATIC_VERTICES;

	if( FAILED( hr = mDevice->CreateBuffer( &bufferInstancedDesc, nullptr, &mBuffers[BUFFERS_SINGLE_STATIC_VERTEX] ) ) )
		return hr;

	//Light buffer for structured buffer
	D3D11_BUFFER_DESC lightBufferDesc;
	ZeroMemory( &lightBufferDesc, sizeof( lightBufferDesc ) );
	lightBufferDesc.BindFlags			= D3D11_BIND_SHADER_RESOURCE;
	lightBufferDesc.Usage				= D3D11_USAGE_DYNAMIC;
	lightBufferDesc.CPUAccessFlags		= D3D11_CPU_ACCESS_WRITE;
	lightBufferDesc.MiscFlags			= D3D11_RESOURCE_MISC_BUFFER_STRUCTURED;
	lightBufferDesc.ByteWidth			= sizeof( LightStructure );
	lightBufferDesc.StructureByteStride	= sizeof( PointLight );

	if( FAILED( hr = mDevice->CreateBuffer( &lightBufferDesc, nullptr, &mBuffers[BUFFERS_LIGHT] ) ) )
		return hr;

	if( FAILED( hr = mDevice->CreateShaderResourceView( mBuffers[BUFFERS_LIGHT], nullptr, &mLightStructuredBuffer ) ) )
		return hr;

	/////////////////////////
	// INITIATE VERTEXBUFFER FOR 2D
	/////////////////////////
	D3D11_BUFFER_DESC bufferDesc2d;
	ZeroMemory( &bufferDesc2d, sizeof(bufferDesc2d) );
	bufferDesc2d.BindFlags		= D3D11_BIND_VERTEX_BUFFER;
	bufferDesc2d.ByteWidth		= sizeof(StaticVertex) * 4;
	bufferDesc2d.Usage			= D3D11_USAGE_DYNAMIC;
	bufferDesc2d.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

	hr = mDevice->CreateBuffer( &bufferDesc2d, nullptr, &mBuffers[BUFFERS_2D] );
	if ( FAILED( hr ) )
	{
		//Failed to create vertex buffer
		return hr;
	}

	StaticVertex boxVertices[]		= 
	{
		//BackBottomLeft
		{ -0.5, -0.5, -0.5,		0.0f, 0.0f, -1.0f,	0.0f, 0.0f, 0.0f,	0.0f, 1.0f },
		//BackBottomRight
		{  0.5, -0.5, -0.5,		0.0f, 0.0f, -1.0f,	0.0f, 0.0f, 0.0f,	0.0f, 1.0f },
		//BackTopRight
		{  0.5,  0.5, -0.5,		0.0f, 0.0f, -1.0f,	0.0f, 0.0f, 0.0f,	0.0f, 1.0f },
		//BackTopLeft 
		{ -0.5,  0.5, -0.5,		0.0f, 0.0f, -1.0f,	0.0f, 0.0f, 0.0f,	0.0f, 1.0f },
		//FrontBottomLeft
		{ -0.5, -0.5,  0.5,		0.0f, 0.0f, -1.0f,	0.0f, 0.0f, 0.0f,	0.0f, 1.0f },
		//FrontBottomRight
		{  0.5, -0.5,  0.5,		0.0f, 0.0f, -1.0f,	0.0f, 0.0f, 0.0f,	0.0f, 1.0f },
		//FrontTopRight
		{  0.5,  0.5,  0.5,		0.0f, 0.0f, -1.0f,	0.0f, 0.0f, 0.0f,	0.0f, 1.0f },
		//FrontTopLeft
		{ -0.5,  0.5,  0.5,		0.0f, 0.0f, -1.0f,	0.0f, 0.0f, 0.0f,	0.0f, 1.0f },
	};

	D3D11_BUFFER_DESC debugBoxBuffer;
	ZeroMemory( &debugBoxBuffer, sizeof(debugBoxBuffer) );
	debugBoxBuffer.BindFlags		= D3D11_BIND_VERTEX_BUFFER;
	debugBoxBuffer.ByteWidth		= sizeof(StaticVertex) * 8;
	debugBoxBuffer.Usage			= D3D11_USAGE_IMMUTABLE;

	D3D11_SUBRESOURCE_DATA subData;
	subData.pSysMem = boxVertices;

	hr = mDevice->CreateBuffer( &debugBoxBuffer, &subData, &mBuffers[BUFFERS_DEBUG_BOX] );
	if ( FAILED( hr ) )
	{
		//Failed to create vertex buffer
		return hr;
	}

	UINT boxIndices[] = 
	{
		0, 1,
		1, 2,
		2, 3,
		3, 0,

		4, 5,
		5, 6,
		6, 7,
		7, 4,

		3, 7,
		2, 6,
		5, 1,
		0, 4
	};
	debugBoxBuffer.BindFlags		= D3D11_BIND_INDEX_BUFFER;
	debugBoxBuffer.ByteWidth		= sizeof( UINT ) * 24;

	subData.pSysMem = boxIndices; 

	return hr = mDevice->CreateBuffer( &debugBoxBuffer, &subData, &mBuffers[BUFFERS_DEBUG_BOX_INDICES] );
}

HRESULT Graphics::InitializeEffects()
{
	HRESULT hr = S_OK;
	//Effect
	for( int i = 0; i < EFFECTS_AMOUNT; i++ )
		mEffects[i] = new Effect;

	EffectInfo effectInfo;
	ZeroMemory( &effectInfo, sizeof( EffectInfo ) );
	effectInfo.filePath					= "../Content/Effects/Static3dEffect.hlsl";
	effectInfo.fileName					= "Static3dEffect";
	effectInfo.vertexType				= STATIC_VERTEX_TYPE;
	effectInfo.isVertexShaderIncluded	= true;
	effectInfo.isPixelShaderIncluded	= true;

	if( FAILED( hr = mEffects[EFFECTS_STATIC_VERTEX]->Intialize( mDevice, &effectInfo ) ) )
		return hr;

	//DebugEffect
	effectInfo.filePath					= "../Content/Effects/DebugShaderEffect.hlsl";
	effectInfo.fileName					= "DebugShaderEffect";

	if( FAILED( hr = mEffects[EFFECTS_DEBUG_BOX]->Intialize( mDevice, &effectInfo ) ) )
		return hr;

	//Static instanced effect
	effectInfo.filePath		= "../Content/Effects/Static3dInstancedEffect.hlsl";
	effectInfo.fileName		= "Static3dInstancedEffect";
	effectInfo.vertexType	= STATIC_INSTANCED_VERTEX_TYPE;

	if( FAILED( hr = mEffects[EFFECTS_STATIC_INSTANCED]->Intialize( mDevice, &effectInfo ) ) )
		return hr;
	//--------------------------

	//Static instanced effect
	effectInfo.filePath		= "../Content/Effects/Static3dInstancedShadowEffect.hlsl";
	effectInfo.fileName		= "Static3dInstancedShadowEffect";

	if( FAILED( hr = mEffects[EFFECTS_STATIC_INSTANCED_SHADOW]->Intialize( mDevice, &effectInfo ) ) )
		return hr;
	//--------------------------

	//2d effect
	effectInfo.filePath		= "../Content/Effects/2dEffect.hlsl";
	effectInfo.fileName		= "2dEffect";
	effectInfo.vertexType	= STATIC_VERTEX_TYPE;

	if( FAILED( hr = mEffects[EFFECTS_2D]->Intialize( mDevice, &effectInfo ) ) )
		return hr;
	//--------------------------

	//Animated effect
	
	effectInfo.filePath		= "../Content/Effects/Animated3dEffect.hlsl";
	effectInfo.fileName		= "Animated3dEffect";
	effectInfo.vertexType	= ANIMATED_VERTEX_TYPE;

	if( FAILED( hr = mEffects[EFFECTS_ANIMATED]->Intialize( mDevice, &effectInfo ) ) )
		return hr;
	//--------------------------

	//Animated instanced effect
	effectInfo.filePath		= "../Content/Effects/AnimatedInstanced3dEffect.hlsl";
	effectInfo.fileName		= "AnimatedInstanced3dEffect";
	effectInfo.vertexType	= ANIMATED_VERTEX_INSTANCED_TYPE;

	if( FAILED( hr = mEffects[EFFECTS_ANIMATED_INSTANCED]->Intialize( mDevice, &effectInfo ) ) )
		return hr;

	//--------------------------

	//Animated instanced shadow effect
	effectInfo.filePath		= "../Content/Effects/AnimatedInstanced3dShadowEffect.hlsl";
	effectInfo.fileName		= "AnimatedInstanced3dShadowEffect";

	if( FAILED( hr = mEffects[EFFECTS_ANIMATED_INSTANCED_SHADOW]->Intialize( mDevice, &effectInfo ) ) )
		return hr;

	//--------------------------
	
	//Deferred effect
	effectInfo.filePath		= "../Content/Effects/DeferredPassEffect.hlsl";
	effectInfo.fileName		= "DeferredPassEffect";
	effectInfo.vertexType	= STATIC_VERTEX_TYPE;

	if( FAILED( hr = mEffects[EFFECTS_DEFERRED]->Intialize( mDevice, &effectInfo ) ) )
		return hr;
	//--------------------------

	//NodeGrid effect
	effectInfo.filePath		= "../Content/Effects/NodeGridEffect.hlsl";
	effectInfo.fileName		= "NodeGridEffect";

	if( FAILED( hr = mEffects[EFFECTS_NODEGRID]->Intialize( mDevice, &effectInfo ) ) )
		return hr;
	//--------------------------

	//Billboard effect
	effectInfo.filePath					= "../Content/Effects/BillboardEffect.hlsl";
	effectInfo.fileName					= "BillboardEffect";
	effectInfo.vertexType				= BILLBOARD_VERTEX_TYPE;
	effectInfo.isGeometryShaderIncluded = true;

	if( FAILED( hr = mEffects[EFFECTS_BILLBOARD]->Intialize( mDevice, &effectInfo ) ) )
		return hr;

	//=======================================
	//			PARTICLE EFFECTS			|
	//=======================================
	//Explosion smoke effect
	effectInfo.filePath					= "../Content/Effects/Particle Effects/ExplosionSmoke.hlsl";
	effectInfo.fileName					= "ExplosionSmoke";
	effectInfo.vertexType				= PARTICLE_VERTEX_TYPE;
	effectInfo.isGeometryShaderIncluded = true;
	
	if( FAILED( hr = mEffects[EFFECTS_EXPLOSION_SMOKE]->Intialize( mDevice, &effectInfo ) ) )
		return hr;
	//--------------------------

	//Fire effect
	effectInfo.filePath					= "../Content/Effects/Particle Effects/FireEffect.hlsl";
	effectInfo.fileName					= "FireEffect";
	effectInfo.vertexType				= PARTICLE_VERTEX_TYPE;
	effectInfo.isGeometryShaderIncluded = true;

	if( FAILED( hr = mEffects[EFFECTS_FIRE]->Intialize( mDevice, &effectInfo ) ) )
		return hr;
	//--------------------------

	//FireSmoke effect
	effectInfo.filePath					= "../Content/Effects/Particle Effects/FireSmokeEffect.hlsl";
	effectInfo.fileName					= "FireSmokeEffect";
	effectInfo.vertexType				= PARTICLE_VERTEX_TYPE;
	effectInfo.isGeometryShaderIncluded = true;

	if( FAILED( hr = mEffects[EFFECTS_FIRE_SMOKE]->Intialize( mDevice, &effectInfo ) ) )
		return hr;
	//--------------------------

	//Muzzle Flash effect
	effectInfo.filePath					= "../Content/Effects/Particle Effects/MuzzleFlashEffect.hlsl";
	effectInfo.fileName					= "MuzzleFlashEffect";
	effectInfo.vertexType				= PARTICLE_VERTEX_TYPE;
	effectInfo.isGeometryShaderIncluded = true;

	if( FAILED( hr = mEffects[EFFECTS_MUZZLEFLASH]->Intialize( mDevice, &effectInfo ) ) )
		return hr;
	//--------------------------

	//Explosion effect
	effectInfo.filePath					= "../Content/Effects/Particle Effects/ExplosionEffect.hlsl";
	effectInfo.fileName					= "ExplosionEffect";
	effectInfo.vertexType				= PARTICLE_VERTEX_TYPE;
	effectInfo.isGeometryShaderIncluded = true;

	if( FAILED( hr = mEffects[EFFECTS_EXPLOSION]->Intialize( mDevice, &effectInfo ) ) )
		return hr;
	//--------------------------

	//Level inner effect
	effectInfo.filePath					= "../Content/Effects/Particle Effects/Level_InnerEffect.hlsl";
	effectInfo.fileName					= "Level_InnerEffect";
	effectInfo.vertexType				= PARTICLE_VERTEX_TYPE;
	effectInfo.isGeometryShaderIncluded = true;

	if( FAILED( hr = mEffects[EFFECTS_LEVEL_INNER]->Intialize( mDevice, &effectInfo ) ) )
		return hr;
	//--------------------------

	//Level up effect
	effectInfo.filePath					= "../Content/Effects/Particle Effects/Level_UpEffect.hlsl";
	effectInfo.fileName					= "Level_UpEffect";
	effectInfo.vertexType				= PARTICLE_VERTEX_TYPE;
	effectInfo.isGeometryShaderIncluded = true;

	if( FAILED( hr = mEffects[EFFECTS_LEVEL_UP]->Intialize( mDevice, &effectInfo ) ) )
		return hr;
	//--------------------------

	//Spark effect
	effectInfo.filePath					= "../Content/Effects/Particle Effects/SparkEffect.hlsl";
	effectInfo.fileName					= "SparkEffect";
	effectInfo.vertexType				= PARTICLE_VERTEX_TYPE;
	effectInfo.isGeometryShaderIncluded = true;

	if( FAILED( hr = mEffects[EFFECTS_SPARK]->Intialize( mDevice, &effectInfo ) ) )
		return hr;
	//--------------------------

	//Blood effect
	effectInfo.filePath					= "../Content/Effects/Particle Effects/BloodEffect.hlsl";
	effectInfo.fileName					= "BloodEffect";
	effectInfo.vertexType				= PARTICLE_VERTEX_TYPE;
	effectInfo.isGeometryShaderIncluded = true;

	if( FAILED( hr = mEffects[EFFECTS_BLOOD]->Intialize( mDevice, &effectInfo ) ) )
		return hr;
	//--------------------------

	//Muzzle Flash effect
	effectInfo.filePath					= "../Content/Effects/Particle Effects/MuzzleFlashEffect.hlsl";
	effectInfo.fileName					= "MuzzleFlashEffect";
	effectInfo.vertexType				= PARTICLE_VERTEX_TYPE;
	effectInfo.isGeometryShaderIncluded = true;

	if( FAILED( hr = mEffects[EFFECTS_MUZZLEFLASH]->Intialize( mDevice, &effectInfo ) ) )
		return hr;
	//--------------------------


	//Minigun Smoke effect
	effectInfo.filePath					= "../Content/Effects/Particle Effects/Smoke_MiniGunEffect.hlsl";
	effectInfo.fileName					= "Smoke_MiniGunEffect";
	effectInfo.vertexType				= PARTICLE_VERTEX_TYPE;
	effectInfo.isGeometryShaderIncluded = true;

	if( FAILED( hr = mEffects[EFFECTS_SMOKE_MINIGUN]->Intialize( mDevice, &effectInfo ) ) )
		return hr;
	//--------------------------


	//Test Fountain effect
	effectInfo.filePath					= "../Content/Effects/Particle Effects/Test_FountainEffect.hlsl";
	effectInfo.fileName					= "Test_FountainEffect";
	effectInfo.vertexType				= PARTICLE_VERTEX_TYPE;
	effectInfo.isGeometryShaderIncluded = true;

	if( FAILED( hr = mEffects[EFFECTS_TEST_FOUNTAIN]->Intialize( mDevice, &effectInfo ) ) )
		return hr;
	//--------------------------
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

HRESULT Graphics::LoadStatic2dAsset( std::string fileName, AssetID &assetId )
{
	return mAssetManager->LoadStatic2dAsset( mDevice, mDeviceContext, fileName, assetId ); 
}

//Load a static 3d asset to the AssetManager.
HRESULT Graphics::LoadStatic3dAsset( std::string filePath, std::string fileName, AssetID &assetId )
{
	return mAssetManager->LoadStatic3dAsset( mDevice, mDeviceContext, filePath, fileName, assetId );
}

HRESULT Graphics::LoadAnimated3dAsset( std::string filePath, std::string fileName, AssetID skeletonId, AssetID &assetId )
{
	return mAssetManager->LoadAnimated3dAsset( mDevice, mDeviceContext, filePath, fileName, skeletonId, assetId );
}

HRESULT Graphics::LoadSkeletonAsset( std::string filePath, std::string fileName, AssetID &assetId )
{
	return mAssetManager->LoadSkeletonAsset( filePath, fileName, assetId );
}

HRESULT Graphics::LoadAnimationAsset( std::string filePath, std::string fileName, AssetID &assetId )
{
	return mAssetManager->LoadAnimationAsset( filePath, fileName, assetId );
}

void Graphics::Render2dAsset( Object2dInfo* info, UINT sizeOfList )
{
	UINT32 vertexSize	= sizeof(StaticVertex);
	UINT32 offset		= 0;

	for( UINT i = 0; i < sizeOfList; i++ )
	{
		float left		= ( ( info[i].mTopLeftCorner.x / (float)mScreenWidth ) * 2.0f ) - 1.0f;
		float right		= ( ( ( info[i].mTopLeftCorner.x + info[i].mWidthHeight.x ) / (float)mScreenWidth ) * 2.0f ) - 1.0f;
		float bottom	= ( ( ( info[i].mTopLeftCorner.y + info[i].mWidthHeight.y ) / (float)mScreenHeight ) * -2.0f ) + 1.0f;
		float top		= ( ( info[i].mTopLeftCorner.y / (float)mScreenHeight ) * -2.0f ) + 1.0f;

		StaticVertex bottomleft		= { left, bottom, 0.0f,		0.0f, 0.0f, -1.0f,	0.0f, 0.0f, 0.0f,	0.0f, 1.0f };
		StaticVertex topleft		= { left, top, 0.0f,		0.0f, 0.0f, -1.0f,	0.0f, 0.0f, 0.0f,	0.0f, 0.0f };
		StaticVertex bottomright	= { right, bottom, 0.0f,	0.0f, 0.0f, -1.0f,	0.0f, 0.0f, 0.0f,	1.0f, 1.0f };
		StaticVertex topright		= { right, top, 0.0f,		0.0f, 0.0f, -1.0f,	0.0f, 0.0f, 0.0f,	1.0f, 0.0f };

		StaticVertex vertices[4] = { bottomleft, topleft, bottomright, topright };
		MapBuffer( mBuffers[BUFFERS_2D], &vertices, sizeof(StaticVertex) * 4 );
		mDeviceContext->IASetVertexBuffers( 0, 1, &mBuffers[BUFFERS_2D], &vertexSize, &offset );

		CbufferPerObject2D cbuff;
		cbuff.color = info[i].mColor;
		MapBuffer( mBuffers[BUFFERS_CBUFFER_PER_OBJECT_2D], &cbuff, sizeof( CbufferPerObject2D ) );
		mDeviceContext->PSSetConstantBuffers( 0, 1, &mBuffers[BUFFERS_CBUFFER_PER_OBJECT_2D] );

		mDeviceContext->PSSetShaderResources( 0, 1, &( (Static2dAsset*)mAssetManager->mAssetContainer[info[i].mAssetId] )->mSRV );
		mDeviceContext->Draw( 4, 0 );
	}
}

void Graphics::RenderPlane2dAsset( AssetID assetId, DirectX::XMFLOAT3 x, DirectX::XMFLOAT3 y )
{
	mDeviceContext->IASetPrimitiveTopology( D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP );

	UINT32 vertexSize	= sizeof(StaticVertex);
	UINT32 offset		= 0;

	StaticVertex bottomleft		= { x.x, x.y + 0.001f, y.z,		0.0f, 1.0f, 0.0f,	0.71f, 0.0f, 0.71f,		0.0f, 1.0f };
	StaticVertex topleft		= { x.x, x.y + 0.001f, x.z,		0.0f, 1.0f, 0.0f,	0.71f, 0.0f, 0.71f,		0.0f, 0.0f };
	StaticVertex bottomright	= { y.x, x.y + 0.001f, y.z,		0.0f, 1.0f, 0.0f,	0.71f, 0.0f, 0.71f,		1.0f, 1.0f };
	StaticVertex topright		= { y.x, x.y + 0.001f, x.z,		0.0f, 1.0f, 0.0f,	0.71f, 0.0f, 0.71f,		1.0f, 0.0f };

	StaticVertex vertices[4]	= { bottomleft, topleft, bottomright, topright };
	MapBuffer( mBuffers[BUFFERS_2D], &vertices, sizeof(StaticVertex) * 4 );
	mDeviceContext->IASetVertexBuffers( 0, 1, &mBuffers[BUFFERS_2D], &vertexSize, &offset );

	mDeviceContext->IASetInputLayout( mEffects[EFFECTS_STATIC_VERTEX]->GetInputLayout() );

	mDeviceContext->VSSetShader( mEffects[EFFECTS_STATIC_VERTEX]->GetVertexShader(), nullptr, 0 );
	mDeviceContext->HSSetShader( nullptr, nullptr, 0 );
	mDeviceContext->DSSetShader( nullptr, nullptr, 0 );
	mDeviceContext->GSSetShader( nullptr, nullptr, 0 );
	mDeviceContext->PSSetShader( mEffects[EFFECTS_STATIC_VERTEX]->GetPixelShader(), nullptr, 0 );

	//Map CbufferPerObject
	CbufferPerObject data;
	data.worldMatrix = DirectX::XMMatrixIdentity();
	MapBuffer( mBuffers[BUFFERS_CBUFFER_PER_OBJECT], &data, sizeof( CbufferPerObject ) );

	mDeviceContext->VSSetConstantBuffers( 1, 1, &mBuffers[BUFFERS_CBUFFER_PER_OBJECT] );

	mDeviceContext->PSSetShaderResources( 0, 1, &( (Static2dAsset*)mAssetManager->mAssetContainer[assetId] )->mSRV );
	mDeviceContext->PSSetShaderResources( 1, 1, &( (Static2dAsset*)mAssetManager->mAssetContainer[NORMAL_PLACEHOLDER] )->mSRV );
	mDeviceContext->PSSetShaderResources( 2, 1, &( (Static2dAsset*)mAssetManager->mAssetContainer[SPECULAR_PLACEHOLDER] )->mSRV );
	mDeviceContext->Draw( 4, 0 );
}

void Graphics::RenderStatic3dAsset( Object3dInfo* info, UINT sizeOfList )
{
	///////////////////////////////Code for rendering Oct tree boxes, DO NOT REMOVE!
	/*for( UINT i = 0; i < sizeOfList; i++ )
	{
		Static3dAsset* derpface = (Static3dAsset*)mAssetManager->mAssetContainer[info[i].mAssetId];
		if(derpface->mFileName != "NO PATHCUBE")
		{
			for(UINT j = 0; j < 8; j++)
			{
				for(UINT k = 0; k < 8; k++)
				{
					for(UINT l = 0; l < 8; l++)
					{
						DirectX::XMFLOAT4X4 ident;
						DirectX::XMStoreFloat4x4( &ident, DirectX::XMMatrixIdentity() );
						if(derpface->mOctTree.childrenCollides[j])
						{
							if(derpface->mOctTree.children[j]->childrenCollides[k])
							{
								if(derpface->mOctTree.children[j]->children[k]->childrenCollides[l])
								{
									Graphics::RenderDebugBox( derpface->mOctTree.children[j]->children[k]->children[l]->boundingBox.min, 
																derpface->mOctTree.children[j]->children[k]->children[l]->boundingBox.max,
																	info[i].mWorld );
								}
							}
						}
					}
				}
			}
		}
	}*/
	///////////////////////////////


	//////////////////////////////////////////////////////////////////
	//						RENDER CALL
	//////////////////////////////////////////////////////////////////
	mDeviceContext->IASetPrimitiveTopology( D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST );
	
	UINT32 vertexSize[2]			= { sizeof( StaticVertex ), sizeof( StaticInstance ) };
	UINT32 offset[2]				= { 0, 0 };
	mDeviceContext->IASetInputLayout( mEffects[EFFECTS_STATIC_INSTANCED]->GetInputLayout() );

	mDeviceContext->VSSetShader( mEffects[EFFECTS_STATIC_INSTANCED]->GetVertexShader(), nullptr, 0 );
	mDeviceContext->HSSetShader( nullptr, nullptr, 0 );
	mDeviceContext->DSSetShader( nullptr, nullptr, 0 );
	mDeviceContext->GSSetShader( nullptr, nullptr, 0 );
	mDeviceContext->PSSetShader( mEffects[EFFECTS_STATIC_INSTANCED]->GetPixelShader(), nullptr, 0 );

	UINT objectToRender = 0;
	UINT currAssetID	= (UINT)-1;
	UINT strider		= 0;

	UINT assetIDs[MAX_AMOUNT_OF_OBJECT3D];
	for( UINT i = 0; i < sizeOfList; i++ )
		assetIDs[i] = info[i].mAssetId;

	while( true )
	{
		objectToRender = 0;
		currAssetID = (UINT)-1;
		for( UINT i = strider; i < sizeOfList; i++ )
		{
			if( info[i].mAssetId != (UINT)-1 )
			{
				if( currAssetID == (UINT)-1 )
				{
					currAssetID = info[i].mAssetId;
					strider		= i;
				}

				if( currAssetID == info[i].mAssetId )
				{
					
					mStatic3dInstanced[objectToRender].world = info[i].mWorld;
	
					info[i].mAssetId = (UINT)-1;
					objectToRender++;

					if( objectToRender == MAX_ANIM_INSTANCE_BATCH )
					{
						break;
					}
				}
			}
		}
		
		if( objectToRender > 0 )
		{
			//////////////////////////////////////////////////////////////////
			//						RENDER CALL
			//////////////////////////////////////////////////////////////////

			Static3dAsset* model = (Static3dAsset*)mAssetManager->mAssetContainer[currAssetID];
			
			for( UINT i = 0; i < model->mMeshes.size(); i++ )
			{
				ID3D11ShaderResourceView* texturesToSet[] = {	( (Static2dAsset*)mAssetManager->mAssetContainer[model->mMeshes[i].mTextures[TEXTURES_DIFFUSE]] )->mSRV,
																( (Static2dAsset*)mAssetManager->mAssetContainer[model->mMeshes[i].mTextures[TEXTURES_NORMAL]] )->mSRV,
																( (Static2dAsset*)mAssetManager->mAssetContainer[model->mMeshes[i].mTextures[TEXTURES_SPECULAR]] )->mSRV,
															};

				mDeviceContext->PSSetShaderResources( 0, TEXTURES_AMOUNT, texturesToSet );

				MapBuffer( mBuffers[BUFFERS_STATIC3D_PER_INSTANCED_OBJECT], mStatic3dInstanced, ( sizeof( StaticInstance ) * objectToRender ) );
				ID3D11Buffer* buffersToSet[2] = { model->mMeshes[i].mVertexBuffer, mBuffers[BUFFERS_STATIC3D_PER_INSTANCED_OBJECT] };
				mDeviceContext->IASetVertexBuffers( 0, 2, buffersToSet, vertexSize, offset );

				mDeviceContext->DrawInstanced( model->mMeshes[i].mVertexCount, objectToRender, 0, 0 );
			}
		}
		else break;
	}
	/////////////////////////
	//		SHADOWS
	/////////////////////////
	mDeviceContext->IASetInputLayout( mEffects[EFFECTS_STATIC_INSTANCED_SHADOW]->GetInputLayout() );
	mDeviceContext->VSSetShader( mEffects[EFFECTS_STATIC_INSTANCED_SHADOW]->GetVertexShader(), nullptr, 0 );
	mDeviceContext->PSSetShader( mEffects[EFFECTS_STATIC_INSTANCED_SHADOW]->GetPixelShader(), nullptr, 0 );

	ID3D11RenderTargetView* rtvShadow[] = { nullptr };
	mDeviceContext->OMSetRenderTargets( 1, rtvShadow, mShadowMapDSV );
	mDeviceContext->RSSetViewports( 1, &mShadowView );

	objectToRender	= 0;
	currAssetID		= (UINT)-1;
	strider			= 0;

	while( true )
	{
		objectToRender = 0;
		currAssetID = (UINT)-1;
		for( UINT i = strider; i < sizeOfList; i++ )
		{
			if( assetIDs[i] != (UINT)-1 )
			{
				if( currAssetID == (UINT)-1 )
				{
					currAssetID = assetIDs[i];
					strider		= i;
				}

				if( currAssetID == assetIDs[i] )
				{
					
					mStatic3dInstanced[objectToRender].world = info[i].mWorld;
	
					assetIDs[i] = (UINT)-1;
					objectToRender++;

					if( objectToRender == MAX_ANIM_INSTANCE_BATCH )
					{
						break;
					}
				}
			}
		}
		
		if( objectToRender > 0 )
		{
			//////////////////////////////////////////////////////////////////
			//						RENDER CALL
			//////////////////////////////////////////////////////////////////
			Static3dAsset* model = (Static3dAsset*)mAssetManager->mAssetContainer[currAssetID];
			
			for( UINT i = 0; i < model->mMeshes.size(); i++ )
			{
				mDeviceContext->PSSetShaderResources( 0, 1, &( (Static2dAsset*)mAssetManager->mAssetContainer[model->mMeshes[i].mTextures[TEXTURES_DIFFUSE]] )->mSRV );

				MapBuffer( mBuffers[BUFFERS_STATIC3D_PER_INSTANCED_OBJECT], mStatic3dInstanced, ( sizeof( StaticInstance ) * objectToRender ) );
				ID3D11Buffer* buffersToSet[2] = { model->mMeshes[i].mVertexBuffer, mBuffers[BUFFERS_STATIC3D_PER_INSTANCED_OBJECT] };
				mDeviceContext->IASetVertexBuffers( 0, 2, buffersToSet, vertexSize, offset );

				mDeviceContext->DrawInstanced( model->mMeshes[i].mVertexCount, objectToRender, 0, 0 );
			}
		}
		else break;
	}

	ID3D11RenderTargetView* rtvsToSet[NUM_GBUFFERS];
	for( int i = 0; i < NUM_GBUFFERS; i++)
		rtvsToSet[i] = mGbuffers[i]->mRenderTargetView;
	mDeviceContext->OMSetRenderTargets( 3, rtvsToSet, mDepthStencilView );
	mDeviceContext->RSSetViewports( 1, &mStandardView );
}

void Graphics::RenderAnimated3dAsset( Anim3dInfo* info, UINT sizeOfList )
{
	mDeviceContext->IASetPrimitiveTopology( D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST );
	UINT32 vertexSize[2]	= { sizeof( AnimatedVertex ), sizeof( AnimatedInstance ) };
	UINT32 offset[2]		= { 0, 0 };
	
	mDeviceContext->IASetInputLayout( mEffects[EFFECTS_ANIMATED_INSTANCED]->GetInputLayout() );

	mDeviceContext->VSSetShader( mEffects[EFFECTS_ANIMATED_INSTANCED]->GetVertexShader(), nullptr, 0 );
	mDeviceContext->HSSetShader( nullptr, nullptr, 0 );
	mDeviceContext->DSSetShader( nullptr, nullptr, 0 );
	mDeviceContext->GSSetShader( nullptr, nullptr, 0 );
	mDeviceContext->PSSetShader( mEffects[EFFECTS_ANIMATED_INSTANCED]->GetPixelShader(), nullptr, 0 );


	UINT objectToRender = 0;
	UINT currAssetID = (UINT)-1;
	UINT strider = 0;

	UINT assetIDs[MAX_AMOUNT_OF_ANIM3D];
	for( UINT i = 0; i < sizeOfList; i++ )
		assetIDs[i] = info[i].mModelId;

	while( true )
	{
		objectToRender = 0;
		currAssetID = (UINT)-1;
		for( UINT i = strider; i < sizeOfList; i++ )
		{
			if( info[i].mModelId != (UINT)-1 )
			{
				if( currAssetID == (UINT)-1 )
				{
					currAssetID = info[i].mModelId;
					strider		= i;
				}

				if( currAssetID == info[i].mModelId )
				{
					
					mAnimInstanced[objectToRender].world = info[i].mWorld;
					for( int j = 0; j < NUM_SUPPORTED_JOINTS; j++ )
						mAnimCbufferInstanced[objectToRender].boneTransforms[j] = DirectX::XMLoadFloat4x4( &info[i].mBoneTransforms[j] );

					info[i].mModelId = (UINT)-1;
					objectToRender++;

					if( objectToRender == MAX_ANIM_INSTANCE_BATCH )
					{
						break;
					}
				}
			}
		}

		if( objectToRender > 0 )
		{
			//////////////////////////////////////////////////////////////////
			//						RENDER CALL
			//////////////////////////////////////////////////////////////////
			
			Animated3dAsset* model = (Animated3dAsset*)mAssetManager->mAssetContainer[currAssetID];

			ID3D11ShaderResourceView* texturesToSet[] = {	( (Static2dAsset*)mAssetManager->mAssetContainer[model->mTextures[TEXTURES_DIFFUSE]] )->mSRV,
															( (Static2dAsset*)mAssetManager->mAssetContainer[model->mTextures[TEXTURES_NORMAL]] )->mSRV,
															( (Static2dAsset*)mAssetManager->mAssetContainer[model->mTextures[TEXTURES_SPECULAR]] )->mSRV,
														};

			mDeviceContext->PSSetShaderResources( 0, TEXTURES_AMOUNT, texturesToSet );
			MapBuffer( mBuffers[BUFFERS_CBUFFER_PER_INSTANCED_ANIMATED], mAnimCbufferInstanced, ( sizeof( CbufferPerObjectAnimated ) * objectToRender ) );
			MapBuffer( mBuffers[BUFFERS_STATIC3D_PER_INSTANCED_OBJECT], mAnimInstanced, ( sizeof( AnimatedInstance ) * objectToRender ) );
			ID3D11Buffer* buffersToSet[2] = { model->mVertexBuffer, mBuffers[BUFFERS_STATIC3D_PER_INSTANCED_OBJECT] };

			mDeviceContext->IASetVertexBuffers( 0, 2, buffersToSet, vertexSize, offset );
			mDeviceContext->VSSetConstantBuffers( 1, 1, &mBuffers[BUFFERS_CBUFFER_PER_INSTANCED_ANIMATED] );
			mDeviceContext->DrawInstanced( model->mVertexCount, objectToRender, 0, 0 );
		}
		else break;
	}

	/////////////////////////
	//		SHADOWS
	/////////////////////////
	mDeviceContext->IASetInputLayout( mEffects[EFFECTS_ANIMATED_INSTANCED_SHADOW]->GetInputLayout() );
	mDeviceContext->VSSetShader( mEffects[EFFECTS_ANIMATED_INSTANCED_SHADOW]->GetVertexShader(), nullptr, 0 );
	mDeviceContext->PSSetShader( mEffects[EFFECTS_ANIMATED_INSTANCED_SHADOW]->GetPixelShader(), nullptr, 0 );

	ID3D11RenderTargetView* rtvShadow[] = { nullptr };
	mDeviceContext->OMSetRenderTargets( 1, rtvShadow, mShadowMapDSV );
	mDeviceContext->RSSetViewports( 1, &mShadowView );

	objectToRender	= 0;
	currAssetID		= (UINT)-1;
	strider			= 0;

	while( true )
	{
		objectToRender = 0;
		currAssetID = (UINT)-1;
		for( UINT i = strider; i < sizeOfList; i++ )
		{
			if( assetIDs[i] != (UINT)-1 )
			{
				if( currAssetID == (UINT)-1 )
				{
					currAssetID = assetIDs[i];
					strider		= i;
				}

				if( currAssetID == assetIDs[i] )
				{
					
					mAnimInstanced[objectToRender].world = info[i].mWorld;
					for( int j = 0; j < NUM_SUPPORTED_JOINTS; j++ )
						mAnimCbufferInstanced[objectToRender].boneTransforms[j] = DirectX::XMLoadFloat4x4( &info[i].mBoneTransforms[j] );

					assetIDs[i] = (UINT)-1;
					objectToRender++;

					if( objectToRender == MAX_ANIM_INSTANCE_BATCH )
					{
						break;
					}
				}
			}
		}

		if( objectToRender > 0 )
		{
			//////////////////////////////////////////////////////////////////
			//						RENDER CALL
			//////////////////////////////////////////////////////////////////
			
			Animated3dAsset* model = (Animated3dAsset*)mAssetManager->mAssetContainer[currAssetID];

			ID3D11ShaderResourceView* texturesToSet[] = {	( (Static2dAsset*)mAssetManager->mAssetContainer[model->mTextures[TEXTURES_DIFFUSE]] )->mSRV,
															( (Static2dAsset*)mAssetManager->mAssetContainer[model->mTextures[TEXTURES_NORMAL]] )->mSRV,
															( (Static2dAsset*)mAssetManager->mAssetContainer[model->mTextures[TEXTURES_SPECULAR]] )->mSRV,
														};

			mDeviceContext->PSSetShaderResources( 0, TEXTURES_AMOUNT, texturesToSet );
			MapBuffer( mBuffers[BUFFERS_CBUFFER_PER_INSTANCED_ANIMATED], mAnimCbufferInstanced, ( sizeof( CbufferPerObjectAnimated ) * objectToRender ) );
			MapBuffer( mBuffers[BUFFERS_STATIC3D_PER_INSTANCED_OBJECT], mAnimInstanced, ( sizeof( AnimatedInstance ) * objectToRender ) );
			ID3D11Buffer* buffersToSet[2] = { model->mVertexBuffer, mBuffers[BUFFERS_STATIC3D_PER_INSTANCED_OBJECT] };

			mDeviceContext->IASetVertexBuffers( 0, 2, buffersToSet, vertexSize, offset );
			mDeviceContext->VSSetConstantBuffers( 1, 1, &mBuffers[BUFFERS_CBUFFER_PER_INSTANCED_ANIMATED] );
			mDeviceContext->DrawInstanced( model->mVertexCount, objectToRender, 0, 0 );
		}
		else break;
	}

	mDeviceContext->VSSetConstantBuffers( 0, 1, &mBuffers[BUFFERS_CBUFFER_PER_FRAME] );

	ID3D11RenderTargetView* rtvsToSet[NUM_GBUFFERS];
	for( int i = 0; i < NUM_GBUFFERS; i++)
		rtvsToSet[i] = mGbuffers[i]->mRenderTargetView;
	mDeviceContext->OMSetRenderTargets( 3, rtvsToSet, mDepthStencilView );
	mDeviceContext->RSSetViewports( 1, &mStandardView );
}

void Graphics::RenderBillboard( BillboardInfo* info, UINT sizeOfList )
{
	//////////////////////////////////////////////////////////////////
	//						RENDER CALL
	//////////////////////////////////////////////////////////////////
	mDeviceContext->IASetPrimitiveTopology( D3D11_PRIMITIVE_TOPOLOGY_POINTLIST );
	
	UINT32 vertexSize[2]			= { sizeof( Vertex12 ), sizeof( BillboardInstanced ) };
	UINT32 offset[2]				= { 0, 0 };
	mDeviceContext->IASetInputLayout( mEffects[EFFECTS_BILLBOARD]->GetInputLayout() );

	mDeviceContext->VSSetShader( mEffects[EFFECTS_BILLBOARD]->GetVertexShader(), nullptr, 0 );
	mDeviceContext->HSSetShader( nullptr, nullptr, 0 );
	mDeviceContext->DSSetShader( nullptr, nullptr, 0 );
	mDeviceContext->GSSetShader( mEffects[EFFECTS_BILLBOARD]->GetGeometryShader(), nullptr, 0 );
	mDeviceContext->PSSetShader( mEffects[EFFECTS_BILLBOARD]->GetPixelShader(), nullptr, 0 );

	mDeviceContext->GSSetConstantBuffers( 0, 1, &mBuffers[BUFFERS_CBUFFER_PER_FRAME] );

	UINT objectToRender = 0;
	UINT currAssetID = (UINT)-1;
	UINT strider = 0;



	while( true )
	{
		objectToRender = 0;
		currAssetID = (UINT)-1;
		for( UINT i = strider; i < sizeOfList; i++ )
		{
			if( info[i].mAssetId != (UINT)-1 )
			{
				if( currAssetID == (UINT)-1 )
				{
					currAssetID = info[i].mAssetId;
					strider		= i;
				}

				if( currAssetID == info[i].mAssetId )
				{
					mBillboardInstanced[objectToRender].position[0] = info[i].mWorldPosition.x;
					mBillboardInstanced[objectToRender].position[1] = info[i].mWorldPosition.y;
					mBillboardInstanced[objectToRender].position[2] = info[i].mWorldPosition.z;
					mBillboardInstanced[objectToRender].width		= info[i].mWidth;
					mBillboardInstanced[objectToRender].height		= info[i].mHeight;
					info[i].mAssetId = (UINT)-1;
					objectToRender++;

					if( objectToRender == MAX_BILLBOARD_BATCH )
					{
						break;
					}
				}
			}
		}
		
		if( objectToRender > 0 )
		{
			//////////////////////////////////////////////////////////////////
			//						RENDER CALL
			//////////////////////////////////////////////////////////////////
			ID3D11ShaderResourceView* texturesToSet[] = { ( (Static2dAsset*)mAssetManager->mAssetContainer[currAssetID] )->mSRV };

			mDeviceContext->PSSetShaderResources( 0, 1, texturesToSet );

			MapBuffer( mBuffers[BUFFERS_BILLBOARD], mBillboardInstanced, ( sizeof( BillboardInstanced ) * objectToRender ) );
			ID3D11Buffer* buffersToSet[2] = { mBuffers[BUFFERS_SINGLE_VERTEX], mBuffers[BUFFERS_BILLBOARD] };
			mDeviceContext->IASetVertexBuffers( 0, 2, buffersToSet, vertexSize, offset );
			mDeviceContext->DrawInstanced( 1, objectToRender, 0, 0 );
		}
		else break;
	}
}

void Graphics::RenderParticleSystems( ParticleInfo* info, UINT sizeOfList )
{


	//////////////////////////////////////////////////////////////////
	//						RENDER CALL
	//////////////////////////////////////////////////////////////////
	mDeviceContext->IASetPrimitiveTopology( D3D11_PRIMITIVE_TOPOLOGY_POINTLIST );
	mDeviceContext->OMSetDepthStencilState( mDepthStencils[DEPTHSTENCILS_MASK_DISABLED], 1 );
	
	UINT32 vertexSize[2]			= { sizeof( Vertex12 ), sizeof( ParticleVertex16 ) };
	UINT32 offset[2]				= { 0, 0 };
	mDeviceContext->GSSetConstantBuffers( 0, 1, &mBuffers[BUFFERS_CBUFFER_PER_FRAME] );

	UINT objectToRender = 0;
	UINT currAssetID = (UINT)-1;
	UINT strider = 0;

	UINT offsetToParticleType = 0;

	while( true )
	{
		objectToRender = 0;
		currAssetID = (UINT)-1;
		for( UINT i = strider; i < sizeOfList; i++ )
		{

			if( i == offsetToParticleType )
			{
				currAssetID				= info[i].mAssetId;
				offsetToParticleType	= info[i].mOffsetToNextParticleType;				
			}


			if( i < offsetToParticleType )
			{
				mParticleInstanced[objectToRender].position[0]  = info[i].mWorldPosition.x;
				mParticleInstanced[objectToRender].position[1]  = info[i].mWorldPosition.y;
				mParticleInstanced[objectToRender].position[2]  = info[i].mWorldPosition.z;

				mParticleInstanced[objectToRender].age				= info[i].mAge;
				mParticleInstanced[objectToRender].timeTillDeath	= info[i].mTimeTillDeath;
				mParticleInstanced[objectToRender].randomRotation	= info[i].mRandomRotation;

				objectToRender++;
				strider++;
				if( objectToRender == MAX_PARTICLE_BATCH || strider == offsetToParticleType )
				{
					//Test
					mDeviceContext->IASetInputLayout( mEffects[info[i].mParticleType]->GetInputLayout() );

					// Add particletype you want to apply additive blending on
					if( info[i].mParticleType == EFFECTS_TEST_FOUNTAIN || info[i].mParticleType == EFFECTS_SPARK || info[i].mParticleType == EFFECTS_LEVEL_UP 
						|| info[i].mParticleType == EFFECTS_FIRE )
						mDeviceContext->OMSetBlendState( mBlendStates[BLEND_ADD], 0, 0xFFFFFFFF );
					
					else if( info[i].mParticleType == EFFECTS_EXPLOSION )
						mDeviceContext->OMSetBlendState( mBlendStates[BLEND_ADD], 0, 0xFFFFFFFF );
					
					else
						mDeviceContext->OMSetBlendState( mBlendStates[BLEND_NORMAL], 0, 0xFFFFFFFF );

					mDeviceContext->VSSetShader( mEffects[info[i].mParticleType]->GetVertexShader(), nullptr, 0 );
					mDeviceContext->HSSetShader( nullptr, nullptr, 0 );
					mDeviceContext->DSSetShader( nullptr, nullptr, 0 );
					mDeviceContext->GSSetShader( mEffects[info[i].mParticleType]->GetGeometryShader(), nullptr, 0 );
					mDeviceContext->PSSetShader( mEffects[info[i].mParticleType]->GetPixelShader(), nullptr, 0 );
					break;
				}
			}
		}
	
		
		if( objectToRender > 0 )
		{
			//////////////////////////////////////////////////////////////////
			//						RENDER CALL
			//////////////////////////////////////////////////////////////////
			ID3D11ShaderResourceView* texturesToSet[] = { ( (Static2dAsset*)mAssetManager->mAssetContainer[currAssetID] )->mSRV };

			mDeviceContext->PSSetShaderResources( 0, 1, texturesToSet );

			MapBuffer( mBuffers[BUFFERS_PARTICLE], mParticleInstanced, ( sizeof( ParticleVertex16 ) * objectToRender ) );
			ID3D11Buffer* buffersToSet[2] = { mBuffers[BUFFERS_SINGLE_VERTEX], mBuffers[BUFFERS_PARTICLE] };
			mDeviceContext->IASetVertexBuffers( 0, 2, buffersToSet, vertexSize, offset );
			mDeviceContext->DrawInstanced( 1, objectToRender, 0, 0 );
		}
		else break;
	}

	mDeviceContext->OMSetDepthStencilState( mDepthStencils[DEPTHSTENCILS_ENABLED], 1 );
}

void Graphics::RenderNodeGrid( NodeGridInfo* info, UINT sizeOfList )
{
	mDeviceContext->IASetPrimitiveTopology( D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST );
	
	UINT32 vertexSize	= sizeof( StaticVertex );
	UINT32 offset		= 0;
	mDeviceContext->IASetInputLayout( mEffects[EFFECTS_NODEGRID]->GetInputLayout() );

	mDeviceContext->VSSetShader( mEffects[EFFECTS_NODEGRID]->GetVertexShader(), nullptr, 0 );
	mDeviceContext->HSSetShader( nullptr, nullptr, 0 );
	mDeviceContext->DSSetShader( nullptr, nullptr, 0 );
	mDeviceContext->GSSetShader( nullptr, nullptr, 0 );
	mDeviceContext->PSSetShader( mEffects[EFFECTS_NODEGRID]->GetPixelShader(), nullptr, 0 );

	for( UINT i = 0; i < sizeOfList; i++ )
	{
		CbufferPerObject cbuff;
		cbuff.worldMatrix = DirectX::XMLoadFloat4x4( &info[i].mWorld );
		MapBuffer( mBuffers[BUFFERS_CBUFFER_PER_OBJECT], &cbuff, sizeof( CbufferPerObject ) );
		mDeviceContext->VSSetConstantBuffers( 1, 1, &mBuffers[BUFFERS_CBUFFER_PER_OBJECT] );

		MapBuffer( mBuffers[BUFFERS_SINGLE_STATIC_VERTEX], info[i].mVertices, sizeof(StaticVertex) * info[i].mNrOfVertices );
		mDeviceContext->IASetVertexBuffers( 0, 1, &mBuffers[BUFFERS_SINGLE_STATIC_VERTEX], &vertexSize, &offset );

		mDeviceContext->PSSetShaderResources( 0, 1, &( (Static2dAsset*)mAssetManager->mAssetContainer[MUD_BLENDMAP] )->mSRV );
		mDeviceContext->PSSetShaderResources( 1, 1, &( (Static2dAsset*)mAssetManager->mAssetContainer[GRASS_BLENDMAP] )->mSRV );
		mDeviceContext->PSSetShaderResources( 2, 1, &( (Static2dAsset*)mAssetManager->mAssetContainer[SAND_BLENDMAP] )->mSRV );
		mDeviceContext->PSSetShaderResources( 3, 1, &( (Static2dAsset*)mAssetManager->mAssetContainer[info[i].mBlendMap] )->mSRV );
		mDeviceContext->Draw( info[i].mNrOfVertices, 0 );

	}
}

void Graphics::RenderDebugBox( BoxInfo* info, UINT sizeOfList )
{
	//mDeviceContext->OMSetDepthStencilState( mDepthStencils[DEPTHSTENCILS_DISABLED], 1 );
	DirectX::XMFLOAT3 min, max;

	mDeviceContext->IASetPrimitiveTopology( D3D11_PRIMITIVE_TOPOLOGY_LINELIST );
	mDeviceContext->VSSetShader( mEffects[EFFECTS_DEBUG_BOX]->GetVertexShader(), nullptr, 0 );
	mDeviceContext->HSSetShader( nullptr, nullptr, 0 );
	mDeviceContext->DSSetShader( nullptr, nullptr, 0 );
	mDeviceContext->GSSetShader( nullptr, nullptr, 0 );
	mDeviceContext->PSSetShader( mEffects[EFFECTS_DEBUG_BOX]->GetPixelShader(), nullptr, 0 );

	UINT32 vertexSize	= sizeof(StaticVertex);
	UINT32 offset		= 0;

	mDeviceContext->IASetIndexBuffer( mBuffers[BUFFERS_DEBUG_BOX_INDICES], DXGI_FORMAT_R32_UINT, 0 );
	mDeviceContext->IASetVertexBuffers( 0, 1, &mBuffers[BUFFERS_DEBUG_BOX], &vertexSize, &offset );
	mDeviceContext->VSSetConstantBuffers( 1, 1, &mBuffers[BUFFERS_CBUFFER_PER_OBJECT] );
	mDeviceContext->IASetInputLayout( mEffects[EFFECTS_DEBUG_BOX]->GetInputLayout() );

	for( UINT i = 0; i < sizeOfList; i++ )
	{
		max = info[i].max;
		min = info[i].min;


		DirectX::XMFLOAT3 boxSize = DirectX::XMFLOAT3( max.x - min.x, max.y - min.y, max.z - min.z );
		DirectX::XMFLOAT3 center  = DirectX::XMFLOAT3( ( min.x + max.x ) / 2, ( min.y + max.y ) / 2, ( min.z + max.z ) / 2 );

		//Map CbufferPerObject
		CbufferPerObject data;
		DirectX::XMMATRIX scaling		= DirectX::XMMatrixScaling( boxSize.x, boxSize.y, boxSize.z );
		DirectX::XMMATRIX translation	= DirectX::XMMatrixTranslation( center.x, center.y, center.z );

		data.worldMatrix = DirectX::XMMatrixTranspose( scaling * translation );

		MapBuffer( mBuffers[BUFFERS_CBUFFER_PER_OBJECT], &data, sizeof( CbufferPerObject ) );

		//mDeviceContext->Draw( ( (Static3dAsset*)mAssetManager->mAssetContainer[CUBE_PLACEHOLDER] )->mMeshes[0].mVertexCount, 0 );//, 0, 0 );
		mDeviceContext->DrawIndexed( 24, 0, 0 );
		//mDeviceContext->OMSetDepthStencilState( mDepthStencils[DEPTHSTENCILS_ENABLED], 1 );
	}
}

void Graphics::RenderDebugBox( DirectX::XMFLOAT3 min, DirectX::XMFLOAT3 max, DirectX::XMFLOAT4X4 world )
{
	//mDeviceContext->OMSetDepthStencilState( mDepthStencils[DEPTHSTENCILS_DISABLED], 1 );

	mDeviceContext->IASetPrimitiveTopology( D3D11_PRIMITIVE_TOPOLOGY_LINELIST );

	UINT32 vertexSize	= sizeof(StaticVertex);
	UINT32 offset		= 0;

	DirectX::XMFLOAT3 boxSize = DirectX::XMFLOAT3( max.x - min.x, max.y - min.y, max.z - min.z );
	DirectX::XMFLOAT3 center  = DirectX::XMFLOAT3( ( min.x + max.x ) / 2, ( min.y + max.y ) / 2, ( min.z + max.z ) / 2 );

	ID3D11Buffer* buffersToSet[] = { ( (Static3dAsset*)mAssetManager->mAssetContainer[CUBE_PLACEHOLDER] )->mMeshes[0].mVertexBuffer };
	
	mDeviceContext->IASetVertexBuffers( 0, 1, &mBuffers[BUFFERS_DEBUG_BOX], &vertexSize, &offset );
	mDeviceContext->IASetIndexBuffer( mBuffers[BUFFERS_DEBUG_BOX_INDICES], DXGI_FORMAT_R32_UINT, 0 );

	//Map CbufferPerObject
	CbufferPerObject data;
	DirectX::XMMATRIX worldMat		= DirectX::XMLoadFloat4x4( &world );
	DirectX::XMMATRIX scaling		= DirectX::XMMatrixScaling( boxSize.x, boxSize.y, boxSize.z );
	DirectX::XMMATRIX translation	= DirectX::XMMatrixTranslation( center.x, center.y, center.z );

	data.worldMatrix = worldMat * DirectX::XMMatrixTranspose( scaling * translation );
	//data.worldMatrix = DirectX::XMMatrixIdentity();
	
	MapBuffer( mBuffers[BUFFERS_CBUFFER_PER_OBJECT], &data, sizeof( CbufferPerObject ) );

	mDeviceContext->VSSetConstantBuffers( 1, 1, &mBuffers[BUFFERS_CBUFFER_PER_OBJECT] );
	
	mDeviceContext->IASetInputLayout( mEffects[EFFECTS_DEBUG_BOX]->GetInputLayout() );

	mDeviceContext->VSSetShader( mEffects[EFFECTS_DEBUG_BOX]->GetVertexShader(), nullptr, 0 );
	mDeviceContext->HSSetShader( nullptr, nullptr, 0 );
	mDeviceContext->DSSetShader( nullptr, nullptr, 0 );
	mDeviceContext->GSSetShader( nullptr, nullptr, 0 );
	mDeviceContext->PSSetShader( mEffects[EFFECTS_DEBUG_BOX]->GetPixelShader(), nullptr, 0 );

	//mDeviceContext->Draw( ( (Static3dAsset*)mAssetManager->mAssetContainer[CUBE_PLACEHOLDER] )->mMeshes[0].mVertexCount, 0 );//, 0, 0 );
	mDeviceContext->DrawIndexed( 24, 0, 0 );
	//mDeviceContext->OMSetDepthStencilState( mDepthStencils[DEPTHSTENCILS_ENABLED], 1 );
}
void Graphics::RenderLine( LineInfo* info, UINT sizeOfList )
{


	mDeviceContext->IASetPrimitiveTopology( D3D11_PRIMITIVE_TOPOLOGY_LINELIST );
	
	UINT32 vertexSize	= sizeof( StaticVertex );
	UINT32 offset		= 0;
	mDeviceContext->IASetInputLayout( mEffects[EFFECTS_DEBUG_BOX]->GetInputLayout() );

	mDeviceContext->VSSetShader( mEffects[EFFECTS_DEBUG_BOX]->GetVertexShader(), nullptr, 0 );
	mDeviceContext->HSSetShader( nullptr, nullptr, 0 );
	mDeviceContext->DSSetShader( nullptr, nullptr, 0 );
	mDeviceContext->GSSetShader( nullptr, nullptr, 0 );
	mDeviceContext->PSSetShader( mEffects[EFFECTS_DEBUG_BOX]->GetPixelShader(), nullptr, 0 );

	for( UINT i = 0; i < sizeOfList; i++ )
	{
		StaticVertex tempBuff[2];

		tempBuff[0].position[0] = info[i].start.x;
		tempBuff[0].position[1] = info[i].start.y;
		tempBuff[0].position[2] = info[i].start.z;

		tempBuff[1].position[0] = info[i].end.x;
		tempBuff[1].position[1] = info[i].end.y;
		tempBuff[1].position[2] = info[i].end.z;

		CbufferPerObject cbuff;
		cbuff.worldMatrix = DirectX::XMMatrixIdentity();
		MapBuffer( mBuffers[BUFFERS_CBUFFER_PER_OBJECT], &cbuff, sizeof( CbufferPerObject ) );
		mDeviceContext->VSSetConstantBuffers( 1, 1, &mBuffers[BUFFERS_CBUFFER_PER_OBJECT] );

		MapBuffer( mBuffers[BUFFERS_SINGLE_STATIC_VERTEX], tempBuff, sizeof(StaticVertex) * 2 );
		mDeviceContext->IASetVertexBuffers( 0, 1, &mBuffers[BUFFERS_SINGLE_STATIC_VERTEX], &vertexSize, &offset );

		mDeviceContext->PSSetShaderResources( 0, 1, &( (Static2dAsset*)mAssetManager->mAssetContainer[DIFFUSE_PLACEHOLDER] )->mSRV );
		mDeviceContext->PSSetShaderResources( 1, 1, &( (Static2dAsset*)mAssetManager->mAssetContainer[NORMAL_PLACEHOLDER] )->mSRV );
		mDeviceContext->PSSetShaderResources( 2, 1, &( (Static2dAsset*)mAssetManager->mAssetContainer[SPECULAR_PLACEHOLDER] )->mSRV );
		mDeviceContext->Draw( 2, 0 );

	}
}

DirectX::XMFLOAT4X4 Graphics::GetRootMatrix( AnimationTrack animTrack )
{
	Animated3dAsset*	model		= (Animated3dAsset*)mAssetManager->mAssetContainer[animTrack.mModelID];
	Skeleton*			skeleton	= &( (SkeletonAsset*)mAssetManager->mAssetContainer[model->mSkeletonId] )->mSkeleton;
	AnimationData*		animation	= &( (AnimationAsset*)mAssetManager->mAssetContainer[animTrack.mCurrentAnimation] )->mAnimationData;

	bool isAnimationBlending		= false;

	DirectX::XMMATRIX currMatrix;
	DirectX::XMMATRIX nextMatrix;

	//////////////////////////////////////////////////////////
	//			Interpolation evaluation step
	//////////////////////////////////////////////////////////
	if( animTrack.mInterpolation > 0.0f ) // generate matrices to blend between interpolation
	{
		isAnimationBlending = true;

		AnimationData*	animation2	= &( (AnimationAsset*)mAssetManager->mAssetContainer[animTrack.mNextAnimation] )->mAnimationData;

		float calcTime = animTrack.mNextAnimationTime * 60.0f;

		float prevTime		= 1.0f;
		float targetTime	= 1.0f;

		DirectX::XMFLOAT4X4 targetMatrix	= animation2->joints.at(0).matricies.at(0);
		DirectX::XMFLOAT4X4	previousMatrix	= targetMatrix;

		for( int j = 0; j < (int)animation2->joints.at(0).keys.size() - 1; j++ )
		{
			if( (float)animation2->joints.at(0).keys.at(j) < calcTime )
			{
				prevTime		= targetTime;
				previousMatrix	= targetMatrix;
				targetTime		= (float)animation2->joints.at(0).keys.at(j+1);
				targetMatrix	= animation2->joints.at(0).matricies.at(j+1);
			}
		}

		float interpolation = 1.0f;
		if( targetTime - prevTime > 0.0f )
			interpolation = ( calcTime - prevTime ) / ( targetTime - prevTime );

		DirectX::XMMATRIX child	= DirectX::XMLoadFloat4x4( &previousMatrix );

		DirectX::XMVECTOR targetComp[3];
		DirectX::XMVECTOR childComp[3];

		DirectX::XMMatrixDecompose( &targetComp[0], &targetComp[1], &targetComp[2], DirectX::XMLoadFloat4x4( &targetMatrix ) );
		DirectX::XMMatrixDecompose( &childComp[0], &childComp[1], &childComp[2], child );

		nextMatrix = DirectX::XMMatrixAffineTransformation(	DirectX::XMVectorLerp( childComp[0], targetComp[0], interpolation ),
														DirectX::XMVectorZero(),
														DirectX::XMQuaternionSlerp( childComp[1], targetComp[1], interpolation ),
														DirectX::XMVectorLerp( childComp[2], targetComp[2], interpolation ) );
	}

	/////////////////////////////////////////////////////////////
	//			Calculate matrices for current animation
	/////////////////////////////////////////////////////////////

	float calcTime = animTrack.mCurrentAnimationTime * 60.0f;

	float prevTime		= 1.0f;
	float targetTime	= 1.0f;

	DirectX::XMFLOAT4X4 targetMatrix	= animation->joints.at(0).matricies.at(0);
	DirectX::XMFLOAT4X4	previousMatrix	= targetMatrix;

	for( int j = 0; j < (int)animation->joints.at(0).keys.size() - 1; j++ )
	{
		if( (float)animation->joints.at(0).keys.at(j) < calcTime )
		{
			prevTime		= targetTime;
			previousMatrix	= targetMatrix;
			targetTime		= (float)animation->joints.at(0).keys.at(j+1);
			targetMatrix	= animation->joints.at(0).matricies.at(j+1);
		}
	}

	float interpolation = 1.0f;
	if( targetTime - prevTime > 0.0f )
		interpolation = ( calcTime - prevTime ) / ( targetTime - prevTime );

	DirectX::XMMATRIX child		= DirectX::XMLoadFloat4x4( &previousMatrix );

	DirectX::XMVECTOR targetComp[3];
	DirectX::XMVECTOR childComp[3];

	DirectX::XMMatrixDecompose( &targetComp[0], &targetComp[1], &targetComp[2], DirectX::XMLoadFloat4x4( &targetMatrix ) );
	DirectX::XMMatrixDecompose( &childComp[0], &childComp[1], &childComp[2], child );

	currMatrix = DirectX::XMMatrixAffineTransformation(	DirectX::XMVectorLerp( childComp[0], targetComp[0], interpolation ),
														DirectX::XMVectorZero(),
														DirectX::XMQuaternionSlerp( childComp[1], targetComp[1], interpolation ),
														DirectX::XMVectorLerp( childComp[2], targetComp[2], interpolation ) );		

	if( isAnimationBlending )
	{
		float blendInterpolation = animTrack.mInterpolation / 0.2f;
		DirectX::XMVECTOR currComp[3];
		DirectX::XMVECTOR nextComp[3];

		DirectX::XMMatrixDecompose( &currComp[0], &currComp[1], &currComp[2], currMatrix );
		DirectX::XMMatrixDecompose( &nextComp[0], &nextComp[1], &nextComp[2], nextMatrix );

		currMatrix = DirectX::XMMatrixAffineTransformation(	DirectX::XMVectorLerp( nextComp[0], currComp[0], blendInterpolation ),
																				DirectX::XMVectorZero(),
																				DirectX::XMQuaternionSlerp( nextComp[1], currComp[1], blendInterpolation ),
																				DirectX::XMVectorLerp( nextComp[2], currComp[2], blendInterpolation ) );
	}

	DirectX::XMFLOAT4X4 output;
	DirectX::XMStoreFloat4x4( &output, currMatrix );
	return output;
}

bool Graphics::GetAnimationMatrices( AnimationTrack &animTrack, int playType, Anim3dInfo &info )
{
	Animated3dAsset*	model		= (Animated3dAsset*)mAssetManager->mAssetContainer[animTrack.mModelID];
	Skeleton*			skeleton	= &( (SkeletonAsset*)mAssetManager->mAssetContainer[model->mSkeletonId] )->mSkeleton;
	AnimationData*		animation	= &( (AnimationAsset*)mAssetManager->mAssetContainer[animTrack.mCurrentAnimation] )->mAnimationData;

	bool isAnimationBlending		= false;

	DirectX::XMMATRIX currentBoneTransforms[NUM_SUPPORTED_JOINTS];
	DirectX::XMMATRIX nextBoneTransforms[NUM_SUPPORTED_JOINTS];
	for( int i = 0; i < NUM_SUPPORTED_JOINTS; i++ )
	{
		currentBoneTransforms[i] = DirectX::XMMatrixIdentity();
	}

	//////////////////////////////////////////////////////////
	//			Interpolation evaluation step
	//////////////////////////////////////////////////////////
	if( animTrack.mInterpolation > 0.0f ) // generate matrices to blend between interpolation
	{
		isAnimationBlending = true;

		for( int i = 0; i < NUM_SUPPORTED_JOINTS; i++ )
		{
			nextBoneTransforms[i] = DirectX::XMMatrixIdentity();
		}
		AnimationData*	animation2	= &( (AnimationAsset*)mAssetManager->mAssetContainer[animTrack.mNextAnimation] )->mAnimationData;

		if( animTrack.mNextAnimationTime >= (float)animation2->AnimLength / 60.0f )
		{
			if( playType == ANIMATION_PLAY_LOOPED )
				animTrack.mNextAnimationTime -= ( (float)animation2->AnimLength / 60.0f - 1.0f / 60.0f );
			else if( playType == ANIMATION_PLAY_ONCE )
				animTrack.mNextAnimationTime = ( (float)animation2->AnimLength / 60.0f );
		}

			float calcTime = animTrack.mNextAnimationTime * 60.0f;

			for( int i = 0; i < (int)skeleton->joints.size(); i++ )
			{
				float prevTime		= 1.0f;
				float targetTime	= 1.0f;

				DirectX::XMFLOAT4X4 targetMatrix	= animation2->joints.at(i).matricies.at(0);
				DirectX::XMFLOAT4X4	previousMatrix	= targetMatrix;

				for( int j = 0; j < (int)animation2->joints.at(i).keys.size() - 1; j++ )
				{
					if( (float)animation2->joints.at(i).keys.at(j) < calcTime )
					{
						prevTime		= targetTime;
						previousMatrix	= targetMatrix;
						targetTime		= (float)animation2->joints.at(i).keys.at(j+1);
						targetMatrix	= animation2->joints.at(i).matricies.at(j+1);
					}
				}

				float interpolation = 1.0f;
				if( targetTime - prevTime > 0.0f )
					interpolation = ( calcTime - prevTime ) / ( targetTime - prevTime );

				DirectX::XMMATRIX child		= DirectX::XMLoadFloat4x4( &previousMatrix );

				DirectX::XMVECTOR targetComp[3];
				DirectX::XMVECTOR childComp[3];

				DirectX::XMMatrixDecompose( &targetComp[0], &targetComp[1], &targetComp[2], DirectX::XMLoadFloat4x4( &targetMatrix ) );
				DirectX::XMMatrixDecompose( &childComp[0], &childComp[1], &childComp[2], child );

				child = DirectX::XMMatrixAffineTransformation(	DirectX::XMVectorLerp( childComp[0], targetComp[0], interpolation ),
																	DirectX::XMVectorZero(),
																	DirectX::XMQuaternionSlerp( childComp[1], targetComp[1], interpolation ),
																	DirectX::XMVectorLerp( childComp[2], targetComp[2], interpolation ) );		

				DirectX::XMMATRIX parent	= animation2->joints.at(i).parentIndex == -1 ? DirectX::XMMatrixIdentity() :
													nextBoneTransforms[animation2->joints.at(i).parentIndex];

				nextBoneTransforms[i] = child * parent;
			}

	}

	/////////////////////////////////////////////////////////////
	//			Calculate matrices for current animation
	/////////////////////////////////////////////////////////////

	bool localReturn = false;

	if( animTrack.mCurrentAnimationTime >= (float)animation->AnimLength / 60.0f )
	{
		if( playType == ANIMATION_PLAY_LOOPED )
			animTrack.mCurrentAnimationTime -= ( (float)animation->AnimLength / 60.0f - 1.0f / 60.0f );
		else if( playType == ANIMATION_PLAY_ONCE )
			animTrack.mCurrentAnimationTime = ( (float)animation->AnimLength / 60.0f );

		if( !isAnimationBlending )
			localReturn = true;
	}

	float calcTime = animTrack.mCurrentAnimationTime * 60.0f;

	for( int i = 0; i < (int)skeleton->joints.size(); i++ )
	{
		float prevTime		= 1.0f;
		float targetTime	= 1.0f;

		DirectX::XMFLOAT4X4 targetMatrix	= animation->joints.at(i).matricies.at(0);
		DirectX::XMFLOAT4X4	previousMatrix	= targetMatrix;

		for( int j = 0; j < (int)animation->joints.at(i).keys.size() - 1; j++ )
		{
			if( (float)animation->joints.at(i).keys.at(j) < calcTime )
			{
				prevTime		= targetTime;
				previousMatrix	= targetMatrix;
				targetTime		= (float)animation->joints.at(i).keys.at(j+1);
				targetMatrix	= animation->joints.at(i).matricies.at(j+1);
			}
		}

		float interpolation = 1.0f;
		if( targetTime - prevTime > 0.0f )
			interpolation = ( calcTime - prevTime ) / ( targetTime - prevTime );

		DirectX::XMMATRIX child		= DirectX::XMLoadFloat4x4( &previousMatrix );

		DirectX::XMVECTOR targetComp[3];
		DirectX::XMVECTOR childComp[3];

		DirectX::XMMatrixDecompose( &targetComp[0], &targetComp[1], &targetComp[2], DirectX::XMLoadFloat4x4( &targetMatrix ) );
		DirectX::XMMatrixDecompose( &childComp[0], &childComp[1], &childComp[2], child );

		child = DirectX::XMMatrixAffineTransformation(	DirectX::XMVectorLerp( childComp[0], targetComp[0], interpolation ),
															DirectX::XMVectorZero(),
															DirectX::XMQuaternionSlerp( childComp[1], targetComp[1], interpolation ),
															DirectX::XMVectorLerp( childComp[2], targetComp[2], interpolation ) );		

		DirectX::XMMATRIX parent	= animation->joints.at(i).parentIndex == -1 ? DirectX::XMMatrixIdentity() :
											currentBoneTransforms[animation->joints.at(i).parentIndex];

		currentBoneTransforms[i] = child * parent;
	}

	if( isAnimationBlending )
	{
		float blendInterpolation = animTrack.mInterpolation / ( animTrack.mBlendWithCurrent ? 0.2f : 0.2f );

		if( animTrack.mBlendWithCurrent )
		{
			blendInterpolation *= 2.0f;
			if( blendInterpolation < 1.0f )
			{
				blendInterpolation = 2.0f - blendInterpolation;
			}
			blendInterpolation -= 1.0f;
		}

		DirectX::XMVECTOR currComp[3];
		DirectX::XMVECTOR nextComp[3];

		for( int j = 0; j < NUM_SUPPORTED_JOINTS; j++ )
		{
			DirectX::XMMatrixDecompose( &currComp[0], &currComp[1], &currComp[2], currentBoneTransforms[j] );
			DirectX::XMMatrixDecompose( &nextComp[0], &nextComp[1], &nextComp[2], nextBoneTransforms[j] );

			currentBoneTransforms[j] = DirectX::XMMatrixAffineTransformation(	DirectX::XMVectorLerp( nextComp[0], currComp[0], blendInterpolation ),
																				DirectX::XMVectorZero(),
																				DirectX::XMQuaternionSlerp( nextComp[1], currComp[1], blendInterpolation ),
																				DirectX::XMVectorLerp( nextComp[2], currComp[2], blendInterpolation ) );
		}
	}

	for( int i = 0; i < NUM_SUPPORTED_JOINTS; i++ )
		DirectX::XMStoreFloat4x4( &info.mBoneTransforms[i], DirectX::XMMatrixIdentity() );
	for( int i = 0; i < skeleton->nrOfJoints; i++ )
		DirectX::XMStoreFloat4x4( &info.mBoneTransforms[i], DirectX::XMMatrixTranspose( DirectX::XMMatrixMultiply( DirectX::XMLoadFloat4x4( &model->mBoneOffsets[i] ), currentBoneTransforms[i] ) ) );

	return localReturn;
}

void Graphics::ChangeCamera()
{
	if( mIsDeveloperCameraActive )
	{
		mIsDeveloperCameraActive = false;
		mCurrentCamera = CAMERAS_MAIN;
	}

	else
	{
		mIsDeveloperCameraActive = true;
		mCurrentCamera = CAMERAS_DEV;
	}
}

void Graphics::ZoomInDeveloperCamera()
{
	if ( mCamera[CAMERAS_DEV]->GetHeight() >= 1.0f )
		mCamera[CAMERAS_DEV]->ZoomIn();

	mCamera[CAMERAS_DEV]->ZoomIn();
}

void Graphics::ZoomOutDeveloperCamera()
{
	mCamera[CAMERAS_DEV]->ZoomOut();
}

void Graphics::GetViewMatrix( DirectX::XMMATRIX &view )
{
	view = mCamera[mCurrentCamera]->GetViewMatrix();
}

void Graphics::GetInverseViewMatrix( DirectX::XMMATRIX &inverseViewMatrix )
{
	inverseViewMatrix = mCamera[mCurrentCamera]->GetInverseViewMatrix();
}

void Graphics::GetProjectionMatrix( DirectX::XMMATRIX &proj )
{
	proj = mCamera[mCurrentCamera]->GetProjMatrix();
}

void Graphics::GetInverseProjectionMatrix( DirectX::XMMATRIX &projectionViewMatrix )
{
	projectionViewMatrix = mCamera[mCurrentCamera]->GetInverseProjectionMatrix();
}

void Graphics::ChangeRasterizerState( RasterizerStates rasterState )
{
	mDeviceContext->RSSetState( mRasterizerState[rasterState] );
}

void Graphics::MapLightStructuredBuffer( LightStructure* lightStructure, int numPointLights )
{
	MapBuffer( mBuffers[BUFFERS_LIGHT], (void*)lightStructure, sizeof( LightStructure ) );
	mNumPointLights = numPointLights;
}

void Graphics::SetNDCSpaceCoordinates( float &mousePositionX, float &mousePositionY )
{
	//Calculate mouse position in NDC space
	mousePositionX	= ( ( 2.0f *  mousePositionX ) / mScreenWidth  - 1.0f );
	mousePositionY	= ( ( 2.0f * -mousePositionY ) / mScreenHeight + 1.0f );
}

void Graphics::SetEyePosition( Cameras camera, DirectX::XMFLOAT3 &eyePosition )
{
	if( camera == CAMERAS_MAIN )
	{
		mCamera[CAMERAS_MAIN]->SetEyePosition( eyePosition );
		DirectX::XMFLOAT4 camPos = mCamera[CAMERAS_DEV]->GetPos();
		mCamera[CAMERAS_DEV]->SetEyePosition( DirectX::XMFLOAT3(  eyePosition.x, camPos.y,  eyePosition.z ) );
	}
	else if( camera == CAMERAS_SHADOWMAP )
	{
		mCamera[CAMERAS_SHADOWMAP]->SetEyePosition( eyePosition );
	}
}

void Graphics::SetFocus( Cameras camera, DirectX::XMFLOAT3 &focusPoint )
{
	if( camera == CAMERAS_MAIN )
	{
		mCamera[CAMERAS_MAIN]->SetFocus( focusPoint );
		mCamera[CAMERAS_DEV]->SetFocus( focusPoint );
	}
	else if( camera == CAMERAS_SHADOWMAP )
	{
		mCamera[CAMERAS_SHADOWMAP]->SetFocus( focusPoint );
	}
}

//Clear canvas and prepare for rendering.
void Graphics::BeginScene()
{
	mDeviceContext->ClearState();

	/////////////////////////////////
	// Clear errything
	/////////////////////////////////
	static float clearColor[4] = { 0.0f, 0.0f, 0.0f, 1.0f };
	mDeviceContext->ClearRenderTargetView( mRenderTargetView, clearColor );

	static float blendColor[4] = { 0.0f, 0.0f, 0.0f, 0.0f };
	mDeviceContext->OMSetBlendState( nullptr, blendColor, 0xffffffff );

	mDeviceContext->ClearDepthStencilView( mDepthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0 );
	mDeviceContext->OMSetDepthStencilState( mDepthStencils[DEPTHSTENCILS_ENABLED], 1 );
}

void Graphics::GbufferPass()
{

	mCamera[mCurrentCamera]->Update();

	mCamera[CAMERAS_SHADOWMAP]->Update();

	static float clearColor[4] = { 0.0f, 0.0f, 0.0f, 1.0f };
	for( int i = 0; i < NUM_GBUFFERS; i++ )
		mDeviceContext->ClearRenderTargetView( mGbuffers[i]->mRenderTargetView, clearColor );
	mDeviceContext->ClearDepthStencilView( mDepthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0 );

	mDeviceContext->ClearDepthStencilView( mShadowMapDSV, D3D11_CLEAR_DEPTH, 1.0f, 0 );
	
	/////////////////////////////////
	// Gbuffer generation pass
	/////////////////////////////////
	ID3D11RenderTargetView* rtvsToSet[NUM_GBUFFERS];
	for( int i = 0; i < NUM_GBUFFERS; i++ )
		rtvsToSet[i] = mGbuffers[i]->mRenderTargetView;
	mDeviceContext->OMSetRenderTargets( 3, rtvsToSet, mDepthStencilView );
	mDeviceContext->RSSetViewports( 1, &mStandardView );

	//Map CbufferPerFrame
	CbufferPerFrame data;


	data.viewMatrix			= mCamera[mCurrentCamera]->GetViewMatrix();
	data.projectionMatrix	= mCamera[mCurrentCamera]->GetProjMatrix();
	data.cameraPosition		= mCamera[mCurrentCamera]->GetPos();
	
	data.numPointLights = mNumPointLights;
	MapBuffer( mBuffers[BUFFERS_CBUFFER_PER_FRAME], &data, sizeof( CbufferPerFrame ) );

	CbufferPerFrameShadow dataShadow;
	//Map ShadowPerFrame
	dataShadow.viewMatrix		= mCamera[CAMERAS_SHADOWMAP]->GetViewMatrix();
	dataShadow.projectionMatrix	= mCamera[CAMERAS_SHADOWMAP]->GetProjMatrix();
	dataShadow.cameraPosition	= mCamera[CAMERAS_SHADOWMAP]->GetPos();

	MapBuffer( mBuffers[BUFFERS_CBUFFER_PER_FRAME_SHADOW], &dataShadow, sizeof( CbufferPerFrameShadow ) );

	mDeviceContext->VSSetConstantBuffers( 0, 1, &mBuffers[BUFFERS_CBUFFER_PER_FRAME] );
	mDeviceContext->VSSetConstantBuffers( 2, 1, &mBuffers[BUFFERS_CBUFFER_PER_FRAME_SHADOW] );

	for( int i = 0; i < SAMPLERS_AMOUNT; i++ )
	mDeviceContext->PSSetSamplers( i, 1, &mSamplerStates[i] );
}

void Graphics::DeferredPass()
{
	/////////////////////////////////
	// Deferred rendering pass
	/////////////////////////////////
	mDeviceContext->ClearState();

	CbufferPerFrameShadow data;
	data.viewMatrix			= mCamera[CAMERAS_SHADOWMAP]->GetViewMatrix();
	data.projectionMatrix	= mCamera[CAMERAS_SHADOWMAP]->GetProjMatrix();
	data.cameraPosition		= mCamera[CAMERAS_SHADOWMAP]->GetPos();

	MapBuffer( mBuffers[BUFFERS_CBUFFER_PER_FRAME_SHADOW], &data, sizeof( CbufferPerFrameShadow ) );
	mDeviceContext->PSSetConstantBuffers( 1, 1, &mBuffers[BUFFERS_CBUFFER_PER_FRAME_SHADOW] );

	mDeviceContext->OMSetRenderTargets( 1, &mRenderTargetView, nullptr );
	mDeviceContext->RSSetViewports( 1, &mStandardView );
	mDeviceContext->IASetPrimitiveTopology( D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP );
	
	for( int i = 0; i < NUM_GBUFFERS; i++ )
		mDeviceContext->PSSetShaderResources( i, 1, &mGbuffers[i]->mShaderResourceView );

	mDeviceContext->PSSetShaderResources( 4, 1, &mShadowMapSRV );
	mDeviceContext->PSSetShaderResources( 5, 1, &mLightStructuredBuffer );

	mDeviceContext->VSSetShader( mEffects[EFFECTS_DEFERRED]->GetVertexShader(), nullptr, 0 );
	mDeviceContext->HSSetShader( nullptr, nullptr, 0 );
	mDeviceContext->DSSetShader( nullptr, nullptr, 0 );
	mDeviceContext->GSSetShader( nullptr, nullptr, 0 );
	mDeviceContext->PSSetShader( mEffects[EFFECTS_DEFERRED]->GetPixelShader(), nullptr, 0 );

	for( int i = 0; i < SAMPLERS_AMOUNT; i++ )
		mDeviceContext->PSSetSamplers( i, 1, &mSamplerStates[i] );

	mDeviceContext->PSSetConstantBuffers( 0, 1, &mBuffers[BUFFERS_CBUFFER_PER_FRAME] );

	mDeviceContext->Draw( 4, 0 );

	mDeviceContext->OMSetRenderTargets( 1, &mRenderTargetView, mDepthStencilView );
}

void Graphics::ScreenSpacePass()
{
	mDeviceContext->IASetPrimitiveTopology( D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP );
	mDeviceContext->OMSetDepthStencilState( mDepthStencils[DEPTHSTENCILS_DISABLED], 1 );
	static float blend[4] = { 1.0f, 1.0f, 1.0f, 1.0f };
	mDeviceContext->OMSetBlendState( mBlendStates[BLEND_NORMAL], blend, 0xffffffff );

	mDeviceContext->IASetInputLayout( mEffects[EFFECTS_2D]->GetInputLayout() );

	mDeviceContext->VSSetShader( mEffects[EFFECTS_2D]->GetVertexShader(), nullptr, 0 );
	mDeviceContext->HSSetShader( nullptr, nullptr, 0 );
	mDeviceContext->DSSetShader( nullptr, nullptr, 0 );
	mDeviceContext->GSSetShader( nullptr, nullptr, 0 );
	mDeviceContext->PSSetShader( mEffects[EFFECTS_2D]->GetPixelShader(), nullptr, 0 );
}

//Finalize rendering.
void Graphics::EndScene()
{
	mSwapChain->Present( 1, 0 );
}

void Graphics::ToggleFullscreen()
{
	mFullscreen = !mFullscreen;
	mSwapChain->SetFullscreenState( mFullscreen, NULL );
}

//Singleton for the Graphics dll.
Graphics* Graphics::GetInstance()
{
	static Graphics instance;
	return &instance;
}

//Initialize graphics interfaces.
HRESULT Graphics::Initialize( HWND hWnd, UINT screenWidth, UINT screenHeight, bool fullscreen )
{
	mHWnd			= hWnd;
	mScreenWidth	= screenWidth;
	mScreenHeight	= screenHeight;
	mFullscreen		= fullscreen;
	mCurrentCamera	= CAMERAS_MAIN;

	HRESULT hr		= E_FAIL;

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
	swapChainDesc.Windowed								= !mFullscreen;
	swapChainDesc.Flags									= DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;
	swapChainDesc.SwapEffect							= DXGI_SWAP_EFFECT_DISCARD;

	D3D_FEATURE_LEVEL featureLevelsToTry[] = { D3D_FEATURE_LEVEL_11_0 };
	D3D_FEATURE_LEVEL initiatedFeatureLevel;

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

	// DepthStencilStates Init
	if( FAILED( InitializeDepthStencilStates() ) ) return hr;

	//////////////////////////////
	// CREATE VIEWPORT
	//////////////////////////////
	mStandardView.Height	= (float)screenHeight;
	mStandardView.Width		= (float)screenWidth;
	mStandardView.MaxDepth	= 1.0f;
	mStandardView.MinDepth	= 0.0f;
	mStandardView.TopLeftX	= 0.0f;
	mStandardView.TopLeftY	= 0.0f;

	// SamplerStates Init
	if( FAILED( InitializeSamplerStates() ) ) return hr;

	//////////////////////////////
	// CREATE BLEND STATE
	//////////////////////////////
	D3D11_BLEND_DESC blendDesc;
	ZeroMemory( &blendDesc, sizeof( blendDesc ) );

	//2d blend state
	blendDesc.AlphaToCoverageEnable					= FALSE;
	blendDesc.IndependentBlendEnable				= FALSE;
	blendDesc.RenderTarget[0].BlendEnable			= TRUE;
	blendDesc.RenderTarget[0].SrcBlend				= D3D11_BLEND_SRC_ALPHA;
	blendDesc.RenderTarget[0].DestBlend				= D3D11_BLEND_INV_SRC_ALPHA;
	blendDesc.RenderTarget[0].BlendOp				= D3D11_BLEND_OP_ADD;
	blendDesc.RenderTarget[0].SrcBlendAlpha			= D3D11_BLEND_INV_DEST_ALPHA;
	blendDesc.RenderTarget[0].DestBlendAlpha		= D3D11_BLEND_ONE;
	blendDesc.RenderTarget[0].BlendOpAlpha			= D3D11_BLEND_OP_ADD;
	blendDesc.RenderTarget[0].RenderTargetWriteMask	= D3D11_COLOR_WRITE_ENABLE_ALL;

	if( FAILED( hr = mDevice->CreateBlendState( &blendDesc, &mBlendStates[BLEND_NORMAL] ) ) )
		return hr;

	//Additive blend state
	blendDesc.AlphaToCoverageEnable					= FALSE;
	blendDesc.IndependentBlendEnable				= TRUE;
	blendDesc.RenderTarget[0].BlendEnable			= TRUE;
	blendDesc.RenderTarget[0].SrcBlend				= D3D11_BLEND_SRC_ALPHA;
	blendDesc.RenderTarget[0].DestBlend				= D3D11_BLEND_ONE;
	blendDesc.RenderTarget[0].BlendOp				= D3D11_BLEND_OP_ADD;
	blendDesc.RenderTarget[0].SrcBlendAlpha			= D3D11_BLEND_SRC_ALPHA;
	blendDesc.RenderTarget[0].DestBlendAlpha		= D3D11_BLEND_ONE;
	blendDesc.RenderTarget[0].BlendOpAlpha			= D3D11_BLEND_OP_ADD;
	blendDesc.RenderTarget[0].RenderTargetWriteMask	= D3D11_COLOR_WRITE_ENABLE_ALL;

	if( FAILED( hr = mDevice->CreateBlendState( &blendDesc, &mBlendStates[BLEND_ADD] ) ) )
		return hr;

	//////////////////////////////
	// CREATE BLEND STATE
	//////////////////////////////
	D3D11_RASTERIZER_DESC rasterizerDesc;
	ZeroMemory(&rasterizerDesc, sizeof(rasterizerDesc));

	rasterizerDesc.DepthBias			= 1;
	rasterizerDesc.SlopeScaledDepthBias	= 2.0f;
	rasterizerDesc.DepthBiasClamp		= 2.0f;
	rasterizerDesc.DepthClipEnable		= true;

	//No cull
	rasterizerDesc.CullMode				= D3D11_CULL_NONE;
	rasterizerDesc.FillMode				= D3D11_FILL_SOLID;
	if( FAILED( hr = mDevice->CreateRasterizerState( &rasterizerDesc, &mRasterizerState[CULL_NONE] ) ) )
		return hr;

	//cull back
	rasterizerDesc.CullMode				= D3D11_CULL_BACK;
	if( FAILED( hr = mDevice->CreateRasterizerState( &rasterizerDesc, &mRasterizerState[CULL_BACK] ) ) )
		return hr;

	//wireframe
	rasterizerDesc.FillMode				= D3D11_FILL_WIREFRAME;
	if( FAILED( hr = mDevice->CreateRasterizerState( &rasterizerDesc, &mRasterizerState[WIREFRAME] ) ) )
		return hr;

	if( FAILED( InitializeBuffers() ) ) return hr;

	//AssetManager
	mAssetManager = new AssetManager;
	mAssetManager->Initialize( mDevice, mDeviceContext );

	if( FAILED( InitializeEffects() ) ) return hr;


	//Gbuffers
	for( int i = 0; i < NUM_GBUFFERS; i++ )
	{
		mGbuffers[i] = new Gbuffer;
		mGbuffers[i]->Initialize( mDevice, mScreenWidth, mScreenHeight );
	}

	////////////////////////////////////
	//		SHADOW MAP VARIABLES
	////////////////////////////////////

	ID3D11Texture2D* shadowMap;

	D3D11_TEXTURE2D_DESC texDesc;
	ZeroMemory( &texDesc, sizeof( texDesc ) );
	texDesc.Width				= SHADOW_MAP_WIDTH;
	texDesc.Height				= SHADOW_MAP_HEIGHT;
	texDesc.MipLevels			= 1;
	texDesc.ArraySize			= 1;
	texDesc.Format				= DXGI_FORMAT_R32_TYPELESS;
	texDesc.SampleDesc.Count	= 1;
	texDesc.SampleDesc.Quality	= 0;
	texDesc.Usage				= D3D11_USAGE_DEFAULT;
	texDesc.BindFlags			= D3D11_BIND_DEPTH_STENCIL | D3D11_BIND_SHADER_RESOURCE;

	if( FAILED( hr = mDevice->CreateTexture2D( &texDesc, nullptr, &shadowMap ) ) )
		return hr;

	D3D11_DEPTH_STENCIL_VIEW_DESC depDesc;
	ZeroMemory( &depDesc, sizeof( depDesc ) );
	depDesc.Format				= DXGI_FORMAT_D32_FLOAT;
	depDesc.ViewDimension		= D3D11_DSV_DIMENSION_TEXTURE2D;
	depDesc.Texture2D.MipSlice	= 0;

	if( FAILED( hr = mDevice->CreateDepthStencilView( shadowMap, &depDesc, &mShadowMapDSV ) ) )
		return hr;

	D3D11_SHADER_RESOURCE_VIEW_DESC shaDesc;
	ZeroMemory( &shaDesc, sizeof( shaDesc ) );
	shaDesc.Format				= DXGI_FORMAT_R32_FLOAT;
	shaDesc.ViewDimension		= D3D11_SRV_DIMENSION_TEXTURE2D;
	shaDesc.Texture2D.MipLevels	= 1;

	if( FAILED( hr = mDevice->CreateShaderResourceView( shadowMap, &shaDesc, &mShadowMapSRV ) ) )
		return hr;

	SAFE_RELEASE( shadowMap );

	mShadowView.Height		= (float)SHADOW_MAP_HEIGHT;
	mShadowView.Width		= (float)SHADOW_MAP_WIDTH;
	mShadowView.MaxDepth	= 1.0f;
	mShadowView.MinDepth	= 0.0f;
	mShadowView.TopLeftX	= 0.0f;
	mShadowView.TopLeftY	= 0.0f;

	//Camera
	mCamera[CAMERAS_MAIN] = new Camera;

	CameraInfo cameraInfo;
	ZeroMemory( &cameraInfo, sizeof( cameraInfo ) );
	cameraInfo.eyePos		= DirectX::XMFLOAT4( 0.0f, 27.0f, -22.25f, 0.0f );
	cameraInfo.focusPoint	= DirectX::XMFLOAT4( 0.0f, 0.0f, 0.0f, 1.0f );
	cameraInfo.up			= DirectX::XMFLOAT4( 0.0f, 1.0f, 0.0f, 1.0f );
	cameraInfo.width		= (float)screenWidth;
	cameraInfo.height		= (float)screenHeight;
	cameraInfo.foVY			= 3.14159265f * 0.25f;
	cameraInfo.nearZ		= 0.1f;
	cameraInfo.farZ			= 100.0f;

	if( FAILED( hr = mCamera[CAMERAS_MAIN]->Initialize( &cameraInfo ) ) )
		return hr;

	//Developer Camera
	mCamera[CAMERAS_DEV] = new Camera;

	cameraInfo.eyePos		= DirectX::XMFLOAT4( 0.0f, 50.0f, -50.0f, 0.0f );
	cameraInfo.focusPoint	= DirectX::XMFLOAT4( 0.0f, 0.0f, 0.0f, 1.0f );
	cameraInfo.up			= DirectX::XMFLOAT4( 0.0f, 1.0f, 0.0f, 1.0f );
	cameraInfo.farZ			= 100.0f;

	hr = mCamera[CAMERAS_DEV]->Initialize( &cameraInfo );
	if( FAILED( hr ) )
		return hr;

	//ShadowMap Camera
	mCamera[CAMERAS_SHADOWMAP] = new Camera;

	CameraInfo shadowMapCameraInfo;
	ZeroMemory( &shadowMapCameraInfo, sizeof( shadowMapCameraInfo ) );
	shadowMapCameraInfo.eyePos		= DirectX::XMFLOAT4( 0.0f, 30.0f, 0.0f, 1.0f );
	shadowMapCameraInfo.focusPoint	= DirectX::XMFLOAT4( 0.0f, 0.0f, 0.0f, 1.0f );
	shadowMapCameraInfo.up			= DirectX::XMFLOAT4( 0.0f, 0.0f, 1.0f, 0.0f );
	shadowMapCameraInfo.width		= (float)SHADOW_MAP_WIDTH;
	shadowMapCameraInfo.height		= (float)SHADOW_MAP_HEIGHT;
	shadowMapCameraInfo.foVY		= 3.14159265f * 0.4f;
	shadowMapCameraInfo.nearZ		= 15.0f;
	shadowMapCameraInfo.farZ		= 40.0f;

	hr = mCamera[CAMERAS_SHADOWMAP]->Initialize( &shadowMapCameraInfo );
	if( FAILED( hr ) )
		return hr;


	OutputDebugString( L"----- Graphics Initialization Complete. -----" );

	return hr;
}

//Release all the stuff.
void Graphics::Release()
{
	//ID3D11Debug* debug;
	//mDevice->QueryInterface( __uuidof( ID3D11Debug ), reinterpret_cast<void**>( &debug ) );

	SAFE_RELEASE( mSwapChain );
	SAFE_RELEASE( mDevice );
	SAFE_RELEASE( mDeviceContext );
	SAFE_RELEASE( mRenderTargetView );
	SAFE_RELEASE( mDepthStencilView );
	SAFE_RELEASE( mLightStructuredBuffer );
	SAFE_RELEASE( mShadowMapDSV );
	SAFE_RELEASE( mShadowMapSRV );

	SAFE_RELEASE_DELETE( mAssetManager );

	for( int i = 0; i < CAMERAS_AMOUNT; i++ )
	{
		SAFE_RELEASE_DELETE( mCamera[i] );
	}

	for( int i = 0; i < BLEND_STATES_AMOUNT; i++ )
	{
		SAFE_RELEASE( mBlendStates[i] );
	}

	for( int i = 0; i < BUFFERS_AMOUNT; i++ )
	{
		SAFE_RELEASE( mBuffers[i] );
	}

	for( int i = 0; i < DEPTHSTENCILS_AMOUNT; i++ )
	{
		SAFE_RELEASE( mDepthStencils[i] );
	}

	for( int i = 0; i < EFFECTS_AMOUNT; i++ )
	{
		SAFE_RELEASE_DELETE( mEffects[i] );
	}

	for( int i = 0; i < SAMPLERS_AMOUNT; i++ )
	{
		SAFE_RELEASE( mSamplerStates[i] );
	}
	for( int i = 0; i < RASTERIZER_STATES_AMOUNT; i++ )
	{
		SAFE_RELEASE( mRasterizerState[i] );
	}

	for( int i = 0; i < NUM_GBUFFERS; i++ )
	{
		SAFE_RELEASE_DELETE( mGbuffers[i] );
	}

	//debug->ReportLiveDeviceObjects( D3D11_RLDO_DETAIL );
}
