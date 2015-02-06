#include "Server.h"

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Start of eventlistening functions

void Server::ClientJoined( IEventPtr eventPtr )
{
	if( eventPtr->GetEventType() == Event_Client_Joined::GUID )
	{
		std::shared_ptr<Event_Client_Joined> data = std::static_pointer_cast<Event_Client_Joined>( eventPtr );
		UINT id = data->ID();
		UINT teamID = CurrentTeamDelegate();

		mClientMap[id].TeamID = teamID;
		mClientMap[id].NEF.Initialize( id, mSocketManager );

		// Sends the ID of the newly connected client to the newly connected client
		IEventPtr E1( new Event_Local_Joined( id, teamID ) );
		SendEvent( E1, id );
		SendEnemies( id );

		std::cout << "Client with ID: " << id << " joined team: " << teamID << ". There are now " << mClientMap.size() << " clients online." << std::endl;

		// Sends the incoming ID to the existing remotes
		IEventPtr E2( new Event_Remote_Joined( id, teamID ) ); 
		BroadcastEvent( E2, id );

		// Sends the list of existing remotes to the newly connected client
		for( auto& remote : mClientMap )
		{
			if( remote.first != id )
			{
				IEventPtr E3( new Event_Remote_Joined( remote.first, remote.second.TeamID ) ); // The key of the map is the ID of the remote
				SendEvent( E3, id );
			}
		}
	}
}

void Server::ClientLeft( IEventPtr eventPtr )
{
	if( eventPtr->GetEventType() == Event_Client_Left::GUID )
	{
		std::shared_ptr<Event_Client_Left> data = std::static_pointer_cast<Event_Client_Left>( eventPtr );
		UINT id = data->ID();
		mClientMap.erase( id );
		std::cout << "Client with ID: " << id << " left. There are now " << mClientMap.size() << " client(s) online." << std::endl;

		IEventPtr E1( new Event_Remote_Left( id ) );
		BroadcastEvent( E1 );
	}
}

void Server::ClientUpdate( IEventPtr eventPtr )
{
	if( eventPtr->GetEventType() == Event_Client_Update::GUID )
	{
		std::shared_ptr<Event_Client_Update> data = std::static_pointer_cast<Event_Client_Update>( eventPtr );
		UINT id = data->ID();
		XMFLOAT3 pos = data->LowerBodyPos();
		XMFLOAT3 vel = data->Velocity();
		XMFLOAT3 dir = data->UpperBodyDirection();

		std::cout << "Client with ID: " << id << " just updated." << std::endl;

		IEventPtr E1( new Event_Remote_Update( id, pos, vel, dir ) );
		BroadcastEvent( E1, id );
	}
}

void Server::ClientDied( IEventPtr eventPtr )
{
	if( eventPtr->GetEventType() == Event_Client_Died::GUID )
	{
		std::shared_ptr<Event_Client_Died> data = std::static_pointer_cast<Event_Client_Died>( eventPtr );
		UINT id = data->ID();
		UINT killerID = data->KillerID();

		std::cout << "Client with ID: " << id << " was killed by: " << killerID << std::endl;
		IEventPtr E1( new Event_Remote_Died( id, killerID ) );
		BroadcastEvent( E1, id );
	}
}

void Server::ClientDamaged( IEventPtr eventPtr )
{
	if( eventPtr->GetEventType() == Event_Client_Damaged::GUID )
	{
		std::shared_ptr<Event_Client_Damaged> data = std::static_pointer_cast<Event_Client_Damaged>( eventPtr );
		UINT id = data->ID();
		UINT projectileID = data->ProjectileID();

		std::cout << "Client with ID: " << id << " was shot by bullet with ID: " << projectileID << std::endl;
		IEventPtr E1( new Event_Remote_Damaged( id, projectileID ) );
		BroadcastEvent( E1, id );
	}
}

void Server::ClientSpawned( IEventPtr eventPtr )
{
	if( eventPtr->GetEventType() == Event_Client_Spawned::GUID )
	{
		std::shared_ptr<Event_Client_Spawned> data = std::static_pointer_cast<Event_Client_Spawned>( eventPtr );
		UINT id = data->ID();

		std::cout << "Client with ID: " << id << " just spawned." << std::endl;
		IEventPtr E1( new Event_Remote_Spawned( id ) );
		BroadcastEvent( E1, id );
	}
}

