#ifndef GAME_H
#define GAME_H

#include <Windows.h>
#include "StateMachine.h"
#include "EventManager.h"
#include "Events.h"

class Game
{
	// Class members
	private:
		StateMachine*		mStateMachine;
		EventManager*		mEventManager;

	protected:
	public:

		// Class functions
	private:
	protected:
	public:
		HRESULT Update( float deltaTime );
		HRESULT Render();

		HRESULT Initialize();
		void	Release();
				Game();
				~Game();
};

#endif