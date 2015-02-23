#include "Server.h"
#include "Enemy.h"

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Start of eventlistening functions

void Server::ClientJoined( IEventPtr eventPtr )
{
	if( eventPtr->GetEventType() == Event_Client_Joined::GUID && !mStopAccept )
	{
		std::shared_ptr<Event_Client_Joined> data = std::static_pointer_cast<Event_Client_Joined>( eventPtr );
		auto& it = mClientMap.find(data->ID());
		if( it == mClientMap.end() )
		{
			UINT teamID = CurrentTeamDelegate();

			mClientMap[data->ID()]			= new ClientNEF();
			mClientMap[data->ID()]->NEF.Initialize( data->ID(), mSocketManager );
			mClientMap[data->ID()]->TeamID	= teamID;
			mClientMap[data->ID()]->ID		= data->ID();
			//mClientMap[data->ID()]->AggroCircle	= new BoundingCircle( 1.0f );

			mPlayers[mNrOfPlayers]				= new ServerPlayer();
			mPlayers[mNrOfPlayers]->ID			= data->ID();
			mPlayers[mNrOfPlayers]->TeamID		= teamID;
			mPlayers[mNrOfPlayers]->AggroCircle	= new BoundingCircle( 1.0f );
			mNrOfPlayers++;

			// Sends necessary information of the newly connected client to the newly connected client
			IEventPtr E1( new Event_Local_Joined( data->ID(), teamID ) );
			SendEvent( E1, data->ID() );

			// Initializes the ships in their current state to the newly connected client
			for( auto& s : mShips )
			{
				IEventPtr SpawnShip( new Event_Server_Spawn_Ship( s->mID, s->mTeamID, s->mPos, s->mRot, s->mScale, s->mCurrentHP ) );
				SendEvent( SpawnShip, data->ID() );
				IEventPtr E1( new Event_Server_Change_Ship_Levels( s->mTeamID, s->mTurretLevel, s->mShieldLevel, s->mBuffLevel ) );
				SendEvent( E1, data->ID() );
			}

			// Sends the incoming ID to the existing remotes
			IEventPtr E2( new Event_Remote_Joined( data->ID(), teamID ) ); 
			BroadcastEvent( E2, data->ID() );

			// Sends the list of existing remotes to the newly connected client
			for( auto& remote : mClientMap )
			{
				if( remote.first != data->ID() )
				{
					IEventPtr E3( new Event_Remote_Joined( remote.first, remote.second->TeamID ) ); // The key of the map is the ID of the remote
					SendEvent( E3, data->ID() );
				}
			}
			// Synchronize list of enemy spawners to connecting player
			for ( size_t i = 0; i < MAX_NR_OF_ENEMY_SPAWNERS; i++ )
			{
				IEventPtr EvSpawn( new Event_Server_Sync_Spawn( mSpawners[i]->GetID(), mSpawners[i]->GetPosition() ) );
				SendEvent( EvSpawn, data->ID() );
			}

			// Synchronize list of enemies to connecting player
			for ( size_t i = 0; i < MAX_NR_OF_ENEMIES; i++ )
			{
				IEventPtr EvEnemy( new Event_Server_Create_Enemy(	mEnemies[i]->GetID(), 
																	mEnemies[i]->GetEnemyState(), 
																	mEnemies[i]->GetEnemyType(), 
																	mEnemies[i]->GetPosition(), 
																	mEnemies[i]->GetDirection() ) );
				SendEvent( EvEnemy, data->ID() );
			}

			for( int i = 0; i < MAX_ENERGY_CELLS; i++ )
			{
				IEventPtr EvEnergyCell( new Event_Server_Sync_Energy_Cell(	i,
																			mEnergyCells[i]->GetOwnerID(),
																			mEnergyCells[i]->GetPosition(),
																			mEnergyCells[i]->GetPickedUp() ) );
				SendEvent( EvEnergyCell, data->ID() );
			}
		}
	}
}

void Server::ClientLeft( IEventPtr eventPtr )
{
	if( eventPtr->GetEventType() == Event_Client_Left::GUID )
	{
		std::shared_ptr<Event_Client_Left> data = std::static_pointer_cast<Event_Client_Left>( eventPtr );
		auto& it = mClientMap.find(data->ID());
		if( it != mClientMap.end() )
		{
			SAFE_DELETE( mClientMap[data->ID()] );
			mClientMap.erase( data->ID() );

			IEventPtr E1( new Event_Remote_Left( data->ID() ) );
			BroadcastEvent( E1 );
		}
	}
}