void Server::ClientFiredProjectile( IEventPtr eventPtr )
{
	if( eventPtr->GetEventType() == Event_Client_Fired_Projectile::GUID )
	{
		std::shared_ptr<Event_Client_Fired_Projectile> data = std::static_pointer_cast<Event_Client_Fired_Projectile>( eventPtr );
		UINT id = data->ID();
		UINT projectileID = data->ProjectileID();
		XMFLOAT3 pos = data->BodyPos();
		XMFLOAT3 dir = data->Direction();

		std::cout << "Client with ID: " << id << " just fired bullet: " << projectileID << " X-Pos: " << pos.x << " X-dir: " << dir.x << std::endl;

		IEventPtr E1( new Event_Remote_Fired_Projectile( id, projectileID, pos, dir ) );
		BroadcastEvent( E1, id );
	}
}

void Server::ClientUpdateHP( IEventPtr eventPtr )
{
	if( eventPtr->GetEventType() == Event_Client_Update_HP::GUID )
	{
		std::shared_ptr<Event_Client_Update_HP> data = std::static_pointer_cast<Event_Client_Update_HP>( eventPtr );
		UINT id = data->ID();
		float hp = data->HP();

		if( hp < 0.0f )
		{
			std::cout << "Client with ID: " << id << " lost " << (-1.0f)*hp << " HP." << std::endl;
		}
		else if( hp > 0.0f )
		{
			std::cout << "Client with ID: " << id << " gained " << hp << " HP." << std::endl;
		}
		else
		{
			std::cout << "Client with ID: " << id << " updated it's HP for no reason." << std::endl;
		}

		IEventPtr E1( new Event_Remote_Update_HP( id, hp ) );
		BroadcastEvent( E1, id );
	}
}

void Server::ClientMeleeHit( IEventPtr eventPtr )
{
	if( eventPtr->GetEventType() == Event_Client_Melee_Hit::GUID )
	{
		std::shared_ptr<Event_Client_Melee_Hit> data = std::static_pointer_cast<Event_Client_Melee_Hit>( eventPtr );
		UINT id = data->ID();
		UINT victim = data->VictimID();
		float damage = data->Damage();
		float knockBack = data->KnockBack();
		XMFLOAT3 dir = data->Direction();

		std::cout << "Remote with ID: " << id << " hit " << victim << " for " << damage << " with ";
		std::cout << knockBack << " knockback, facing: (" << dir.x << ", " << dir.y << ", " << dir.z << ")" << std::endl;

		IEventPtr E1( new Event_Remote_Melee_Hit( id, victim, damage, knockBack, dir ) );
		BroadcastEvent( E1, id );
	}
}

void Server::ClientAttack( IEventPtr eventPtr )
{
	if( eventPtr->GetEventType() == Event_Client_Attack::GUID )
	{
		std::shared_ptr<Event_Client_Attack> data = std::static_pointer_cast<Event_Client_Attack>( eventPtr );
		UINT id = data->ID();
		UINT armID = data->ArmID();
		UINT anim = data->Animation();

		std::cout << "Client with ID: " << id << " hit with arm: " << armID << " which is doing animation: #" << anim << std::endl;

		IEventPtr E1( new Event_Remote_Attack( id, armID, anim ) );
		BroadcastEvent( E1, id );
	}
}

void Server::ClientDown( IEventPtr eventPtr )
{
	if( eventPtr->GetEventType() == Event_Client_Down::GUID )
	{
		std::shared_ptr<Event_Client_Down> data = std::static_pointer_cast<Event_Client_Down>( eventPtr );
		UINT id = data->ID();

		std::cout << "Client with ID: " << id << " went down." << std::endl;
	}
}

void Server::ClientUp( IEventPtr eventPtr )
{
	if( eventPtr->GetEventType() == Event_Client_Up::GUID )
	{
		std::shared_ptr<Event_Client_Up> data = std::static_pointer_cast<Event_Client_Up>( eventPtr );
		UINT id = data->ID();

		std::cout << "Client with ID: " << id << " went up." << std::endl;
	}
}

