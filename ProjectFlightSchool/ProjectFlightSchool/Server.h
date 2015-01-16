#ifndef SERVER_H
#define SERVER_H
#include "Connection.h"
#include "Package.h"
#include <vector>

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
		bool			AcceptConnection();
		bool			ReceiveLoop( int index );
		void			DisconnectClient( SOCKET s );

	protected:
	public:
		bool			Connect();
		bool			Run();
		bool			Initialize( const char* port );
		void			Release();
						Server();
		virtual			~Server();
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
		case Net_Event::EV_PLAYER_UPDATE:
		{
			EvPlayerUpdate msg = (EvPlayerUpdate&)p->body.content;
			for ( auto& socket : mClientSockets )
			{
				if ( socket != s )
				{
					mConn->SendPkg( socket, 0, Net_Event::EV_PLAYER_UPDATE, msg );
				}
			}
			
		}
			break;
		case Net_Event::EV_PLAYER_JOINED:
		{
			EvPlayerID toAll; // Contains the ID of the joining client
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
			EvPlayerID toAll = (EvPlayerID&)p->body.content;
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
			EvPlayerID toAll = (EvPlayerID&)p->body.content;
			for (auto& socket : mClientSockets)
			{
				if ( socket != s && socket != INVALID_SOCKET )
				{
					mConn->SendPkg( socket, 0, Net_Event::EV_PLAYER_DAMAGED, toAll );
				}
			}
		}
			break;
		case Net_Event::EV_PLAYER_SPAWNED:
		{
			EvPlayerID toAll = (EvPlayerID&)p->body.content;
			for ( auto& socket : mClientSockets )
			{
				if ( socket != s && socket != INVALID_SOCKET )
				{
					mConn->SendPkg( socket, 0, Net_Event::EV_PLAYER_SPAWNED, toAll );
				}
			}
		}
			break;
		case Net_Event::EV_PROJECTILE_FIRED:
		{
			EvProjectileFired toAll = (EvProjectileFired&)p->body.content;
			for ( auto& socket : mClientSockets )
			{
				if ( socket != INVALID_SOCKET )
				{
					mConn->SendPkg( socket, 0, Net_Event::EV_PROJECTILE_FIRED, toAll );
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
