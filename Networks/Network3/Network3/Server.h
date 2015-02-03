#ifndef SERVER_H
#define SERVER_H

#include "Network.h"

// This class will be responsible for passing messages to the clients
class Server : public Network
{
	private:
		typedef std::shared_ptr<BinaryPacket> Packet;
		SocketManager* mSocketManager;
		std::vector<int> mSocketList;

	protected:
	public:

	private:
		// Sends a packet to every connected socket, except for the exception if there is one
		void	Broadcast( NetSocket* exception, Packet pkt );
		// Sends a packet to only the specified socket
		void	Send( NetSocket* receiver, Packet pkt );
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