void Server::ClientUpdate( IEventPtr eventPtr )
{
	if( eventPtr->GetEventType() == Event_Client_Update::GUID )
	{
		std::shared_ptr<Event_Client_Update> data = std::static_pointer_cast<Event_Client_Update>( eventPtr );
		auto& it = mClientMap.find(data->ID());
		if( it != mClientMap.end() )
		{
			for ( size_t i = 0; i < MAX_NR_OF_PLAYERS; i++ )
			{
				if( mPlayers[i] != nullptr )
				{
					if( mPlayers[i]->ID == data->ID() )
					{
						mPlayers[i]->IsAlive		= data->IsAlive();
						mPlayers[i]->IsBuffed		= data->IsBuffed();
						mPlayers[i]->Pos			= data->LowerBodyPos();
					}
				}
			}

			mClientMap[data->ID()]->Pos.center = data->LowerBodyPos();
			XMFLOAT3 vel = data->Velocity();
			XMFLOAT3 dir = data->UpperBodyDirection();
			std::string name = data->Name();
			mClientMap[data->ID()]->IsBuffed = data->IsBuffed();
			mClientMap[data->ID()]->IsAlive = data->IsAlive();

			IEventPtr E1( new Event_Remote_Update( data->ID(), mClientMap[data->ID()]->Pos.center, vel, dir, name, data->IsAlive() ) );
			BroadcastEvent( E1, data->ID() );
		}
	}
}

void Server::ClientDied( IEventPtr eventPtr )
{
	if( eventPtr->GetEventType() == Event_Client_Died::GUID )
	{
		std::shared_ptr<Event_Client_Died> data = std::static_pointer_cast<Event_Client_Died>( eventPtr );
		auto& it = mClientMap.find(data->ID());
		if( it != mClientMap.end() )
		{
			UINT killerID = data->KillerID();

			IEventPtr E1( new Event_Remote_Died( data->ID(), killerID ) );
			BroadcastEvent( E1, data->ID() );
		}
		for ( size_t i = 0; i < MAX_NR_OF_PLAYERS; i++ )
			{
				if( mPlayers[i] != nullptr )
				{
					if( mPlayers[i]->ID == data->ID() )
						mPlayers[i]->IsAlive		= false;
				}
			}

	}
}

void Server::ClientDamaged( IEventPtr eventPtr )
{
	if( eventPtr->GetEventType() == Event_Client_Damaged::GUID )
	{
		std::shared_ptr<Event_Client_Damaged> data = std::static_pointer_cast<Event_Client_Damaged>( eventPtr );
		auto& it = mClientMap.find(data->ID());
		if( it != mClientMap.end() )
		{
			UINT projectileID = data->ProjectileID();

			IEventPtr E1( new Event_Remote_Damaged( data->ID(), projectileID ) );
			BroadcastEvent( E1 );
		}
	}
}

void Server::ClientSpawned( IEventPtr eventPtr )
{
	if( eventPtr->GetEventType() == Event_Client_Spawned::GUID )
	{
		std::shared_ptr<Event_Client_Spawned> data = std::static_pointer_cast<Event_Client_Spawned>( eventPtr );
		auto& it = mClientMap.find(data->ID());
		if( it != mClientMap.end() )
		{
			IEventPtr E1( new Event_Remote_Spawned( data->ID() ) );
			BroadcastEvent( E1, data->ID() );
		}

		for ( size_t i = 0; i < MAX_NR_OF_PLAYERS; i++ )
			{
				if( mPlayers[i] != nullptr )
				{
					if( mPlayers[i]->ID == data->ID() )
					{
						mPlayers[i]->IsAlive		= true;
						mPlayers[i]->IsDown			= false;
					}
				}
			}
	}
}

void Server::ClientFiredProjectile( IEventPtr eventPtr )
{
	if( eventPtr->GetEventType() == Event_Client_Fired_Projectile::GUID )
	{
		std::shared_ptr<Event_Client_Fired_Projectile> data = std::static_pointer_cast<Event_Client_Fired_Projectile>( eventPtr );
		auto& it = mClientMap.find(data->ID());
		if( it != mClientMap.end() )
		{
			UINT pid = CurrentPID();
			XMFLOAT3 pos	= data->BodyPos();
			XMFLOAT3 dir	= data->Direction();
			float speed		= data->Speed();
			float range		= data->Range();
			float damage	= data->Damage();

			IEventPtr E1( new Event_Remote_Fired_Projectile( data->ID(), pid, pos, dir, speed, range, damage ) );
			BroadcastEvent( E1 );
		}
	}
}

