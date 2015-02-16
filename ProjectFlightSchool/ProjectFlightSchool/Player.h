#ifndef PLAYER_H
#define PLAYER_H

#include "Projectile.h"
#include "RemotePlayer.h"
#include "Input.h"
#include <stdlib.h>
#include <time.h>

#define VELOCITY_FALLOFF 2.0f
class Map;
class Player: public RemotePlayer
{
	private:
		float			mEventCapTimer;
		PointLight*		mPointLight;

		float		mWeaponCoolDown;
		float		mMeleeCoolDown;
		float		mTimeTillattack;
		bool		mIsMeleeing;
		bool		mHasMeleeStarted;
		bool		mLock;
		bool		mCloseToPlayer;

		float		mMaxVelocity;
		float		mCurrentVelocity;
		float		mMaxAcceleration;
		XMFLOAT3	mAcceleration;
		XMFLOAT3	mFireDirection;
		XMFLOAT3	mPick;
		std::vector<DirectX::XMFLOAT2>::iterator	currStep;
		bool		mFollowPath;

		bool		mIsBuffed;
		float		mBuffMod; // Modifies the damage a player takes by a percentage, should only range between 0 and 1

		std::string		mPlayerName;
		std::vector<DirectX::XMFLOAT2> currentPath;
		
		float		mSpawnTime;
		float		mTimeTillSpawn;
		float		mReviveTime;
		float		mTimeTillRevive;
		int			mLastKiller;

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
		void		Lock();
		void		UnLock();

	protected:
	public:
		HRESULT		UpdateSpecific( float deltaTime, Map* worldMap, std::vector<RemotePlayer*> remotePlayers );
		void		TakeDamage( float damage, unsigned int shooter );
		void		HandleRevive( float deltaTime );
		
		void		Reset();	
		HRESULT		Update( float deltaTime, std::vector<RemotePlayer*> remotePlayers );
		HRESULT		Render( float deltaTime, int position );
		HRESULT		Initialize();
		void		Release();
					Player();
		virtual		~Player();

		//GetSet
		bool		GetIsMeleeing()	const;
		XMFLOAT3	GetPlayerPosition() const;
		XMFLOAT3	GetUpperBodyDirection() const;
		void		SetIsMeleeing( bool isMeleeing );
		void		SetID( unsigned int id );
		void		SetTeam( int team );
		void		SetPosition( XMVECTOR position );

		void		QueueEvent( IEventPtr ptr );
};
#endif

