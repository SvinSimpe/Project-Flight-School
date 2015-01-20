#ifndef PLAYER_H
#define PLAYER_H

#include "Input.h"
#include "Projectile.h"
#include "RemotePlayer.h"
#include "WeaponInfo.h"
#include "RenderManager.h"

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
		LoadOut*	mLoadOut;
		bool		mIsMeleeing;

	private:
		void		HandleInput( float deltaTime );
		void		Move( XMFLOAT3 direction );
		void		Die();
		void		HandleSpawn( float deltaTime );
		void		Spawn();

	public:
		HRESULT		Update( float deltaTime );
		HRESULT		Render( float deltaTime );

		LoadOut*	GetLoadOut() const;
		void		SetID( unsigned int id );
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

