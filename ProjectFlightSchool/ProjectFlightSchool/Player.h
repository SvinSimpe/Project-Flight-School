#ifndef PLAYER_H
#define PLAYER_H

#include "Input.h"
#include "Projectile.h"
#include "RemotePlayer.h"

class Player: public RemotePlayer
{
	private:
		float		mWeaponCoolDown;

	private:
		void		HandleInput( float deltaTime );
		void		Move( XMFLOAT3 direction );
		void		Die();
		void		HandleSpawn( float deltaTime );
		void		Spawn();

	public:
		HRESULT		Update( float deltaTime );
		HRESULT		Render( float deltaTime );


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