void Server::ClientUpdateHP( IEventPtr eventPtr )
{
	if( eventPtr->GetEventType() == Event_Client_Update_HP::GUID )
	{
		std::shared_ptr<Event_Client_Update_HP> data = std::static_pointer_cast<Event_Client_Update_HP>( eventPtr );
		auto& it = mClientMap.find(data->ID());
		if( it != mClientMap.end() )
		{
			mClientMap[data->ID()]->HP = data->HP();

			IEventPtr E1( new Event_Remote_Update_HP( data->ID(), data->HP() ) );
			BroadcastEvent( E1, data->ID() );
		}
	}
}

void Server::ClientMeleeHit( IEventPtr eventPtr )
{
	if( eventPtr->GetEventType() == Event_Client_Melee_Hit::GUID )
	{
		std::shared_ptr<Event_Client_Melee_Hit> data = std::static_pointer_cast<Event_Client_Melee_Hit>( eventPtr );
		auto& it = mClientMap.find(data->ID());
		if( it != mClientMap.end() )
		{
			float damage = data->Damage();
			float knockBack = data->KnockBack();
			XMFLOAT3 dir = data->Direction();

			IEventPtr E1( new Event_Remote_Melee_Hit( data->ID(), damage, knockBack, dir ) );
			BroadcastEvent( E1 );
		}
	}
}

void Server::ClientAttack( IEventPtr eventPtr )
{
	if( eventPtr->GetEventType() == Event_Client_Attack::GUID )
	{
		std::shared_ptr<Event_Client_Attack> data = std::static_pointer_cast<Event_Client_Attack>( eventPtr );
		auto& it = mClientMap.find(data->ID());
		if( it != mClientMap.end() )
		{
			UINT armID = data->ArmID();
			UINT anim = data->Animation();

			IEventPtr E1( new Event_Remote_Attack( data->ID(), armID, anim ) );
			BroadcastEvent( E1, data->ID() );
		}
	}
}

void Server::ClientDown( IEventPtr eventPtr )
{
	if( eventPtr->GetEventType() == Event_Client_Down::GUID )
	{
		std::shared_ptr<Event_Client_Down> data = std::static_pointer_cast<Event_Client_Down>( eventPtr );
		auto& it = mClientMap.find(data->ID());
		if( it != mClientMap.end() )
		{
			mClientMap[data->ID()]->IsAlive = false;
			IEventPtr E1( new Event_Remote_Down( data->ID() ) );
			BroadcastEvent( E1, data->ID() );
		}

		for ( size_t i = 0; i < MAX_NR_OF_PLAYERS; i++ )
		{
			if( mPlayers[i] != nullptr )
			{
				if( mPlayers[i]->ID == data->ID() )
					mPlayers[i]->IsDown		= true;
					
			}
		}
	}
}

void Server::ClientUp( IEventPtr eventPtr )
{
	if( eventPtr->GetEventType() == Event_Client_Up::GUID )
	{
		std::shared_ptr<Event_Client_Up> data = std::static_pointer_cast<Event_Client_Up>( eventPtr );
		auto& it = mClientMap.find(data->ID());
		if( it != mClientMap.end() )
		{
			mClientMap[data->ID()]->IsAlive = true;
			IEventPtr E1( new Event_Remote_Up( data->ID() ) );
			BroadcastEvent( E1, data->ID() );
		}

		for ( size_t i = 0; i < MAX_NR_OF_PLAYERS; i++ )
		{
			if( mPlayers[i] != nullptr )
			{
				if( mPlayers[i]->ID == data->ID() )
					mPlayers[i]->IsDown		= false;
					
			}
		}
	}
}

void Server::ClientAttemptRevive( IEventPtr eventPtr )
{
	if( eventPtr->GetEventType() == Event_Client_Attempt_Revive::GUID )
	{
		std::shared_ptr<Event_Client_Attempt_Revive> data = std::static_pointer_cast<Event_Client_Attempt_Revive>( eventPtr );
		auto& it = mClientMap.find(data->ID());
		if( it != mClientMap.end() )
		{
			UINT downedID = data->DownedID();
			float deltaTime = data->DeltaTime();

			if( mClientMap[data->ID()]->TeamID == mClientMap[downedID]->TeamID ) // This IF causes revives to be within teams, might be better to place somewhere else.
			{
				IEventPtr E1( new Event_Remote_Attempt_Revive( data->ID(), downedID, deltaTime ) );
				BroadcastEvent( E1, data->ID() );
			}
		}
	}
}

