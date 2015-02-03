#ifndef SERVER_H
#define SERVER_H

#include "Network.h"

class Server : public Network
{
	private:
		SocketManager* mSocketManager;
	protected:
	public:

	private:
		void	InitEventListening();
		void	InitForwardingEvents();

	protected:
	public:
		void	DoSelect( int pauseMicroSecs, bool handleInput = true );
		bool	Initialize( UINT port );
		void	Release();
				Server();
		virtual	~Server();
};
#endif