#ifndef PLAYER_H
#define PLAYER_H

#include "Input.h"
#include "Projectile.h"
#include "RemotePlayer.h"
#include "RenderManager.h"

#define VELOCITY_FALLOFF 2.0f

class Player: public RemotePlayer
{
	private:
		float		mEventCapTimer;

		PointLight*		mPointLight[5];

		float		mWeaponCoolDown;
		float		mMeleeCoolDown;
		float		mTimeTillattack;
		bool		mIsMeleeing;
		bool		mHasMeleeStarted;
		float		mMaxVelocity;
		float		mCurrentVelocity;
		float		mMaxAcceleration;
		XMFLOAT3	mAcceleration;
		bool		mIsBuffed;
		float		mBuffMod; // Modifies the damage a player takes by a percentage, should only range between 0 and 1


	protected:
	public:
		
	private:
		void		HandleInput( float deltaTime );
		void		Move( float deltaTime );

	protected:
	public:
		HRESULT		Update( float deltaTime );
		HRESULT		Render( float deltaTime, int position );

		void		TakeDamage( float damage, unsigned int shooter );
		void		TakeEnemyDamage( float damage );
		void		SetBuffed( bool buffed );
		void		SetID( unsigned int id );
		void		SetTeam( int team, AssetID teamColor );
		void		SetColor( AssetID color );
		XMFLOAT3	GetPlayerPosition() const;
		void		SetPosition( XMVECTOR position );
		void		Fire();
		XMFLOAT3	GetUpperBodyDirection() const;
		bool		GetIsMeleeing()	const;
		void		SetIsMeleeing( bool isMeleeing );
			
		HRESULT		Initialize();
		void		Release();

					Player();
		virtual		~Player();
};
#endif

