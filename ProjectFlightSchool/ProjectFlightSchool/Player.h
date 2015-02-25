#ifndef PLAYER_H
#define PLAYER_H

#include "Projectile.h"
#include "RemotePlayer.h"
#include "EnergyCell.h"
#include "Input.h"
#include <stdlib.h>
#include <time.h>
#include "Pathfinder.h"

#define VELOCITY_FALLOFF 2.0f

#define MAX_ROBOT_RANGE		40000.0f   //Squared distance here.
#define LEAVING_AREA_TIME	10.0f

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
		float			mEventCapTimer;
		PointLight*		mPointLight;
		PointLight*		mEnergyCellLight;
		Upgrades		mUpgrades;

		float		mWeaponCoolDown;
		float		mMeleeCoolDown;
		float		mTimeTillattack;
		bool		mIsMeleeing;
		bool		mHasMeleeStarted;
		bool		mLock;
		bool		mCloseToPlayer;
		int			mXP;
		int			mNextLevelXP;
		int			mCurrentUpgrades;

		float		mMaxVelocity;
		float		mCurrentVelocity;
		float		mMaxAcceleration;
		XMFLOAT3	mAcceleration;
		XMFLOAT3	mFireDirection;
		XMFLOAT3	mPick;
		std::vector<DirectX::XMFLOAT2>::iterator	currStep;
		bool		mFollowPath;

		bool		mIsBuffed;
		bool		mIsOutSideZone;
		float		mBuffMod; // Modifies the damage a player takes by a percentage, should only range between 0 and 1

		Path*		currentPath1;
		std::vector<DirectX::XMFLOAT2> currentPath;
		
		float		mSpawnTime;
		float		mTimeTillSpawn;
		float		mReviveTime;
		float		mTimeTillRevive;
		float		mLeavingAreaTime;
		int			mLastKiller;

		UINT		mEnergyCellID;
		float		mPickUpCooldown;

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
		void		FireShotgun( XMFLOAT3* spawnPoint );
		void		AddImpuls( XMFLOAT3 impuls );
		void		QueueEvent( IEvent* ptr );
		void		UpgradeBody();
		void		UpgradeLegs();
		void		UpgradeMelee();
		void		UpgradeRange();

	protected:
	public:
		void		AddXP( int XP );
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

		void		QueueEvent( IEventPtr ptr );
};
#endif