void Server::ClientEnemyProjectileDamage( IEventPtr eventPtr )
{
	if( eventPtr->GetEventType() == Event_Client_Projectile_Damage_Enemy::GUID )
	{
		std::shared_ptr<Event_Client_Projectile_Damage_Enemy> data = std::static_pointer_cast<Event_Client_Projectile_Damage_Enemy>( eventPtr );
		mEnemies[data->EnemyID()]->TakeDamage( data->Damage(), data->ID() );	
	}
}

void Server::SetEnemyState( IEventPtr eventPtr )
{
	if( eventPtr->GetEventType() == Event_Set_Enemy_State::GUID )
	{
		std::shared_ptr<Event_Set_Enemy_State> data = std::static_pointer_cast<Event_Set_Enemy_State>( eventPtr );
		IEventPtr E1( new Event_Server_Sync_Enemy_State( data->ID(), data->State() ) );
		BroadcastEvent( E1 );
	}
}

void Server::ClientInteractEnergyCell( IEventPtr eventPtr )
{
	if( eventPtr->GetEventType() == Event_Client_Sync_Energy_Cell::GUID )
	{
		std::shared_ptr<Event_Client_Sync_Energy_Cell> data = std::static_pointer_cast<Event_Client_Sync_Energy_Cell>( eventPtr );
		IEventPtr E1( new Event_Server_Sync_Energy_Cell( data->EnergyCellID(), data->OwnerID(), data->Position(), data->PickedUp() ) );
		BroadcastEvent( E1, data->OwnerID() );

		mEnergyCells[data->EnergyCellID()]->SetOwnerID( data->OwnerID() );
		mEnergyCells[data->EnergyCellID()]->SetPickedUp( data->PickedUp() );
		mEnergyCells[data->EnergyCellID()]->SetPosition( data->Position() );

		for( UINT i = 0; i < mShips.size(); i++ )
		{
			for( int j = 0; j < MAX_ENERGY_CELLS; j++ )
			{
				mShips[i]->AddEnergyCell( mEnergyCells[j]->GetOwnerID() );
			}
		}

	}
}

void Server::BroadcastEnemyAttackToClients( IEventPtr eventPtr )
{
	if( eventPtr->GetEventType() == Event_Tell_Server_Enemy_Attack_Player::GUID )
	{
		std::shared_ptr<Event_Tell_Server_Enemy_Attack_Player> data = std::static_pointer_cast<Event_Tell_Server_Enemy_Attack_Player>( eventPtr );
		IEventPtr E1( new Event_Server_Enemy_Attack_Player( data->ID(), data->PlayerID(), data->Damage() ) );
		BroadcastEvent( E1 );
	}
}

void Server::ClientWinLose( IEventPtr eventPtr )
{
	if( eventPtr->GetEventType() == Event_Client_Win::GUID )
	{
		std::shared_ptr<Event_Client_Win> data = std::static_pointer_cast<Event_Client_Win>( eventPtr );
		IEventPtr E1( new Event_Remote_Win( data->Team() ) );
		BroadcastEvent( E1 );
	}
}

void Server::ClientChangeShipLevels( IEventPtr eventPtr )
{
	if ( eventPtr->GetEventType() == Event_Client_Change_Ship_Levels::GUID )
	{
		std::shared_ptr<Event_Client_Change_Ship_Levels> data = std::static_pointer_cast<Event_Client_Change_Ship_Levels>( eventPtr );
		for( auto s : mShips )
		{
			if ( s->mTeamID == data->ID() )
			{
				s->ClientChangeShipLevels( data->TurretLevelChange(), data->ShieldLevelChange(), data->BuffLevelChange() );
				IEventPtr E1( new Event_Server_Change_Ship_Levels( s->mTeamID, s->mTurretLevel, s->mShieldLevel, s->mBuffLevel ) );
				BroadcastEvent( E1 );
				break;
			}
		}
	}
}

