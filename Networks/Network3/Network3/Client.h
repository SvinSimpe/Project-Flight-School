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
		UINT					mID;
		bool					mActive;

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
		void	StartUp( IEventPtr eventPtr );
		
		void	InitEventListening();
		void	InitForwardingEvents();

	protected:
		bool	Initialize( std::string ip, unsigned int port );
	public:
		void	Update( float deltaTime );
		void	DoSelect( int pauseMicroSecs, bool handleInput = true );
		void	Release();
				Client();
		virtual	~Client();
};
#endif