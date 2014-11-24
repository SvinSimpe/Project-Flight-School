#ifndef PLAYSTATE_H
#define PLAYSTATE_H

#include "BaseState.h"

class PlayState : public BaseState
{
	// Class members
	private:
	protected:
	public:

	// Class functions
	private:
	protected:
	public:
		virtual HRESULT Update( float deltaTime );
		virtual HRESULT Render();
		virtual HRESULT Initialize();
		virtual void	OnEnter();
		virtual void	OnExit();
		virtual void	Reset();
		virtual void	Release();
						PlayState();
		virtual			~PlayState();
};

#endif