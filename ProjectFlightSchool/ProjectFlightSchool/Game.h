#ifndef GAME_H
#define GAME_H

#include "Client.h" // Includes Eventmanager.h and Events.h
#include "Server.h"
#include "StateMachine.h"
#include <iostream>

class Game
{
	// Class members
	private:
		StateMachine*		mStateMachine;
		EventManager*		mEventManager;

		std::thread			mNetworkThread;
		std::thread			mServerThread;
		Client*				mClient;

	protected:
	public:

		// Class functions
	private:
		void	NetworkInit();

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