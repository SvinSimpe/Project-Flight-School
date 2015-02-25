#ifndef ENEMY_H
#define ENEMY_H

#include "Events.h"
#include "Graphics.h"
#include "BoundingGeometry.h"
#include "RenderManager.h"
#include "Font.h"
#include "RemotePlayer.h"
#include "SteeringBehaviorManager.h"
#include <math.h>

class Enemy;

#define randflt() (((float) rand())/((float) RAND_MAX))

#define MAX_NR_OF_ENEMIES		15

// ---- Define all enemy animations ----
// Standard
#define ENEMY_ANIMATION_STANDARD_IDLE			0
#define ENEMY_ANIMATION_STANDARD_RUN			1
#define ENEMY_ANIMATION_STANDARD_RUN_DAMAGED	2
#define ENEMY_ANIMATION_STANDARD_ATTACK			3
#define ENEMY_ANIMATION_STANDARD_DEATH			4
// Ranged
#define ENEMY_ANIMATION_RANGED_IDLE				5
#define ENEMY_ANIMATION_RANGED_RUN				6
#define ENEMY_ANIMATION_RANGED_RUN_DAMAGED		7
#define ENEMY_ANIMATION_RANGED_ATTACK			8
#define ENEMY_ANIMATION_RANGED_DEATH			9
// Boomer
#define ENEMY_ANIMATION_BOOMER_IDLE				10
#define ENEMY_ANIMATION_BOOMER_RUN				11
#define ENEMY_ANIMATION_BOOMER_RUN_DAMAGED		12
#define ENEMY_ANIMATION_BOOMER_ATTACK			13
#define ENEMY_ANIMATION_BOOMER_DEATH			14
// Tank
#define ENEMY_ANIMATION_TANK_IDLE				15
#define ENEMY_ANIMATION_TANK_RUN				16
#define ENEMY_ANIMATION_TANK_ATTACK				17
#define ENEMY_ANIMATION_TANK_DEATH				18

#define ENEMY_ANIMATION_COUNT					19
//----------------------------------------

enum EnemyType { Standard, Ranged, Boomer, Tank };
enum EnemyState { Idle, HuntPlayer, MoveToShip, TakeDamage, Attack, Death, Stunned, };

///////////////////////////////////////////////////////////////////////////////
//								Behaviors
///////////////////////////////////////////////////////////////////////////////
#pragma region Behaviors

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

		//SteeringBehaviorManager**	mSteeringBehaviors;

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
		bool			mHasAttacked;

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

///////////////////////////////////////////////////////////////////////////////
//							Steering Behaviors
///////////////////////////////////////////////////////////////////////////////
#pragma region Steering Behaviors

class SteeringBehavior
{
	// Class members
	private:
	public:
		Enemy*		mEnemy;
		float		mWeight;
		float		mProbability;
		bool		mDisable;

	// Class functions
	public:
		virtual bool	Update( float deltaTime, XMFLOAT3& totalForce );
		virtual void	SteerTowards( XMFLOAT3& target, XMFLOAT3& result );
		virtual void	SteerAway( XMFLOAT3& target, XMFLOAT3& result );
		
		virtual void	Reset();
		virtual void	Release();
						SteeringBehavior( Enemy* enemy );
		virtual		   ~SteeringBehavior();

};

class SteerApproach : public SteeringBehavior
{
	public:
		virtual bool	Update( float deltaTime, XMFLOAT3& totalForce );
						SteerApproach( Enemy* enemy );
		virtual		   ~SteerApproach();
};

class SteerPursuit : public SteeringBehavior
{
	private:
		bool			FindTarget();

	public:
		virtual bool	Update( float deltaTime, XMFLOAT3& totalForce );
						SteerPursuit( Enemy* enemy );
		virtual		   ~SteerPursuit();
};

class SteerEvade : public SteeringBehavior
{
	public:
		virtual bool	Update( float deltaTime, XMFLOAT3& totalForce );
						SteerEvade( Enemy* enemy );
		virtual		   ~SteerEvade();
};

class SteerWander : public SteeringBehavior
{
	private:
		XMFLOAT3		mCirclePosition;
		XMFLOAT3		mTargetPosition;
		float			mThetaValue;		//theta represents "where" we are on the circle
		float			mWanderCircleRadius;
		float			mWanderCircleDistance;
	public:
		virtual bool	Update( float deltaTime, XMFLOAT3& totalForce );
						SteerWander( Enemy* enemy );
		virtual		   ~SteerWander();
};

