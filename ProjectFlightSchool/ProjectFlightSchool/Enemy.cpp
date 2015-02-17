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
	mMaxHp						= 60.0f;
	mCurrentHp					= mMaxHp;
	mDamage						= 10.0f;
	mSpeed						= 5.0f;
	mAttackRadius->radius		= 0.5f;
	mAttentionRadius->radius	= 10.0f;
	mXpDrop						= 5;
	mSpawnTime					= 10.0f;
	mAttackRate					= 1.2f;
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
	mMaxHp						= 50.0f;
	mCurrentHp					= mMaxHp;
	mDamage						= 10.0f;
	mSpeed						= 7.0f;
	mAttackRadius->radius		= 5.0f;
	mAttentionRadius->radius	= 10.0f;
	mXpDrop						= 5;
	mSpawnTime					= 10.0f;
	mAttackRate					= 1.0f;
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
	mMaxHp						= 20.0f;
	mCurrentHp					= mMaxHp;
	mDamage						= 40.0f;
	mSpeed						= 8.0f;
	mAttackRadius->radius		= 1.0f;
	mAttentionRadius->radius	= 10.0f;
	mXpDrop						= 5;
	mSpawnTime					= 10.0f;
	mAttackRate					= 1.0f;
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
	mEnemyType					= Tank;
	mMaxHp						= 400.0f;
	mCurrentHp					= mMaxHp;
	mDamage						= 20.0f;
	mSpeed						= 1.5f;
	mAttackRadius->radius		= 1.0f;
	mAttentionRadius->radius	= 10.0f;
	mXpDrop						= 15;
	mSpawnTime					= 10.0f;
	mAttackRate					= 1.8f;
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
HRESULT Enemy::Update( float deltaTime, ServerPlayer** players, UINT NrOfPlayers )
{
	mDeltaTime					= deltaTime;
	mAttackRadius->center		= mPosition;
	mAttentionRadius->center	= mPosition;
	mPlayers					= players;
	mNrOfPlayers				= NrOfPlayers;

	if( mStateTimer >= 0.0f )
		mStateTimer -= deltaTime;

	mBehaviors[mCurrentBehavior]->Update( deltaTime );

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

	return S_OK;
}


void Enemy::ChangeBehavior( const int NEW_BEHAVIOR )
{
	if( mCurrentBehavior != NEW_BEHAVIOR )
	{
		mBehaviors[mCurrentBehavior]->OnExit();
		mCurrentBehavior		= NEW_BEHAVIOR;
		mBehaviors[mCurrentBehavior]->OnEnter();
	}
}

void Enemy::ResetBehavior( const int BEHAVIOR )
{
	mBehaviors[BEHAVIOR]->Reset();
}


void Enemy::SetTarget( UINT id )
{
	mTargetID	= id;
	for ( size_t i = 0; i < MAX_NR_OF_PLAYERS; i++ )
	{
		if( mPlayers[i] != nullptr )
		{
			if( mPlayers[i]->ID == id )
				mTargetIndex = i;
		}
	}
}

void Enemy::Hunt( float deltaTime )
{
	mVelocity.x = mPlayers[mTargetIndex]->Pos.x - mPosition.x;
	mVelocity.z = mPlayers[mTargetIndex]->Pos.z - mPosition.z;
	XMStoreFloat3( &mVelocity, XMVector3Normalize( XMLoadFloat3( &mVelocity ) ) );

	mPosition.x += mVelocity.x * mSpeed * deltaTime;
	mPosition.z += mVelocity.z * mSpeed * deltaTime;
	mDirection = mVelocity;
}

void Enemy::TakeDamage( float damage, UINT killer )
{
	mCurrentHp -= damage;
	if( mCurrentHp <= 0.0f )
	{
		Die( killer );
	}
	mTakingDamage	= true;
}

void Enemy::TakeMeleeDamage( float damage, float knockBack, XMFLOAT3 direction, float stun, UINT killer )
{
		direction.x *= knockBack;
		direction.z *= knockBack;
		AddImpuls( direction );
		mCurrentState = Stunned;
		mStunTimer = stun;
		IEventPtr state( new Event_Set_Enemy_State( mID, Idle ) );
		EventManager::GetInstance()->QueueEvent( state );
		TakeDamage( damage, killer );
}

void Enemy::AddImpuls( XMFLOAT3 impuls )
{
	mPosition.x += impuls.x;
	mPosition.z += impuls.z;
}

void Enemy::HandleSpawn( float deltaTime, XMFLOAT3 spawnPos )
{
	if( mTimeTillSapwn <= 0.0f )
	{
		Spawn( spawnPos );
	}
	else
	{
		mTimeTillSapwn -= deltaTime;
	}
}

void Enemy::Spawn( XMFLOAT3 spawnPos )
{
	switch( mID % 4 )
	{
	case 0:
		CreateStandard();
		break;
	case 1:
		CreateRanged();
		break;
	case 2:
		//CreateBoomer();
		CreateStandard();
		break;
	case 3:
		CreateTank();
		break;
	}

	CreateStandard();
	//CreateRanged();
	//CreateBoomer();
	//CreateTank();

	mPosition	= spawnPos;
	mIsAlive	= true;
	

	// Send spawnEv
	IEventPtr state( new Event_Set_Enemy_State( mID, Idle ) );
	EventManager::GetInstance()->QueueEvent( state );
}

