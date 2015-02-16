#ifndef ENEMY_H
#define ENEMY_H

#include "Events.h"
#include "Graphics.h"
#include "BoundingGeometry.h"
#include "RenderManager.h"
#include "Font.h"
#include "RemotePlayer.h"

class Enemy;

#define MAX_NR_OF_ENEMIES		20

// ---- Define all enemy animations ----
// Standard
#define ENEMY_ANIMATION_STANDARD_IDLE	0
#define ENEMY_ANIMATION_STANDARD_RUN	1
#define ENEMY_ANIMATION_STANDARD_ATTACK	2
#define ENEMY_ANIMATION_STANDARD_DEATH	3
// Ranged
#define ENEMY_ANIMATION_RANGED_IDLE		4
#define ENEMY_ANIMATION_RANGED_RUN		5
#define ENEMY_ANIMATION_RANGED_ATTACK	6
#define ENEMY_ANIMATION_RANGED_DEATH	7
// Boomer
#define ENEMY_ANIMATION_BOOMER_IDLE		8
#define ENEMY_ANIMATION_BOOMER_RUN		9
#define ENEMY_ANIMATION_BOOMER_ATTACK	10
#define ENEMY_ANIMATION_BOOMER_DEATH	11
// Tank
#define ENEMY_ANIMATION_TANK_IDLE		12
#define ENEMY_ANIMATION_TANK_RUN		13
#define ENEMY_ANIMATION_TANK_ATTACK		14
#define ENEMY_ANIMATION_TANK_DEATH		15

#define ENEMY_ANIMATION_COUNT	16
//----------------------------------------

enum EnemyType { Standard, Ranged, Boomer, Tank };
enum EnemyState { Idle, HuntPlayer, MoveToShip, TakeDamage, Attack, Death, Stunned, };

#pragma region Behaviors
///////////////////////////////////////////////////////////////////////////////
//								Behaviors
///////////////////////////////////////////////////////////////////////////////

const int IDLE_BEHAVIOR				= 0;
const int HUNT_PLAYER_BEHAVIOR		= 1;
const int MOVE_TO_SHIP_BEHAVIOR		= 2;
const int ATTACK_BEHAVIOR			= 3;
const int TAKE_DAMAGE_BEHAVIOR		= 4;
const int STUNNED_BEHAVIOR			= 5;
const int DEAD_BEHAVIOR				= 6;
const int NR_OF_ENEMY_BEHAVIORS		= 7;

class IEnemyBehavior
{
	// Class members
	protected:
		Enemy*			mEnemy;
		EnemyState		mBehavior;	
		float			mStateTimer;

	// Class functions
	public:
		virtual HRESULT Update( float deltaTime )		= 0;
		virtual void	OnEnter()						= 0;
		virtual void	OnExit()						= 0;
		virtual void	Reset()							= 0;
		EnemyState		GetBehavior() const;
		virtual HRESULT	Initialize( Enemy* enemy )		= 0;
		virtual void	Release()						= 0;
						IEnemyBehavior();
		virtual		   ~IEnemyBehavior();
};

class IdleBehavior : public IEnemyBehavior
{
	// Class members
	private:

	// Class functions
	public:
		virtual HRESULT	Update( float deltaTime );
		virtual	void	OnEnter();
		virtual void	OnExit();
		virtual void	Reset();
		virtual HRESULT	Initialize( Enemy* enemy );
		virtual void	Release();
						IdleBehavior();
					   ~IdleBehavior();
};

class HuntPlayerBehavior : public IEnemyBehavior
{
	// Class members
	private:

	// Class functions
	public:
		virtual HRESULT	Update( float deltaTime );
		virtual	void	OnEnter();
		virtual void	OnExit();
		virtual void	Reset();
		virtual HRESULT	Initialize( Enemy* enemy );
		virtual void	Release();
						HuntPlayerBehavior();
					   ~HuntPlayerBehavior();
};

class MoveToShipBehavior : public IEnemyBehavior
{
	// Class members
	private:

	// Class functions
	public:
		virtual HRESULT	Update( float deltaTime );
		virtual	void	OnEnter();
		virtual void	OnExit();
		virtual void	Reset();
		virtual HRESULT	Initialize( Enemy* enemy );
		virtual void	Release();
						MoveToShipBehavior();
					   ~MoveToShipBehavior();
};

class AttackBehavior : public IEnemyBehavior
{
	// Class members
	private:
		float			mTimeTillAttack;

	// Class functions
	public:
		virtual HRESULT	Update( float deltaTime );
		virtual	void	OnEnter();
		virtual void	OnExit();
		virtual void	Reset();
		virtual HRESULT	Initialize( Enemy* enemy );
		virtual void	Release();
						AttackBehavior();
					   ~AttackBehavior();
};

class TakeDamageBehavior : public IEnemyBehavior
{
	// Class members
	private:

