#ifndef PLAYER_H
#define PLAYER_H

#include "Input.h"
#include "Projectile.h"
#include "RemotePlayer.h"
#include "WeaponInfo.h"
#include "RenderManager.h"

#define VELOCITY_FALLOFF 2.0f

struct LoadOut
{
	RangedInfo*	rangedWeapon;
	MeleeInfo*	meleeWeapon;

	LoadOut()
	{
		rangedWeapon	= nullptr;
		meleeWeapon		= nullptr;
	}

	void Release()
	{
		SAFE_DELETE( rangedWeapon );
		SAFE_DELETE( meleeWeapon );
	}
};

class Player: public RemotePlayer
{
	private:
		float		mWeaponCoolDown;
		float		mMaxVelocity;
		float		mCurrentVelocity;
		float		mMaxAcceleration;
		XMFLOAT3	mAcceleration;
		XMFLOAT3	mVelocity;

		LoadOut*	mLoadOut;
		
	private:
		void		HandleInput( float deltaTime );
		void		Move( float deltaTime );

	public:
		HRESULT		Update( float deltaTime );
		HRESULT		Render( float deltaTime, int position );

		void		SetID( unsigned int id );
		void		SetTeam( int team, AssetID teamColor );
		void		SetColor( AssetID color );
		XMFLOAT3	GetPlayerPosition() const;
		void		SetPosition( XMVECTOR position );
		void		Fire();
		XMFLOAT3	GetUpperBodyDirection() const;
			
		HRESULT		Initialize();
		void		Release();

					Player();
		virtual		~Player();
};
#endif

