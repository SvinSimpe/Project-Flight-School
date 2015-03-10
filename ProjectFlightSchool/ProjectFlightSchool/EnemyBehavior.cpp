#include "Enemy.h"

///////////////////////////////////////////////////////////////////////////////
//								Base Behavior
///////////////////////////////////////////////////////////////////////////////
EnemyState IEnemyBehavior::GetBehavior() const
{
	return mBehavior;
}

IEnemyBehavior::IEnemyBehavior()
{
	mBehavior	= Idle;
}

IEnemyBehavior::~IEnemyBehavior()
{
}

///////////////////////////////////////////////////////////////////////////////
//								Idle Behavior
///////////////////////////////////////////////////////////////////////////////
HRESULT IdleBehavior::Update( float deltaTime )
{

	if( mEnemy->mIsAlive )
	{
		for ( size_t i = 0; i < MAX_NR_OF_PLAYERS; i++ )
		{
			if( mEnemy->mPlayers[i] != nullptr )
			{
				if( ( !mEnemy->mPlayers[i]->IsDown && mEnemy->mPlayers[i]->IsAlive ) && mEnemy->mAttentionRadius->Intersect( mEnemy->mPlayers[i]->AggroCircle ) )
				{
					mEnemy->SetTarget( mEnemy->mPlayers[i]->ID );
					mEnemy->ChangeBehavior( HUNT_PLAYER_BEHAVIOR );
				}
				else
				{
					mEnemy->ChangeBehavior( MOVE_TO_SHIP_BEHAVIOR );
				}
			}
		}
	}
	
	return S_OK;
}

void IdleBehavior::OnEnter()
{
	mEnemy->mIsAlive		= true;
	mEnemy->mCurrentState	= Idle;
	mEnemy->mVelocity = XMFLOAT3( 0.0f, 0.0f, 0.0f );
	IEventPtr state( new Event_Set_Enemy_State( mEnemy->GetID(), Idle ) );
	EventManager::GetInstance()->QueueEvent( state );
}

void IdleBehavior::OnExit()
{
	mEnemy->mLastState	= Idle;
}

void IdleBehavior::Reset()
{

}

HRESULT IdleBehavior::Initialize( Enemy* enemy )
{
	mEnemy		= enemy;
	mBehavior	= Idle;
	return S_OK;
}

void IdleBehavior::Release()
{

}

IdleBehavior::IdleBehavior()
{
	mBehavior	= Idle;
}

IdleBehavior::~IdleBehavior()
{

}

///////////////////////////////////////////////////////////////////////////////
//								Hunt Player Behavior
///////////////////////////////////////////////////////////////////////////////
HRESULT HuntPlayerBehavior::Update( float deltaTime )
{
	//mEnemy->mSteeringBehaviorManager->Update( deltaTime );
	mEnemy->Hunt( deltaTime );

	// If enemy damaged check, go to take Damage
	mEnemy->mTakingDamageTimer += deltaTime;
	if( mEnemy->mTakingDamage )
	{	
		if( mEnemy->mTakingDamageTimer > 0.4f )
		{
			mEnemy->ChangeBehavior( TAKE_DAMAGE_BEHAVIOR );
			mEnemy->mTakingDamageTimer = 0.0f;
		}
		mEnemy->mTakingDamage = false;
	}

	// If enemy lost track of target, go back to Idle
	if( mEnemy->mPlayers[mEnemy->mTargetIndex] != nullptr )
	{
		if( !mEnemy->mPlayers[mEnemy->mTargetIndex]->IsAlive || !mEnemy->mAttentionRadius->Intersect( mEnemy->mPlayers[mEnemy->mTargetIndex]->AggroCircle ) )
		{
			mEnemy->ChangeBehavior( IDLE_BEHAVIOR );
		}
	}

	// If enemy is in attack range of target, go to Attack
	if( mEnemy->mPlayers[mEnemy->mTargetIndex] != nullptr )
	{
		if( mEnemy->mPlayers[mEnemy->mTargetIndex]->IsAlive && mEnemy->mAttackRadius->Intersect( mEnemy->mPlayers[mEnemy->mTargetIndex]->AggroCircle ) )
		{
			mEnemy->ChangeBehavior( ATTACK_BEHAVIOR );
		}
	}

	return S_OK;
}

