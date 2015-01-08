#ifndef MENUSTATE_H
#define MENUSTATE_H

#include "BaseState.h"

class MenuState : public BaseState
{
	// Member variables
	private:
	protected:
	public:

	// Member functions
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
						MenuState();
		virtual			~MenuState();
};
#endif