	// Class functions
	public:
		virtual HRESULT	Update( float deltaTime );
		virtual	void	OnEnter();
		virtual void	OnExit();
		virtual void	Reset();
		virtual HRESULT	Initialize( Enemy* enemy );
		virtual void	Release();
						TakeDamageBehavior();
					   ~TakeDamageBehavior();
};

class StunnedBehavior : public IEnemyBehavior
{
	// Class members
	private:

	// Class functions
	public:
		virtual HRESULT	Update( float deltaTime );
		virtual	void	OnEnter();
		virtual void	OnExit();
		virtual void	Reset();
		virtual HRESULT	Initialize( Enemy* enemy );
		virtual void	Release();
						StunnedBehavior();
					   ~StunnedBehavior();
};

class DeadBehavior : public IEnemyBehavior
{
	// Class members
	private:

	// Class functions
	public:
		virtual HRESULT	Update( float deltaTime );
		virtual	void	OnEnter();
		virtual void	OnExit();
		virtual void	Reset();
		virtual HRESULT	Initialize( Enemy* enemy );
		virtual void	Release();
						DeadBehavior();
					   ~DeadBehavior();
};

#pragma endregion

class Enemy
{
	// Member variables
	private:
#pragma region Friends
		friend class		IEnemyBehavior; 
		friend class		IdleBehavior;
		friend class		HuntPlayerBehavior;
		friend class		MoveToShipBehavior; 
		friend class		AttackBehavior;
		friend class		TakeDamageBehavior;
		friend class		StunnedBehavior;
		friend class		DeadBehavior;
#pragma endregion

		unsigned int		mID;
		EnemyType			mEnemyType;
		EnemyState			mCurrentState;
		float				mCurrentHp;
		float				mMaxHp;
		float				mDamage;
		float				mSpeed;
		bool				mIsAlive;
		XMFLOAT3			mPosition;
		XMFLOAT3			mDirection;
		XMFLOAT3			mVelocity;
		BoundingCircle*		mAttackRadius;
		BoundingCircle*		mAttentionRadius;
		unsigned int		mXpDrop;
		bool				mHasAggro;
		XMFLOAT3			mAggroTarget;
		UINT				mTargetID;
		UINT				mTargetIndex;

		ServerPlayer**		mPlayers;
		UINT				mNrOfPlayers;

		// Timers
		float				mSpawnTime;
		float				mTimeTillSapwn;
		float				mAttackRate;
		float				mTimeTillAttack;
		float				mDeltaTime;
		float				mStateTimer;
		float				mStunTimer;

		// Behaviors
		IEnemyBehavior**	mBehaviors;
		int					mCurrentBehavior;

	protected:
	public:

	// Member functions
	private:
		void				CreateStandard();
		void				CreateRanged();
		void				CreateBoomer();
		void				CreateTank();

		void				StandardLogic( float deltaTime );
		void				RangedLogic( float deltaTime );
		void				BoomerLogic( float deltaTime );
		void				TankLogic( float deltaTime );

	protected:
	public:
		HRESULT				Update( float deltaTime, ServerPlayer** players, UINT NrOfPlayers );
		//void				SetAggro( bool isAggro, XMFLOAT3 target );		
		//bool				IsAggro() const;
		void				ChangeBehavior( const int NEW_BEHAVIOR );
		void				ResetBehavior( const int BEHAVIOR );
		XMFLOAT3			GetTarget() const;

		void				TakeDamage( float damage );
		void				TakeMeleeDamage( float damage, float knockBack, XMFLOAT3 direction, float stun );
		void				AddImpuls( XMFLOAT3 impuls );
		void				SetTarget( UINT id );
		
		void				Hunt( float deltaTime );
		

		void				HandleSpawn( float deltaTime, XMFLOAT3 spawnPos );
		void				Spawn( XMFLOAT3 spawnPos );
		
		void				Die();
		
		unsigned int		GetID() const;
		void				SetID( unsigned int id );
		EnemyType			GetEnemyType() const;
		EnemyState			GetEnemyState() const;
		
		bool				IsAlive() const;
		XMFLOAT3			GetPosition() const;
		XMFLOAT3			GetDirection() const;
		

		HRESULT				Initialize( int id, ServerPlayer** players, UINT NrOfPlayers );
		void				Reset();
		void				Release();
							Enemy();
							~Enemy();

		/////////////////////////////////////////////
		//				CLEAN UP
		/////////////////////////////////////////////

		//float				GetAttackRate() const;
		//void				SetState( EnemyState state );
		//void				SetVelocity( XMFLOAT3 velocity );
		//void				Attack( XMFLOAT3 target, UINT id );
		//float				GetDamage() const;
		//UINT				GetTargetID() const;
		/*BoundingCircle*		GetAttackCircle()	 const;
		BoundingCircle*		GetAttentionCircle() const;*/
		//float				HandleAttack();
		//float				GetHP() const;
		//void				SetHP( float hp );
		//void				SetIsAlive( bool isAlive );
		//void				SetDirection( XMFLOAT3 direction );
		//void				SetPosition( XMFLOAT3 position );
};

#endif