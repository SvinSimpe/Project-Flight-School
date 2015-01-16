#ifndef STARTMENUSTATE_H
#define STARTMENUSTATE_H

#include "BaseMenuState.h"
#include "EventManager.h"
#include "Events.h"
#include "MenuButton.h"

class StartMenuState : public BaseMenuState
{
	// Class members
	private:
		const int
			CREATE	= 0,
			JOIN	= 1,
			OPTIONS	= 2,
			EXIT	= 3;

		MenuButton		mButtons[4];
		Image			mTexts[4];

	protected:
	public:

	// Class functions
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
				StartMenuState();
		virtual	~StartMenuState();
};

#endif