void Server::TurretFiredProjectile( IEventPtr eventPtr )
{
	if ( eventPtr->GetEventType() == Event_Turret_Fired_Projectile::GUID )
	{
		std::shared_ptr<Event_Turret_Fired_Projectile> data = std::static_pointer_cast<Event_Turret_Fired_Projectile>( eventPtr );
		for( auto s : mShips )
		{
			ServerTurret* t = s->mServerTurret;
			if( t->mID == data->ID() )
			{
				IEventPtr E1( new Event_Server_Turret_Fired_Projectile( data->ID(), t->mTeamID, CurrentPID(), data->Position(), data->Direction(), data->Speed(), data->Range() ));
				BroadcastEvent( E1 );
				break;
			}
		}
	}
}

void Server::LobbyPlayer( IEventPtr eventPtr )
{
	if ( eventPtr->GetEventType() == Event_Client_Initialize_LobbyPlayer::GUID )
	{
		std::shared_ptr<Event_Client_Initialize_LobbyPlayer> data = std::static_pointer_cast<Event_Client_Initialize_LobbyPlayer>( eventPtr );
		IEventPtr E1( new Event_Server_Initialize_LobbyPlayer( data->ID(), data->TeamID(), data->Name() ) );
		BroadcastEvent( E1 );
	}
}

void Server::StopLobby( IEventPtr eventPtr )
{
	if ( eventPtr->GetEventType() == Event_Client_Lobby_Finished::GUID )
	{
		std::shared_ptr<Event_Client_Lobby_Finished> data = std::static_pointer_cast<Event_Client_Lobby_Finished>( eventPtr );
		mStopAccept = true;
		IEventPtr E1( new Event_Server_Lobby_Finished() );
		BroadcastEvent( E1 );
	}
}

void Server::SwitchTeam( IEventPtr eventPtr )
{
	if( eventPtr->GetEventType() == Event_Client_Switch_Team::GUID )
	{
		std::shared_ptr<Event_Client_Switch_Team> data = std::static_pointer_cast<Event_Client_Switch_Team>( eventPtr );
		for( size_t i = 0; i < mNrOfPlayers; i++ )
		{
			if( mPlayers[i]->ID == data->ID() )
			{
				mPlayers[i]->TeamID = data->TeamID();
				IEventPtr E1( new Event_Server_Switch_Team( mPlayers[i]->ID, mPlayers[i]->TeamID ) );
				BroadcastEvent( E1 );
			}
		}
		for( auto& remote : mClientMap )
		{
			if( remote.first == data->ID() )
			{
				remote.second->TeamID = data->TeamID();
			}
		}
	}
}

// End of eventlistening functions
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// This is technically also an eventlistening function, but it's special so it can't be with the other ones
void Server::StartUp( IEventPtr eventPtr )
{
	if( eventPtr->GetEventType() == Event_Start_Server::GUID )
	{
		std::shared_ptr<Event_Start_Server> data = std::static_pointer_cast<Event_Start_Server>( eventPtr );
		std::string port = data->Port();

		// Makes sure everything is clean before starting
		Reset();
		CreateShips();
		
		std::stringstream sstr;
		sstr << port << " ";
		UINT iPort;
		sstr >> iPort;

		if( Connect( iPort ) )
		{
			mActive = true;
			IEventPtr E1( new Event_Connect_Server_Success () );
			EventManager::GetInstance()->QueueEvent( E1 );
		}
		else
		{
			IEventPtr E1( new Event_Connect_Server_Fail ( "Server failed at connecting!" ) );
			EventManager::GetInstance()->QueueEvent( E1 );
			Release();
		}
	}
}

void Server::DoSelect( int pauseMicroSecs, bool handleInput )
{
	mSocketManager->DoSelect( pauseMicroSecs, handleInput );
}

void Server::BroadcastEvent( IEventPtr eventPtr, UINT exception )
{
	for( auto& to : mClientMap )
	{
		if( to.first != exception )
		{
			//mClientMap[to.first]->NEF.ForwardEvent( eventPtr );
			mEventList.push_front( ServerEvent( eventPtr, to.first ) );
		}
	}
}

void Server::SendEvent( IEventPtr eventPtr, UINT to )
{
	//mClientMap[to]->NEF.ForwardEvent( eventPtr );
	mEventList.push_front( ServerEvent( eventPtr, to ) );
}

UINT Server::CurrentTeamDelegate()
{
	UINT currentTeam = mTeamDelegate;
	mTeamDelegate++;
	if( mTeamDelegate > MAX_TEAMS )
		mTeamDelegate = 1;
	return currentTeam;
}

