#ifndef GAME_H
#define GAME_H

#include <Windows.h>
#include "StateMachine.h"

class Game
{
	// Class members
	private:
		StateMachine*		mStateMachine;

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