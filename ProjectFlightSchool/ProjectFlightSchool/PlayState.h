#ifndef PLAYSTATE_H
#define PLAYSTATE_H

#include "Map.h"
#include "BaseState.h"
#include "Player.h"
#include "RemotePlayer.h"

#define MAX_REMOTE_PLAYERS 10

class PlayState : public BaseState
{
	// Class members
	private:
		AssetID mPlaneAsset;

		AssetID	mTestAsset;

		AssetID	mTestAnimation;
		AssetID	mTestSkeleton;
		AssetID	mTestAnimationAnimation;

		AssetID mNest1Asset;
		AssetID mStoneAssets[6];
		AssetID mTree1Asset;
		AssetID mTest2dAsset;

		Map*		mWorldMap;

		float	mAnimationTime;

	// Debug
		Player*						mPlayer;
		std::vector<RemotePlayer*>	mRemotePlayers;

	// Game Data
		std::vector<Projectile*>	mPlayerProjectiles;			// Fired from local player
		std::vector<Projectile*>	mRemoteProjectiles;			// Information on projectiles fired from remote player
		int							mNrOfPlayerProjectiles;
		int							mNrOfRemoteProjectiles;
		
		Projectile*					mProjectiles;				// A collection

		int							mNrOfPlayerProjectilesFired;
		int							mNrOfRemoteProjectilesFired;

	protected:
	public:

	// Class functions
	private:
		void			EventListener( IEventPtr newEvent );
		void			BroadcastDamage();						// Tell server that local  player has taken damage

		void			UpdateProjectiles( float deltaTime );
		void			RenderProjectiles();

	protected:
	public:
		void HandleDeveloperCameraInput();

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