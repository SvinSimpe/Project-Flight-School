#ifndef CLIENT_H
#define CLIENT_H

#include "Connection.h"
#include "EventManager.h"
#include "Events.h"

class Client // The class used by clients to connect to the server
{
	// Members
	private:
		unsigned int	mID;
		int				mResult;
		addrinfo*		mAddrResult;
		SOCKET			mServerSocket;
		Connection*		mConn;

	protected:

	public:

		// Template functions
	private:
		template <typename T>
		void HandlePkg( Package<T>* p );

	protected:
	public:
		
		// Functions
	private:
		bool	ReceiveLoop();
		void	EventListener( IEventPtr newEvent );

	protected:

	public:
		bool	Connect();
		bool	Run();
		bool	Initialize( std::string ip, std::string port ); // Sets up and connects to the server
		void	Release();
				Client();
		virtual	~Client();
};

template <typename T>
void Client::HandlePkg( Package<T>* p )
{
	switch ( p->head.eventType )
	{
		case Net_Event::EV_PLAYER_UPDATE:
		{
			EvPlayerUpdate msg = (EvPlayerUpdate&)p->body.content;
			IEventPtr E1( new Event_Remote_Player_Update( msg.id, msg.lowerBodyPosition, msg.lowerBodyDirection, msg.lowerBodyAnimation, msg.lowerBodyAnimationTime, msg.upperBodyPosition, msg.upperBodyDirection ) );
			EventManager::GetInstance()->QueueEvent( E1 );
		}
			break;
		case Net_Event::EV_PLAYER_JOINED:
		{
			EvInitialize msg = (EvInitialize&)p->body.content;
			IEventPtr E1( new Event_Remote_Player_Joined( msg.ID, msg.team ) );
			EventManager::GetInstance()->QueueEvent( E1 );
			printf( "Remote player with ID: %d joined.\n", msg.ID );
		}
			break;
		case Net_Event::EV_PLAYER_LEFT:
		{
			EvPlayerID msg = (EvPlayerID&)p->body.content;
			IEventPtr E1( new Event_Remote_Player_Left( msg.ID ) );
			EventManager::GetInstance()->QueueEvent( E1 );
			printf( "Remote player with ID: %d left.\n", msg.ID );
		}
			break;
		case Net_Event::YOUR_ID:
		{
			EvInitialize msg		= (EvInitialize&)p->body.content;
			mID						= msg.ID;

			IEventPtr E1( new Event_Local_Player_Joined( msg.ID, msg.team ) );
			EventManager::GetInstance()->QueueEvent( E1 );

			printf( "Your id is %d.\n", msg.ID );
		}
			break;
		case Net_Event::EV_PLAYER_DIED:
		{
			EvKilled deadPlayer = (EvKilled&)p->body.content;
			IEventPtr E1( new Event_Remote_Player_Died( deadPlayer.ID, deadPlayer.killerID ) );
			EventManager::GetInstance()->QueueEvent( E1 );
		}
			break;
		case Net_Event::EV_PLAYER_DAMAGED:
		{
			EvPlayerID damagedPlayer = (EvPlayerID&)p->body.content;
			IEventPtr E1( new Event_Remote_Player_Damaged( damagedPlayer.ID, damagedPlayer.projectileID ) );
			EventManager::GetInstance()->QueueEvent( E1 );
		}
			break;
		case Net_Event::EV_PLAYER_SPAWNED:
		{
			EvPlayerID spawnedPlayer = (EvPlayerID&)p->body.content;
			IEventPtr E1( new Event_Remote_Player_Spawned( spawnedPlayer.ID ) );
			EventManager::GetInstance()->QueueEvent( E1 );
		}
			break;
		case Net_Event::EV_PROJECTILE_FIRED:
		{
			EvProjectileFired projectileFired = (EvProjectileFired&)p->body.content;
			IEventPtr E1( new Event_Remote_Projectile_Fired( projectileFired.ID, projectileFired.projectileID, projectileFired.position, projectileFired.direction  ) );
			EventManager::GetInstance()->QueueEvent( E1 );
		}
			break;
		case Net_Event::EV_UPDATE_HP:
		{
			EvPlayerID player = (EvPlayerID&)p->body.content;
			IEventPtr E1( new Event_Remote_Player_Update_HP( player.ID, (float)player.HP  ) );
			EventManager::GetInstance()->QueueEvent( E1 );
		}
			break;
		case Net_Event::EV_SYNC_ENEMY:
		{
			EvSyncEnemy enemy = (EvSyncEnemy&)p->body.content;
			IEventPtr E1( new Event_Sync_Enemy( enemy.ID, enemy.model, enemy.animation, enemy.hp, enemy.isAlive, enemy.position, enemy.direction  ) );
			EventManager::GetInstance()->QueueEvent( E1 );
		}
			break;
		case Net_Event::EV_ENEMY_LIST_SYNCED:
		{
			IEventPtr E1( new Event_Enemy_List_Synced() );
			EventManager::GetInstance()->QueueEvent( E1 );
		}
			break;
		case Net_Event::EV_MELEE_HIT:
		{
			EvMeleeHit meleeHit = (EvMeleeHit&)p->body.content;
			IEventPtr E1( new Event_Remote_Player_Melee_Hit( meleeHit.ID, meleeHit.damage, meleeHit.knockBack, meleeHit.direction ) );
			EventManager::GetInstance()->QueueEvent( E1 );
		}
			break;
		default:
		{
			printf( "Error handling event from server.\n" );
		}
	}
}
#endif
