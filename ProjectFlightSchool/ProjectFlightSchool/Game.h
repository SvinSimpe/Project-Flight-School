#ifndef GAME_H
#define GAME_H

#include "Client.h"
#include "Server.h"
#include "StateMachine.h"

class Game
{
	// Class members
	private:
		StateMachine*		mStateMachine;

		std::thread			mNetworkThread;
		std::thread			mServerThread;
		Client*				mClient;
		Server*				mServer;
		bool				mServerIsActive;

	protected:
	public:

		// Class functions
	private:
		void ServerInit();
		void ClientInit();
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