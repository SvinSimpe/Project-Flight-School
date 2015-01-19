#include "RenderManager.h"
#include <Graphics.h>

#pragma region Private functions
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
	/*clearAnim3d.mAnimId		= (UINT)-1;
	clearAnim3d.mAnimTime	= 0;
	clearAnim3d.mPosition	= DirectX::XMFLOAT3( 0.0f, 0.0f, 0.0f );*/
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
	DirectX::XMStoreFloat4x4( &info.mWorld, ( DirectX::XMMatrixRotationRollPitchYaw( rotation.x, rotation.y, rotation.z ) *										
											  DirectX::XMMatrixTranslation( position.x, position.y, position.z ) ) );
	
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
	/*info.mAnimId	= animationAssetId;
	info.mAnimTime	= animationTime;
	info.mPosition	= position;*/

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
	Graphics::GetInstance()->BeginScene();

	for( UINT i = 0; i < mNrOfObject3d; i++ )
	{
		Graphics::GetInstance()->RenderStatic3dAsset( mObject3dArray[i].mAssetId, &mObject3dArray[i].mWorld );
	}

	for( UINT i = 0; i < mNrOfPlane; i++ )
	{
		Graphics::GetInstance()->RenderPlane2dAsset( mPlaneArray[i].mAssetId, mPlaneArray[i].mTopTriangle, mPlaneArray[i].mBottomTriangle );
	}

	for( UINT i = 0; i < mNrOfAnim3d; i++ )
	{
		//Graphics::GetInstance()->RenderAnimated3dAsset( mAnim3dArray[i].mModelId, mAnim3dArray[i].mAnimId, *mAnim3dArray[i].mAnimTime, mAnim3dArray[i].mPosition.x, mAnim3dArray[i].mPosition.y, mAnim3dArray[i].mPosition.z );
	}

	Graphics::GetInstance()->RenderAnimated3dAsset( mAnim3dArray, mNrOfAnim3d );

	for( UINT i = 0; i < mNrOfObject2d; i++ )
	{
		Graphics::GetInstance()->Render2dAsset( mObject2dArray[i].mAssetId, mObject2dArray[i].mTopLeftCorner.x, mObject2dArray[i].mTopLeftCorner.y, mObject2dArray[i].mWidthHeight.x, mObject2dArray[i].mWidthHeight.y );
	}

	Graphics::GetInstance()->EndScene();

	return S_OK;
}

HRESULT RenderManager::Initialize()
{

	/*mObject3dArray	= new Object3dInfo[MAX_AMOUNT_OF_OBJECT3D];
	mObject2dArray	= new Object2dInfo[MAX_AMOUNT_OF_OBJECT2D];
	mAnim3dArray	= new Anim3dInfo[MAX_AMOUNT_OF_ANIM3D];
	mPlaneArray		= new PlaneInfo[MAX_AMOUNT_OF_PLANES];*/
	Clear();

	return S_OK;
}

void RenderManager::Release()
{
	//SAFE_DELETE( mObject3dArray );
	//SAFE_DELETE( mObject2dArray );
	//SAFE_DELETE( mAnim3dArray );
	//SAFE_DELETE( mPlaneArray );
}

RenderManager* RenderManager::GetInstance()
{
	static RenderManager instance;
	return &instance;
}

#pragma endregion
