#ifndef PLAYER_H
#define PLAYER_H

#include "Input.h"
#include "Projectile.h"
#include "RemotePlayer.h"
#include "WeaponInfo.h"

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
		std::vector<Projectile*>	mProjectiles;

		int			mNrOfProjectiles;
		int			mNrOfProjectilesFired;
		float		mWeaponCoolDown;
		LoadOut*	mLoadOut;

	private:
		void		HandleInput( float deltaTime );
		void		Move( XMFLOAT3 direction );
		void		Die();
		void		HandleSpawn( float deltaTime );
		void		Spawn();

	public:
		HRESULT		Update( float deltaTime );
		HRESULT		Render( float deltaTime );

		void		SetPosition( XMVECTOR position );
		float		WeaponCoolDown();
		int			NrOfProjectiles();
		XMFLOAT3	GetPlayerPosition() const;
		XMFLOAT3	Fire();
		XMFLOAT3	GetUpperBodyDirection() const;

		//TEMPORARY
		std::vector<Projectile*>	GetProjectiles();

		HRESULT		Initialize();
		void		Release();
					Player();
		virtual		~Player();
};
#endif

