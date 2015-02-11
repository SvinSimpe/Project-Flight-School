#ifndef GAME_H
#define GAME_H

#include "Server.h"
#include "StateMachine.h"
#include "RenderManager.h"

class Game
{
	// Class members
	private:
		StateMachine*		mStateMachine;
		Server*				mServer;

	protected:
	public:

		// Class functions
	private:
		void	ResetGame( IEventPtr eventPtr );
		void	StartPlayState( IEventPtr eventPtr );
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