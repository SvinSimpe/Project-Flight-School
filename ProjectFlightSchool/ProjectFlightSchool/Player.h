#ifndef PLAYER_H
#define PLAYER_H

#include "Input.h"
#include "Projectile.h"
#include "RemotePlayer.h"

class Player: public RemotePlayer
{
	private:
		std::vector<Projectile*>	mProjectiles;
		int							mNrOfProjectilesFired;
		float						mWeaponCoolDown;

	private:
		void		HandleInput( float deltaTime );
		void		Move( XMFLOAT3 direction );

	public:
		HRESULT		Update( float deltaTime );
		HRESULT		Render( float deltaTime );

		void		Fire();
		void		SetPosition( XMVECTOR position );

		HRESULT		Initialize();
		void		Release();
					Player();
					~Player();
};
#endif

