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
	mAttackRate					= 0.8f;
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
	mDamage						= ENEMY_PROJECTILE_DAMAGE;
	mSpeed						= 7.0f;
	mAttackRadius->radius		= 5.0f;
	mAttentionRadius->radius	= 15.0f;
	mXpDrop						= 7;
	mSpawnTime					= 10.0f;
	mAttackRate					= 1.5f;
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
	mMaxHp						= 5.0f;
	mCurrentHp					= mMaxHp;
	mDamage						= ENEMY_BOOMER_DAMAGE;
	mSpeed						= 8.0f;
	mAttackRadius->radius		= 1.0f;
	mAttentionRadius->radius	= 15.0f;
	mXpDrop						= 8;
	mSpawnTime					= 30.0f;
	mAttackRate					= 0.01f;
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
	mMaxHp						= 500.0f;
	mCurrentHp					= mMaxHp;
	mDamage						= 50.0f;
	mSpeed						= 1.5f;
	mAttackRadius->radius		= 1.0f;
	mAttentionRadius->radius	= 10.0f;
	mXpDrop						= 15;
	mSpawnTime					= 10.0f;
	mAttackRate					= 1.1f;
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

//void Enemy::DamageFromPlayer( IEventPtr eventPtr )
//{
//	if( eventPtr->GetEventType() == Event_Client_Projectile_Damage_Enemy::GUID )
//	{
//		std::shared_ptr<Event_Client_Projectile_Damage_Enemy> data = std::static_pointer_cast<Event_Client_Projectile_Damage_Enemy>( eventPtr );
//		if( data->EnemyID() == mID )
//		{
//			TakeDamage( data->Damage(), data->ID() );
//		}
//	}
//}

