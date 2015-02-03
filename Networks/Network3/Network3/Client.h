#ifndef CLIENT_H
#define CLIENT_H

#include "Network.h"
#include <iostream>

class Client : public Network
{
	private:
		std::string				mIP;
		NetworkEventForwarder*	mNEF;
		ClientSocketManager*	mSocketManager;
		bool					mActive;
		int						mID;
	protected:
	public:

	private:
		void	HandleEvents( IEventPtr evtPtr );
		void	InitEventListening();
		void	InitForwardingEvents();

	protected:
	public:
		void	Update( float deltaTime );
		void	DoSelect( int pauseMicroSecs, bool handleInput = true );
		bool	Initialize( std::string ip, unsigned int port );
		void	Release();
				Client();
		virtual	~Client();
};
#endif