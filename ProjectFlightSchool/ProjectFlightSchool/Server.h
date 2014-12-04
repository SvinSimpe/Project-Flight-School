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
		void HandlePkg( SOCKET &s, Package<T> p );
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
void Server::HandlePkg( SOCKET &s, Package<T> p )
{
	switch (p.head.eventType)
	{
		case Net_Event::QUIT:
		{
			Empty e;
			printf("%d disconnected.\n", s);
			mConn->SendPkg(s, e, Net_Event::QUIT);
			mConn->DisconnectSocket(s);
		}
			break;5
		case Net_Event::MESSAGE:
		{
			Message msg = (Message&)p.body.content;
			printf("%d sent: %s\n", s, msg.msg);
		}
			break;
		default:
		{
			printf( "Error handling event from %d.\n", s );
		}
	}
}
#endif
