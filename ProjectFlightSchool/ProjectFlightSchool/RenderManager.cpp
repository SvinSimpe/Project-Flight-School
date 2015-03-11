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
	mObject3dArray[mNrOfObject3d].mAssetId = assetId;
	DirectX::XMStoreFloat4x4( &mObject3dArray[mNrOfObject3d].mWorld, ( DirectX::XMMatrixTranspose( DirectX::XMMatrixRotationRollPitchYaw( rotation.x, rotation.y, rotation.z ) *										
											  DirectX::XMMatrixTranslation( position.x, position.y, position.z ) ) ) );
	mNrOfObject3d++;
}

void RenderManager::AddObject3dToList( AssetID assetId, DirectX::XMFLOAT4X4 world )
{
	mObject3dArray[mNrOfObject3d].mAssetId = assetId;
	DirectX::XMStoreFloat4x4( &mObject3dArray[mNrOfObject3d].mWorld, DirectX::XMMatrixTranspose( DirectX::XMLoadFloat4x4( &world ) ) );
	mNrOfObject3d++;
}

void RenderManager::AddObject2dToList( AssetID assetId, DirectX::XMFLOAT2 topLeftCorner, DirectX::XMFLOAT2 widthHeight, DirectX::XMFLOAT4 color )
{
	mObject2dArray[mNrOfObject2d].mAssetId			= assetId;
	mObject2dArray[mNrOfObject2d].mTopLeftCorner	= topLeftCorner;
	mObject2dArray[mNrOfObject2d].mWidthHeight		= widthHeight;
	mObject2dArray[mNrOfObject2d].mColor			= color;

	mNrOfObject2d++;
}

void RenderManager::AddBoxToList( DirectX::XMFLOAT3 min, DirectX::XMFLOAT3 max, DirectX::XMFLOAT4X4 world )
{
	mBoxArray[mNrOfBoxes].min	= min;
	mBoxArray[mNrOfBoxes].max	= max;
	mBoxArray[mNrOfBoxes].world	= world;
	mNrOfBoxes++;
}

void RenderManager::AddLineToList( DirectX::XMFLOAT3 start, DirectX::XMFLOAT3 end )
{
	mLineArray[mNrOfLines].start	= start;
	mLineArray[mNrOfLines].end		= end;

	mNrOfLines++;
}

bool RenderManager::AddAnim3dToList( AnimationTrack &animTrack, int playType, DirectX::XMFLOAT3 position, DirectX::XMFLOAT3 rotation )
{
	mAnim3dArray[mNrOfAnim3d].mModelId = animTrack.mModelID;
	DirectX::XMStoreFloat4x4( &mAnim3dArray[mNrOfAnim3d].mWorld, DirectX::XMMatrixTranspose( DirectX::XMMatrixRotationRollPitchYaw( rotation.x, rotation.y, rotation.z ) *										
											DirectX::XMMatrixTranslation( position.x, position.y, position.z ) ) );
	bool localReturn = Graphics::GetInstance()->GetAnimationMatrices( animTrack, playType, mAnim3dArray[mNrOfAnim3d] ); 

	mNrOfAnim3d++;

	return localReturn;
}

bool RenderManager::AddAnim3dToList( AnimationTrack &animTrack, int playType, XMFLOAT4X4* world )
{
	mAnim3dArray[mNrOfAnim3d].mModelId = animTrack.mModelID;
	DirectX::XMStoreFloat4x4( &mAnim3dArray[mNrOfAnim3d].mWorld, DirectX::XMMatrixTranspose( DirectX::XMLoadFloat4x4( world ) ) );
	bool localReturn = Graphics::GetInstance()->GetAnimationMatrices( animTrack, playType, mAnim3dArray[mNrOfAnim3d] ); 

	mNrOfAnim3d++;

	return localReturn;
}

void RenderManager::AddPlaneToList( AssetID assetId, DirectX::XMFLOAT3 topTriangle, DirectX::XMFLOAT3 bottomTriangle )
{
	mPlaneArray[mNrOfPlane].mAssetId		= assetId;
	mPlaneArray[mNrOfPlane].mTopTriangle	= topTriangle;
	mPlaneArray[mNrOfPlane].mBottomTriangle	= bottomTriangle;

	mNrOfPlane++;
}

void RenderManager::AddBillboardToList( AssetID assetId, DirectX::XMFLOAT3 worldPosition, float width, float height )
{
	mBillboardArray[mNrOfBillboard].mAssetId		= assetId;
	mBillboardArray[mNrOfBillboard].mWorldPosition	= worldPosition;
	mBillboardArray[mNrOfBillboard].mWidth			= width;
	mBillboardArray[mNrOfBillboard].mHeight			= height;

	mNrOfBillboard++;
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
				mParticleInfoArray[mNrOfParticles].mAssetId						= info.mAssetId;
				mParticleInfoArray[mNrOfParticles].mOffsetToNextParticleType	= info.mOffsetToNextParticleType;
				mParticleInfoArray[mNrOfParticles].mParticleType				= info.mParticleType;

				mParticleInfoArray[mNrOfParticles].mWorldPosition.x	= particleSystem[i][j]->xPosition[k];
				mParticleInfoArray[mNrOfParticles].mWorldPosition.y	= particleSystem[i][j]->yPosition[k];
				mParticleInfoArray[mNrOfParticles].mWorldPosition.z	= particleSystem[i][j]->zPosition[k];

				mParticleInfoArray[mNrOfParticles].mAge				= particleSystem[i][j]->lifeTime[k];
				mParticleInfoArray[mNrOfParticles].mTimeTillDeath	= particleSystem[i][j]->deathTime[k] - particleSystem[i][j]->lifeTime[k];
				mParticleInfoArray[mNrOfParticles].mRandomRotation	= particleSystem[i][j]->randRot[k];

				mNrOfParticles++;
			}
		}
	}	
}

void RenderManager::AddNodeGridToList( StaticVertex* vertices, UINT nrOfVertices, AssetID blendMap, DirectX::XMFLOAT4X4 world )
{
	mNodeGridArray[mNrOfNodeGrid].mVertices		= vertices;
	mNodeGridArray[mNrOfNodeGrid].mNrOfVertices	= nrOfVertices;
	mNodeGridArray[mNrOfNodeGrid].mBlendMap		= blendMap;
	DirectX::XMStoreFloat4x4( &mNodeGridArray[mNrOfNodeGrid].mWorld, ( DirectX::XMMatrixTranspose( XMLoadFloat4x4( &world ) ) ) );

	mNrOfNodeGrid++;
}

void RenderManager::AddCircleToList( DirectX::XMFLOAT3 pos, DirectX::XMFLOAT3 color, float radius )
{
	mCircleArray[mNrOfCircles].center	= pos;
	mCircleArray[mNrOfCircles].color	= color;
	mCircleArray[mNrOfCircles].radius	= radius;

	mNrOfCircles++;
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

	SetLightStructuredBuffer();

	Graphics::GetInstance()->ChangeRasterizerState( mRasterState );

	//Prepare the scene to be rendered with Gbuffers
	Graphics::GetInstance()->GbufferPass();

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

	//Render billboarded assets
	Graphics::GetInstance()->RenderBillboard( mBillboardArray, mNrOfBillboard );

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
