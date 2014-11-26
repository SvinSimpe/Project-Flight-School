#ifndef PLAYSTATE_H
#define PLAYSTATE_H

#include "BaseState.h"

class PlayState : public BaseState
{
	// Class members
	private:

		UINT		mPlaneAsset;
		UINT		mCubeAsset;
		UINT		mTestAsset;

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