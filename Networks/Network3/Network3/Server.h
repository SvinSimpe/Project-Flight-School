#ifndef SERVER_H
#define SERVER_H

#include "Network.h"

// This class will be responsible for passing messages to the clients
class Server : public Network
{
	private:
		typedef std::shared_ptr<BinaryPacket> Packet;
		SocketManager* mSocketManager;
		std::vector<UINT> mSocketList;
		NetworkEventForwarder* mNEF;

	protected:
	public:

	private:
		void	BroadcastPkt( Packet pkt, UINT exception = 0 );
		void	BroadcastEvent( IEventPtr eventPtr, UINT exception = 0 );
		void	SendPkt( UINT receiver, Packet pkt );
		void	SendEvent( UINT receiver, IEventPtr eventPtr );
		void	HandleEvents( IEventPtr eventPtr );
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