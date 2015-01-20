#ifndef JOINMENUSTATE_H
#define JOINMENUSTATE_H

#include "BaseMenuState.h"
#include "InputSquare.h"

class JoinMenuState : public BaseMenuState
{
	private:
		MenuButton		mBackButton;
		InputSquare		mIPBox;
		InputSquare		mPortBox;
		Image			mText;
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