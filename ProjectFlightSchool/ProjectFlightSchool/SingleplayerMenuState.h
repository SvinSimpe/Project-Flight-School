#ifndef SINGLEPLAYERMENUSTATE_H
#define SINGLEPLAYERMENUSTATE_H

#include "BaseMenuState.h"
#include "Button.h"
#include "Image.h"

class SingleplayerMenuState : public BaseMenuState
{
	private:
		static const int
			EASY			= 0,
			MEDIUM			= 1,
			HARD			= 2,
			BACK			= 3,
			BUTTON_AMOUNT	= 4;

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
				SingleplayerMenuState();
		virtual ~SingleplayerMenuState();
};
#endif