#pragma endregion

///////////////////////////////////////////////////////////////////////////////
//						Steering Behaviors Manager
///////////////////////////////////////////////////////////////////////////////
#pragma region Steering Behavior Manager
class SteeringBehaviorManager
{
	// Class members
	protected:
		std::vector<SteeringBehavior*>		mBehaviors;
		std::vector<SteeringBehavior*>		mActive;
		std::vector<float>					mActiveForce;
		int									mNumberOfBehaviors;
		Enemy*								mEnemy;
		XMFLOAT3							mTotalSteeringForce;
		float								mMaxSteeringForce;

	// Class functions
	public:
		virtual HRESULT		Update( float deltaTime );
		virtual void		AddBehavior( SteeringBehavior* behavior );
		virtual void		DisableBehavior( int index );
		virtual void		SetUpBehavior( int behaviorIndex, float weight, float probability, bool disable = false );
		virtual bool		CombinedForceWeighted( XMFLOAT3& steeringForce, float weight );
		virtual bool		CombineForcePrioritySum( XMFLOAT3& steeringForce );
		virtual XMFLOAT3	GetFinalSteeringForce() const;

		virtual HRESULT		Initialize( Enemy* enemy = nullptr );
		virtual void		Release();
		virtual void		Reset();
							SteeringBehaviorManager();
						   ~SteeringBehaviorManager();

};

#pragma endregion

///////////////////////////////////////////////////////////////////////////////
//									Enemy
///////////////////////////////////////////////////////////////////////////////
class Enemy
{
	// Member variables
	private:
#pragma region Friends
		// Behaviors
		friend class		IEnemyBehavior; 
		friend class		IdleBehavior;
		friend class		HuntPlayerBehavior;
		friend class		MoveToShipBehavior; 
		friend class		AttackBehavior;
		friend class		TakeDamageBehavior;
		friend class		StunnedBehavior;
		friend class		DeadBehavior;
		// Steering Behaviors
		friend class		SteeringBehavior;
		friend class		SteerApproach;
		friend class		SteerPursuit;
		friend class		SteerEvade;
		friend class		SteerWander;
		friend class		SteeringBehaviorManager;

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
		BoundingCircle*		mEvadeRadius;
		unsigned int		mXpDrop;
		UINT				mTargetID;
		UINT				mTargetIndex;
		bool				mTakingDamage;

		ServerPlayer**		mPlayers;
		UINT				mNrOfPlayers;
		Enemy**				mOtherEnemies;

		// Timers
		float				mSpawnTime;
		float				mTimeTillSapwn;
		float				mAttackRate;
		float				mTimeTillAttack;
		float				mDeltaTime;
		float				mStateTimer;
		float				mStunTimer;
		float				mTakingDamageTimer;

		// Behaviors
		IEnemyBehavior**			mBehaviors;
		int							mCurrentBehavior;

		SteeringBehaviorManager*	mSteeringBehaviorManager;

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
		void				ChangeBehavior( const int NEW_BEHAVIOR );
		void				ResetBehavior( const int BEHAVIOR );
		void				TakeDamage( float damage, UINT killer );
		void				TakeMeleeDamage( float damage, float knockBack, XMFLOAT3 direction, float stun, UINT killer );

		void				AddImpuls( XMFLOAT3 impuls );
		void				SetTarget( UINT id );
		void				Hunt( float deltaTime );
		void				HandleSpawn( float deltaTime, XMFLOAT3 spawnPos );
		void				Spawn( XMFLOAT3 spawnPos );
		BoundingCircle*		GetAttackCircle()	 const;
		BoundingCircle*		GetAttentionCircle() const;
		void				Die( UINT killer );
		float				HandleAttack();

		unsigned int		GetID() const;
		void				SetID( unsigned int id );
		EnemyType			GetEnemyType() const;
		EnemyState			GetEnemyState() const;
		bool				IsAlive() const;
		XMFLOAT3			GetPosition() const;
		XMFLOAT3			GetDirection() const;
		float				GetHP() const;
		float				GetSpeed() const;
		XMFLOAT3			GetVelocity() const;

		HRESULT				Initialize( int id, ServerPlayer** players, UINT NrOfPlayers, Enemy** otherEnemies );
		void				Reset();
		void				Release();
							Enemy();
							~Enemy();
};

#endif