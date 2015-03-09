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
	mNrOfNodeGrid	= 0;
	mNrOfBoxes		= 0;
	mNrOfLines		= 0;
	mNrOfCircles	= 0;
}

RenderManager::RenderManager()
{
	mParticleManager		= nullptr;
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

void RenderManager::AddObject2dToList( AssetID assetId, DirectX::XMFLOAT2 topLeftCorner, DirectX::XMFLOAT2 widthHeight, DirectX::XMFLOAT4 color )
{
	Object2dInfo info;
	info.mAssetId		= assetId;
	info.mTopLeftCorner	= topLeftCorner;
	info.mWidthHeight	= widthHeight;
	info.mColor			= color;

	mObject2dArray[mNrOfObject2d++] = info;
}

void RenderManager::AddBoxToList( DirectX::XMFLOAT3 min, DirectX::XMFLOAT3 max, DirectX::XMFLOAT4X4 world )
{
	BoxInfo info;
	info.min	= min;
	info.max	= max;
	info.world	= world;
	mBoxArray[mNrOfBoxes++] = info;
}

void RenderManager::AddLineToList( DirectX::XMFLOAT3 start, DirectX::XMFLOAT3 end )
{
	LineInfo info;
	info.start	= start;
	info.end	= end;

	mLineArray[mNrOfLines++] = info;
}

bool RenderManager::AddAnim3dToList( AnimationTrack &animTrack, int playType, DirectX::XMFLOAT3 position, DirectX::XMFLOAT3 rotation )
{
    static Anim3dInfo info;
	info.mModelId = animTrack.mModelID;
	DirectX::XMStoreFloat4x4( &info.mWorld, DirectX::XMMatrixTranspose( DirectX::XMMatrixRotationRollPitchYaw( rotation.x, rotation.y, rotation.z ) *										
											DirectX::XMMatrixTranslation( position.x, position.y, position.z ) ) );

	bool localReturn = Graphics::GetInstance()->GetAnimationMatrices( animTrack, playType, info ); 

	mAnim3dArray[mNrOfAnim3d++] = info;

	return localReturn;
}

bool RenderManager::AddAnim3dToList( AnimationTrack &animTrack, int playType, XMFLOAT4X4* world )
{
	static Anim3dInfo info;
	info.mModelId = animTrack.mModelID;
	DirectX::XMStoreFloat4x4( &info.mWorld, DirectX::XMMatrixTranspose( DirectX::XMLoadFloat4x4( world ) ) );

	bool localReturn = Graphics::GetInstance()->GetAnimationMatrices( animTrack, playType, info ); 

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

void RenderManager::AddParticleSystemToList( ParticleSystem*** particleSystem, int* nrOfActiveParticleSystemsPerType  )
{
	ParticleInfo info;
	UINT offset = 0;
	for ( int i = 0; i < NR_OF_PARTICLE_TYPES; i++ )
	{
		// Set AssetID per Particle Type
		info.mAssetId = particleSystem[i][0]->assetID;

		// Calculate offset per Particle Type
		for ( int l = 0; l < nrOfActiveParticleSystemsPerType[i]; l++ )
		{
			offset += particleSystem[i][l]->nrOfParticlesAlive;
		}

		// Set offset
		info.mOffsetToNextParticleType = offset;

		// Set Particle Type
		info.mParticleType = particleSystem[i][0]->particleType;

		// Fill Array with Particle Info
		for ( int j = 0; j < nrOfActiveParticleSystemsPerType[i]; j++ )
		{
			for ( int k = 0; k < particleSystem[i][j]->nrOfParticlesAlive; k++ )
			{
				info.mWorldPosition.x	= particleSystem[i][j]->xPosition[k];
				info.mWorldPosition.y	= particleSystem[i][j]->yPosition[k];
				info.mWorldPosition.z	= particleSystem[i][j]->zPosition[k];

				info.mAge				= particleSystem[i][j]->lifeTime[k];
				info.mTimeTillDeath		= particleSystem[i][j]->deathTime[k] - particleSystem[i][j]->lifeTime[k];
				info.mRandomRotation	= particleSystem[i][j]->randRot[k];

				mParticleInfoArray[mNrOfParticles++] = info;
			}
		}
	}	
}

void RenderManager::AddNodeGridToList( StaticVertex* vertices, UINT nrOfVertices, AssetID blendMap, DirectX::XMFLOAT4X4 world )
{
	NodeGridInfo info;
	info.mVertices		= vertices;
	info.mNrOfVertices	= nrOfVertices;
	info.mBlendMap		= blendMap;
	DirectX::XMStoreFloat4x4( &info.mWorld, ( DirectX::XMMatrixTranspose( XMLoadFloat4x4( &world ) ) ) );

	mNodeGridArray[mNrOfNodeGrid++] = info;
}

void RenderManager::AddCircleToList( DirectX::XMFLOAT3 pos, DirectX::XMFLOAT3 color, float radius )
{
	CircleInfo info;
	info.center	= pos;
	info.color	= color;
	info.radius	= radius;

	mCircleArray[mNrOfCircles++] = info;
}

void RenderManager::AnimationInitialize( AnimationTrack &animationTrack, AssetID model, AssetID defaultAnimation )
{
	animationTrack.mModelID					= model;
	animationTrack.mCurrentAnimation		= defaultAnimation;
	animationTrack.mCurrentAnimationTime	= 1.0f / 60.0f;
	animationTrack.mNextAnimation			= defaultAnimation;
	animationTrack.mNextAnimationTime		= 1.0f / 60.0f;
	animationTrack.mInterpolation			= 0.0f;
	animationTrack.mBlendWithCurrent		= false;
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
			if( animationTrack.mBlendWithCurrent )
			{
				animationTrack.mBlendWithCurrent	= false;
				animationTrack.mNextAnimation		= animationTrack.mCurrentAnimation;
				animationTrack.mNextAnimationTime	= animationTrack.mCurrentAnimationTime;
			}
			else
			{
				animationTrack.mCurrentAnimation		= animationTrack.mNextAnimation;
				animationTrack.mCurrentAnimationTime	= animationTrack.mNextAnimationTime;
			}
		}
	}
}

