#ifndef SERVER_H
#define SERVER_H

#include "Network.h"

class Server : public Network
{
	private:
		struct ClientNEF
		{
			NetworkEventForwarder NEF;
			UINT TeamID;
		};
		const UINT MAX_TEAMS = 2;

		SocketManager*				mSocketManager;
		std::map<UINT, ClientNEF>	mClientMap;
		std::map<UINT, NetEnemy>		mEnemies;
		UINT						mTeamDelegate;
		bool						mActive;

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

		void	CreateEnemies();
		void	SendEnemies( UINT toClient );
		void	BroadcastEvent( IEventPtr eventPtr, UINT exception = (UINT)-1 );
		void	SendEvent( IEventPtr eventPtr, UINT to );
		UINT	CurrentTeamDelegate();

	protected:
		bool	Connect( UINT port );

	public:
		void	Update( float deltaTime );
		void	DoSelect( int pauseMicroSecs, bool handleInput = true );
		bool	Initialize();
		void	Release();
				Server();
		virtual	~Server();
};
#endif