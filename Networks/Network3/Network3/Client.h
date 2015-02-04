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
		std::list<UINT>			mRemoteIDs;
		bool					mActive;
		UINT					mID;

		// TESTING CODE FOR EVENTS BELOW
		XMFLOAT3	mLowerBodyPos;
		XMFLOAT3	mVelocity;
		XMFLOAT3	mUpperBodyDirection;

	protected:
	public:

	private:
		void	LocalJoin( IEventPtr eventPtr );
		void	RemoteJoined( IEventPtr eventPtr );
		void	RemoteLeft( IEventPtr eventPtr );
		void	RemoteUpdate( IEventPtr eventPtr );
		
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