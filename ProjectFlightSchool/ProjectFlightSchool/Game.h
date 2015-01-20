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

		std::thread			mNetworkThread;
		std::thread			mServerThread;
		Client*				mClient;
		Server*				mServer;
		bool				mServerIsActive;

	protected:
	public:

		// Class functions
	private:
		void ServerInit( std::string port );
		void ClientInit( std::string ip, std::string port );
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