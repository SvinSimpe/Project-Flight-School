#ifndef SERVER_H
#define SERVER_H
#pragma once

#include "Network.h"
#include "ServerShip.h"
#include "EnemySpawn.h"
#include "RemotePlayer.h"
#include <vector>
#include <stdlib.h>
#include <time.h>
#include "EnergyCell.h"
#include <queue>

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
			float					HP			= 100.0f;
			UINT					ID			= (UINT)-1;
			UINT					TeamID		= (UINT)-1;
			BoundingCircle			Pos			= BoundingCircle( XMFLOAT3( 0.0f, 0.0f, 0.0f ), 1.0f );
			bool					IsBuffed	= false;
			bool					IsAlive		= false;
			bool					IsDown		= false;
		};

		const UINT MAX_TEAMS = 2;
		const UINT MAX_PROJECTILE_ID = 999;
		const float ENEMY_UPDATE_RANGE = 900.0f;

		SocketManager*				mSocketManager;
		std::map<UINT, ClientNEF*>	mClientMap;
		UINT						mCurrentPID;
		bool						mActive;
		std::vector<ServerShip*>	mShips;

		// Game Logic
		ServerPlayer**				mPlayers;
		UINT						mTeamDelegate;
		Enemy**						mEnemies;
		EnemySpawn**				mSpawners;
		
		UINT						mNrOfPlayers;
		UINT						mNrOfEnemiesSpawned;
		UINT						mNrOfProjectilesFired;

		EnergyCell**				mEnergyCells;
		int							mCurrentCell;
		bool						mStopAccept;
		bool						mGameFull;
		bool						mDropped;

		float						mCellSpawnTimer;
		float						mRespawnCellTimer;

		std::queue<XMFLOAT3>		mCellPositionQueue;
		UINT						mMaxClients;

	protected:
	public:

	private:
		// Eventlistening functions
		void	ClientJoined( IEventPtr eventPtr );
		void	ClientLeft( IEventPtr eventPtr );
		void	ClientUpdate( IEventPtr eventPtr );
		void	ClientDied( IEventPtr eventPtr );
		void	ClientDamaged( IEventPtr eventPtr );
		void	ClientRemovedProjectile( IEventPtr eventPtr );
		void	ClientSpawned( IEventPtr eventPtr );
		void	ClientFiredProjectile( IEventPtr eventPtr );
		void	ClientUpdateHP( IEventPtr eventPtr );
		void	ClientMeleeHit( IEventPtr eventPtr );
		void	ClientAttack( IEventPtr eventPtr );
		void	ClientDash( IEventPtr eventPtr );
		void	ClientDown( IEventPtr eventPtr );
		void	ClientUp( IEventPtr eventPtr );
		void	ClientAttemptRevive( IEventPtr eventPtr );
		void	ClientShipProjectileDamage( IEventPtr eventPtr );
		void	ClientEnemyMeleeDamage( IEventPtr eventPtr );
		void	ClientEnemyProjectileDamage( IEventPtr eventPtr );
		void	SetEnemyState( IEventPtr eventPtr );
		void	BroadcastEnemyAttackToClients( IEventPtr eventPtr );
		void	ClientWinLose( IEventPtr eventPtr );
		void	ClientChangeShipLevels( IEventPtr eventPtr );
		void	TurretFiredProjectile( IEventPtr eventPtr );
		void	EnemyFiredProjectile(IEventPtr eventPtr);
		void	LobbyPlayer( IEventPtr eventPtr );
		void	StopLobby( IEventPtr eventPtr );
		void	SwitchTeam( IEventPtr eventPtr );
		void	XP( IEventPtr eventPtr );
		void	ChangeWeapon( IEventPtr eventPtr );
		void	ClientUpdateShip( IEventPtr eventPtr );
		void	ResetTurretTargets( IEventPtr eventPtr );
		void	ClientChangeReady( IEventPtr eventPtr );
		void	HostStartCountdown( IEventPtr eventPtr );
		void	ClientRequestParticleSystem( IEventPtr eventPtr );
		void	ClientInteractEnergyCell( IEventPtr eventPtr );
		void	ClientSetName( IEventPtr eventPtr );

		void	StartUp( IEventPtr eventPtr );

		void	SendEvent( IEventPtr eventPtr, UINT to );
		UINT	CurrentTeamDelegate();
		UINT	CurrentPID();
		void	CreateShips();
		bool	CheckShipBuff( ServerShip* ship, XMFLOAT3 pos );
		void	UpdateShip( float deltaTime, ServerShip* s );
		void	OnSpawnEnergyCell( IEventPtr e );
		void	OnDroppedEnergyCell( IEventPtr e );
		void	CreateEnergyCells();
		void	CalculateCellPosition( XMFLOAT3 pos, float offSetX, float offSetZ );
		void	CalculateCellSpawnPositions( XMFLOAT3 shipPosition );
		void	SetEnemySpawnerPositions();
		void	CalculateEnemySpawnerPositions();
		bool	IsEnergyCellHere( XMFLOAT3 checkPosition ) const;
		bool	CullEnemyUpdate( XMFLOAT3 playerPos, XMFLOAT3 enemyPos );

		XMFLOAT3	GetNextSpawn();

	protected:
		bool	Connect( UINT port );

	public:
		void	DoSelect( int pauseMicroSecs, bool handleInput = true );
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