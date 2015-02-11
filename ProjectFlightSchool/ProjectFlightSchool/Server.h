#ifndef SERVER_H
#define SERVER_H

#include "Network.h"
#include "Enemy.h"
#include "EnemySpawn.h"
#include "RemotePlayer.h"
#include <stdlib.h>
#include <time.h>
#include "RemotePlayer.h"

class Server : public Network
{
	private:
		struct ClientNEF
		{
			NetworkEventForwarder NEF;
			UINT TeamID;
		};
		const UINT MAX_TEAMS = 2;
		const UINT MAX_PROJECTILE_ID = 999;

		SocketManager*				mSocketManager;
		std::map<UINT, ClientNEF>	mClientMap;
		UINT						mTeamDelegate;
		UINT						mCurrentPID;
		bool						mActive;

		// Game Logic
		Enemy**						mEnemies;
		EnemySpawn**				mSpawners;
		BoundingCircle*				mAggroCircle;
		ServerPlayer				mPlayers[8];
		UINT						mNrOfPlayers;
		UINT						mNrOfEnemiesSpawned;
		UINT						mNrOfProjectilesFired;


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
		void	ClientEnemyProjectileDamage( IEventPtr eventPtr );
		void	SetEnemyState( IEventPtr eventPtr );

		void	StartUp( IEventPtr eventPtr );

		void	BroadcastEvent( IEventPtr eventPtr, UINT exception = (UINT)-1 );
		void	SendEvent( IEventPtr eventPtr, UINT to );
		UINT	CurrentTeamDelegate();
		UINT	CurrentPID();

		void		StateCheck();
		XMFLOAT3	GetNextSpawn();

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