UINT Server::CurrentPID()
{
	UINT currentPID = mCurrentPID;
	mCurrentPID++;
	if( mCurrentPID >= MAX_PROJECTILE_ID )
		mCurrentPID = 0;
	return currentPID;
}

void Server::CreateShips()
{
	UINT shipID = 60;
	float xOffset = -30.0f;

	for( UINT i = 0; i < MAX_TEAMS; i++ )
	{
		mShips.push_back( new ServerShip() );
		mShips.back()->Initialize( shipID, CurrentTeamDelegate(), XMFLOAT3( xOffset, 0.0f, 0.0f ), XMFLOAT4( 0.0f, 0.0f, 0.0f, 0.0f ), XMFLOAT3( 1.0f, 1.0f, 1.0f ) );
		shipID++;
		xOffset += 60.0f;
	}
}

bool Server::CheckShipBuff( ServerShip* ship, XMFLOAT3 pos )
{
	BoundingCircle circle = BoundingCircle( pos, 0.5f );
	return ship->Intersect( &circle );
}

void Server::UpdateShip( float deltaTime, ServerShip* s )
{
	if( s->mWasUpdated )
	{
		IEventPtr E1( new Event_Server_Update_Ship( s->mID, s->mMaxShield, s->mCurrentShield, s->mCurrentHP ) );
		BroadcastEvent( E1 );
	}

	std::vector<BoundingCircle*> enemyCircles;
	for( UINT i = 0; i < MAX_NR_OF_ENEMIES; i++ )
	{
		if( mEnemies[i]->IsAlive() )
		{
			enemyCircles.push_back( mEnemies[i]->GetAttackCircle() );
		}
	}

	for( auto& cm : mClientMap )
	{
		ClientNEF* c = cm.second;
		if( s->mTeamID != c->TeamID && c->HP > 0.0f)
		{
			enemyCircles.push_back( &c->Pos );
		}
	}
	
	s->FindTurretTarget( enemyCircles );

	s->Update( deltaTime );
	IEventPtr E1( new Event_Server_Update_Turret( s->mServerTurret->mID, s->mServerTurret->mTurretHead->rot ) );
	BroadcastEvent( E1 );
}

bool Server::Connect( UINT port )
{
	mSocketManager = new SocketManager();
	if( !mSocketManager->Initialize() )
	{
		return false;
	}
	mSocketManager->AddSocket( new ServerListenSocket( mSocketManager, port ) );
	std::cout << "Server started on port: " << port << std::endl;

	return true;
}

///////////////////////////////////////////////////////////////////////////////
//								Game Logic
///////////////////////////////////////////////////////////////////////////////

void Server::Update( float deltaTime )
{
	if( this && mActive )
	{
		// Handles the client getting buffed by the ship
		bool shipBuff = false;

		for ( size_t i = 0; i < MAX_NR_OF_PLAYERS; i++ )
		{
			if( mPlayers != nullptr )
			{
				if( mPlayers[i] != nullptr )
				{
					mPlayers[i]->AggroCircle->center = mPlayers[i]->Pos;
				}
			}
		}

		for ( size_t i = 0; i < mNrOfPlayers; i++ )
		{
			for( auto& s : mShips )
			{
				if( s->mTeamID == mPlayers[i]->TeamID )
				{
					shipBuff = CheckShipBuff( s, mPlayers[i]->Pos );
					if( shipBuff != mPlayers[i]->IsBuffed )
					{
						IEventPtr BuffEvent( new Event_Server_Change_Buff_State( mPlayers[i]->ID, shipBuff, s->mBuffMod ) );
						SendEvent( BuffEvent, mPlayers[i]->ID );
						break;
					}
				}
			}
		}

		// Enemy update
		for ( size_t i = 0; i < MAX_NR_OF_ENEMIES; i++ )
		{
			if( mEnemies[i]->IsAlive() )
			{
				mEnemies[i]->Update( deltaTime, mPlayers, mNrOfPlayers );

				IEventPtr enemy( new Event_Server_Update_Enemy(		mEnemies[i]->GetID(), 
																	mEnemies[i]->GetPosition(), 
																	mEnemies[i]->GetDirection(), 
																	mEnemies[i]->IsAlive() ) );
				BroadcastEvent( enemy );
			}
			else
			{
				mEnemies[i]->HandleSpawn( deltaTime, GetNextSpawn() );
			}
		}

		//if( mSafeUpdate )
		//StateCheck();
		// Ship updates
		for( auto& s : mShips )
		{
			UpdateShip( deltaTime, s );
		}

		// Sends the events in the queue to the clients
		while( !mEventList.empty() )
		{
			mClientMap[mEventList.back().ToID]->NEF.ForwardEvent( mEventList.back().EventPtr );
			mEventList.pop_back();
		}
		DoSelect( 0 );
	}
}

