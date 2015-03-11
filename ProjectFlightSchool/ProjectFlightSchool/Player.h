#ifndef PLAYER_H
#define PLAYER_H

#include "Projectile.h"
#include "RemotePlayer.h"
#include "EnergyCell.h"
#include "Input.h"
#include <stdlib.h>
#include <time.h>
#include "Pathfinder.h"
#include "RenderManager.h"
#include "SoundBufferHandler.h"

#define VELOCITY_FALLOFF 2.0f

#define MAX_ROBOT_RANGE		40000.0f   //Squared distance here.
#define LEAVING_AREA_TIME	10.0f
#define WATER_DAMAGE_TIME	0.1f
#define WATER_DAMAGE		1.0f

class Map;
class Path;

struct Upgrades
{
	int melee				= 1;
	int range				= 1;
	int legs				= 1;
	int body				= 1;
};

class Player: public RemotePlayer
{
	private:
		PointLight*		mPointLight;
		PointLight*		mEnergyCellLight;
		Upgrades		mUpgrades;

		bool		mWeaponOverheated;
		float		mTimeSinceLastShot;
		float		mWeaponCoolDown;
		float		mMeleeCoolDown;
		float		mTimeTillattack;
		bool		mIsMeleeing;
		bool		mHasMeleeStarted;
		bool		mLock;
		bool		mCloseToPlayer;
		float		mXP;
		float		mNextLevelXP;
		int			mCurrentLevel;
		int			mCurrentUpgrades;

		float		mMaxVelocity;
		float		mCurrentVelocity;
		float		mMaxAcceleration;
		float		mSlowDown;
		XMFLOAT3	mAcceleration;
		XMFLOAT3	mFireDirection;
		XMFLOAT3	mPick;
		std::vector<DirectX::XMFLOAT2>::iterator	currStep;
		bool		mFollowPath;

		bool		mIsOutSideZone;
		bool		mIsInWater;

		Path*		currentPath1;
		std::vector<DirectX::XMFLOAT2> currentPath;
		
		float		mReviveTime;
		float		mTimeTillRevive;
		float		mLeavingAreaTime;
		float		mWaterDamageTime;
		int			mLastKiller;

		UINT		mEnergyCellID;
		float		mPickUpCooldown;

		int			mMiniGunOverheat;
		int			mHammerSound;
		int			mSword;
		int			mPlayerDeath;

	protected:
	public:
		std::list<IEventPtr> gEventList;
		
	private:
		void		EventListener( IEventPtr newEvent );
		void		HandleInput( float deltaTime, std::vector<RemotePlayer*> remotePlayers );
		void		HandleSpawn( float deltaTime );
		void		HandleDeath( float deltaTime );
		void		Move( float deltaTime );
		void		GoDown( int shooter );
		void		GoUp();
		void		ReviveRemotePlayer( int remotePlayerID, float deltaTime );
		void		BroadcastDeath( unsigned int shooter );
		void		Revive();
		void		Die();

		void		Fire();
		void		Melee( float deltaTime );

		void		FireShotgun( XMFLOAT3* spawnPoint );
		void		FireMinigun( XMFLOAT3* projectileOffset );
		void		FireGrenadeLauncher( XMFLOAT3* projectileOffset );
		void		FireSniper( XMFLOAT3* projectileOffset );

		void		HammerMelee( float deltaTime );
		void		BlowtorchMelee( float deltaTime );
		void		ClaymoreMelee( float deltaTime );
		void		SawMelee( float deltaTime );

		float		CalculateLaunchAngle();
		void		AddImpuls( XMFLOAT3 impuls );
		void		QueueEvent( IEvent* ptr );
		void		UpgradeBody();
		void		UpgradeLegs();
		void		UpgradeMelee();
		void		UpgradeRange();
		void		WriteInteractionText( std::string text );

	protected:
	public:
		void		AddXP( float XP );
		void		PickUpEnergyCell( EnergyCell** energyCell );
		void		DropEnergyCell( EnergyCell** energyCells );
		void		GiveEnergyCellToShip( EnergyCell** energyCells, UINT shipID, DirectX::XMFLOAT3 shipPos );
		HRESULT		UpdateSpecific( float deltaTime, Map* worldMap, std::vector<RemotePlayer*> remotePlayers, EnergyCell** energyCells );
		void		TakeDamage( float damage, unsigned int shooter );
		void		HandleRevive( float deltaTime );
		void		Lock();
		void		UnLock();
		
		void		Reset();	
		HRESULT		Update( float deltaTime, std::vector<RemotePlayer*> remotePlayers, EnergyCell** energyCells );
		HRESULT		Render( float deltaTime, int position );
		HRESULT		Initialize();
		void		Release();
					Player();
		virtual		~Player();

		//GetSet
		bool		GetIsMeleeing()	const;
		XMFLOAT3	GetPlayerPosition() const;
		XMFLOAT3	GetUpperBodyDirection() const;
		UINT		GetEnergyCellID() const;
		float		GetXPToNext() const;
		int			Upgradable() const;
		void		SetIsMeleeing( bool isMeleeing );
		void		SetID( unsigned int id );
		void		SetTeam( int team );
		void		SetPosition( XMVECTOR position );
		void		SetEnergyCellID( UINT energyCellID );
		int			GetCurrentLevel() const;

		void		QueueEvent( IEventPtr ptr );
};
#endif

