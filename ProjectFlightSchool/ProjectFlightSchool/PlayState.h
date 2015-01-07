#ifndef PLAYSTATE_H
#define PLAYSTATE_H

#include "Map.h"
#include "BaseState.h"
#include "Player.h"
#include "RemotePlayer.h"
#include "MapNodeManager.h"

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

		Map*		mWorldMap;
		MapNodeManager* mMapNodeMan;

		float	mAnimationTime;

	// Debug
		Player*			mPlayer;
		std::vector<RemotePlayer*> mRemotePlayers;

		
	
	protected:
	public:

	// Class functions
	private:
		void			RemoteUpdate( IEventPtr newEvent );

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