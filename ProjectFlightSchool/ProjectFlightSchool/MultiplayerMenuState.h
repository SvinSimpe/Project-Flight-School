#ifndef MULTIPLAYERMENUSTATE_H
#define MULTIPLAYERMENUSTATE_H

#include "BaseMenuState.h"
#include "InputSquare.h"

class MultiplayerMenuState : public BaseMenuState
{
	private:
		static const int
			TWO_VS_TWO = 0,
			THREE_VS_THREE = 1,
			FOUR_VS_FOUR = 2,
			BACK = 3,
			BUTTON_AMOUNT = 4;

		MenuButton		mButtons[BUTTON_AMOUNT];
		Image			mTexts[BUTTON_AMOUNT];
		InputSquare		mPortBox;

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
		MultiplayerMenuState();
		~MultiplayerMenuState();
};
#endif