BoundingCircle* Enemy::GetAttackCircle() const
{
	return mAttackRadius;
}

BoundingCircle* Enemy::GetAttentionCircle() const
{
	return mAttentionRadius;
}

void Enemy::Die( UINT killer )
{
	mIsAlive		= false;
	mCurrentHp		= 0.0f;
	mTimeTillSapwn	= mSpawnTime;
	
	// Send dieEv
	IEventPtr state( new Event_Set_Enemy_State( mID, Death ) );
	EventManager::GetInstance()->QueueEvent( state );
	IEventPtr E1( new Event_Server_XP( killer, mXpDrop ) );
	EventManager::GetInstance()->QueueEvent( E1 );
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

bool Enemy::IsAlive() const
{
	return mIsAlive;
}

XMFLOAT3 Enemy::GetPosition() const
{
	return mPosition;
}

XMFLOAT3 Enemy::GetDirection() const
{
	return mDirection;
}

HRESULT Enemy::Initialize( int id, ServerPlayer** players, UINT NrOfPlayers )
{
	mID				= id;
	mPlayers		= players;
	mNrOfPlayers	= NrOfPlayers;
	mMaxHp			= 100.0f;
	mCurrentHp		= mMaxHp;
	mDamage			= 0.0f;
	mSpeed			= 1.0f;
	mVelocity		= XMFLOAT3( 0.0f, 0.0f, 0.0f );
	mIsAlive		= false;
	mCurrentState	= MoveToShip;

	mAttackRadius		= new BoundingCircle( 1.0f );
	mAttentionRadius	= new BoundingCircle( 1.0f );
	
	mBehaviors			= new IEnemyBehavior*[NR_OF_ENEMY_BEHAVIORS];

	for ( size_t i = 0; i < NR_OF_ENEMY_BEHAVIORS; i++ )
	{
		mBehaviors[i] = nullptr;
	}

	mBehaviors[IDLE_BEHAVIOR]			= new IdleBehavior();
	mBehaviors[HUNT_PLAYER_BEHAVIOR]	= new HuntPlayerBehavior();
	mBehaviors[MOVE_TO_SHIP_BEHAVIOR]	= new MoveToShipBehavior();
	mBehaviors[ATTACK_BEHAVIOR]			= new AttackBehavior();
	mBehaviors[TAKE_DAMAGE_BEHAVIOR]	= new TakeDamageBehavior();
	mBehaviors[STUNNED_BEHAVIOR]		= new StunnedBehavior();
	mBehaviors[DEAD_BEHAVIOR]			= new DeadBehavior();

	for ( size_t i = 0; i < NR_OF_ENEMY_BEHAVIORS; i++ )
	{
		mBehaviors[i]->Initialize( this );
	}

	return S_OK;
}

void Enemy::Reset()
{
	mEnemyType		= Standard;
	mCurrentState	= Idle;
	mIsAlive		= false;
	mPosition		= XMFLOAT3( 0.0f, 0.0f, 0.0f );
	mDirection		= XMFLOAT3( 0.0f, 0.0f, 0.0f );
	mVelocity		= XMFLOAT3( 0.0f, 0.0f, 0.0f );	
}

void Enemy::Release()
{
	SAFE_DELETE( mAttackRadius );
	SAFE_DELETE( mAttentionRadius );
	
	mCurrentBehavior	= 0;
	
	for ( size_t i = 0; i < NR_OF_ENEMY_BEHAVIORS; i++ )
	{
		mBehaviors[i]->Release();
		SAFE_DELETE( mBehaviors[i] );
	}

	delete [] mBehaviors;
}

Enemy::Enemy()
{
	mID					= 0;
	mCurrentHp			= 0.0f;
	mMaxHp				= 0.0f;
	mDamage				= 0.0f;
	mSpeed				= 0.0f;
	mIsAlive			= false;
	mPosition			= XMFLOAT3( 0.0f, 0.0f, 0.0f );
	mDirection			= XMFLOAT3( 0.0f, 0.0f, 0.0f );
	mVelocity			= XMFLOAT3( 0.0f, 0.0f, 0.0f );
	mAttackRadius		=  nullptr;
	mAttentionRadius	 = nullptr;
	mCurrentState		= Idle;
	mXpDrop				= 0;
	mSpawnTime			= 0.0f;
	mTimeTillSapwn		= 0.0f;
	mAttackRate			= 0.0f;
	mTimeTillAttack		= 0.0f;
	mStateTimer			= 0.0f;
	mStunTimer			= 0.0f;
	mTakingDamageTimer	= 0.0f;
	mTargetIndex		= 0;
	mTargetID			= 0;
	mPlayers			= nullptr;
	mTakingDamage		= false;
}

Enemy::~Enemy()
{
}
