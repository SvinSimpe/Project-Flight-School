#ifndef ENEMY_H
#define ENEMY_H

#include "Graphics.h"
#include "EventManager.h"
#include "Events.h"
#include "BoundingGeometry.h"
#include "RenderManager.h"
#include "Font.h"

#define MAX_NR_OF_ENEMIES		0

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
enum EnemyState { Idle, Run, Attack, Death, MoveToShip, HuntPlayer };

class Enemy
{
	// Member variables
	private:
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
		HRESULT				Update( float deltaTime );
		void				SetState( EnemyState state );
		void				SetHuntedPlayer( XMFLOAT3 player );
		void				Spawn( XMFLOAT3 spawnPos );
		BoundingCircle*		GetAttackCircle()	 const;
		BoundingCircle*		GetAttentionCircle() const;
		void				Die();
		unsigned int		GetID() const;
		void				SetID( unsigned int id );
		EnemyType			GetEnemyType() const;
		EnemyState			GetEnemyState() const;
		float				GetHP() const;
		void				SetHP( float hp );
		bool				IsAlive() const;
		void				SetIsAlive( bool isAlive );
		XMFLOAT3			GetPosition() const;
		void				SetPosition( XMFLOAT3 position );
		XMFLOAT3			GetDirection() const;
		void				SetDirection( XMFLOAT3 direction );
		HRESULT				Initialize( int id );
		void				Release();
							Enemy();
							~Enemy();
};

#endif