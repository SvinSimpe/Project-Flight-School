#ifndef SERVER_H
#define SERVER_H

#include "Network.h"

class Server : public Network
{
	private:
		SocketManager*	mSocketManager;
		std::map<UINT, NetworkEventForwarder> mClientMap;

	protected:
	public:
		bool	mActive;

	private:
		void	ClientJoined( IEventPtr eventPtr );
		void	ClientLeft( IEventPtr eventPtr );
		void	LocalUpdate( IEventPtr eventPtr );
		void	StartServer( IEventPtr eventPtr );

		void	BroadcastEvent( IEventPtr eventPtr, UINT exception = (UINT)-1 );
		void	SendEvent( IEventPtr eventPtr, UINT to );
		void	InitEventListening();

	protected:
	public:
		void	Update( float deltaTime );
		void	DoSelect( int pauseMicroSecs, bool handleInput = true );
		bool	Initialize( UINT port ); // This will later be made private and only called from within StartServer()
		void	Release();
				Server();
		virtual	~Server();
};
#endif