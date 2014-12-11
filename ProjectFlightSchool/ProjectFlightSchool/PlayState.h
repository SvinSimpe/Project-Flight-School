#ifndef PLAYSTATE_H
#define PLAYSTATE_H

#include "BaseState.h"
#include "Player.h"
#include "RemotePlayer.h"

class PlayState : public BaseState
{
	// Class members
	private:

		AssetID			mPlaneAsset;
		AssetID			mCubeAsset;
		AssetID			mTestAsset;

		AssetID		mTestAnimation;
		AssetID		mTestSkeleton;
		AssetID		mTestAnimationAnimation;

		float		mAnimationTime;

	// Debug
		Player*			mPlayer;
		std::vector<RemotePlayer*> mRemotePlayers;

	protected:
	public:

	// Class functions
	private:
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