#ifndef STARTMENUSTATE_H
#define STARTMENUSTATE_H

#include "BaseState.h"

class StartMenuState : public BaseState
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
						StartMenuState();
		virtual			~StartMenuState();
};

#endif