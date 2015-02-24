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
			}
		}
	}

	return S_OK;
}

void IdleBehavior::OnEnter()
{
	mEnemy->mCurrentState	= Idle;
	mEnemy->mVelocity = XMFLOAT3( 0.0f, 0.0f, 0.0f );
	IEventPtr state( new Event_Set_Enemy_State( mEnemy->GetID(), Idle ) );
	EventManager::GetInstance()->QueueEvent( state );
}

void IdleBehavior::OnExit()
{

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
	mEnemy->mSteeringBehaviorManager->Update( deltaTime );
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
	return S_OK;
}

void MoveToShipBehavior::OnEnter()
{
	IEventPtr state( new Event_Set_Enemy_State( mEnemy->GetID(), MoveToShip ) );
	EventManager::GetInstance()->QueueEvent( state );
}

void MoveToShipBehavior::OnExit()
{

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
	if(	( !mHasAttacked && mTimeTillAttack <= 0.0f ) && (!mEnemy->mPlayers[mEnemy->mTargetIndex]->IsDown && mEnemy->mPlayers[mEnemy->mTargetIndex]->IsAlive ) )
	{
		// ... and player intersects the enemy's attack radius
		if( mEnemy->mAttackRadius->Intersect( mEnemy->mPlayers[mEnemy->mTargetIndex]->AggroCircle ) )
		{
			mHasAttacked = true;
			IEventPtr state( new Event_Tell_Server_Enemy_Attack_Player( mEnemy->mID, mEnemy->mTargetID, mEnemy->mDamage ) );
			EventManager::GetInstance()->QueueEvent( state );
			mTimeTillAttack	= mEnemy->mAttackRate;
		}
	}

	if( mStateTimer <= 0.0f )
		mEnemy->ChangeBehavior( IDLE_BEHAVIOR );

	return S_OK;
}

void AttackBehavior::OnEnter()
{
	mTimeTillAttack	= mEnemy->mAttackRate;
	mHasAttacked	= false;

	switch( mEnemy->mEnemyType )
	{
		case Standard:
			mStateTimer = 1.6f;
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
HRESULT TakeDamageBehavior::Update( float deltaTime )
{
	mEnemy->ChangeBehavior( HUNT_PLAYER_BEHAVIOR );
	return S_OK;
}

void TakeDamageBehavior::OnEnter()
{
	IEventPtr state( new Event_Set_Enemy_State( mEnemy->GetID(), TakeDamage ) );
	EventManager::GetInstance()->QueueEvent( state );
}

void TakeDamageBehavior::OnExit()
{

}

void TakeDamageBehavior::Reset()
{

}

HRESULT TakeDamageBehavior::Initialize( Enemy* enemy )
{
	mEnemy		= enemy;
	mBehavior	= TakeDamage;
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
	return S_OK;
}

void StunnedBehavior::OnEnter()
{
	IEventPtr state( new Event_Set_Enemy_State( mEnemy->GetID(), Stunned ) );
	EventManager::GetInstance()->QueueEvent( state );
}

void StunnedBehavior::OnExit()
{

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
	return S_OK;
}

void DeadBehavior::OnEnter()
{
	mEnemy->mIsAlive		= false;
	mEnemy->mCurrentHp		= 0.0f;
	mEnemy->mTimeTillSapwn	= mEnemy->mSpawnTime;
	
	IEventPtr state( new Event_Set_Enemy_State( mEnemy->mID, Death ) );
	EventManager::GetInstance()->QueueEvent( state );
}

void DeadBehavior::OnExit()
{

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