XMFLOAT3 Server::GetNextSpawn()
{
	return mSpawners[mNrOfEnemiesSpawned++ % MAX_NR_OF_ENEMY_SPAWNERS]->GetSpawnPosition();
}

bool Server::Initialize()
{
	mStopAccept = false;

	EventManager::GetInstance()->AddListener( &Server::ClientJoined, this, Event_Client_Joined::GUID );
	EventManager::GetInstance()->AddListener( &Server::ClientLeft, this, Event_Client_Left::GUID );
	EventManager::GetInstance()->AddListener( &Server::ClientUpdate, this, Event_Client_Update::GUID );
	EventManager::GetInstance()->AddListener( &Server::ClientDied, this, Event_Client_Died::GUID );
	EventManager::GetInstance()->AddListener( &Server::ClientDamaged, this, Event_Client_Damaged::GUID );
	EventManager::GetInstance()->AddListener( &Server::ClientSpawned, this, Event_Client_Spawned::GUID );
	EventManager::GetInstance()->AddListener( &Server::ClientFiredProjectile, this, Event_Client_Fired_Projectile::GUID );
	EventManager::GetInstance()->AddListener( &Server::ClientUpdateHP, this, Event_Client_Update_HP::GUID );
	EventManager::GetInstance()->AddListener( &Server::ClientMeleeHit, this, Event_Client_Melee_Hit::GUID );
	EventManager::GetInstance()->AddListener( &Server::ClientAttack, this, Event_Client_Attack::GUID );
	EventManager::GetInstance()->AddListener( &Server::ClientDown, this, Event_Client_Down::GUID );
	EventManager::GetInstance()->AddListener( &Server::ClientUp, this, Event_Client_Up::GUID );
	EventManager::GetInstance()->AddListener( &Server::ClientAttemptRevive, this, Event_Client_Attempt_Revive::GUID );
	EventManager::GetInstance()->AddListener( &Server::ClientEnemyProjectileDamage, this, Event_Client_Projectile_Damage_Enemy::GUID );
	EventManager::GetInstance()->AddListener( &Server::SetEnemyState, this, Event_Set_Enemy_State::GUID );
	EventManager::GetInstance()->AddListener( &Server::ClientInteractEnergyCell, this, Event_Client_Sync_Energy_Cell::GUID );
	EventManager::GetInstance()->AddListener( &Server::BroadcastEnemyAttackToClients, this, Event_Tell_Server_Enemy_Attack_Player::GUID );
	EventManager::GetInstance()->AddListener( &Server::ClientWinLose, this, Event_Client_Win::GUID );
	EventManager::GetInstance()->AddListener( &Server::ClientChangeShipLevels, this, Event_Client_Change_Ship_Levels::GUID );

	EventManager::GetInstance()->AddListener( &Server::StartUp, this, Event_Start_Server::GUID );
	EventManager::GetInstance()->AddListener( &Server::TurretFiredProjectile, this, Event_Turret_Fired_Projectile::GUID );
	EventManager::GetInstance()->AddListener( &Server::LobbyPlayer, this, Event_Client_Initialize_LobbyPlayer::GUID );
	EventManager::GetInstance()->AddListener( &Server::StopLobby, this, Event_Client_Lobby_Finished::GUID );
	EventManager::GetInstance()->AddListener( &Server::SwitchTeam, this, Event_Client_Switch_Team::GUID );

	mTeamDelegate	= 1;
	mCurrentPID		= 0;

	//		GAME LOGIC
	//   Enemies & Spawners

	srand( (UINT)time( NULL ) );
	mSpawners		= new EnemySpawn*[MAX_NR_OF_ENEMY_SPAWNERS];
	for ( size_t i = 0; i < MAX_NR_OF_ENEMY_SPAWNERS; i++ )
	{
		// Map size values
		int X, Y;
		X = ( rand() % 150 ) - 75;
		Y = ( rand() % 150 ) - 75;
		mSpawners[i] = new EnemySpawn();
		mSpawners[i]->Initialize( i );
		mSpawners[i]->SetPosition( XMFLOAT3( (float)(X), 0.0f, (float)(Y) ) );
	}

	mEnemies	= new Enemy*[MAX_NR_OF_ENEMIES];
	for ( size_t i = 0; i < MAX_NR_OF_ENEMIES; i++ )
	{
		mEnemies[i] = new Enemy();
		mEnemies[i]->Initialize( i, mPlayers, mNrOfPlayers );
		mEnemies[i]->Spawn( GetNextSpawn() );
	}

	//mAggroCircle	= new BoundingCircle();

	//Energy cells
	mEnergyCells = new EnergyCell*[MAX_ENERGY_CELLS];
	mEnergyCells[0] = new EnergyCell();
	mEnergyCells[0]->Initialize( DirectX::XMFLOAT3( 1000000.0f, 0.0f, 10000000.0f ) ); //Gfx drivers bug makes us not render the first one so this is an incredible ugly hack around that problem
	for( int i = 1; i < MAX_ENERGY_CELLS; i++ )
	{
		mEnergyCells[i] = new EnergyCell();
		mEnergyCells[i]->Initialize( DirectX::XMFLOAT3( ( -2.0f + ( i * 2 ) ), 0.0f, -30.0f ) );
	}

	return true;
}

