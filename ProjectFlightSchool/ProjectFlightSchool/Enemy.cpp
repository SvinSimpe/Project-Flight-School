#include "Enemy.h"

///////////////////////////////////////////////////////////////////////////////
//									PRIVATE
///////////////////////////////////////////////////////////////////////////////
void Enemy::CreateStandard()
{
	/*
	Standard Enemy
		Med hp
		Med dmg
		Med spd
		Med atkrate
	*/
	mEnemyType	= Standard;
	mMaxHp		= 100.0f;
	mCurrentHp	= mMaxHp;
	mDamage		= 20.0f;
	mVelocity	= 0.15f;
	mAttackRadius->radius	= 5.0f;
	mModel		= mModelStandard;
}

void Enemy::CreateRanged()
{
	/*
	Ranged Enemy
		Med hp
		Low dmg
		High spd
		Hign atkrate
	*/
	mEnemyType				= Ranged;
	mMaxHp					= 80.0f;
	mCurrentHp				= mMaxHp;
	mDamage					= 10.0f;
	mVelocity				= 0.2f;
	mAttackRadius->radius	= 3.0f;
	mModel					= mModelRanged;
}

void Enemy::CreateBoomer()
{
	/*
	Boomer Enemy
		Low hp
		High dmg
		High spd
		Explodes on impact
	*/
	mEnemyType	= Boomer;
	mMaxHp		= 30.0f;
	mCurrentHp	= mMaxHp;
	mDamage		= 50.0f;
	mVelocity	= 0.25f;
	mModel		= mModelBoomer;
}

void Enemy::CreateTank()
{
	/*
	Tank Enemy
		High hp
		Med dmg
		Low spd
		Med atkrate
	*/
	mEnemyType	= Tank;
	mMaxHp		= 400.0f;
	mCurrentHp	= mMaxHp;
	mDamage		= 20.0f;
	mVelocity	= 0.05f;
	mModel		= mModelTank;
}

void Enemy::StandardLogic( float deltaTime )
{
	mPosition.x += mVelocity;
	if( mPosition.x >= 50 )
		Die();
}

void Enemy::RangedLogic( float deltaTime )
{
}

void Enemy::BoomerLogic( float deltaTime )
{
}

void Enemy::TankLogic( float deltaTime )
{
}

///////////////////////////////////////////////////////////////////////////////
//									PUBLIC
///////////////////////////////////////////////////////////////////////////////
HRESULT Enemy::Update( float deltaTime )
{
	mAttackRadius->center = mPosition;

	switch( mEnemyType )
	{
		case Standard:
		{
			StandardLogic( deltaTime );
		}
		break;

		case Ranged:
		{
			RangedLogic( deltaTime );
		}
		break;

		case Boomer:
		{
			BoomerLogic( deltaTime );
		}
		break;

		case Tank:
		{
			TankLogic( deltaTime );
		}
		break;

		default:
			OutputDebugStringA( "> Error: No enemy type " );
	}

	return S_OK;
}

void Enemy::Spawn( XMFLOAT3 spawnPos )
{
	CreateStandard();

	mPosition	= spawnPos;
	mIsAlive	= true;
	

	// Send spawnEv
}

BoundingCircle* Enemy::GetAttackCircle() const
{
	return mAttackRadius;
}

void Enemy::Die()
{
	mIsAlive	= false;
	mCurrentHp	= 0.0f;

	// Send dieEv
}

void Enemy::SetVelocity( float velocity )
{
	mVelocity	= velocity;
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

//void Enemy::SetModelID( AssetID model )
//{
//	mModel = model;
//}

AssetID Enemy::GetAnimation() const
{
	// Change to current animation
	return mAnimations[ENEMY_ANIMATION_STANDARD_RUN];
}

//void Enemy::SetAnimation( AssetID animation )
//{
//	// Change to current animation
//	mAnimations[ENEMY_ANIMATION_STANDARD_IDLE] = animation;
//}

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
	Graphics::GetInstance()->LoadAnimated3dAsset( "../Content/Assets/Raptor/", "scaledScene.apfs", skeleton, mModelStandard );
	// Load Standard run animation asset
	Graphics::GetInstance()->LoadAnimationAsset( "../Content/Assets/Raptor/Animations/", "raptor_idle.PaMan", mAnimations[ENEMY_ANIMATION_STANDARD_IDLE] );
	// Load Standard run animation asset
	Graphics::GetInstance()->LoadAnimationAsset( "../Content/Assets/Raptor/Animations/", "raptor_run.PaMan", mAnimations[ENEMY_ANIMATION_STANDARD_RUN] );
	// Load Standard run animation asset
	Graphics::GetInstance()->LoadAnimationAsset( "../Content/Assets/Raptor/Animations/", "raptor_attack.PaMan", mAnimations[ENEMY_ANIMATION_STANDARD_ATTACK] );
	// Load Standard run animation asset
	Graphics::GetInstance()->LoadAnimationAsset( "../Content/Assets/Raptor/Animations/", "raptor_death.PaMan", mAnimations[ENEMY_ANIMATION_STANDARD_DEATH] );

	mID				= id;
	mMaxHp			= 100.0f;
	mCurrentHp		= mMaxHp;
	mDamage			= 0.0f;
	mVelocity		= 0.15;
	mIsAlive		= false;

	mAttackRadius	= new BoundingCircle( 1.0f );

	return S_OK;
}

void Enemy::Release()
{
	SAFE_DELETE( mAttackRadius );
}

Enemy::Enemy()
{
	mID				= 0;
	mModel			= 0;
	mCurrentHp		= 0.0f;
	mMaxHp			= 0.0f;
	mDamage			= 0.0f;
	mIsAlive		= false;
	mPosition		= XMFLOAT3( 0.0f, 0.0f, 0.0f );
	mDirection		= XMFLOAT3( 0.0f, 0.0f, 0.0f );
	mVelocity		= 0.0f;
	mAttackRadius	= nullptr;
}

Enemy::~Enemy()
{
}