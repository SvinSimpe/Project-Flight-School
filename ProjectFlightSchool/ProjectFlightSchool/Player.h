#ifndef PLAYER_H
#define PLAYER_H

#include "Projectile.h"
#include "RemotePlayer.h"

#define VELOCITY_FALLOFF 2.0f

class Player: public RemotePlayer
{
	private:
		float			mEventCapTimer;

		PointLight*	mPointLight[5];
		float		mMaxVelocity;
		float		mCurrentVelocity;
		float		mMaxAcceleration;
		XMFLOAT3	mAcceleration;
		
		bool		mIsBuffed;
		bool		mIsMeleeing;
		bool		mHasName;
		std::string	mPlayerName;
		float		mWeaponCoolDown;
		float		mMeleeCoolDown;
		float		mBuffMod; // Modifies the damage a player takes by a percentage, should only range between 0 and 1
		float		mSpawnTime;
		float		mTimeTillSpawn;
		float		mDeathTime;
		float		mTimeTillDeath;
		float		mReviveTime;
		float		mTimeTillRevive;
		int			mLastKiller;

	protected:
	public:
		
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
		void		AddImpuls( XMFLOAT3 impuls );

	protected:
	public:
		void		TakeDamage( float damage, unsigned int shooter );
		void		HandleRevive( float deltaTime );
			
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
		void		SetBuffed( bool buffed );
		void		SetID( unsigned int id );
		void		SetTeam( int team, AssetID teamColor );
		void		SetColor( AssetID color );
		void		SetPosition( XMVECTOR position );
};
#endif

