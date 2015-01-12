#ifndef PLAYSTATE_H
#define PLAYSTATE_H

#include "Map.h"
#include "BaseState.h"
#include "Player.h"
#include "RemotePlayer.h"

#define MAX_REMOTE_PLAYERS	10
#define MAX_PROJECTILES		30

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
		Projectile**				mProjectiles;				// A collection of the games projectiles
		int							mNrOfProjectilesFired;

	protected:
	public:

	// Class functions
	private:
		void			EventListener( IEventPtr newEvent );
		void			BroadcastDamage();						// Tell server that local  player has taken damage

		void			FireProjectile( XMFLOAT3 position, XMFLOAT3 direction );
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