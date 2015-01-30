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
	mVelocity	= 0.1f;
	mAttackRadius->radius	= 5.0f;
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
	mVelocity				= 0.14f;
	mAttackRadius->radius	= 3.0f;
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
}

void Enemy::StandardLogic( float deltaTime )
{
	mPosition.x += mVelocity;

	if( mPosition.x >= 50 )
		Die();
}

void Enemy::RangedLogic( float deltaTime )
{
	mPosition.x += mVelocity;

	if( mPosition.x >= 50 )
		Die();
}

void Enemy::BoomerLogic( float deltaTime )
{
	mPosition.x += mVelocity;

	if( mPosition.x >= 50 )
		Die();
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
	switch( mID % 3 )
	{
	case 0:
		CreateStandard();
		break;
	case 1:
		CreateRanged();
		break;
	case 2:
		CreateRanged();
		//CreateBoomer();
		break;
	}

	//CreateStandard();
	//CreateRanged();
	//CreateBoomer();
	//CreateTank();

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

EnemyType Enemy::GetEnemyType() const
{
	return mEnemyType;
}

EnemyState Enemy::GetEnemyState() const
{
	// Change to current animation
	return mCurrentState;
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
	mID				= id;
	mMaxHp			= 100.0f;
	mCurrentHp		= mMaxHp;
	mDamage			= 0.0f;
	mVelocity		= 0.15;
	mIsAlive		= false;
	mCurrentState	= Run;

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
	mCurrentHp		= 0.0f;
	mMaxHp			= 0.0f;
	mDamage			= 0.0f;
	mIsAlive		= false;
	mPosition		= XMFLOAT3( 0.0f, 0.0f, 0.0f );
	mDirection		= XMFLOAT3( 0.0f, 0.0f, 0.0f );
	mVelocity		= 0.0f;
	mAttackRadius	= nullptr;
	mCurrentState	= Idle;
}

Enemy::~Enemy()
{
}