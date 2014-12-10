#ifndef SERVER_H
#define SERVER_H
#include "Connection.h"
#include "Package.h"
#include <vector>
#include "EventManager.h"

class Server
{
	// Members
	private:
		int							mResult;
		addrinfo*					mAddrResult;
		SOCKET						mListenSocket;
		std::vector<SOCKET>			mClientSockets;
		Connection*					mConn;
		std::vector<std::thread>	mListenThreads;

	protected:

	public:

	// Template functions
	private:
		template <typename T>
		void HandlePkg( SOCKET &s, Package<T>* p );
	protected:
	public:

	// Functions
	private:
						Server();
		virtual			~Server();
		bool			AcceptConnection();
		bool			ReceiveLoop( int index );
		void			HandleMsg( SOCKET &socket, char* msg );

	protected:

	public:
		static Server*	GetInstance();
		bool			Connect();
		bool			Run();
		bool			Initialize( const char* port );
		void			Release();
};

template <typename T>
void Server::HandlePkg( SOCKET &s, Package<T>* p )
{
	switch ( p->head.eventType )
	{
		case Net_Event::QUIT:
		{
			Message msg;
			msg.msg = "Goodbye!";
			printf( "%d disconnected.\n", s );
			mConn->SendPkg( s, 0, Net_Event::QUIT, msg );
			mConn->DisconnectSocket( s );
		}
			break;
		case Net_Event::MESSAGE:
		{
			Message msg = (Message&)p->body.content;
			printf( "%d sent: %s\n", s, msg.msg );
		}
		case Net_Event::EV_PLAYER_MOVED:
		{
			for (auto& t : mClientSockets)
			{
				EvPlayerMoved msg = (EvPlayerMoved&)p->body.content;
				if (t != s)
				{
					mConn->SendPkg(t, 0, Net_Event::EV_PLAYER_MOVED, msg);
				}
			}
			
		}
			break;
		default:
		{
			printf( "Error handling event from %d.\n", s );
		}
	}
}
#endif
