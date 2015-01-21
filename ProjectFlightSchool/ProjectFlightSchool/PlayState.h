#ifndef PLAYSTATE_H
#define PLAYSTATE_H

#include "Map.h"
#include "BaseState.h"
#include "Player.h"
#include "RemotePlayer.h"
#include "Font.h"
#include "Turret.h"

#define MAX_REMOTE_PLAYERS		14 //There is only 14 colorIDs.
#define COLLISION_CHECK_OFFSET	1	// 0 == Every frame
#define animTestNr 10
#define MAX_PROJECTILES			1000

class PlayState : public BaseState
{
	// Class members
	private:
		AssetID mPlaneAsset;

		AssetID	mTestAsset;

		AssetID	mTestAnimation[animTestNr];
		AssetID	mTestAnimationAnimation[animTestNr];

		AssetID mNest1Asset;
		AssetID mStoneAssets[6];
		AssetID mTree1Asset;
		AssetID mTest2dAsset;
		AssetID mTeams[2];
		AssetID	mColorIDs[MAX_REMOTE_PLAYERS];

		Map*		mWorldMap;

		float	mAnimationTime;

		//Collision
		unsigned int	mFrameCounter;

		//TEST
		std::vector<RemotePlayer*> mAllPlayers;

		Turret						mTurret;
	
		//Game Data
		Player*						mPlayer;
		std::vector<RemotePlayer*>	mRemotePlayers;
		Projectile**				mProjectiles;				// A collection of the games projectiles
		int							mNrOfProjectilesFired;
		int							mCurrentColor;
		Font						mFont;
	
	protected:
	public:

	// Class functions
	private:
		void			RemoteUpdate( IEventPtr newEvent );
		void			HandleDeveloperCameraInput();
		void			CheckPlayerCollision();
		void			CheckProjectileCollision();
		void			CheckMeeleCollision();
		void			CheckTurretTarget();
		void			EventListener( IEventPtr newEvent );
		void			BroadcastDamage( unsigned int playerID, unsigned int projectileID );						// Tell server that local  player has taken damage
		void			BroadcastMeleeDamage( unsigned playerID, float damage, float knockBack, XMFLOAT3 direction );

		void			FireProjectile( unsigned int id, unsigned int projectileID, XMFLOAT3 position, XMFLOAT3 direction );
		void			UpdateProjectiles( float deltaTime );
		void			RenderProjectiles();
		void			HandleRemoteProjectileHit( unsigned int id, unsigned int projectileID );
		void			HandleRemoteMeleeHit( unsigned int id, float damage, float knockBack, XMFLOAT3 direction );

	protected:
	public:
		

		virtual HRESULT Update( float deltaTime );
		virtual HRESULT Render();
		virtual void	OnEnter();
		virtual void	OnExit();
		virtual void	Reset();

		virtual HRESULT Initialize();
		virtual void	Release();
						PlayState();
		virtual			~PlayState();
};

#endif