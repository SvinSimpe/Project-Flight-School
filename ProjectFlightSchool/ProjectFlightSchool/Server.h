#ifndef SERVER_H
#define SERVER_H

#include "Network.h"
#include "ServerShip.h"
#include <vector>

class Server : public Network
{
	private:
		struct ClientNEF
		{
			NetworkEventForwarder	NEF;
			UINT					TeamID;
			XMFLOAT3				Pos = XMFLOAT3( 0.0f, 0.0f, 0.0f );
			bool					IsBuffed = false;
		};
		const UINT MAX_TEAMS = 2;
		const UINT MAX_PROJECTILE_ID = 999;

		SocketManager*				mSocketManager;
		std::map<UINT, ClientNEF*>	mClientMap;
		UINT						mTeamDelegate;
		UINT						mCurrentPID;
		bool						mActive;
		std::vector<ServerShip*>	mShips;

	protected:
	public:

	private:
		// Eventlistening functions
		void	ClientJoined( IEventPtr eventPtr );
		void	ClientLeft( IEventPtr eventPtr );
		void	ClientUpdate( IEventPtr eventPtr );
		void	ClientDied( IEventPtr eventPtr );
		void	ClientDamaged( IEventPtr eventPtr );
		void	ClientSpawned( IEventPtr eventPtr );
		void	ClientFiredProjectile( IEventPtr eventPtr );
		void	ClientUpdateHP( IEventPtr eventPtr );
		void	ClientMeleeHit( IEventPtr eventPtr );
		void	ClientAttack( IEventPtr eventPtr );
		void	ClientDown( IEventPtr eventPtr );
		void	ClientUp( IEventPtr eventPtr );
		void	ClientAttemptRevive( IEventPtr eventPtr );

		void	StartUp( IEventPtr eventPtr );

		void	SendEvent( IEventPtr eventPtr, UINT to );
		UINT	CurrentTeamDelegate();
		UINT	CurrentPID();
		void	CreateShips();
		bool	CheckShipBuff( ServerShip* ship, XMFLOAT3 pos );

	protected:
		bool	Connect( UINT port );

	public:
		void	BroadcastEvent( IEventPtr eventPtr, UINT exception = (UINT)-1 );
		void	Shutdown();
		void	Update( float deltaTime );
		void	DoSelect( int pauseMicroSecs, bool handleInput = true );
		bool	Initialize();
		void	Release();
				Server();
		virtual	~Server();
};
#endif