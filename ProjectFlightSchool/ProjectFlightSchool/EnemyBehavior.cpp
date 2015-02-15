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
	if( mEnemy->IsAggro() )
		mEnemy->ChangeBehavior( HUNT_PLAYER_BEHAVIOR );

	return S_OK;
}

void IdleBehavior::OnEnter()
{
	mEnemy->SetState( Idle );
	mEnemy->SetVelocity( XMFLOAT3( 0.0f, 0.0f, 0.0f ) );
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
	mEnemy->Hunt( deltaTime );
	if( mEnemy->GetEnemyState() == Attack )
		mEnemy->ChangeBehavior( ATTACK_BEHAVIOR );

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

	if( mTimeTillAttack <= 0.0f )
	{
		IEventPtr state( new Event_Tell_Server_Enemy_Attack_Player( mEnemy->GetID(), mEnemy->GetTargetID(), mEnemy->GetDamage() ) );
		EventManager::GetInstance()->QueueEvent( state );
	}

	if( mStateTimer <= 0.0f )
		mEnemy->ChangeBehavior( IDLE_BEHAVIOR );

	return S_OK;
}

void AttackBehavior::OnEnter()
{
	mTimeTillAttack	= mEnemy->GetAttackRate();

	switch( mEnemy->GetEnemyType() )
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
			mStateTimer = 2.0f;
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
	mTimeTillAttack	= mEnemy->GetAttackRate();
	
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
	IEventPtr state( new Event_Set_Enemy_State( mEnemy->GetID(), Death ) );
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