void RenderManager::AnimationStartNew( AnimationTrack &animationTrack, AssetID newAnimation, bool blendWithCurrent )
{
	if( newAnimation != animationTrack.mCurrentAnimation )
	{
		animationTrack.mNextAnimation		= newAnimation;
		animationTrack.mBlendWithCurrent	= blendWithCurrent;
		if( blendWithCurrent )
		{
			animationTrack.mNextAnimationTime	= animationTrack.mCurrentAnimationTime;
			animationTrack.mInterpolation		= 0.2f;
		}
		else
		{
			animationTrack.mNextAnimationTime	= 1.0f / 60.0f;
			animationTrack.mInterpolation		= 0.2f;
		}
	}
}

void RenderManager::ChangeRasterizerState( RasterizerStates rasterState )
{
	mRasterState = rasterState;
}

void RenderManager::AnimationReset( AnimationTrack &animationTrack, AssetID defaultAnimation )
{
	animationTrack.mCurrentAnimation		= defaultAnimation;
	animationTrack.mCurrentAnimationTime	= 1.0f / 60.0f;
	animationTrack.mNextAnimation			= defaultAnimation;
	animationTrack.mNextAnimationTime		= 1.0f / 60.0f;
	animationTrack.mInterpolation			= 0.0f;
	animationTrack.mBlendWithCurrent		= false;
}

void RenderManager::RequestParticleSystem( size_t entityID, ParticleType particleType, XMFLOAT3 position, XMFLOAT3 direction, XMFLOAT3 initialVelocity )
{
	mParticleManager->RequestParticleSystem( entityID, particleType, position, direction, initialVelocity );
}

HRESULT RenderManager::Update( float deltaTime )
{
	Clear();
	mParticleManager->Update( deltaTime );
	return S_OK;
}

HRESULT RenderManager::Render()
{
	//Reset the scene to default values
	Graphics::GetInstance()->BeginScene();

	Graphics::GetInstance()->ChangeRasterizerState( mRasterState );

	//Prepare the scene to be rendered with Gbuffers
	Graphics::GetInstance()->GbufferPass();
	
	SetLightStructuredBuffer();

	//------------------------Fill the Gbuffers with data----------------------
	Graphics::GetInstance()->RenderStatic3dAsset( mObject3dArray, mNrOfObject3d );

	Graphics::GetInstance()->RenderNodeGrid( mNodeGridArray, mNrOfNodeGrid );

	for( UINT i = 0; i < mNrOfPlane; i++ )
	{
		Graphics::GetInstance()->RenderPlane2dAsset( mPlaneArray[i].mAssetId, mPlaneArray[i].mTopTriangle, mPlaneArray[i].mBottomTriangle );
	}

	Graphics::GetInstance()->RenderDebugBox( mBoxArray, mNrOfBoxes );

	Graphics::GetInstance()->RenderLine( mLineArray, mNrOfLines );

	//Graphics::GetInstance()->RenderCircle( mCircleArray, mNrOfCircles );	

	Graphics::GetInstance()->RenderAnimated3dAsset( mAnim3dArray, mNrOfAnim3d );

	////------------------------Finished filling the Gbuffers----------------------

	////Render the scene with deferred
	Graphics::GetInstance()->DeferredPass();

	//Render the particles
	mParticleManager->Render(); // Check these separately?
	Graphics::GetInstance()->RenderParticleSystems( mParticleInfoArray, mNrOfParticles );

	//Prepare the scene to render Screen space located assets
	Graphics::GetInstance()->ScreenSpacePass();

	//Render screen space located assets
	Graphics::GetInstance()->Render2dAsset( mObject2dArray, mNrOfObject2d );

	//Present the scene onto the screen
	Graphics::GetInstance()->EndScene();

	return S_OK;
}

HRESULT RenderManager::Initialize()
{
	Clear();
	mLightManager = new LightManager;
	mLightManager->Initialize();

	mParticleManager = new ParticleManager();
	mParticleManager->Initialize();
	return S_OK;
}

void RenderManager::Release()
{
	SAFE_RELEASE_DELETE( mLightManager );
	SAFE_RELEASE_DELETE( mParticleManager );
}

RenderManager* RenderManager::GetInstance()
{
	static RenderManager instance;
	return &instance;
}

#pragma endregion
