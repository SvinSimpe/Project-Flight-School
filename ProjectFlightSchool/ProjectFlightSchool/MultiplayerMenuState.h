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

		InputSquare	mPortBox;
		InputSquare	mNameBox;
		std::string mPort;

	protected:
	public:

	private:
		void	HandleInput();

	protected:
	public:
		HRESULT Update( float deltaTime );
		HRESULT Render( float deltaTime );
		void	OnEnter();
		void	OnExit();
		void	Reset();
		HRESULT Initialize();
		void	Release();
				MultiplayerMenuState();
		virtual	~MultiplayerMenuState();
};
#endif