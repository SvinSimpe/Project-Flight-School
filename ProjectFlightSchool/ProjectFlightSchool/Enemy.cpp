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
	mAnimationTime += deltaTime;
	//mPosition.x += mVelocity;
	return S_OK;
}

HRESULT Enemy::Render()
{
	RenderManager::GetInstance()->AddAnim3dToList( mModel, mAnimations[ENEMY_ANIMATION_IDLE], &mAnimationTime, mPosition );
	return S_OK;
}

void Enemy::Spawn( XMFLOAT3 spawnPos )
{
	mPosition	= spawnPos;
	mIsAlive	= true;
	mCurrentHp	= mMaxHp;

	// Send spawnEv

	// Debug
	mPosition.x = rand() % 200;
	mPosition.z = rand() % 200;
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
	return mModel;
}

void Enemy::SetModelID( AssetID model )
{
	mModel = model;
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
	// Load skeleton
	AssetID skeleton = 0;
	Graphics::GetInstance()->LoadSkeletonAsset( "../Content/Assets/Raptor/Animations/", "raptor.Skel", skeleton ); // Debug, raptor
	// Load animated 3d asset
	Graphics::GetInstance()->LoadAnimated3dAsset( "../Content/Assets/Raptor/", "scaledScene.apfs", skeleton, mModel );
	// Load animation asset
	Graphics::GetInstance()->LoadAnimationAsset( "../Content/Assets/Raptor/Animations/", "raptor_idle.PaMan", mAnimations[ENEMY_ANIMATION_IDLE] );

	mID				= id;
	mMaxHp			= 100.0f;
	mCurrentHp		= mMaxHp;
	mAnimationTime	= 1.0f;
	mVelocity		= 0.02;

	return S_OK;
}

void Enemy::Release()
{
}

Enemy::Enemy()
{
	mID				= 0;
	mModel			= 0;
	mAnimationTime	= 0.0f;
	mCurrentHp		= 0.0f;
	mMaxHp			= 0.0f;
	mIsAlive		= false;
	mPosition		= XMFLOAT3( 0.0f, 0.0f, 0.0f );
	mDirection		= XMFLOAT3( 0.0f, 0.0f, 0.0f );
	mVelocity		= 0.0f;

	srand( time( NULL ) );
}

Enemy::~Enemy()
{
}