void Server::ClientAttemptRevive( IEventPtr eventPtr )
{
	if( eventPtr->GetEventType() == Event_Client_Attempt_Revive::GUID )
	{
		std::shared_ptr<Event_Client_Attempt_Revive> data = std::static_pointer_cast<Event_Client_Attempt_Revive>( eventPtr );
		UINT id = data->ID();
		UINT downedID = data->DownedID();
		float deltaTime = data->DeltaTime();

		std::cout << "Client with ID: " << id << " is attempting to revive " << downedID << " with " << deltaTime << " time left." << std::endl;
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
		UINT port = data->Port();
		if( Connect( port ) )
		{
			CreateEnemies();
		}
		else
		{
			std::cout << "Failed to start server!" << std::endl;
		}
	}
}

void Server::CreateEnemies()
{
	for( UINT i = 0; i < 5; i++ )
	{
		mEnemies[i] = Enemy( i, 0, 0, XMFLOAT3( (float)i, (float)i, (float)i ), XMFLOAT3( (float)i, (float)i, (float)i ) );

		Enemy e = mEnemies[i];
		std::cout << "Created enemy: " << e.id << " with state: " << e.state << " of type: " << e.type;
		std::cout << " at: (" << e.pos.x << ", " << e.pos.y << ", " << e.pos.z << ")";
		std::cout << " facing: (" << e.dir.x << ", " << e.dir.y << ", " << e.dir.z << ")" << std::endl;
	}
}

// Syncs the newly connected player with the current enemy list
void Server::SendEnemies( UINT toClient )
{
	for( auto& enemy : mEnemies )
	{
		Enemy e = enemy.second;
		IEventPtr E1( new Event_Server_Create_Enemy( e.id, e.state, e.type, e.pos, e.dir ) );
		SendEvent( E1, toClient );
	}
}

void Server::BroadcastEvent( IEventPtr eventPtr, UINT exception )
{
	for( auto& to : mClientMap )
	{
		if( to.first != exception )
		{
			to.second.NEF.ForwardEvent( eventPtr );
		}
	}
}

void Server::SendEvent( IEventPtr eventPtr, UINT to )
{
	mClientMap[to].NEF.ForwardEvent( eventPtr );
}

UINT Server::CurrentTeamDelegate()
{
	UINT currentTeam = mTeamDelegate;
	mTeamDelegate++;
	if( mTeamDelegate > MAX_TEAMS )
		mTeamDelegate = 1;
	return currentTeam;
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

void Server::Update( float deltaTime )
{
	//if( !mClientMap.empty() )
	//{
	//	int counter = 0;
	//	for( auto& enemy : mEnemies )
	//	{
	//		enemy.second.pos.x += 0.1f;
	//		enemy.second.dir.z += 0.1f;
	//		if( ( counter % 2 ) > 0 )
	//		{
	//			enemy.second.isAlive = false;
	//		}
	//		else
	//		{
	//			enemy.second.isAlive = true;
	//		}
	//		counter++;

	//		IEventPtr E1( PFS_NEW Event_Server_Update_Enemy( enemy.first, enemy.second.pos, enemy.second.dir, enemy.second.isAlive ) );
	//		BroadcastEvent( E1 );
	//	}
	//}
}

void Server::DoSelect( int pauseMicroSecs, bool handleInput )
{
	mSocketManager->DoSelect( pauseMicroSecs, handleInput );
}

bool Server::Initialize()
{
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

	EventManager::GetInstance()->AddListener( &Server::StartUp, this, Event_Start_Server::GUID );

	mTeamDelegate = 1;
	return true;
}

void Server::Release()
{
	mSocketManager->Release();
	SAFE_DELETE( mSocketManager );
	mClientMap.clear();
}

Server::Server() : Network()
{
	mSocketManager	= nullptr;
	mClientMap		= std::map<UINT, ClientNEF>();
	mEnemies		= std::map<UINT, Enemy>();
	mTeamDelegate	= (UINT)-1;
}

Server::~Server()
{
}