void HuntPlayerBehavior::OnEnter()
{
	IEventPtr state( new Event_Set_Enemy_State( mEnemy->GetID(), HuntPlayer ) );
	EventManager::GetInstance()->QueueEvent( state );
}

void HuntPlayerBehavior::OnExit()
{
	mEnemy->mLastState	= HuntPlayer;
}

void HuntPlayerBehavior::Reset()
{

}

HRESULT HuntPlayerBehavior::Initialize( Enemy* enemy )
{
	mEnemy		= enemy;
	mBehavior	= HuntPlayer;
	return S_OK;
}

void HuntPlayerBehavior::Release()
{

}

HuntPlayerBehavior::HuntPlayerBehavior()
{
	mBehavior	= HuntPlayer;
}

HuntPlayerBehavior::~HuntPlayerBehavior()
{
}

///////////////////////////////////////////////////////////////////////////////
//								Move To Ship Behavior
///////////////////////////////////////////////////////////////////////////////
HRESULT MoveToShipBehavior::Update( float deltaTime )
{
	if( mEnemy->mShips.at(mEnemy->mTargetShipIndex)->GetHitCircle() != nullptr )
	{
		if( mEnemy->mAttackRadius->Intersect( mEnemy->mShips.at(mEnemy->mTargetShipIndex)->GetHitCircle() ) )
			mEnemy->ChangeBehavior( ATTACK_BEHAVIOR );
		else
			mEnemy->Hunt( deltaTime );
	}

	return S_OK;
}

void MoveToShipBehavior::OnEnter()
{
	mEnemy->mCurrentState	= MoveToShip;
	IEventPtr state( new Event_Set_Enemy_State( mEnemy->GetID(), MoveToShip ) );
	EventManager::GetInstance()->QueueEvent( state );
}

void MoveToShipBehavior::OnExit()
{
	mEnemy->mLastState	= MoveToShip;
}

void MoveToShipBehavior::Reset()
{

}

HRESULT MoveToShipBehavior::Initialize( Enemy* enemy )
{
	mEnemy		= enemy;
	mBehavior	= MoveToShip;
	return S_OK;
}

void MoveToShipBehavior::Release()
{

}

MoveToShipBehavior::MoveToShipBehavior()
{
	mBehavior	= MoveToShip;
}

MoveToShipBehavior::~MoveToShipBehavior()
{
}

