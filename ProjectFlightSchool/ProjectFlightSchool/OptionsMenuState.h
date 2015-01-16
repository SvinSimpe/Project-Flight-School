#ifndef OPTIONSMENUSTATE_H
#define OPTIONSMENUSTATE_H

#include "BaseMenuState.h"

class OptionsMenuState : public BaseMenuState
{
	private:
	protected:
	public:

	private:
		void	HandleInput();
	protected:
	public:
		HRESULT Update( float deltaTime );
		HRESULT Render();
		void	OnEnter();
		void	OnExit();
		void	Reset();
		HRESULT Initialize();
		void	Release();
		OptionsMenuState();
		~OptionsMenuState();
};
#endif