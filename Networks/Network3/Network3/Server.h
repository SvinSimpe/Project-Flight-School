#ifndef SERVER_H
#define SERVER_H

#include "Network.h"

class Server : public Network
{
	private:
		SocketManager*	mSocketManager;
		std::list<UINT> mSocketIDs;
		std::list<NetworkEventForwarder> mNEFs;
		bool mActive;

	protected:
	public:

	private:
		void	HandleEvents( IEventPtr evtPtr );
		void	InitEventListening();
		void	InitEventForwarding();

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