///////////////////////////////////////////////////////////////////////////////
//								Attack Behavior
///////////////////////////////////////////////////////////////////////////////
HRESULT AttackBehavior::Update( float deltaTime )
{
	mStateTimer -= deltaTime;
	mTimeTillAttack -= deltaTime;

	// If player is not dead or downed and attackTimer < 0.0f...
	if( mEnemy->mLastState == HuntPlayer )
	{
		if(	( !mHasAttacked && mTimeTillAttack <= 0.0f ) && (!mEnemy->mPlayers[mEnemy->mTargetIndex]->IsDown && mEnemy->mPlayers[mEnemy->mTargetIndex]->IsAlive ) )
		{
			// ... and player intersects the enemy's attack radius
			if( mEnemy->mAttackRadius->Intersect( mEnemy->mPlayers[mEnemy->mTargetIndex]->AggroCircle ) )
			{
				mHasAttacked = true;
				if( mEnemy->mEnemyType == Ranged )
				{
					XMFLOAT3 direction;
					direction.x = mEnemy->mPlayers[mEnemy->mTargetIndex]->Pos.x - mEnemy->GetPosition().x;
					direction.z = mEnemy->mPlayers[mEnemy->mTargetIndex]->Pos.z - mEnemy->GetPosition().z;
					direction.y = 0.0f;

					IEventPtr E1(new Event_Enemy_Fired_Projectile(
						ENEMY_PROJECTILE_ID,
						mEnemy->mPosition,
						direction,
						ENEMY_PROJECTILE_SPEED,
						ENEMY_PROJECTILE_RANGE ) );
					EventManager::GetInstance()->QueueEvent( E1 );
				}
				else
				{
					IEventPtr state( new Event_Tell_Server_Enemy_Attack_Player( mEnemy->mID, mEnemy->mTargetID, mEnemy->mDamage ) );
					EventManager::GetInstance()->QueueEvent( state );
				}
				mTimeTillAttack	= mEnemy->mAttackRate;
			}
		}
	}
	else if( mEnemy->mLastState	== MoveToShip )
	{
		if(	 !mHasAttacked && mTimeTillAttack <= 0.0f )
		{
			// Ship intersects the enemy's attack radius
				if( mEnemy->mAttackRadius->Intersect( mEnemy->mShips.at(mEnemy->mTargetShipIndex)->GetHitCircle() ) )
				{
					mHasAttacked = true;
					if (mEnemy->mEnemyType == Ranged)
					{
						XMFLOAT3 direction;
						direction.x = mEnemy->mPlayers[mEnemy->mTargetShipIndex]->Pos.x - mEnemy->GetPosition().x;
						direction.z = mEnemy->mPlayers[mEnemy->mTargetShipIndex]->Pos.z - mEnemy->GetPosition().z;
						direction.y = 0.0f;

						IEventPtr E1(new Event_Enemy_Fired_Projectile(
							ENEMY_PROJECTILE_ID,
							mEnemy->mPosition,
							direction,
							ENEMY_PROJECTILE_SPEED,
							ENEMY_PROJECTILE_RANGE));
						EventManager::GetInstance()->QueueEvent(E1);
					}
					else
					{
						mEnemy->mShips.at(mEnemy->mTargetShipIndex)->TakeDamage( mEnemy->mDamage );
					}
					mTimeTillAttack	= mEnemy->mAttackRate;
				}
		}
	}

	if( mStateTimer <= 0.0f )
		mEnemy->ChangeBehavior( IDLE_BEHAVIOR );

	return S_OK;
}

void AttackBehavior::OnEnter()
{
	mEnemy->mCurrentState	= Attack;
	mTimeTillAttack	= mEnemy->mAttackRate;
	mHasAttacked	= false;
	mEnemy->mVelocity = XMFLOAT3( 0.0f, 0.0f, 0.0f );

	switch( mEnemy->mEnemyType )
	{
		case Standard:
			mStateTimer = 2.0f;
			break;

		case Ranged:
			mStateTimer = 2.0f;
			break;

		case Boomer:
			mStateTimer = 2.0f;
			break;

		case Tank:
			mStateTimer = 2.5f;
			break;
	}

	IEventPtr state( new Event_Set_Enemy_State( mEnemy->GetID(), Attack ) );
	EventManager::GetInstance()->QueueEvent( state );
}

void AttackBehavior::OnExit()
{
	mEnemy->mLastState	= Attack;
}

void AttackBehavior::Reset()
{

}

HRESULT AttackBehavior::Initialize( Enemy* enemy )
{
	mEnemy			= enemy;
	mBehavior		= Attack;
	mTimeTillAttack	= mEnemy->mAttackRate;
	
	return S_OK;
}

void AttackBehavior::Release()
{

}

AttackBehavior::AttackBehavior()
{
	mBehavior	= Attack;
}

AttackBehavior::~AttackBehavior()
{

}

///////////////////////////////////////////////////////////////////////////////
//								Take Damage Behavior
///////////////////////////////////////////////////////////////////////////////
void TakeDamageBehavior::DamageFromPlayer( IEventPtr eventPtr )
{
	if( eventPtr->GetEventType() == Event_Client_Projectile_Damage_Enemy::GUID )
	{
		std::shared_ptr<Event_Client_Projectile_Damage_Enemy> data = std::static_pointer_cast<Event_Client_Projectile_Damage_Enemy>( eventPtr );
		if( data->EnemyID() == mEnemy->mID )
		{
			mEnemy->TakeDamage( data->Damage(), data->ID() );
		}
	}
}

