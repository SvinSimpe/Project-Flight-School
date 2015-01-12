#include "RenderManager.h"
#include <Graphics.h>


RenderManager::RenderManager()
{
	
}

RenderManager::~RenderManager()
{

}

void RenderManager::AddObject3dToList( AssetID assetId, DirectX::XMFLOAT3 position, DirectX::XMFLOAT3 rotation )
{
	Object3dInfo info;
	info.mAssetId = assetId;
	DirectX::XMStoreFloat4x4( &info.mWorld, ( DirectX::XMMatrixRotationRollPitchYaw( rotation.x, rotation.y, rotation.z ) *										
											  DirectX::XMMatrixTranslation( position.x, position.y, position.z ) ) );
	
	mObject3dArray[mNrOfObject3d++] = info;
}

void RenderManager::AddAnim3dToList( AssetID modelAssetId, AssetID animationAssetId, float* animationTime )
{
	Anim3dInfo info;
	info.mModelId	= modelAssetId;
	info.mAnimId	= animationAssetId;
	info.mAnimTime	= animationTime;

	mAnim3dArray[mNrOfAnim3d++] = info;
}

HRESULT RenderManager::Update( float deltaTime )
{
	Object3dInfo clearObject3d;
	clearObject3d.mAssetId = (UINT)-1;
	DirectX::XMStoreFloat4x4( &clearObject3d.mWorld, DirectX::XMMatrixIdentity() );

	for( UINT i = 0; i < mNrOfObject3d; i++ )
	{
		mObject3dArray[i] = clearObject3d;
	}

	mNrOfObject3d = 0;

	Anim3dInfo clearAnim3d;
	clearAnim3d.mModelId	= (UINT)-1;
	clearAnim3d.mAnimId		= (UINT)-1;
	clearAnim3d.mAnimTime	= 0;

	for( UINT i = 0; i < mNrOfAnim3d; i++ )
	{
		mAnim3dArray[i] = clearAnim3d;
	}
	
	mNrOfAnim3d = 0;

	return S_OK;
}

HRESULT RenderManager::Render( float deltaTime )
{
	for( UINT i = 0; i < mNrOfObject3d; i++ )
	{
		Graphics::GetInstance()->RenderStatic3dAsset( mObject3dArray[i].mAssetId, &mObject3dArray[i].mWorld );
	}

	for( UINT i = 0; i < mNrOfAnim3d; i++ )
	{
		Graphics::GetInstance()->RenderAnimated3dAsset( mAnim3dArray[i].mModelId, mAnim3dArray[i].mAnimId, *mAnim3dArray[i].mAnimTime );
	}

	return S_OK;
}

HRESULT RenderManager::Initialize()
{
	Object3dInfo initObject3d;
	initObject3d.mAssetId = (UINT)-1;
	DirectX::XMStoreFloat4x4( &initObject3d.mWorld, DirectX::XMMatrixIdentity() );

	for( UINT i = 0; i < MAX_AMOUNT_OF_OBJECT3D; i++ )
	{
		mObject3dArray[i] = initObject3d;
	}

	Anim3dInfo initAnim3d;
	initAnim3d.mModelId		= (UINT)-1;
	initAnim3d.mAnimId		= (UINT)-1;
	initAnim3d.mAnimTime	= 0;

	for( UINT i = 0; i < MAX_AMOUNT_OF_ANIM3D; i++ )
	{
		mAnim3dArray[i] = initAnim3d;
	}

	return S_OK;
}

void RenderManager::Release()
{

}

RenderManager* RenderManager::GetInstance()
{
	static RenderManager instance;
	return &instance;
}
