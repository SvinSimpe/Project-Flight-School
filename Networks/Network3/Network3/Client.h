#ifndef CLIENT_H
#define CLIENT_H

#include "Network.h"
#include <iostream>

class Client : public Network
{
	private:
		ClientSocketManager*	mSocketManager;
		std::string				mIP;
		NetworkEventForwarder*	mNEF;
		UINT					mID;
		std::list<UINT>			mRemoteIDs;
		bool					mActive;

	protected:
	public:

	private:
		void	LocalJoin( IEventPtr eventPtr );
		void	RemoteJoined( IEventPtr eventPtr );
		void	RemoteLeft( IEventPtr eventPtr );
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