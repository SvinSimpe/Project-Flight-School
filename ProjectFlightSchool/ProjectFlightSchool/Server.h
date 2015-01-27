#ifndef SERVER_H
#define SERVER_H
#include "Connection.h"
#include "Package.h"
#include <vector>
#include "Enemy.h"
#include <stdlib.h>
#include <time.h>

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

		// Enemies
		bool						mInGame;
		Enemy**						mEnemies;

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
		void			EventListener( IEventPtr newEvent );
		bool			AcceptConnection();
		bool			ReceiveLoop( int index );
		void			DisconnectClient( SOCKET s );

	protected:
	public:
		HRESULT			Update( float deltaTime );
		bool			Connect();
		bool			Run();
		bool			Initialize( std::string port );
		void			Release();
						Server();
		virtual			~Server();
};

template <typename T>
void Server::HandlePkg( SOCKET &fromSocket, Package<T>* p )
{
	Clientinfo s;
	s.s = fromSocket;
	for ( auto& socket : mClientSockets )
	{
		if ( socket.s == s.s )
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
		case Net_Event::EV_PLAYER_UPDATE:
		{
			EvPlayerUpdate msg = (EvPlayerUpdate&)p->body.content;
			for ( auto& socket : mClientSockets )
			{
				if ( socket.s != s.s )
				{
					mConn->SendPkg( socket.s, 0, Net_Event::EV_PLAYER_UPDATE, msg );
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

			// Synchronize enemy list to connecting player
			EvSyncEnemy enemy;
			for ( size_t i = 0; i < MAX_NR_OF_ENEMIES; i++ )
			{
				enemy.ID			= mEnemies[i]->GetID();
				enemy.model			= mEnemies[i]->GetModelID();
				enemy.animation		= mEnemies[i]->GetAnimation();
				enemy.hp			= mEnemies[i]->GetHP();
				enemy.isAlive		= mEnemies[i]->IsAlive();
				enemy.position		= mEnemies[i]->GetPosition();
				enemy.direction		= mEnemies[i]->GetDirection();

				mConn->SendPkg( s.s, 0, Net_Event::EV_SYNC_ENEMY, enemy );
				Sleep( 10 );
			}
			
			// List synchronized
			//mConn->SendPkg( s.s, 0, Net_Event::EV_ENEMY_LIST_SYNCED, 0 );
		}
			break;
		case Net_Event::EV_PLAYER_DIED:
		{
			EvKilled toAll = (EvKilled&)p->body.content;
			for( auto& socket : mClientSockets )
			{
				if( socket.s != INVALID_SOCKET )
				{
					mConn->SendPkg( socket.s, 0, Net_Event::EV_PLAYER_DIED, toAll );
				}
			}
		}
			break;
		case Net_Event::EV_PLAYER_DAMAGED:
		{
			EvPlayerID toAll = (EvPlayerID&)p->body.content;
			for ( auto& socket : mClientSockets )
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
		case Net_Event::EV_UPDATE_HP:
		{
			EvPlayerID toAll = (EvPlayerID&)p->body.content;
			toAll.projectileID = mNrOfProjectilesFired++;
			for ( auto& socket : mClientSockets )
			{
				if ( socket.s != s.s && socket.s != INVALID_SOCKET )
				{
					mConn->SendPkg( socket.s, 0, Net_Event::EV_UPDATE_HP, toAll );
				}
			}
		}
			break;
		case Net_Event::EV_MELEE_HIT:
		{
			EvMeleeHit toAll = (EvMeleeHit&)p->body.content;
			for ( auto& socket : mClientSockets )
			{
				if ( socket.s != INVALID_SOCKET )
				{
					mConn->SendPkg( socket.s, 0, Net_Event::EV_MELEE_HIT, toAll );
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
