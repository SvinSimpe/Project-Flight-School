#ifndef SERVER_H
#define SERVER_H

#include "Network.h"

// A server representation of each Client, the server will hold a copy of these for updating purposes
struct LocalClient
{
	int ID; // ID assigned by the SocketManager
};

// This class will be responsible for passing messages to the clients
class Server : public Network
{
	private:
		typedef std::shared_ptr<BinaryPacket> Packet;
		SocketManager* mSocketManager;
	protected:
	public:

	private:
		void	HandleEvents( IEventPtr evtPtr );
		void	InitEventListening();

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