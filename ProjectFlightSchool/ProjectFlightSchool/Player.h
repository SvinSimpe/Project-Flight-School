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
		float		mWeaponCoolDown;
		float		mMaxVelocity;
		float		mCurrentVelocity;
		float		mMaxAcceleration;
		XMFLOAT3	mAcceleration;
		XMFLOAT3	mVelocity;

	private:
		void		HandleInput( float deltaTime );
		void		Move( float deltaTime );
		void		Die();
		void		HandleSpawn( float deltaTime );
		void		Spawn();

	public:
		HRESULT		Update( float deltaTime );

		float		WeaponCoolDown();
		XMFLOAT3	GetPlayerPosition() const;

		void		Fire();
		void		SetPosition( XMVECTOR position );

		HRESULT		Initialize();
		void		Release();
					Player();
		virtual		~Player();
};
#endif