///////////////////////////////////////////////////////////////////////////////
//									PUBLIC
///////////////////////////////////////////////////////////////////////////////
HRESULT Enemy::Update( float deltaTime, ServerPlayer** players, UINT NrOfPlayers )
{
	mDeltaTime		= deltaTime;

	//mSteeringBehaviorManager->Update( deltaTime );

	XMStoreFloat3( &mVelocity, XMVector3Normalize( XMLoadFloat3( &mVelocity ) ) );

	mDeltaTime					= deltaTime;
	mAttackRadius->center		= mPosition;
	mAttentionRadius->center	= mPosition;
	mEvadeRadius->center		= mPosition;
	mPlayers					= players;
	mNrOfPlayers				= NrOfPlayers;

	if( mStateTimer >= 0.0f )
		mStateTimer -= deltaTime;
		
	mBehaviors[mCurrentBehavior]->Update( deltaTime );

	mPosition.x += mVelocity.x * mSpeed * deltaTime;
	mPosition.z += mVelocity.z * mSpeed * deltaTime;

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

	mHasEvaded	= false;

	

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

void Enemy::SetShipTarget( UINT id, std::vector<ServerShip*>& ships )
{
	mShips			= ships;
	mTargetShipID	= id;

	for ( size_t i = 0; i < 2; i++ )
	{
		if( mShips[i]->GetID() == id )
		{
			mTargetShipIndex = i;
		}
	}
}

void Enemy::Hunt( float deltaTime )
{
	mSteeringBehaviorManager->Update( deltaTime );
	XMFLOAT3 totalSteeringForce = mSteeringBehaviorManager->GetFinalSteeringForce();

	/*mVelocity.x = mPlayers[mTargetIndex]->Pos.x - mPosition.x;
	mVelocity.z = mPlayers[mTargetIndex]->Pos.z - mPosition.z;
	XMStoreFloat3( &mVelocity, XMVector3Normalize( XMLoadFloat3( &mVelocity ) ) );
	*/

	//mVelocity.x += totalSteeringForce.x;
	//mVelocity.z += totalSteeringForce.z;
	//mVelocity.y += totalSteeringForce.y;

	float interpolation = max( 0.0f, 1.0f - deltaTime * 0.05f );

	XMStoreFloat3( &mVelocity, XMLoadFloat3( &mVelocity ) * interpolation
					+ XMLoadFloat3( &totalSteeringForce ) * ( 1.0f - interpolation ) );

	XMStoreFloat3( &mVelocity, XMVector3Normalize( XMLoadFloat3( &mVelocity ) ) );

	mDirection = mVelocity;

	//mPosition.x += mVelocity.x * mSpeed * deltaTime;
	//mPosition.z += mVelocity.z * mSpeed * deltaTime;
	//mDirection = mVelocity;

	/*mPosition.x	 += totalSteeringForce.x * mSpeed * deltaTime;
	mPosition.z	 += totalSteeringForce.z * mSpeed * deltaTime;
	mPosition.y	 += totalSteeringForce.y * mSpeed * deltaTime;
	mDirection = mVelocity;	*/			 
}

void Enemy::TakeDamage( float damage, UINT killer )
{
	if( mIsAlive )
	{
		mCurrentHp -= damage;
		if( mCurrentHp <= 0.0f )
		{
			Die( killer );
		}
		mTakingDamage	= true;
	}
}

void Enemy::TakeMeleeDamage( float damage, float knockBack, XMFLOAT3 direction, float stun, UINT killer )
{
	direction.x *= knockBack;
	direction.z *= knockBack;
	AddImpuls( direction );
	mCurrentState = Stunned;
	mStunTimer = stun;
	IEventPtr state( new Event_Set_Enemy_State( mID, Stunned ) );
	EventManager::GetInstance()->QueueEvent( state );
	TakeDamage( damage, killer );
}

void Enemy::AddImpuls( XMFLOAT3 impuls )
{
	mPosition.x += impuls.x;
	mPosition.z += impuls.z;
}

void Enemy::HandleSpawn()
{
	if( mTimeTillSapwn <= 0.0f )
	{
		Spawn();
	}
	else
	{
		mTimeTillSapwn -= mDeltaTime;
	}
}

void Enemy::Spawn()
{
	//switch( mID % 4 )
	//{
	//case 0:
	//	CreateStandard();
	//	break;
	//case 1:
	//	CreateRanged();
	//	//CreateStandard();
	//	break;
	//case 2:
	//	CreateBoomer();
	//	//CreateStandard();
	//	break;
	//case 3:
	//	CreateTank();
	//	break;
	//}

	switch( mID % 10 )
	{
	case 9:
		CreateTank();
		break;
	case 1:
	case 2:
		CreateRanged();
		break;
	case 4:
	case 5:
		CreateBoomer();
		break;
	default:
		CreateStandard();
	}

	if( mEnemyType == Tank )
	{
		if( mShips[0]->TanksTargetMe < mShips[1]->TanksTargetMe )
		{
			mTargetShipIndex = 0;
			mShips[0]->TanksTargetMe++;
		}
		else
		{
			mTargetShipIndex = 1;
			mShips[1]->TanksTargetMe++;
		}
	}
	//CreateStandard();
	//CreateRanged();
	//CreateBoomer();
	//CreateTank();

	mPosition		= mSpawnPos;
	mIsAlive		= true;
	mHasSpawnPos	= false;
	
	ChangeBehavior( IDLE_BEHAVIOR );
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
	// Send dieEv
	ChangeBehavior( DEAD_BEHAVIOR );
	if( mEnemyType == Boomer )
	{
		IEventPtr E1( new Event_Client_Request_ParticleSystem( mID, (int)BoomerExplosion, XMFLOAT3( mPosition.x, 2.0f, mPosition.z ), XMFLOAT3( 1.0f, 1.0f, 1.0f ), XMFLOAT3( 0.0f, 1.0f, 0.0f ) ) );
		EventManager::GetInstance()->QueueEvent( E1 );
	}
	if( killer != 0 )
	{
		IEventPtr E1( new Event_XP( killer, mXpDrop ) );
		EventManager::GetInstance()->QueueEvent( E1 );
	}

	IEventPtr resetTurrets( new Event_Reset_Turret_Targets() );
	EventManager::GetInstance()->QueueEvent( resetTurrets );
}

bool Enemy::HasSpawnPos() const
{
	return mHasSpawnPos;
}

void Enemy::SetSpawnPos( XMFLOAT3 spawnPos )
{
	mSpawnPos		= spawnPos;
	mHasSpawnPos	= true;
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

float Enemy::GetHP() const
{
	return mCurrentHp;
}

float Enemy::GetMaxHP() const
{
	return mMaxHp;
}

float Enemy::GetSpeed() const
{
	return mSpeed;
}

XMFLOAT3 Enemy::GetVelocity() const
{
	return mVelocity;
}

HRESULT Enemy::Initialize( int id, ServerPlayer** players, UINT NrOfPlayers, Enemy** otherEnemies )
{
	mID				= id;
	mPlayers		= players;
	mNrOfPlayers	= NrOfPlayers;
	mOtherEnemies	= otherEnemies;
	mMaxHp			= 100.0f;
	mCurrentHp		= mMaxHp;
	mDamage			= 0.0f;
	mSpeed			= 1.0f;
	mVelocity		= XMFLOAT3( 0.0f, 0.0f, 0.0f );
	mIsAlive		= false;
	mCurrentState	= MoveToShip;

	mAttackRadius		= new BoundingCircle( 1.0f );
	mAttentionRadius	= new BoundingCircle( 1.0f );
	mEvadeRadius		= new BoundingCircle( 2.0f );

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

	mSteeringBehaviorManager			  = new SteeringBehaviorManager();
	mSteeringBehaviorManager->Initialize(	this );
	//mSteeringBehaviorManager->AddBehavior( new SteerWander( this ) );
	mSteeringBehaviorManager->AddBehavior(  new SteerApproach( this ) );
	mSteeringBehaviorManager->AddBehavior(  new SteerEvade( this ) );
	mSteeringBehaviorManager->AddBehavior(  new SteerAvoidObjects( this ) );
	mSteeringBehaviorManager->SetUpBehavior( 0, 4.0f, 1.0f );
	mSteeringBehaviorManager->SetUpBehavior( 1, 10.0f, 1.0f );
	mSteeringBehaviorManager->SetUpBehavior( 2, 500.0f, 1.0f );

	//EventManager::GetInstance()->AddListener( &Enemy::DamageFromPlayer, this, Event_Client_Projectile_Damage_Enemy::GUID );

	return S_OK;
}

void Enemy::Reset()
{
	mCurrentState	= Idle;
	mIsAlive		= false;
	mHasSpawnPos	= false;
	mPosition		= XMFLOAT3( 0.0f, 0.0f, 0.0f );
	mDirection		= XMFLOAT3( 0.0f, 0.0f, 0.0f );
	mVelocity		= XMFLOAT3( 0.0f, 0.0f, 0.0f );	
	mPlayers		= nullptr;
	
	ChangeBehavior( DEAD_BEHAVIOR );
}

void Enemy::Release()
{
	SAFE_DELETE( mAttackRadius );
	SAFE_DELETE( mAttentionRadius );
	SAFE_DELETE( mEvadeRadius );
	
	mCurrentBehavior	= 0;
	
	for ( size_t i = 0; i < NR_OF_ENEMY_BEHAVIORS; i++ )
	{
		mBehaviors[i]->Release();
		SAFE_DELETE( mBehaviors[i] );
	}
	delete [] mBehaviors;

	SAFE_RELEASE_DELETE( mSteeringBehaviorManager );
}

Enemy::Enemy()
{
	mID							= 0;
	mCurrentHp					= 0.0f;
	mMaxHp						= 0.0f;
	mDamage						= 0.0f;
	mSpeed						= 0.0f;
	mIsAlive					= false;
	mPosition					= XMFLOAT3( 0.0f, 0.0f, 0.0f );
	mDirection					= XMFLOAT3( 0.0f, 0.0f, 0.0f );
	mVelocity					= XMFLOAT3( 0.0f, 0.0f, 0.0f );
	mAttackRadius				= nullptr;
	mAttentionRadius			= nullptr;
	mEvadeRadius				= nullptr;
	mCurrentState				= Idle;
	mXpDrop						= 0;
	mSpawnTime					= 0.0f;
	mTimeTillSapwn				= 0.0f;
	mAttackRate					= 0.0f;
	mTimeTillAttack				= 0.0f;
	mStateTimer					= 0.0f;
	mStunTimer					= 0.0f;
	mTakingDamageTimer			= 0.0f;
	mTargetIndex				= 0;
	mTargetID					= 0;
	mTargetShipIndex			= 0;
	mTargetShipID				= 0;
	mPlayers					= nullptr;
	//mShips						= nullptr;
	mOtherEnemies				= nullptr;
	mTakingDamage				= false;
	mSteeringBehaviorManager	= nullptr;
	mSpawnPos					= XMFLOAT3( 0.0f, 0.0f, 0.0f );
	mDeltaTime					= 0.0f;
	mHasSpawnPos				= false;
}

Enemy::~Enemy()
{
}
