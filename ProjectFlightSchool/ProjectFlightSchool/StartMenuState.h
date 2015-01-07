#ifndef STARTMENUSTATE_H
#define STARTMENUSTATE_H

#include "BaseState.h"
#include "Input.h"
#include "EventManager.h"
#include "Events.h"

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
		virtual void	OnEnter();
		virtual void	OnExit();
		virtual void	Reset();

		virtual HRESULT Initialize();
		virtual void	Release();
						StartMenuState();
		virtual			~StartMenuState();
};

#endif