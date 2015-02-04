#ifndef SERVER_H
#define SERVER_H

#include "Network.h"

class Server : public Network
{
	private:
		SocketManager*	mSocketManager;
		std::map<UINT, NetworkEventForwarder> mForwardMap;

	protected:
	public:

	private:
		void	ClientJoined( IEventPtr eventPtr );
		void	ClientLeft( IEventPtr eventPtr );
		void	InitEventListening();
		void	InitEventForwarding( NetworkEventForwarder* nef );

	protected:
	public:
		void	Update( float deltaTime );
		void	DoSelect( int pauseMicroSecs, bool handleInput = true );
		bool	Initialize( UINT port );
		void	Release();
				Server();
		virtual	~Server();
};
#endif