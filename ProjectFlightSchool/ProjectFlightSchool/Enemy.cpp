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
	mEnemyType					= Standard;
	mMaxHp						= 100.0f;
	mCurrentHp					= mMaxHp;
	mDamage						= 20.0f;
	mSpeed						= 5.0f;
	mAttackRadius->radius		= 1.0f;
	mAttentionRadius->radius	= 10.0f;
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
	mEnemyType					= Ranged;
	mMaxHp						= 80.0f;
	mCurrentHp					= mMaxHp;
	mDamage						= 10.0f;
	mAttackRadius->radius		= 5.0f;
	mAttentionRadius->radius	= 10.0f;
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
	mEnemyType					= Boomer;
	mMaxHp						= 30.0f;
	mCurrentHp					= mMaxHp;
	mDamage						= 50.0f;
	mAttackRadius->radius		= 1.0f;
	mAttentionRadius->radius	= 10.0f;
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
	mAttackRadius->radius		= 1.0f;
	mAttentionRadius->radius	= 10.0f;
}

void Enemy::StandardLogic( float deltaTime )
{

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
	mAttackRadius->center		= mPosition;
	mAttentionRadius->center	= mPosition;

	// Update specific enemy logic
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
			OutputDebugStringA( "\n--Error: No enemy type " );
	}

	// Update enemy state
	switch( mCurrentState )
	{
		case Idle:
		{
		}
		break;

		case Run:
		{
		}
		break;

		case Attack:
		{
		}
		break;

		case Death:
		{
		}
		break;

		case MoveToShip:
		{
		}
		break;

		case HuntPlayer:
		{
			mPosition.x += mVelocity.x * mSpeed * deltaTime;
			mPosition.z += mVelocity.z * mSpeed * deltaTime;
			mDirection = mVelocity;
		}
		break;

		default:
			OutputDebugStringA( "--Error: No enemy state " );
	}
	return S_OK;
}

void Enemy::SetState( EnemyState state )
{
	// If state is an EnemyState set mCurrentState to state
	if( state == Idle		||
		state == Run		||
		state == Attack		||
		state == Death		||
		state == MoveToShip ||
		state == HuntPlayer		)
	{
		mCurrentState = state;
		IEventPtr state( new Event_Set_Enemy_State( mID, mCurrentState ) );
		EventManager::GetInstance()->QueueEvent( state );
	}
}

void Enemy::SetHuntedPlayer( XMFLOAT3 player )
{
	mVelocity.x = player.x - mPosition.x;
	mVelocity.z = player.z - mPosition.z;
	XMStoreFloat3( &mVelocity, XMVector3Normalize( XMLoadFloat3( &mVelocity ) ) );
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

BoundingCircle* Enemy::GetAttentionCircle() const
{
	return mAttentionRadius;
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
	mSpeed			= 1.0f;
	mVelocity		= XMFLOAT3( 0.0f, 0.0f, 0.0f );
	mIsAlive		= false;
	mCurrentState	= MoveToShip;

	mAttackRadius		= new BoundingCircle( 1.0f );
	mAttentionRadius	= new BoundingCircle( 1.0f );

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
	mSpeed			= 0.0f;
	mIsAlive		= false;
	mPosition		= XMFLOAT3( 0.0f, 0.0f, 0.0f );
	mDirection		= XMFLOAT3( 0.0f, 0.0f, 0.0f );
	mVelocity		= XMFLOAT3( 0.0f, 0.0f, 0.0f );
	mAttackRadius	=  nullptr;
	mAttentionRadius = nullptr;
	mCurrentState	= Idle;
}

Enemy::~Enemy()
{
}