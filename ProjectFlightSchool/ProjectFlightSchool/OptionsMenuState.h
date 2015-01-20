#ifndef OPTIONSMENUSTATE_H
#define OPTIONSMENUSTATE_H

#include "BaseMenuState.h"

class OptionsMenuState : public BaseMenuState
{
	private:
		static const int
			FULLSCREEN		= 0,
			SOUND			= 1,
			BACK			= 2,
			BUTTON_AMOUNT	= 3;

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
		virtual ~OptionsMenuState();
};
#endif