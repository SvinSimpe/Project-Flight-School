#include "RemoteEnemy.h"

XMFLOAT3 RemoteEnemy::GetPosition() const
{
	return mPosition;
}

HRESULT RemoteEnemy::Update( float deltaTime )
{
	RenderManager::GetInstance()->AnimationUpdate( mAnimationTrack, deltaTime );
	return S_OK;
}

HRESULT RemoteEnemy::Render()
{
	RenderManager::GetInstance()->AddAnim3dToList( mAnimationTrack, ANIMATION_PLAY_LOOPED, mPosition );
	return S_OK;
}

void RemoteEnemy::SetID( unsigned int id )
{
	mID = id;
}

void RemoteEnemy::SetModelID( AssetID model )
{
	mAnimationTrack.mModelID = model;
}

void RemoteEnemy::SetAnimation( AssetID animation )
{
	mAnimations[ENEMY_ANIMATION_IDLE] = animation;
}

void RemoteEnemy::SetPosition( XMFLOAT3 position )
{
	mPosition	= position;
}

void RemoteEnemy::SetDirection( XMFLOAT3 direction )
{
	mDirection = direction;
}

HRESULT RemoteEnemy::Initialize( int id )
{
	// Load animation asset
	Graphics::GetInstance()->LoadAnimationAsset( "../Content/Assets/Enemies/Raptor/Animations/", "raptor_run.PaMan", mAnimations[ENEMY_ANIMATION_IDLE] );

	// Load skeleton
	AssetID skeleton	= 0;
	AssetID model		= 0;
	Graphics::GetInstance()->LoadSkeletonAsset( "../Content/Assets/Enemies/Raptor/Animations/", "raptor.Skel", skeleton ); // Debug, raptor
	// Load animated 3d asset
	Graphics::GetInstance()->LoadAnimated3dAsset( "../Content/Assets/Enemies/Raptor/", "scaledScene.apfs", skeleton, model );
	// Load animationTrack with model and standard animation
	RenderManager::GetInstance()->AnimationInitialize( mAnimationTrack, model, mAnimations[ENEMY_ANIMATION_IDLE] );

	mID				= id;
	mPosition		= XMFLOAT3( 0.0f, 0.0f, 0.0f );
	mDirection		= XMFLOAT3( 0.0f, 0.0f, 0.0f );

	return S_OK;
}

void RemoteEnemy::Release()
{
}

RemoteEnemy::RemoteEnemy()
{
	mID				= 0;
	mPosition		= XMFLOAT3( 0.0f, 0.0f, 0.0f );
	mDirection		= XMFLOAT3( 0.0f, 0.0f, 0.0f );
}

RemoteEnemy::~RemoteEnemy()
{
}