void Server::Reset()
{
	mStopAccept = false;

	for ( size_t i = 0; i < MAX_NR_OF_ENEMIES; i++ )
	{
		mEnemies[i]->Reset();
	}

	mTeamDelegate	= 1;
	mCurrentPID		= 0;
	mActive			= false;

	if( mSocketManager )
		mSocketManager->Release();

	SAFE_DELETE( mSocketManager );

	for( auto& c : mClientMap )
	{
		SAFE_DELETE( c.second );
	}
	mClientMap.clear();
	mEventList.clear();

	//for( UINT i = 0; i < MAX_ENERGY_CELLS; i++ )
	//{
	//	mEnergyCells[i]->Reset();
	//}

	for( auto& s : mShips )
	{
		SAFE_RELEASE_DELETE( s );
	}
	mShips.clear();

}

void Server::Release()
{
	// Enemies
	for ( size_t i = 0; i < MAX_NR_OF_ENEMIES; i++ )
	{
		mEnemies[i]->Release();
		SAFE_DELETE( mEnemies[i] );
	}

	delete [] mEnemies;

	for ( size_t i = 0; i < MAX_NR_OF_ENEMY_SPAWNERS; i++ )
	{
		mSpawners[i]->Release();
		SAFE_DELETE( mSpawners[i] );
	}

	delete [] mSpawners;

	//SAFE_DELETE( mAggroCircle );

	for ( size_t i = 0; i < MAX_NR_OF_PLAYERS; i++ )
		SAFE_DELETE( mPlayers[i] );
	
	delete [] mPlayers;

	mTeamDelegate	= 1;
	mCurrentPID		= 0;
	mActive			= false;

	for( auto& s : mShips )
	{
		if( s )
			s->Release();
		SAFE_DELETE( s );
	}
	mShips.clear();
	if( mSocketManager )
		mSocketManager->Release();

	SAFE_DELETE( mSocketManager );
	mClientMap.clear();

	mEventList.clear();

	//Energy cells
	for( int i = 0; i < MAX_ENERGY_CELLS; i++ )
	{
		mEnergyCells[i]->Release();
		SAFE_DELETE( mEnergyCells[i] );
	}

	delete [] mEnergyCells;
}

Server::Server() : Network()
{
	mSocketManager			= nullptr;
	mClientMap				= std::map<UINT, ClientNEF*>();
	mTeamDelegate			= (UINT)-1;
	mCurrentPID				= (UINT)-1;
	mActive					= false;
	mShips					= std::vector<ServerShip*>();
	mShips.reserve( 2 );
	mEventList				= std::list<ServerEvent>();
	mEnergyCells			= nullptr;
	mEnemies				= nullptr;
	mSpawners				= nullptr;
	//mAggroCircle			= nullptr;
	mNrOfEnemiesSpawned		= 0;
	mNrOfPlayers			= 0;
	mNrOfProjectilesFired	= 0;
	mEventList		= std::list<ServerEvent>();
	mPlayers		= nullptr;
	
	mPlayers		= new ServerPlayer*[MAX_NR_OF_PLAYERS];
	for ( size_t i = 0; i < MAX_NR_OF_PLAYERS; i++ )
		mPlayers[i]	= nullptr;

	mNrOfPlayers	= 0;
}

Server::~Server()
{
}