HRESULT TakeDamageBehavior::Update( float deltaTime )
{
	if( mEnemy->mIsAlive )
		mEnemy->ChangeBehavior( HUNT_PLAYER_BEHAVIOR );
	//else
	//	mEnemy->ChangeBehavior( DEAD_BEHAVIOR );

	return S_OK;
}

void TakeDamageBehavior::OnEnter()
{
	mEnemy->mCurrentState	= TakeDamage;
	IEventPtr state( new Event_Set_Enemy_State( mEnemy->GetID(), TakeDamage ) );
	EventManager::GetInstance()->QueueEvent( state );
}

void TakeDamageBehavior::OnExit()
{
	mEnemy->mLastState	= TakeDamage;
}

void TakeDamageBehavior::Reset()
{

}

HRESULT TakeDamageBehavior::Initialize( Enemy* enemy )
{
	mEnemy		= enemy;
	mBehavior	= TakeDamage;
	EventManager::GetInstance()->AddListener( &TakeDamageBehavior::DamageFromPlayer, this, Event_Client_Projectile_Damage_Enemy::GUID );
	return S_OK;
}

void TakeDamageBehavior::Release()
{

}

TakeDamageBehavior::TakeDamageBehavior()
{
	mBehavior	= TakeDamage;
}

TakeDamageBehavior::~TakeDamageBehavior()
{

}

///////////////////////////////////////////////////////////////////////////////
//								Stunned Behavior
///////////////////////////////////////////////////////////////////////////////
HRESULT StunnedBehavior::Update( float deltaTime )
{

	mStateTimer -= deltaTime;

	if( mStateTimer < 0.0f )
	{
		mEnemy->ChangeBehavior( IDLE_BEHAVIOR );
	}

	return S_OK;
}

void StunnedBehavior::OnEnter()
{
	mEnemy->mCurrentState	= Stunned;
	mStateTimer = mEnemy->mStunTimer;
	IEventPtr state( new Event_Set_Enemy_State( mEnemy->GetID(), Stunned ) );
	EventManager::GetInstance()->QueueEvent( state );
}

void StunnedBehavior::OnExit()
{
	mEnemy->mLastState	= Stunned;
}

void StunnedBehavior::Reset()
{

}

HRESULT StunnedBehavior::Initialize( Enemy* enemy )
{
	mEnemy		= enemy;
	mBehavior	= Stunned;
	return S_OK;
}

void StunnedBehavior::Release()
{

}

StunnedBehavior::StunnedBehavior()
{
	mBehavior	= Stunned;
}

StunnedBehavior::~StunnedBehavior()
{

}

///////////////////////////////////////////////////////////////////////////////
//								Dead Behavior
///////////////////////////////////////////////////////////////////////////////
HRESULT DeadBehavior::Update( float deltaTime )
{
	mEnemy->HandleSpawn();
	return S_OK;
}

void DeadBehavior::OnEnter()
{
	mEnemy->mCurrentState	= Death;
	mEnemy->mVelocity		= XMFLOAT3( 0.0f, 0.0f, 0.0f );
	mEnemy->mCurrentState	= Death;
	mEnemy->mIsAlive		= false;
	mEnemy->mCurrentHp		= 0.0f;
	mEnemy->mTimeTillSapwn	= mEnemy->mSpawnTime;
	
	IEventPtr state( new Event_Set_Enemy_State( mEnemy->mID, Death ) );
	EventManager::GetInstance()->QueueEvent( state );
}

void DeadBehavior::OnExit()
{
	mEnemy->mLastState	= Death;
}

void DeadBehavior::Reset()
{

}

HRESULT DeadBehavior::Initialize( Enemy* enemy )
{
	mEnemy		= enemy;
	mBehavior	= Death;
	return S_OK;
}

void DeadBehavior::Release()
{

}

DeadBehavior::DeadBehavior()
{
	mBehavior	= Death;
}

DeadBehavior::~DeadBehavior()
{

}