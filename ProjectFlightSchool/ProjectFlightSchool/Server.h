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
		void			DisconnectClient( SOCKET s );

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
		case Net_Event::MESSAGE:
		{
			Message msg = (Message&)p->body.content;
			printf( "%d sent: %s\n", s, msg.msg );
		}
		case Net_Event::EV_PLAYER_MOVED:
		{
			EvPlayerMoved msg = (EvPlayerMoved&)p->body.content;
			for ( auto& socket : mClientSockets )
			{
				if ( socket != s )
				{
					mConn->SendPkg( socket, 0, Net_Event::EV_PLAYER_MOVED, msg );
				}
			}
			
		}
			break;
		case Net_Event::EV_PLAYER_JOINED:
		{
			EvPlayerConnection toAll; // Contains the ID of the joining client
			toAll.ID = (unsigned int)s;
			for ( auto& socket : mClientSockets )
			{
				if ( socket != s && socket != INVALID_SOCKET )
				{
					mConn->SendPkg( socket, 0, Net_Event::EV_PLAYER_JOINED, toAll ); // Sends the ID of the joining client to each already existing client
				}
			}
		}
			break;
		case Net_Event::EV_PLAYER_DIED:
		{
			EvPlayerConnection toAll = (EvPlayerConnection&)p->body.content;
			for( auto& socket : mClientSockets )
			{
				if( socket != s && socket != INVALID_SOCKET )
				{
					mConn->SendPkg( socket, 0, Net_Event::EV_PLAYER_DIED, toAll );
				}
			}
		}
			break;
		case Net_Event::EV_PLAYER_DAMAGED:
		{
			EvPlayerConnection toAll = (EvPlayerConnection&)p->body.content;
			for (auto& socket : mClientSockets)
			{
				if (socket != s && socket != INVALID_SOCKET)
				{
					mConn->SendPkg( socket, 0, Net_Event::EV_PLAYER_DAMAGED, toAll );
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
