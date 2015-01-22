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
		float		mMeleeCoolDown;
		bool		mIsMeleeing;

		
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
		bool		GetIsMeleeing()	const;
		void		SetIsMeleeing( bool isMeleeing );
			
		HRESULT		Initialize();
		void		Release();

					Player();
		virtual		~Player();
};
#endif

