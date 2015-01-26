#include "RenderManager.h"
#include <Graphics.h>

#pragma region Private functions
void RenderManager::SetLightStructuredBuffer()
{
	Graphics::GetInstance()->MapLightStructuredBuffer( mLightManager->GetLightStructure() );
}

void RenderManager::Clear()
{
	//Object3d
	Object3dInfo clearObject3d;
	clearObject3d.mAssetId = (UINT)-1;
	DirectX::XMStoreFloat4x4( &clearObject3d.mWorld, DirectX::XMMatrixIdentity() );

	for( UINT i = 0; i < mNrOfObject3d; i++ )
	{
		mObject3dArray[i] = clearObject3d;
	}

	mNrOfObject3d = 0;
	//------------------------------

	//Object2d
	Object2dInfo clearObject2d;
	clearObject2d.mAssetId			= (UINT)-1;
	clearObject2d.mTopLeftCorner	= DirectX::XMFLOAT2( 0.0f, 0.0f );
	clearObject2d.mWidthHeight		= DirectX::XMFLOAT2( 0.0f, 0.0f );

	for( UINT i = 0; i < mNrOfObject2d; i++ )
	{
		mObject2dArray[i] = clearObject2d;
	}

	mNrOfObject2d = 0;
	//------------------------------

	//Anim3d
	Anim3dInfo clearAnim3d;
	clearAnim3d.mModelId	= (UINT)-1;
	DirectX::XMStoreFloat4x4( &clearAnim3d.mWorld, DirectX::XMMatrixIdentity() );

	for( UINT i = 0; i < NUM_SUPPORTED_JOINTS; i++ )
		DirectX::XMStoreFloat4x4( &clearAnim3d.mBoneTransforms[i], DirectX::XMMatrixIdentity() );

	for( UINT i = 0; i < mNrOfAnim3d; i++ )
	{
		mAnim3dArray[i] = clearAnim3d;
	}
	
	mNrOfAnim3d = 0;
	//------------------------------

	//Plane
	PlaneInfo clearPlane;
	clearPlane.mAssetId			= (UINT)-1;
	clearPlane.mTopTriangle		= DirectX::XMFLOAT3( 0.0f, 0.0f, 0.0f );
	clearPlane.mBottomTriangle	= DirectX::XMFLOAT3( 0.0f, 0.0f, 0.0f );

	for( UINT i = 0; i < mNrOfPlane; i++ )
	{
		mPlaneArray[i] = clearPlane;
	}
	
	mNrOfPlane = 0;
	//------------------------------
}

RenderManager::RenderManager()
{
	
}

RenderManager::~RenderManager()
{

}

#pragma endregion

#pragma region Public functions
void RenderManager::AddObject3dToList( AssetID assetId, DirectX::XMFLOAT3 position, DirectX::XMFLOAT3 rotation )
{
	Object3dInfo info;
	info.mAssetId = assetId;
	DirectX::XMStoreFloat4x4( &info.mWorld, ( DirectX::XMMatrixTranspose( DirectX::XMMatrixRotationRollPitchYaw( rotation.x, rotation.y, rotation.z ) *										
											  DirectX::XMMatrixTranslation( position.x, position.y, position.z ) ) ) );
	
	mObject3dArray[mNrOfObject3d++] = info;
}
void RenderManager::AddObject3dToList( AssetID assetId, DirectX::XMFLOAT4X4 world )
{
	Object3dInfo info;
	info.mAssetId = assetId;

	DirectX::XMStoreFloat4x4( &info.mWorld, DirectX::XMMatrixTranspose( DirectX::XMLoadFloat4x4( &world ) ) );

	mObject3dArray[mNrOfObject3d++] = info;
}
void RenderManager::AddObject2dToList( AssetID assetId, DirectX::XMFLOAT2 topLeftCorner, DirectX::XMFLOAT2 widthHeight )
{
	Object2dInfo info;
	info.mAssetId		= assetId;
	info.mTopLeftCorner	= topLeftCorner;
	info.mWidthHeight	= widthHeight;

	mObject2dArray[mNrOfObject2d++] = info;
}

void RenderManager::AddAnim3dToList( AssetID modelAssetId, AssetID animationAssetId, float* animationTime, DirectX::XMFLOAT3 position, DirectX::XMFLOAT3 rotation )
{
    static Anim3dInfo info;
	info.mModelId	= modelAssetId;

	Graphics::GetInstance()->GetAnimationMatrices( modelAssetId, animationAssetId, *animationTime, position, rotation, info ); 

	mAnim3dArray[mNrOfAnim3d++] = info;
}

void RenderManager::AddPlaneToList( AssetID assetId, DirectX::XMFLOAT3 topTriangle, DirectX::XMFLOAT3 bottomTriangle )
{
	PlaneInfo info;
	info.mAssetId			= assetId;
	info.mTopTriangle		= topTriangle;
	info.mBottomTriangle	= bottomTriangle;

	mPlaneArray[mNrOfPlane++] = info;
}

HRESULT RenderManager::Update( float deltaTime )
{
	Clear();

	return S_OK;
}

HRESULT RenderManager::Render()
{
	
	//Reset the scene to default values
	Graphics::GetInstance()->BeginScene();

	//Prepare the scene to be rendered with Gbuffers
	Graphics::GetInstance()->GbufferPass();
	SetLightStructuredBuffer();

	//------------------------Fill the Gbuffers with data----------------------
	Graphics::GetInstance()->RenderStatic3dAsset( mObject3dArray, mNrOfObject3d );

	for( UINT i = 0; i < mNrOfPlane; i++ )
	{
		Graphics::GetInstance()->RenderPlane2dAsset( mPlaneArray[i].mAssetId, mPlaneArray[i].mTopTriangle, mPlaneArray[i].mBottomTriangle );
	}

	Graphics::GetInstance()->RenderAnimated3dAsset( mAnim3dArray, mNrOfAnim3d );
	//------------------------Finished filling the Gbuffers----------------------

	//Render the scene with deferred
	Graphics::GetInstance()->DeferredPass();

	//Prepare the scene to render Screen space located assets
	Graphics::GetInstance()->ScreenSpacePass();

	//Render screen space located assets
	for( UINT i = 0; i < mNrOfObject2d; i++ )
	{
		Graphics::GetInstance()->Render2dAsset( mObject2dArray[i].mAssetId, mObject2dArray[i].mTopLeftCorner.x, mObject2dArray[i].mTopLeftCorner.y, mObject2dArray[i].mWidthHeight.x, mObject2dArray[i].mWidthHeight.y );
	}

	//Present the scene onto the screen
	Graphics::GetInstance()->EndScene();

	return S_OK;
}

HRESULT RenderManager::Initialize()
{
	Clear();
	mLightManager = new LightManager;
	mLightManager->Initialize();
	return S_OK;
}

void RenderManager::Release()
{
	SAFE_RELEASE_DELETE( mLightManager );
}

RenderManager* RenderManager::GetInstance()
{
	static RenderManager instance;
	return &instance;
}

#pragma endregion
