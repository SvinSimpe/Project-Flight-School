#include "RenderManager.h"
#include <Graphics.h>

#pragma region Private functions
void RenderManager::SetLightStructuredBuffer()
{
	Graphics::GetInstance()->MapLightStructuredBuffer( mLightManager->GetLightStructure(), mLightManager->GetNumActivePointLights() );
}

void RenderManager::Clear()
{
	mNrOfObject3d	= 0;
	mNrOfObject2d	= 0;
	mNrOfAnim3d		= 0;
	mNrOfPlane		= 0;
	mNrOfBillboard	= 0;
	mNrOfParticles	= 0;
	mNrOfBoxes		= 0;
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
void RenderManager::AddBoxToList( DirectX::XMFLOAT3 min, DirectX::XMFLOAT3 max )
{
	BoxInfo info;
	info.min = min;
	info.max = max;
	mBoxArray[mNrOfBoxes++] = info;
}
bool RenderManager::AddAnim3dToList( AnimationTrack &animTrack, int playType, DirectX::XMFLOAT3 position, DirectX::XMFLOAT3 rotation )
{
    static Anim3dInfo info;
	info.mModelId	= animTrack.mModelID;

	bool localReturn = Graphics::GetInstance()->GetAnimationMatrices( animTrack, playType, position, rotation, info ); 

	mAnim3dArray[mNrOfAnim3d++] = info;

	return localReturn;
}

void RenderManager::AddPlaneToList( AssetID assetId, DirectX::XMFLOAT3 topTriangle, DirectX::XMFLOAT3 bottomTriangle )
{
	PlaneInfo info;
	info.mAssetId			= assetId;
	info.mTopTriangle		= topTriangle;
	info.mBottomTriangle	= bottomTriangle;

	mPlaneArray[mNrOfPlane++] = info;
}

void RenderManager::AddBillboardToList( AssetID assetId, DirectX::XMFLOAT3 worldPosition, float width, float height )
{
	BillboardInfo info;
	info.mAssetId		= assetId;
	info.mWorldPosition	= worldPosition;
	info.mWidth			= width;
	info.mHeight		= height;

	mBillboardArray[mNrOfBillboard++] = info;
}

void RenderManager::AddParticleSystemToList( ParticleSystem*** particleSystem, size_t* nrOfActiveParticleSystemsPerType  )
{
	ParticleInfo info;
	UINT offset = 0;
	for ( size_t i = 0; i < NR_OF_PARTICLE_TYPES; i++ )
	{
		// Set AssetID per Particle Type
		info.mAssetId = particleSystem[i][0]->assetID;

		// Calculate offset per Particle Type
		for (size_t l = 0; l < nrOfActiveParticleSystemsPerType[i]; l++)
		{
			offset += particleSystem[i][l]->nrOfParticlesAlive;
		}

		// Set offset
		info.mOffsetToNextParticleType = offset;

		// Fill Array with Particle Info
		for (size_t j = 0; j < nrOfActiveParticleSystemsPerType[i]; j++)
		{

			for (size_t k = 0; k < particleSystem[i][j]->nrOfParticlesAlive; k++)
			{
				info.mWorldPosition.x	= particleSystem[i][j]->xPosition[k];
				info.mWorldPosition.y	= particleSystem[i][j]->yPosition[k];
				info.mWorldPosition.z	= particleSystem[i][j]->zPosition[k];

				info.mLifeTime			= particleSystem[i][j]->lifeTime[k];

				mParticleInfoArray[mNrOfParticles++] = info;
			}
		}
	}	
}

void RenderManager::AnimationInitialize( AnimationTrack &animationTrack, AssetID model, AssetID defaultAnimation )
{
	animationTrack.mModelID					= model;
	animationTrack.mCurrentAnimation		= defaultAnimation;
	animationTrack.mCurrentAnimationTime	= 1.0f / 60.0f;
	animationTrack.mNextAnimation			= defaultAnimation;
	animationTrack.mNextAnimationTime		= 1.0f / 60.0f;
	animationTrack.mInterpolation			= 0.0f;
}

void RenderManager::AnimationUpdate( AnimationTrack &animationTrack, float deltaTime )
{
	animationTrack.mCurrentAnimationTime += deltaTime;
	if( animationTrack.mInterpolation > 0.0f )
	{
		animationTrack.mNextAnimationTime	+= deltaTime;
		animationTrack.mInterpolation		-= deltaTime;
		if( animationTrack.mInterpolation <= 0.0f )
		{
			animationTrack.mCurrentAnimation		= animationTrack.mNextAnimation;
			animationTrack.mCurrentAnimationTime	= animationTrack.mNextAnimationTime;
		}
	}
}

void RenderManager::AnimationStartNew( AnimationTrack &animationTrack, AssetID newAnimation )
{
	animationTrack.mNextAnimation		= newAnimation;
	animationTrack.mNextAnimationTime	= 1.0f / 60.0f;
	animationTrack.mInterpolation		= 0.2f;
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
	for( UINT i = 0; i < mNrOfBoxes; i++ )
	{
		Graphics::GetInstance()->RenderDebugBox( mBoxArray[i].min, mBoxArray[i].max );
	}
	Graphics::GetInstance()->RenderAnimated3dAsset( mAnim3dArray, mNrOfAnim3d );
	//------------------------Finished filling the Gbuffers----------------------

	//Test data for billboarding
	mBillboardArray[0].mWorldPosition = DirectX::XMFLOAT3( 3, 1, 0 );
	mBillboardArray[0].mAssetId = DIFFUSE_PLACEHOLDER;
	mBillboardArray[0].mWidth = 2.3f;
	mBillboardArray[0].mHeight = 1.3f;
	mBillboardArray[1].mWorldPosition = DirectX::XMFLOAT3( 3, 1, 6 );
	mBillboardArray[1].mAssetId = DIFFUSE_PLACEHOLDER;
	mBillboardArray[2].mWorldPosition = DirectX::XMFLOAT3( -2, 1, -4 );
	mBillboardArray[2].mAssetId = DIFFUSE_PLACEHOLDER;
	mBillboardArray[3].mWorldPosition = DirectX::XMFLOAT3( 7, 1, 0 );
	mBillboardArray[3].mAssetId = DIFFUSE_PLACEHOLDER;
	mBillboardArray[4].mWorldPosition = DirectX::XMFLOAT3( 0, 1, 0 );
	mBillboardArray[4].mAssetId = DIFFUSE_PLACEHOLDER;
	mBillboardArray[5].mWorldPosition = DirectX::XMFLOAT3( -6, 1, 0 );
	mBillboardArray[5].mAssetId = DIFFUSE_PLACEHOLDER;

	for( int i = 1; i < 6; i++)
	{
		mBillboardArray[i].mHeight = 1.0f;
		mBillboardArray[i].mWidth	= 1.0f;
	}
	//---------------------------------------------------
	Graphics::GetInstance()->RenderBillboard( mBillboardArray, 6 );

	Graphics::GetInstance()->RenderParticleSystems( mParticleInfoArray, mNrOfParticles );

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
