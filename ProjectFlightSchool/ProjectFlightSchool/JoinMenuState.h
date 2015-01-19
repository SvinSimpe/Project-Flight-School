#ifndef JOINMENUSTATE_H
#define JOINMENUSTATE_H

#include "BaseMenuState.h"
#include "InputSquare.h"

class JoinMenuState : public BaseMenuState
{
	private:
		static const int
			JOIN			= 0,
			BACK			= 1,
			BUTTON_AMOUNT	= 2;

		MenuButton		mButtons[BUTTON_AMOUNT];
		InputSquare		mIPBox;
		InputSquare		mPortBox;
		Image			mText[BUTTON_AMOUNT];
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
		JoinMenuState();
		~JoinMenuState();
};
#endif