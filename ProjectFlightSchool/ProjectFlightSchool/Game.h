#ifndef GAME_H
#define GAME_H

#include "Client.h"
#include "Server.h"
#include "StateMachine.h"
#include <iostream>
#include "RenderManager.h"

class Game
{
	// Class members
	private:
		StateMachine*		mStateMachine;

	protected:
	public:

		// Class functions
	private:
		void EventListener( IEventPtr newEvent );

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