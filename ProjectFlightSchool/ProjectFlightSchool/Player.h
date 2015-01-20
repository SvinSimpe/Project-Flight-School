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
		float		mMeleeCoolDown;
		LoadOut*	mLoadOut;
		bool		mIsMeleeing;

		
	private:
		void		HandleInput( float deltaTime );
		void		Move( float deltaTime );

	public:
		HRESULT		Update( float deltaTime );
		HRESULT		Render( float deltaTime, int position );

		LoadOut*	GetLoadOut() const;
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

