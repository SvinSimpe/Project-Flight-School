#ifndef SERVER_H
#define SERVER_H

#include "Connection.h"

class Server
{
	private:
		NetworkEventForwarder*	mNEF;

	protected:
	public:
		static SocketManager*	mSocketManager;

	private:
		void	InitEventListening();
		void	InitForwardingEvents();

	protected:
	public:
		void	ForwardEvent( IEventPtr ePtr );
		bool	Initialize( unsigned int port );
		void	Release();
				Server();
		virtual	~Server();
};
#endif