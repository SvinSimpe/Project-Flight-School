
#ifndef SERVER_H
#define SERVER_H

#include "Network.h"
#include "ServerShip.h"
#include "EnemySpawn.h"
#include "RemotePlayer.h"
#include <vector>
#include <stdlib.h>
#include <time.h>
#pragma once

class Enemy;

class Server : public Network
{
	private:
		#pragma region Friends
		friend class		IEnemyBehavior; 
		friend class		IdleBehavior;
		friend class		HuntPlayerBehavior;
		friend class		MoveToShipBehavior; 
		friend class		AttackBehavior;
		friend class		TakeDamageBehavior;
		friend class		StunnedBehavior;
		friend class		DeadBehavior;
		#pragma endregion

		struct ClientNEF // Server player
		{
			NetworkEventForwarder	NEF;
			UINT					ID;
			UINT					TeamID;
			XMFLOAT3				Pos = XMFLOAT3( 0.0f, 0.0f, 0.0f );
			bool					IsBuffed = false;
			bool					IsAlive = false;
			bool					IsDown = false;
		};

		struct ServerEvent
		{
			IEventPtr EventPtr;
			UINT ToID;

			ServerEvent( IEventPtr eventPtr, UINT toID )
			{
				this->EventPtr = eventPtr;
				this->ToID = toID;
			}
		};
		const UINT MAX_TEAMS = 2;
		const UINT MAX_PROJECTILE_ID = 999;

		SocketManager*				mSocketManager;
		std::map<UINT, ClientNEF*>	mClientMap;
		UINT						mCurrentPID;
		bool						mActive;
		std::vector<ServerShip*>	mShips;
		std::list<ServerEvent>		mEventList;

		// Game Logic
		ServerPlayer**				mPlayers;
		UINT						mTeamDelegate;
		Enemy**						mEnemies;
		EnemySpawn**				mSpawners;
		
		UINT						mNrOfPlayers;
		UINT						mNrOfEnemiesSpawned;
		UINT						mNrOfProjectilesFired;

		bool						mStopAccept;

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
		void	ClientShipProjectileDamage( IEventPtr eventPtr );
		void	ClientEnemyProjectileDamage( IEventPtr eventPtr );
		void	SetEnemyState( IEventPtr eventPtr );
		void	BroadcastEnemyAttackToClients( IEventPtr eventPtr );
		void	ClientWinLose( IEventPtr eventPtr );
		void	ClientChangeShipLevels( IEventPtr eventPtr );
		void	LobbyPlayer( IEventPtr eventPtr );
		void	StopLobby( IEventPtr eventPtr );
		void	SwitchTeam( IEventPtr eventPtr );

		void	StartUp( IEventPtr eventPtr );
		void	DoSelect( int pauseMicroSecs, bool handleInput = true );

		void	SendEvent( IEventPtr eventPtr, UINT to );
		UINT	CurrentTeamDelegate();
		UINT	CurrentPID();
		void	CreateShips();
		bool	CheckShipBuff( ServerShip* ship, XMFLOAT3 pos );

		XMFLOAT3	GetNextSpawn();

	protected:
		bool	Connect( UINT port );

	public:
		void	BroadcastEvent( IEventPtr eventPtr, UINT exception = (UINT)-1 );
		void	Shutdown();
		void	Update( float deltaTime );
		bool	Initialize();
		void	Reset();
		void	Release();
				Server();
		virtual	~Server();
};
#endif