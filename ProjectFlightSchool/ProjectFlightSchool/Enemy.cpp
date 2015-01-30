#include "Enemy.h"

///////////////////////////////////////////////////////////////////////////////
//									PRIVATE
///////////////////////////////////////////////////////////////////////////////
void Enemy::RemoteUpdate( IEventPtr newEvent )
{
}

///////////////////////////////////////////////////////////////////////////////
//									PUBLIC
///////////////////////////////////////////////////////////////////////////////
HRESULT Enemy::Update( float deltaTime )
{
	mPosition.x += mVelocity;
	RenderManager::GetInstance()->AnimationUpdate( mAnimationTrack,deltaTime );
	if( mPosition.x >= 50 )
		Die();

	return S_OK;
}

HRESULT Enemy::Render(  )
{
	RenderManager::GetInstance()->AddAnim3dToList( mAnimationTrack, ANIMATION_PLAY_LOOPED, mPosition );
	return S_OK;
}

void Enemy::Spawn( XMFLOAT3 spawnPos )
{
	mPosition	= spawnPos;
	mIsAlive	= true;
	mCurrentHp	= mMaxHp;

	// Send spawnEv
}

void Enemy::Die()
{
	mIsAlive	= false;
	mCurrentHp	= 0.0f;

	// Send dieEv
}

unsigned int Enemy::GetID() const
{
	return mID;
}

void Enemy::SetID( unsigned int id )
{
	mID = id;
}

AssetID Enemy::GetModelID() const
{
	return mAnimationTrack.mModelID;
}

void Enemy::SetModelID( AssetID model )
{
	mAnimationTrack.mModelID = model;
}

AssetID Enemy::GetAnimation() const
{
	// Change to current animation
	return mAnimations[ENEMY_ANIMATION_IDLE];
}

void Enemy::SetAnimation( AssetID animation )
{
	// Change to current animation
	mAnimations[ENEMY_ANIMATION_IDLE] = animation;
}

float Enemy::GetHP() const
{
	return mCurrentHp;
}

void Enemy::SetHP( float hp )
{
	mCurrentHp	= hp;
}

bool Enemy::IsAlive() const
{
	return mIsAlive;
}

void Enemy::SetIsAlive( bool isAlive )
{
	mIsAlive = isAlive;
}

XMFLOAT3 Enemy::GetPosition() const
{
	return mPosition;
}

void Enemy::SetPosition( XMFLOAT3 position )
{
	mPosition = position;
}

XMFLOAT3 Enemy::GetDirection() const
{
	return mDirection;
}

void Enemy::SetDirection( XMFLOAT3 direction )
{
	mDirection = direction;
}

HRESULT Enemy::Initialize( int id )
{
	// Load animation asset
	Graphics::GetInstance()->LoadAnimationAsset( "../Content/Assets/Raptor/Animations/", "raptor_run.PaMan", mAnimations[ENEMY_ANIMATION_IDLE] );

	// Load skeleton
	AssetID skeleton	= 0;
	AssetID model		= 0;
	Graphics::GetInstance()->LoadSkeletonAsset( "../Content/Assets/Raptor/Animations/", "raptor.Skel", skeleton ); // Debug, raptor
	// Load animated 3d asset
	Graphics::GetInstance()->LoadAnimated3dAsset( "../Content/Assets/Raptor/", "scaledScene.apfs", skeleton, model );
	// Load animationTrack with model and standard animation
	RenderManager::GetInstance()->AnimationInitialize( mAnimationTrack, model, mAnimations[ENEMY_ANIMATION_IDLE] );

	mID				= id;
	mMaxHp			= 100.0f;
	mCurrentHp		= mMaxHp;
	mVelocity		= 0.15f;
	mIsAlive		= false;

	return S_OK;
}

void Enemy::Release()
{
}

Enemy::Enemy()
{
	mID				= 0;
	mCurrentHp		= 0.0f;
	mMaxHp			= 0.0f;
	mIsAlive		= false;
	mPosition		= XMFLOAT3( 0.0f, 0.0f, 0.0f );
	mDirection		= XMFLOAT3( 0.0f, 0.0f, 0.0f );
	mVelocity		= 0.0f;
}

Enemy::~Enemy()
{
}