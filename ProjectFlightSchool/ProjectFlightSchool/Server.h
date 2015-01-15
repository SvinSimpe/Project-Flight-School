#ifndef SERVER_H
#define SERVER_H
#include "Connection.h"
#include "Package.h"
#include <vector>

struct Clientinfo
{
	SOCKET	s;
	int		team;
};

class Server
{
	// Members
	private:
		int							mResult;
		addrinfo*					mAddrResult;
		SOCKET						mListenSocket;
		std::vector<Clientinfo>		mClientSockets;
		Connection*					mConn;
		std::vector<std::thread>	mListenThreads;
		int							mNextTeamDelegation;
		int							mNrOfTeams;
		unsigned int				mNrOfProjectilesFired;

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
void Server::HandlePkg( SOCKET &fromSocket, Package<T>* p )
{
	Clientinfo s;
	s.s = fromSocket;
	for (auto& socket : mClientSockets)
	{
		if (socket.s == s.s)
		{
			s.team = socket.team;
		}
	}

	switch ( p->head.eventType )
	{
		case Net_Event::MESSAGE:
		{
			Message msg = (Message&)p->body.content;
			printf( "%d sent: %s\n", s.s, msg.msg );
		}
		case Net_Event::EV_PLAYER_MOVED:
		{
			EvPlayerMoved msg = (EvPlayerMoved&)p->body.content;
			for ( auto& socket : mClientSockets )
			{
				if ( socket.s != s.s )
				{
					mConn->SendPkg( socket.s, 0, Net_Event::EV_PLAYER_MOVED, msg );
				}
			}
			
		}
			break;
		case Net_Event::EV_PLAYER_JOINED:
		{
			EvInitialize toAll; // Contains the ID of the joining client
			toAll.ID	= (unsigned int)s.s;
			toAll.team	= s.team;
			for ( auto& socket : mClientSockets )
			{
				if ( socket.s != s.s && socket.s != INVALID_SOCKET )
				{
					mConn->SendPkg( socket.s, 0, Net_Event::EV_PLAYER_JOINED, toAll ); // Sends the ID of the joining client to each already existing client
				}
			}
		}
			break;
		case Net_Event::EV_PLAYER_DIED:
		{
			EvPlayerID toAll = (EvPlayerID&)p->body.content;
			for( auto& socket : mClientSockets )
			{
				if( socket.s != s.s && socket.s != INVALID_SOCKET )
				{
					mConn->SendPkg( socket.s, 0, Net_Event::EV_PLAYER_DIED, toAll );
				}
			}
		}
			break;
		case Net_Event::EV_PLAYER_DAMAGED:
		{
			EvPlayerID toAll = (EvPlayerID&)p->body.content;
			for (auto& socket : mClientSockets)
			{
				if ( socket.s != INVALID_SOCKET )
				{
					mConn->SendPkg( socket.s, 0, Net_Event::EV_PLAYER_DAMAGED, toAll );
				}
			}
		}
			break;
		case Net_Event::EV_PLAYER_SPAWNED:
		{
			EvPlayerID toAll = (EvPlayerID&)p->body.content;
			for ( auto& socket : mClientSockets )
			{
				if ( socket.s != s.s && socket.s != INVALID_SOCKET )
				{
					mConn->SendPkg( socket.s, 0, Net_Event::EV_PLAYER_SPAWNED, toAll );
				}
			}
		}
			break;
		case Net_Event::EV_PROJECTILE_FIRED:
		{
			EvProjectileFired toAll = (EvProjectileFired&)p->body.content;
			toAll.projectileID = mNrOfProjectilesFired++;
			for ( auto& socket : mClientSockets )
			{
				if ( socket.s != INVALID_SOCKET )
				{
					mConn->SendPkg( socket.s, 0, Net_Event::EV_PROJECTILE_FIRED, toAll );
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
