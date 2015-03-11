#include "PlayState.h"
#include "HelperFunctions.h"

///////////////////////////////////////////////////////////////////////////////
//									PRIVATE
///////////////////////////////////////////////////////////////////////////////

void PlayState::EventListener( IEventPtr newEvent )
{
	if ( newEvent->GetEventType() == Event_Local_Joined::GUID ) // Add a remote player to the list when they connect
	{
		std::shared_ptr<Event_Local_Joined> data = std::static_pointer_cast<Event_Local_Joined>( newEvent );
		if ( mPlayer != nullptr )
		{
			mPlayer->SetID( data->ID() );
			mPlayer->SetTeam( data->TeamID() );

			//TestSound
			//SoundBufferHandler::GetInstance()->Play( mSoundAsset );

			IEventPtr E1( new Event_Client_Initialize_LobbyPlayer( mPlayer->GetID(), mPlayer->GetTeam(), mPlayer->GetName() ) );
			Client::GetInstance()->SendEvent( E1 );
		}
	}

	else if ( newEvent->GetEventType() == Event_Remote_Joined::GUID ) // Add a remote player to the list when they connect
	{
		std::shared_ptr<Event_Remote_Joined> data = std::static_pointer_cast<Event_Remote_Joined>( newEvent );
		mRemotePlayers.push_back( new RemotePlayer() );
		mRemotePlayers.at(mRemotePlayers.size() - 1)->Initialize();
		mRemotePlayers.at(mRemotePlayers.size() - 1)->RemoteInit( data->ID(), data->TeamID() );

		IEventPtr E1( new Event_Client_Initialize_LobbyPlayer( mPlayer->GetID(), mPlayer->GetTeam(), mPlayer->GetName() ) );
		Client::GetInstance()->SendEvent( E1 );
	}
	else if ( newEvent->GetEventType() == Event_Remote_Left::GUID ) // Remove a remote player from the list when they disconnect
	{
		std::shared_ptr<Event_Remote_Left> data = std::static_pointer_cast<Event_Remote_Left>( newEvent );

		for( auto& p : mRemotePlayers )
		{
			if( data->ID() == p->GetID() )
			{
				XMFLOAT3 pos = p->GetPosition();
				UINT ecID = p->GetEnergyCellID();
				p->Release();
				SAFE_DELETE( p );
				std::swap( p, mRemotePlayers.back() );
				mRemotePlayers.pop_back();

				if( ecID != (UINT)-1 )
				{
					mEnergyCells[ecID]->SetPosition( p->GetPosition() );
					mEnergyCells[ecID]->SetOwnerID( (UINT)-1 );
					mEnergyCells[ecID]->SetPickedUp( false );
					mEnergyCells[ecID]->SetSecured( false );

					IEventPtr E1( new Event_Client_Sync_Energy_Cell( 
						ecID, 
						mEnergyCells[ecID]->GetOwnerID(), 
						mEnergyCells[ecID]->GetPosition(), 
						mEnergyCells[ecID]->GetPickedUp() ) );
					Client::GetInstance()->SendEvent( E1 );
				}
				break;
			}
		}
	}
	else if ( newEvent->GetEventType() == Event_Remote_Damaged::GUID )
	{
		// Damage remote player
		std::shared_ptr<Event_Remote_Damaged> data = std::static_pointer_cast<Event_Remote_Damaged>(newEvent);
		HandleRemoteProjectileHit( data->ID(), data->ProjectileID() );
	}
	else if( newEvent->GetEventType() == Event_Remote_Removed_Projectile::GUID )
	{
		std::shared_ptr<Event_Remote_Removed_Projectile> data = std::static_pointer_cast<Event_Remote_Removed_Projectile>(newEvent);
		HandleRemoteProjectileRemoved( data->ProjectileID() );
	}
	else if ( newEvent->GetEventType() == Event_Remote_Fired_Projectile::GUID )
	{
		// Fire projectile
		std::shared_ptr<Event_Remote_Fired_Projectile> data = std::static_pointer_cast<Event_Remote_Fired_Projectile>(newEvent);
		UINT teamID = (UINT)-1;
		for( UINT i = 0; i < mRemotePlayers.size(); i++ )
		{
			if( mRemotePlayers.at(i)->GetID() == data->ID() )
			{
				teamID = mRemotePlayers.at(i)->GetTeam();
				break;
			}
		}
		FireProjectile( data->ID(), data->ProjectileID(), teamID, data->BodyPos(), data->Direction(), data->Speed(), data->Range(), data->Damage(), (WeaponType)data->Weapon() );

		//TestSound
		if ( (WeaponType)data->Weapon() == MINIGUN )
			SoundBufferHandler::GetInstance()->Play3D( mMiniGun , data->BodyPos());

		else if ( (WeaponType)data->Weapon() == SHOTGUN )
			SoundBufferHandler::GetInstance()->Play3D( mShotGun , data->BodyPos());

		else if ( (WeaponType)data->Weapon() == SNIPER )
			SoundBufferHandler::GetInstance()->Play3D( mSniper , data->BodyPos());
		
		
		// Request Muzzle Flash from Particle Manager
		//RenderManager::GetInstance()->RequestParticleSystem( data->ID(), SniperTrail, data->BodyPos(), data->Direction() );

		XMFLOAT3 cross;
		XMStoreFloat3( &cross, XMVector3Cross( XMLoadFloat3( &XMFLOAT3( 0.0f, 1.0f, 0.0f ) ), XMLoadFloat3( &data->Direction() ) ) );
		RenderManager::GetInstance()->RequestParticleSystem( data->ID(), Shell, XMFLOAT3(data->BodyPos().x - data->Direction().x, data->BodyPos().y, data->BodyPos().z - data->Direction().z), cross );

		//RenderManager::GetInstance()->RequestParticleSystem( data->ID(), Explosion, XMFLOAT3( 5.0f, 0.5f, 0.0f ), XMFLOAT3( 1.0f, 1.0f, 1.0f ) );
		//RenderManager::GetInstance()->RequestParticleSystem( data->ID(), ExplosionSmoke, XMFLOAT3( 5.0f, 0.5f, 0.0f ), XMFLOAT3( 1.0f, 1.0f, 1.0f ) );

	}
	else if ( newEvent->GetEventType() == Event_Server_Create_Enemy::GUID )
	{
		std::shared_ptr<Event_Server_Create_Enemy> data = std::static_pointer_cast<Event_Server_Create_Enemy>( newEvent );
		SyncEnemy( data->ID(), (EnemyState)data->State(), (EnemyType)data->Type(), data->Position(), data->Direction(), data->MaxHP() );
	}
	else if ( newEvent->GetEventType() == Event_Server_Update_Enemy::GUID )
	{
		std::shared_ptr<Event_Server_Update_Enemy> data = std::static_pointer_cast<Event_Server_Update_Enemy>( newEvent );
		UpdateEnemyPosition( data->ID(), data->Position(), data->Direction(), data->IsAlive(), data->EnemyHP() );
	}
	else if ( newEvent->GetEventType() == Event_Server_Enemies_Created::GUID )
	{
		mEnemyListSynced = true;
	}
	else if ( newEvent->GetEventType() == Event_Connect_Server_Success::GUID )
	{
		mServerInitialized = true;
	}
	else if ( newEvent->GetEventType() == Event_Server_Sync_Spawn::GUID )
	{
		std::shared_ptr<Event_Server_Sync_Spawn> data = std::static_pointer_cast<Event_Server_Sync_Spawn>( newEvent );
		SyncSpawn( data->ID(), data->Position() );
	}
	else if ( newEvent->GetEventType() == Event_Server_Sync_Enemy_State::GUID )
	{
		std::shared_ptr<Event_Server_Sync_Enemy_State> data = std::static_pointer_cast<Event_Server_Sync_Enemy_State>( newEvent );
		SetEnemyState( data->ID(), (EnemyState)data->State() );
	}
	else if( newEvent->GetEventType() == Event_Server_Spawn_Ship::GUID )
	{
		std::shared_ptr<Event_Server_Spawn_Ship> data = std::static_pointer_cast<Event_Server_Spawn_Ship>( newEvent );

		if( data->TeamID() == mPlayer->GetTeam() )
		{
			mShips[FRIEND_SHIP] = new ClientShip();
			mShips[FRIEND_SHIP]->Initialize( data->ID(), data->TeamID(), data->Position(), data->Rotation(), data->Scale() );
		}
		else
		{
			mShips[ENEMY_SHIP] = new ClientShip();
			mShips[ENEMY_SHIP]->Initialize( data->ID(), data->TeamID(), data->Position(), data->Rotation(), data->Scale() );
		}	
	}

	
	else if( newEvent->GetEventType() == Event_Remote_Win::GUID )
	{
		std::shared_ptr<Event_Remote_Win> data = std::static_pointer_cast<Event_Remote_Win>( newEvent );
		if( data->Team() == mPlayer->GetTeam() )
		{
			mEndGame = true;
			mWonGame = true;
		}
		else
		{
			mEndGame = true;
			mWonGame = false;
		}
		/*IEventPtr E1( new Event_Reset_Game() );
		EventManager::GetInstance()->QueueEvent( E1 );*/
	}
	else if( newEvent->GetEventType() == Event_Server_Turret_Fired_Projectile::GUID )
	{
		// Fire projectile
		std::shared_ptr<Event_Server_Turret_Fired_Projectile> data = std::static_pointer_cast<Event_Server_Turret_Fired_Projectile>(newEvent);
		FireProjectile( data->ID(), data->ProjectileID(), data->TeamID(), data->Position(), data->Direction(), data->Speed(), data->Range(), 1.0f, TURRET ); // Don't know where to get damage from yet

		//TestSound
		SoundBufferHandler::GetInstance()->Play3D( mMiniGun , data->Position());
		
		// Request Muzzle Flash from Particle Manager
		
		/*RenderManager::GetInstance()->RequestParticleSystem( data->ID(), MuzzleFlash, data->Position(), data->Direction() );
		RenderManager::GetInstance()->RequestParticleSystem( data->ID(), Smoke_MiniGun, data->Position(), data->Direction() );	*/	
	}
	else if (newEvent->GetEventType() == Event_Server_Enemy_Fired_Projectile::GUID)
	{
		// Fire projectile
		std::shared_ptr<Event_Server_Enemy_Fired_Projectile> data = std::static_pointer_cast<Event_Server_Enemy_Fired_Projectile>(newEvent);
		FireProjectile(data->EnemyID(), data->ProjectileID(), 0, data->Position(), data->Direction(), data->Speed(), data->Range(), ENEMY_PROJECTILE_DAMAGE, TURRET); // Don't know where to get damage from yet
	}
	else if( newEvent->GetEventType() == Event_Server_Sync_Energy_Cell::GUID )
	{
		std::shared_ptr<Event_Server_Sync_Energy_Cell> data = std::static_pointer_cast<Event_Server_Sync_Energy_Cell>( newEvent );
		
		mEnergyCells[data->EnergyCellID()]->SetOwnerID( data->OwnerID() );
		mEnergyCells[data->EnergyCellID()]->SetPosition( data->Position() );
		mEnergyCells[data->EnergyCellID()]->SetPickedUp( data->PickedUp() );

		if( data->OwnerID() != (UINT)-1 )
		{
			for( auto& rp : mRemotePlayers )
			{
				if( rp->GetID() == data->OwnerID() )
				{
					rp->SetEnergyCellID( data->EnergyCellID() );
				}
			}
		}
		else
		{
			for( auto& rp : mRemotePlayers )
			{
				if( rp->GetEnergyCellID() == data->EnergyCellID() )
				{
					rp->SetEnergyCellID( (UINT)-1 );
				}
			}
		}
	}
	else if( newEvent->GetEventType() == Event_Server_XP::GUID )
	{
		std::shared_ptr<Event_Server_XP> data = std::static_pointer_cast<Event_Server_XP>( newEvent );
		if( data->PlayerID() == mPlayer->GetID() )
		{
			if( mPlayer->GetCurrentLevel() <= 16 )
			{
				int levelUp = mPlayer->Upgradable();
				mPlayer->AddXP( (float)data->XP() );
				if( mPlayer->Upgradable() != levelUp )
				{
					SoundBufferHandler::GetInstance()->Play3D( mLevelUp , mPlayer->GetBoundingCircle()->center );
					RenderManager::GetInstance()->RequestParticleSystem( mPlayer->GetID(), Level_Up, mPlayer->GetBoundingCircle()->center, XMFLOAT3( 0.0f, 1.0f, 0.0f ) ); //both of these calls are needed for levelup effect.
					RenderManager::GetInstance()->RequestParticleSystem( mPlayer->GetID(), Level_Inner, mPlayer->GetBoundingCircle()->center, XMFLOAT3( 0.1f, 0.1f, 0.1f ) ); //both of these calls are needed for levelup effect.
				}
			}
		}
	}
	else if( newEvent->GetEventType() == Event_Trigger_Client_Fired_Projectile::GUID )
	{
		std::shared_ptr<Event_Trigger_Client_Fired_Projectile> data = std::static_pointer_cast<Event_Trigger_Client_Fired_Projectile>( newEvent );
		IEventPtr E1( new Event_Client_Fired_Projectile( data->ID(), data->BodyPos(), data->Direction(), data->Speed(), data->Range(), data->Damage(), data->Weapon() ) );
		Client::GetInstance()->SendEvent( E1 );
	}
	else if( newEvent->GetEventType() == Event_Trigger_Client_Update::GUID )
	{
		std::shared_ptr<Event_Trigger_Client_Update> data = std::static_pointer_cast<Event_Trigger_Client_Update>( newEvent );
		IEventPtr E1( new Event_Client_Update( data->ID(), data->LowerBodyPos(), data->Velocity(), data->UpperBodyDirection(), data->Name(), data->IsBuffed(), data->IsAlive() ) );
		Client::GetInstance()->SendEvent( E1 );
	}
	else if( newEvent->GetEventType() == Event_Unlock_Player::GUID )
	{
		mPlayer->UnLock();
		SetCursor( mSight );
	}
	else if( newEvent->GetEventType() == Event_Reset_Game::GUID )
	{
		Reset();
	}
	else if( newEvent->GetEventType() == Event_Server_Switch_Team::GUID )
	{
		std::shared_ptr<Event_Server_Switch_Team> data = std::static_pointer_cast<Event_Server_Switch_Team>( newEvent );
		if ( data->ID() == mPlayer->GetID() )
		{
			std::swap( mShips[FRIEND_SHIP], mShips[ENEMY_SHIP] );
		}
	}
}

void PlayState::SyncEnemy( unsigned int id, EnemyState state, EnemyType type, XMFLOAT3 position, XMFLOAT3 direction, float maxHp )
{
	mEnemyListSynced = false;
	mEnemies[id]->SetID( id );
	mEnemies[id]->SetEnemyType( type );
	//mEnemies[id]->SetLoopAnimation( true );
	mEnemies[id]->SetModelID( mEnemyAnimationManager->GetModel( mEnemies[id]->GetEnemyType() ), mEnemyAnimationManager->GetDefaultAnimation( mEnemies[id]->GetEnemyType() ) );
	mEnemies[id]->SetAnimation( mEnemyAnimationManager->GetAnimation( mEnemies[id]->GetEnemyType(), state ) );
	mEnemies[id]->SetPosition( position );
	mEnemies[id]->SetDirection( direction );
	mEnemies[id]->SetMaxHP( maxHp );
	mEnemies[id]->SetSynced( true );

	if( id == (MAX_NR_OF_ENEMIES-1) )
		mEnemyListSynced = true;
}

void PlayState::SyncSpawn( unsigned int id, XMFLOAT3 position )
{
	mSpawners[id] = position;
}

// Tell server that local  player has taken damage
void PlayState::BroadcastProjectileDamage( unsigned int playerID, unsigned int projectileID )
{
	IEventPtr E1( new Event_Client_Damaged( playerID, projectileID ) );
	Client::GetInstance()->SendEvent( E1 );
}

void PlayState::BroadcastMeleeDamage( unsigned playerID, float damage, float knockBack, XMFLOAT3 direction )
{
	IEventPtr E1( new Event_Client_Melee_Hit( playerID, damage, knockBack, direction ) );
	Client::GetInstance()->SendEvent( E1 );
}

void PlayState::BroadcastEnemyMeleeDamage( unsigned enemyID, float damage, float knockBack, XMFLOAT3 direction )
{
	IEventPtr E1( new Event_Client_Enemy_Attack( mPlayer->GetID(), enemyID, damage, knockBack, direction, mPlayer->GetLoadOut()->meleeWeapon->stun ) );
	Client::GetInstance()->SendEvent( E1 );
}

void PlayState::BroadcastEnemyProjectileDamage( unsigned int shooterID, unsigned int projectileID, unsigned int enemyID, float damage )
{
	IEventPtr E1( new Event_Client_Projectile_Damage_Enemy( shooterID, projectileID, enemyID, damage ) );
	Client::GetInstance()->SendEvent( E1 );
}

void PlayState::FireProjectile( unsigned int id, unsigned int projectileID, unsigned int teamID, XMFLOAT3 position, XMFLOAT3 direction, float speed, float range, float damage, WeaponType weaponType )
{
	mProjectiles[mNrOfActiveProjectiles]->SetDirection( id, projectileID, teamID, position, direction, speed, range, damage, weaponType );
	mNrOfActiveProjectiles++;
}

void PlayState::CheckPlayerCollision()
{	
	if( mRemotePlayers.size() > 0 )
	{
		if( mRemotePlayers.at(0)->GetID() != 0 )
		{
			for ( size_t i = 0; i < mRemotePlayers.size(); i++ )
			{
				//if( mPlayer->GetBoundingBox()->Intersect( mRemotePlayers.at(i)->GetBoundingBox() ) )		// BoundingBox
				if ( mPlayer->IsAlive() && mPlayer->GetBoundingCircle()->Intersect( mRemotePlayers.at(i)->GetBoundingCircle() ) && mRemotePlayers.at(i)->IsAlive() )		// BoundingCircle
				{
					//Direction
					XMVECTOR remoteToPlayerVec = (XMLoadFloat3(&mPlayer->GetBoundingCircle()->center) -
						XMLoadFloat3(&mRemotePlayers.at(i)->GetBoundingCircle()->center));

					//Normalize direction
					remoteToPlayerVec = XMVector4Normalize(remoteToPlayerVec);

					//Length of new vector
					float vectorLength = (mRemotePlayers.at(i)->GetBoundingCircle()->radius + mPlayer->GetBoundingCircle()->radius) + 0.0001f;

					//New position of player			 
					XMVECTOR playerPosition = XMLoadFloat3(&mRemotePlayers.at(i)->GetBoundingCircle()->center) + remoteToPlayerVec * vectorLength;
					mPlayer->SetPosition(playerPosition);
				}
			}	
		}
	}
}

void PlayState::CheckProjectileCollision()
{
	for ( int i	 = 0; i < mNrOfActiveProjectiles; i++ )
	{	
		if( mProjectiles[i]->IsActive() )
		{
			// Enemy projectiles
			if ( mProjectiles[i]->GetPlayerID() == ENEMY_PROJECTILE_ID )
			{
				// Players
				if(	mPlayer->IsAlive() && mProjectiles[i]->GetBoundingCircle()->Intersect( mPlayer->GetBoundingCircle() ) )
				{
					mPlayer->TakeDamage(mProjectiles[i]->GetDamage(), mProjectiles[i]->GetPlayerID());
					IEventPtr E1( new Event_Client_Removed_Projectile( mProjectiles[i]->GetID() ) );
					Client::GetInstance()->SendEvent( E1 );
				}
			}
			else
			{
				// Players
				if( mPlayer->IsAlive() &&
					( mProjectiles[i]->GetPlayerID() == 70 || mProjectiles[i]->GetPlayerID() == 71 ) && 
					mPlayer->GetTeam() != mProjectiles[i]->GetTeamID() &&
					mProjectiles[i]->GetBoundingCircle()->Intersect( mPlayer->GetBoundingCircle() ) && mProjectiles[i]->GetWeaponType() != GRENADELAUNCHER )
				{
					mPlayer->TakeDamage( mProjectiles[i]->GetDamage(), mProjectiles[i]->GetPlayerID() );
					RenderManager::GetInstance()->RequestParticleSystem( mPlayer->GetID(), Spark, mProjectiles[i]->GetPosition(), XMFLOAT3( -mProjectiles[i]->GetDirection().x, mProjectiles[i]->GetDirection().y, -mProjectiles[i]->GetDirection().z ) );
					RenderManager::GetInstance()->RequestParticleSystem( mPlayer->GetID(), Debris, mProjectiles[i]->GetPosition(), XMFLOAT3( -mProjectiles[i]->GetDirection().x, mProjectiles[i]->GetDirection().y, -mProjectiles[i]->GetDirection().z ) );
				}
				for ( size_t j = 0; j < mRemotePlayers.size(); j++ )
				{
					if( mRemotePlayers[j]->IsAlive() && mProjectiles[i]->GetWeaponType() != GRENADELAUNCHER )
					{
						if( mProjectiles[i]->GetPlayerID() == mPlayer->GetID() &&
							mProjectiles[i]->GetBoundingCircle()->Intersect( mRemotePlayers[j]->GetBoundingCircle() ) )
						{
							if( mProjectiles[i]->GetWeaponType() != SNIPER )
							{
								BroadcastProjectileDamage( mRemotePlayers[j]->GetID(), mProjectiles[i]->GetID() );
								RenderManager::GetInstance()->RequestParticleSystem( mRemotePlayers[j]->GetID(), Spark, mProjectiles[i]->GetPosition(), XMFLOAT3( -mProjectiles[i]->GetDirection().x, mProjectiles[i]->GetDirection().y, -mProjectiles[i]->GetDirection().z ) );
								RenderManager::GetInstance()->RequestParticleSystem( mRemotePlayers[j]->GetID(), Debris, mProjectiles[i]->GetPosition(), XMFLOAT3( -mProjectiles[i]->GetDirection().x, mProjectiles[i]->GetDirection().y, -mProjectiles[i]->GetDirection().z ) );
							}
							if( mProjectiles[i]->GetWeaponType() != SNIPER )
							{
								BroadcastProjectileDamage( mRemotePlayers[j]->GetID(), mProjectiles[i]->GetID() );
								RenderManager::GetInstance()->RequestParticleSystem( mRemotePlayers[j]->GetID(), Spark, mProjectiles[i]->GetPosition(), XMFLOAT3( -mProjectiles[i]->GetDirection().x, mProjectiles[i]->GetDirection().y, -mProjectiles[i]->GetDirection().z ) );
								RenderManager::GetInstance()->RequestParticleSystem( mRemotePlayers[j]->GetID(), Debris, mProjectiles[i]->GetPosition(), XMFLOAT3( -mProjectiles[i]->GetDirection().x, mProjectiles[i]->GetDirection().y, -mProjectiles[i]->GetDirection().z ) );
							}
							else
							{
								if( mProjectiles[i]->SetHit( mRemotePlayers[j]->GetID() ) )
								{
									XMFLOAT3 pos = mProjectiles[i]->GetPosition();
									RenderManager::GetInstance()->RequestParticleSystem( mProjectiles[i]->GetPlayerID(), Spark, pos, XMFLOAT3( mProjectiles[i]->GetDirection().x, mProjectiles[i]->GetDirection().y, mProjectiles[i]->GetDirection().z ) );
									RenderManager::GetInstance()->RequestParticleSystem( mProjectiles[i]->GetPlayerID(), Debris, pos, XMFLOAT3( mProjectiles[i]->GetDirection().x, mProjectiles[i]->GetDirection().y, mProjectiles[i]->GetDirection().z ) );
									pos = XMFLOAT3( mProjectiles[i]->GetPosition().x + mProjectiles[i]->GetDirection().x, mProjectiles[i]->GetPosition().y + mProjectiles[i]->GetDirection().y, mProjectiles[i]->GetPosition().z + mProjectiles[i]->GetDirection().z );
									RenderManager::GetInstance()->RequestParticleSystem( mProjectiles[i]->GetPlayerID(), Spark, pos, XMFLOAT3( mProjectiles[i]->GetDirection().x, mProjectiles[i]->GetDirection().y, mProjectiles[i]->GetDirection().z ) );
									RenderManager::GetInstance()->RequestParticleSystem( mProjectiles[i]->GetPlayerID(), Debris, pos, XMFLOAT3( mProjectiles[i]->GetDirection().x, mProjectiles[i]->GetDirection().y, mProjectiles[i]->GetDirection().z ) );
									pos = XMFLOAT3( mProjectiles[i]->GetPosition().x + mProjectiles[i]->GetDirection().x * 2, mProjectiles[i]->GetPosition().y + mProjectiles[i]->GetDirection().y, mProjectiles[i]->GetPosition().z + mProjectiles[i]->GetDirection().z * 2 );							
									RenderManager::GetInstance()->RequestParticleSystem( mProjectiles[i]->GetPlayerID(), Spark, pos, XMFLOAT3( mProjectiles[i]->GetDirection().x, mProjectiles[i]->GetDirection().y, mProjectiles[i]->GetDirection().z ) );
									RenderManager::GetInstance()->RequestParticleSystem( mProjectiles[i]->GetPlayerID(), Debris, pos, XMFLOAT3( mProjectiles[i]->GetDirection().x, mProjectiles[i]->GetDirection().y, mProjectiles[i]->GetDirection().z ) );
									pos = XMFLOAT3( mProjectiles[i]->GetPosition().x + mProjectiles[i]->GetDirection().x * 0.5f, mProjectiles[i]->GetPosition().y + mProjectiles[i]->GetDirection().y, mProjectiles[i]->GetPosition().z + mProjectiles[i]->GetDirection().z * 0.5f );
									RenderManager::GetInstance()->RequestParticleSystem( mProjectiles[i]->GetPlayerID(), Spark, pos, XMFLOAT3( mProjectiles[i]->GetDirection().x, mProjectiles[i]->GetDirection().y, mProjectiles[i]->GetDirection().z ) );
									RenderManager::GetInstance()->RequestParticleSystem( mProjectiles[i]->GetPlayerID(), Debris, pos, XMFLOAT3( mProjectiles[i]->GetDirection().x, mProjectiles[i]->GetDirection().y, mProjectiles[i]->GetDirection().z ) );
									BroadcastProjectileDamage( mRemotePlayers[j]->GetID(), mProjectiles[i]->GetID() );
								}
							}
							break;
						}
					}
				}

				// Enemies
				if( mProjectiles[i]->GetWeaponType() != GRENADELAUNCHER )
				{
					for ( size_t j = 0; j < MAX_NR_OF_ENEMIES; j++ )
					{
						if( mEnemies[j]->IsAlive() )
						{
							if( mPlayer->GetID() == 1 &&
								( mProjectiles[i]->GetPlayerID() == 70 || mProjectiles[i]->GetPlayerID() == 71 ) &&
								mProjectiles[i]->GetBoundingCircle()->Intersect( mEnemies[j]->GetBoundingCircle() ) )
							{
								// hit
								BroadcastEnemyProjectileDamage( mProjectiles[i]->GetPlayerID(), mProjectiles[i]->GetID(), mEnemies[j]->GetID(), mProjectiles[i]->GetDamage() );
								RenderManager::GetInstance()->RequestParticleSystem( mProjectiles[i]->GetPlayerID(), Blood, mProjectiles[i]->GetPosition(), XMFLOAT3( -mProjectiles[i]->GetDirection().x, mProjectiles[i]->GetDirection().y, -mProjectiles[i]->GetDirection().z ) );
								mProjectiles[i]->Reset();
								break;
							}

							if( mProjectiles[i]->GetPlayerID() == mPlayer->GetID() &&
								mProjectiles[i]->GetBoundingCircle()->Intersect( mEnemies[j]->GetBoundingCircle() ) )
							{
								if( mProjectiles[i]->GetWeaponType() == TURRET )
								{
									RenderManager::GetInstance()->RequestParticleSystem( mProjectiles[i]->GetPlayerID(), Blood, mProjectiles[i]->GetPosition(), XMFLOAT3( -mProjectiles[i]->GetDirection().x, mProjectiles[i]->GetDirection().y, -mProjectiles[i]->GetDirection().z ) );
									BroadcastEnemyProjectileDamage( mProjectiles[i]->GetPlayerID(), mProjectiles[i]->GetID(), mEnemies[j]->GetID(), mProjectiles[i]->GetDamage() );
									mProjectiles[i]->Reset();
								}
								else if( mProjectiles[i]->GetWeaponType() != SNIPER )
								{
									RenderManager::GetInstance()->RequestParticleSystem( mProjectiles[i]->GetPlayerID(), Blood, mProjectiles[i]->GetPosition(), XMFLOAT3( -mProjectiles[i]->GetDirection().x, mProjectiles[i]->GetDirection().y, -mProjectiles[i]->GetDirection().z ) );
									BroadcastEnemyProjectileDamage( mProjectiles[i]->GetPlayerID(), mProjectiles[i]->GetID(), mEnemies[j]->GetID(), mProjectiles[i]->GetDamage() );
									mProjectiles[i]->Reset();
								}
								else
								{
									if( mProjectiles[i]->SetHit( mEnemies[j]->GetID() ) )
									{
										XMFLOAT3 pos = mProjectiles[i]->GetPosition();
										RenderManager::GetInstance()->RequestParticleSystem( mProjectiles[i]->GetPlayerID(), Blood, pos, XMFLOAT3( mProjectiles[i]->GetDirection().x, mProjectiles[i]->GetDirection().y, mProjectiles[i]->GetDirection().z ) );
										pos = XMFLOAT3( mProjectiles[i]->GetPosition().x + mProjectiles[i]->GetDirection().x, mProjectiles[i]->GetPosition().y + mProjectiles[i]->GetDirection().y, mProjectiles[i]->GetPosition().z + mProjectiles[i]->GetDirection().z );
										RenderManager::GetInstance()->RequestParticleSystem( mProjectiles[i]->GetPlayerID(), Blood, pos, XMFLOAT3( mProjectiles[i]->GetDirection().x, mProjectiles[i]->GetDirection().y, mProjectiles[i]->GetDirection().z ) );
										pos = XMFLOAT3( mProjectiles[i]->GetPosition().x + mProjectiles[i]->GetDirection().x * 2, mProjectiles[i]->GetPosition().y + mProjectiles[i]->GetDirection().y, mProjectiles[i]->GetPosition().z + mProjectiles[i]->GetDirection().z * 2 );							
										RenderManager::GetInstance()->RequestParticleSystem( mProjectiles[i]->GetPlayerID(), Blood, pos, XMFLOAT3( mProjectiles[i]->GetDirection().x, mProjectiles[i]->GetDirection().y, mProjectiles[i]->GetDirection().z ) );
										pos = XMFLOAT3( mProjectiles[i]->GetPosition().x + mProjectiles[i]->GetDirection().x * 0.5f, mProjectiles[i]->GetPosition().y + mProjectiles[i]->GetDirection().y, mProjectiles[i]->GetPosition().z + mProjectiles[i]->GetDirection().z * 0.5f );
										RenderManager::GetInstance()->RequestParticleSystem( mProjectiles[i]->GetPlayerID(), Blood, pos, XMFLOAT3( mProjectiles[i]->GetDirection().x, mProjectiles[i]->GetDirection().y, mProjectiles[i]->GetDirection().z ) );
										BroadcastEnemyProjectileDamage( mProjectiles[i]->GetPlayerID(), mProjectiles[i]->GetID(), mEnemies[j]->GetID(), mProjectiles[i]->GetDamage() );
									}
								}
								break;
							}
						}
					}
				}

				// Environment
				if( mProjectiles[i]->GetPlayerID() == mPlayer->GetID() || ( ( mProjectiles[i]->GetPlayerID() == 70 || mProjectiles[i]->GetPlayerID() == 71 ) && mPlayer->GetID() == 1 ) )
				{
					XMFLOAT3 normal;
					if( mWorldMap->BulletVsMap( mProjectiles[i]->GetPosition(), normal ) )
					{
						IEventPtr E1( new Event_Client_Removed_Projectile( mProjectiles[i]->GetID() ) );
						Client::GetInstance()->SendEvent( E1 );
					
					if( mProjectiles[i]->GetWeaponType() == GRENADELAUNCHER )
					{
						RenderManager::GetInstance()->RequestParticleSystem( mPlayer->GetID(), Explosion, mProjectiles[i]->GetPosition(), XMFLOAT3( 1.0f, 1.0f, 1.0f ) );
						RenderManager::GetInstance()->RequestParticleSystem( mPlayer->GetID(), ExplosionSmoke, mProjectiles[i]->GetPosition(), XMFLOAT3( 1.0f, 1.0f, 1.0f ) );
						SoundBufferHandler::GetInstance()->Play3D( mExplosion , mPlayer->GetPosition() );
					}
					else
					{
						RenderManager::GetInstance()->RequestParticleSystem( mPlayer->GetID(), Spark, mProjectiles[i]->GetPosition(), XMFLOAT3( -mProjectiles[i]->GetDirection().x, mProjectiles[i]->GetDirection().y, -mProjectiles[i]->GetDirection().z ) );
						RenderManager::GetInstance()->RequestParticleSystem( mPlayer->GetID(), Debris, mProjectiles[i]->GetPosition(), XMFLOAT3( -mProjectiles[i]->GetDirection().x, mProjectiles[i]->GetDirection().y, -mProjectiles[i]->GetDirection().z ) );
					}
					}
				}

				if( mProjectiles[i]->GetWeaponType() == GRENADELAUNCHER )
				{
					if( mProjectiles[i]->GetPosition().y <= 0.5f )
					{
						for (size_t j = 0; j < MAX_NR_OF_ENEMIES; j++)
						{
							if( mEnemies[j]->IsAlive() )
							{
								if( BoundingCircle( mProjectiles[i]->GetPosition(), mPlayer->GetLoadOut()->rangedWeapon->areaOfEffect ).Intersect( mEnemies[j]->GetBoundingCircle() ) )
								{
									// Hit
									BroadcastEnemyProjectileDamage( mProjectiles[i]->GetPlayerID(), mProjectiles[i]->GetID(), mEnemies[j]->GetID(), mProjectiles[i]->GetDamage() );
									RenderManager::GetInstance()->RequestParticleSystem( mProjectiles[i]->GetPlayerID(), Blood, mProjectiles[i]->GetPosition(), XMFLOAT3( -mProjectiles[i]->GetDirection().x, mProjectiles[i]->GetDirection().y, -mProjectiles[i]->GetDirection().z ) );
								}
							}
						}

						// Player is hit by remote player grenade
						if( mPlayer->IsAlive() && mPlayer->GetTeam() != mProjectiles[i]->GetTeamID() )
						{
							for (size_t j = 0; j < mRemotePlayers.size(); j++)
							{
								if( mProjectiles[i]->GetPlayerID() == mRemotePlayers[j]->GetID() )
								{
									if( BoundingCircle( mProjectiles[i]->GetPosition(), mRemotePlayers[j]->GetLoadOut()->rangedWeapon->areaOfEffect ).Intersect( mPlayer->GetBoundingCircle() ) )
									{
										mPlayer->TakeDamage( mProjectiles[i]->GetDamage(), mProjectiles[i]->GetPlayerID() );
										RenderManager::GetInstance()->RequestParticleSystem( mPlayer->GetID(), Spark, mProjectiles[i]->GetPosition(), XMFLOAT3( -mProjectiles[i]->GetDirection().x, mProjectiles[i]->GetDirection().y, -mProjectiles[i]->GetDirection().z ) );
										RenderManager::GetInstance()->RequestParticleSystem( mPlayer->GetID(), Debris, mProjectiles[i]->GetPosition(), XMFLOAT3( -mProjectiles[i]->GetDirection().x, mProjectiles[i]->GetDirection().y, -mProjectiles[i]->GetDirection().z ) );
									}
								}
							}						
						}

						// Remote player is hit by Player grenade
						if( mRemotePlayers.size() > 0 )
						{
							for (size_t j = 0; j < mRemotePlayers.size(); j++)
							{
								if( mRemotePlayers[j]->IsAlive() )
								{
									if( BoundingCircle( mProjectiles[i]->GetPosition(), mPlayer->GetLoadOut()->rangedWeapon->areaOfEffect ).Intersect( mRemotePlayers[j]->GetBoundingCircle() ) )
									{
										// Hit
										BroadcastProjectileDamage(mRemotePlayers[j]->GetID() , mProjectiles[i]->GetID() );
										RenderManager::GetInstance()->RequestParticleSystem( mProjectiles[i]->GetPlayerID(), Blood, mProjectiles[i]->GetPosition(), XMFLOAT3( -mProjectiles[i]->GetDirection().x, mProjectiles[i]->GetDirection().y, -mProjectiles[i]->GetDirection().z ) );
									}
								}
							}
						}

						RenderManager::GetInstance()->RequestParticleSystem( mPlayer->GetID(), Explosion, mProjectiles[i]->GetPosition(), XMFLOAT3( 1.0f, 1.0f, 1.0f ) );
						RenderManager::GetInstance()->RequestParticleSystem( mPlayer->GetID(), ExplosionSmoke, mProjectiles[i]->GetPosition(), XMFLOAT3( 1.0f, 1.0f, 1.0f ) );
						SoundBufferHandler::GetInstance()->Play3D( mExplosion , mPlayer->GetPosition() );

						IEventPtr E1( new Event_Client_Removed_Projectile( mProjectiles[i]->GetID() ) );
						Client::GetInstance()->SendEvent( E1 );

					}
				}
			}
		}
	}
}

void PlayState::CheckMeeleCollision()
{

	MeleeInfo* currWeapon = mPlayer->GetLoadOut()->meleeWeapon;
	XMVECTOR meeleRadiusVector =  ( XMLoadFloat3( &mPlayer->GetUpperBodyDirection() ) * currWeapon->radius );

	float halfRadian = XMConvertToRadians( currWeapon->spread * 18.0f ) * 0.5f;
	XMFLOAT3 pos = currWeapon->boundingCircle->center;

	for ( size_t i = 0; i < mRemotePlayers.size(); i++ )
	{
		//Check intersection with melee circle & remotePlayer
		if( mRemotePlayers[i]->GetTeam() != mPlayer->GetTeam() && mRemotePlayers[i]->IsAlive() && currWeapon->boundingCircle->Intersect( mRemotePlayers.at(i)->GetBoundingCircle() ) )
		{
			//Spread to Radians
			float angleRemoteToAim = 0.0f;
			XMVECTOR playerToCenter = XMLoadFloat3( &mRemotePlayers.at(i)->GetBoundingCircle()->center ) - XMLoadFloat3( &pos );
			XMStoreFloat( &angleRemoteToAim, XMVector3AngleBetweenVectors( playerToCenter, meeleRadiusVector ) );

			if( angleRemoteToAim <= halfRadian )
			{
				DirectX::XMFLOAT3 direction = DirectX::XMFLOAT3( 0.0f, 0.0f, 0.0f );
				XMStoreFloat3( &direction, XMVector4Normalize( XMLoadFloat3( &mPlayer->GetUpperBodyDirection() ) ) );
				BroadcastMeleeDamage( mRemotePlayers.at(i)->GetID(), mPlayer->GetLoadOut()->meleeWeapon->damage, currWeapon->knockBack, direction );
			}
		}
	}

	for ( size_t i = 0; i < MAX_NR_OF_ENEMIES; i++ )
	{
		//Check intersection with melee circle & enemy
		if( mEnemies[i]->IsAlive() && currWeapon->boundingCircle->Intersect( mEnemies[i]->GetBoundingCircle() ) )
		{
			float angleRemoteToAim = 0.0f;
			XMVECTOR playerToCenter = XMLoadFloat3( &mEnemies[i]->GetBoundingCircle()->center ) - XMLoadFloat3( &pos );
			XMStoreFloat( &angleRemoteToAim, XMVector4AngleBetweenVectors( playerToCenter, meeleRadiusVector ) );

			if( angleRemoteToAim <= halfRadian )
			{
				DirectX::XMFLOAT3 direction = DirectX::XMFLOAT3( 0.0f, 0.0f, 0.0f );
				XMStoreFloat3( &direction, XMVector4Normalize( XMLoadFloat3( &mPlayer->GetUpperBodyDirection() ) ) );
				BroadcastEnemyMeleeDamage( mEnemies[i]->GetID(), currWeapon->damage, currWeapon->knockBack, direction );
			}
		}
	}
}

void PlayState::HandleDeveloperCameraInput()
{
	// TOGGLE CAM
	if( Input::GetInstance()->IsKeyPressed( KEYS::KEYS_RCTRL ) )
		Graphics::GetInstance()->ChangeCamera();
	// ZOOM IN
	if( Input::GetInstance()->IsKeyDown( KEYS::KEYS_DOWN ) )
		Graphics::GetInstance()->ZoomOutDeveloperCamera();
	// ZOOM OUT
	if( Input::GetInstance()->IsKeyDown( KEYS::KEYS_UP) )
		Graphics::GetInstance()->ZoomInDeveloperCamera();

	if( Input::GetInstance()->IsKeyDown( KEYS::KEYS_O ) )
	{
		mPlayer->AddXP( 3000.0f );
		if( mShips[FRIEND_SHIP]->Intersect( mPlayer->GetBoundingCircle() ) )
		{
			IEventPtr E1( new Event_Client_Win( mPlayer->GetTeam() ) );
			Client::GetInstance()->SendEvent( E1 );
		}
	}
	if( Input::GetInstance()->IsKeyPressed( KEYS::KEYS_E ) )
	{
		if( mShips[FRIEND_SHIP]->Intersect( mPlayer->GetBoundingCircle() ) )
		{
			if( mGui->UpgradeShipWindowIsActive() )
			{
				mPlayer->UnLock();
				mGui->DeActivateUpgradeShipWindow();
				mGui->DeActivateUpgradePlayerWindow();
			}
			else if( mPlayer->IsAlive() )
			{
				SetCursor( mCursor );
				mPlayer->Lock();
				mGui->ActivateUpgradeShipWindow();
				mGui->ActivateUpgradePlayerWindow();
			}
		}
		else if( mGui->EnergyCellsActive() )
		{
			mGui->DeActivateEnergyCellsShowing();
		}
		else
		{
			mGui->ActivateEnergyCellsShowing();
		}
	}
	if( Input::GetInstance()->IsKeyPressed( KEYS::KEYS_ESCAPE ) )
	{
		if( mGui->InGameWindowIsActive() )
		{
			mPlayer->UnLock();
			mGui->DeActivateInGameWindow();
		}
		else
		{
			SetCursor( mCursor );
			mPlayer->Lock();
			mGui->ActivateInGameWindow();
		}
	}
	if( Input::GetInstance()->IsKeyDown( KEYS::KEYS_1 ) )
	{
		RenderManager::GetInstance()->ChangeRasterizerState( CULL_NONE );

		IEventPtr E1( new Event_Client_Change_Ship_Levels( mShips[FRIEND_SHIP]->GetID(), 0, -1, 0, 0 ) );
		Client::GetInstance()->SendEvent( E1 );
	}
	if( Input::GetInstance()->IsKeyDown( KEYS::KEYS_2 ) )
		RenderManager::GetInstance()->ChangeRasterizerState( CULL_BACK );
	if( Input::GetInstance()->IsKeyDown( KEYS::KEYS_3 ) )
		RenderManager::GetInstance()->ChangeRasterizerState( WIREFRAME );
}

void PlayState::HandleRemoteProjectileHit( unsigned int id, unsigned int projectileID )
{
	unsigned int	shooter = 0;
	float			damage	= 0.0f;
	for ( int i = 0; i < mNrOfActiveProjectiles; i++ )
	{
		if( mProjectiles[i]->GetID() == projectileID )
		{
			shooter = mProjectiles[i]->GetPlayerID();
			damage	= mProjectiles[i]->GetDamage();
			if( mProjectiles[i]->GetWeaponType() != SNIPER )
			{
				mProjectiles[i]->Reset();
				Projectile* temp							= mProjectiles[mNrOfActiveProjectiles - 1];
				mProjectiles[mNrOfActiveProjectiles - 1]	= mProjectiles[i];
				mProjectiles[i]								= temp;
				mNrOfActiveProjectiles--;
			}
		}
	}

	if( mPlayer->GetID() == id )
	{
		// Projectile damage
		for ( size_t i = 0; i < mRemotePlayers.size(); i++ )
		{
			if( mRemotePlayers.at(i)->GetID() == shooter )
			{
				if( mRemotePlayers.at(i)->GetTeam() != mPlayer->GetTeam() )
				{
					mPlayer->TakeDamage( damage, shooter );
				}
			}
		}
	}
}

void PlayState::HandleRemoteProjectileRemoved( UINT projectileID )
{
	for ( int i = 0; i < mNrOfActiveProjectiles; i++ )
	{
		if( mProjectiles[i]->GetID() == projectileID )
		{
			mProjectiles[i]->Reset();
			Projectile* temp							= mProjectiles[mNrOfActiveProjectiles - 1];
			mProjectiles[mNrOfActiveProjectiles - 1]	= mProjectiles[i];
			mProjectiles[i]								= temp;
			mNrOfActiveProjectiles--;
		}
	}
}

void PlayState::UpdateProjectiles( float deltaTime )
{
	for ( int i = 0; i < mNrOfActiveProjectiles; i++ )
	{
		if( mProjectiles[i]->IsActive() )
		{
			mProjectiles[i]->Update( deltaTime );
		}
		else
		{
			Projectile* temp							= mProjectiles[mNrOfActiveProjectiles - 1];
			mProjectiles[mNrOfActiveProjectiles - 1]	= mProjectiles[i];
			mProjectiles[i]								= temp;
			mProjectiles[mNrOfActiveProjectiles - 1]->Reset();
			mProjectiles[i]->Update( deltaTime );
			mNrOfActiveProjectiles--;
		}
	}
}

void PlayState::UpdateEnemyPosition( unsigned int id, XMFLOAT3 position, XMFLOAT3 direction, bool isAlive, float enemyHP )
{
	mEnemies[id]->SetPosition( position );
	mEnemies[id]->SetDirection( direction );
	mEnemies[id]->SetIsAlive( isAlive );
	mEnemies[id]->SetHP( enemyHP );
}

void PlayState::RenderProjectiles()
{
	for ( int i = 0; i < mNrOfActiveProjectiles; i++ )
	{
		if( CullEntity( mProjectiles[i]->GetPosition() ) )
			mProjectiles[i]->Render();
	}
}

void PlayState::SetEnemyState( unsigned int id, EnemyState state )
{
	if( state == Death || state == Attack )
	{
		mEnemies[id]->SetLoopAnimation( false );
		mEnemies[id]->SetIsAlive( false );

	}
	else
	{
		mEnemies[id]->SetLoopAnimation( true );
		mEnemies[id]->SetIsAlive( true );
	}

	mEnemies[id]->SetAnimation( mEnemyAnimationManager->GetAnimation( mEnemies[id]->GetEnemyType(), state ), state == TakeDamage );
}

bool PlayState::CullEntity( XMFLOAT3 entityPos )
{
	return HelperFunctions::Dist3Squared( mPlayer->GetPosition(), entityPos  ) <= ENTITY_CULLDISTANCE;
}
void PlayState::WriteInteractionText( std::string text, float xPos, float yPos, float scale, XMFLOAT4 color )
{
	float offset = mFont.GetMiddleXPoint( text, scale );
	mFont.WriteText( text, xPos - offset, yPos, scale, COLOR_CYAN );
}

///////////////////////////////////////////////////////////////////////////////
//									PUBLIC
///////////////////////////////////////////////////////////////////////////////

HRESULT PlayState::Update( float deltaTime )
{
	if( mActive )
	{
		if( mGui->InGameWindowIsActive() || mGui->UpgradeShipWindowIsActive() || mGui->UpgradePlayerWindowIsActive() )
		{
			SetCursor( mCursor );
		}
		else
		{
			SetCursor( mSight );
		}

		//Fps update
		mFPS = mFPS * 0.1f + 0.9f / deltaTime;
		HandleDeveloperCameraInput();

		if( mFrameCounter >= COLLISION_CHECK_OFFSET )
		{
			CheckPlayerCollision();

			if( mPlayer->GetIsMeleeing() )
			{
				CheckMeeleCollision();
				mPlayer->SetIsMeleeing( false );
			}

			mFrameCounter = 0;
		}
		else
		{
			mFrameCounter++;
		}

		GuiUpdate guiUpdate;
		UINT nrOfRadarObj = 0;
		PlayerName pName[MAX_REMOTE_PLAYERS - 1];
		int nrOfAllies				= 0;
		guiUpdate.mNrOfPlayerNames	= (UINT)mRemotePlayers.size();
		guiUpdate.mPlayerTeamID		= mPlayer->GetTeam();

		for ( size_t i = 0; i < mRemotePlayers.size(); i++)
		{
			if ( mRemotePlayers.at(i) )
			{
				mRemotePlayers.at(i)->Update( deltaTime );

				std::string remotePlayerName = "";
				if( mRemotePlayers[i]->IsAlive() )
				{
					remotePlayerName							= mRemotePlayers[i]->GetName();
					mRadarObjects[nrOfRadarObj].mRadarObjectPos = mRemotePlayers[i]->GetPosition();

					if( mRemotePlayers[i]->GetEnergyCellID() != (UINT)-1 )
					{
						mRadarObjects[nrOfRadarObj++].mType	= RADAR_TYPE::PICKED_UP;
					}
					else
					{
						if( mRemotePlayers[i]->GetTeam() == mPlayer->GetTeam() )
						{
							mRadarObjects[nrOfRadarObj++].mType	= RADAR_TYPE::FRIENDLY;
						}
						else
						{
							mRadarObjects[nrOfRadarObj++].mType	= RADAR_TYPE::HOSTILE;
						}
					}
				}
				pName[i].mRemotePlayerPos		= mRemotePlayers[i]->GetPosition();
				pName[i].mRemotePlayerName		= remotePlayerName;
				pName[i].mRemotePlayerTeamID	= mRemotePlayers[i]->GetTeam();
				pName[i].mRemotePlayerID		= i;

				if( mRemotePlayers[i]->GetTeam() == mPlayer->GetTeam() )
				{
					mAlliesHP[nrOfAllies] = (float)( mRemotePlayers[i]->GetHP() / mRemotePlayers[i]->GetMaxHP() );
					nrOfAllies++;
				}
			}
		}

		guiUpdate.mPlayerNames	= pName;
		guiUpdate.mNrOfAllies	= nrOfAllies;
		guiUpdate.mAlliesHP		= mAlliesHP;

		if( mShips[FRIEND_SHIP] )
			guiUpdate.mShipHP	= mShips[FRIEND_SHIP]->PercentHP();
		else
			guiUpdate.mShipHP	= 1.0f;


		if( mPlayer->GetEnergyCellID() != (UINT)-1 )
		{
			if( mShips[FRIEND_SHIP]->Intersect( mPlayer->GetBoundingCircle() ) )
			{
				mPlayer->GiveEnergyCellToShip( mEnergyCells, mShips[FRIEND_SHIP]->GetID(), mShips[FRIEND_SHIP]->GetPos() );
				mShips[FRIEND_SHIP]->AddEnergyCell( mShips[FRIEND_SHIP]->GetID() );
			}
		}
		mPlayer->UpdateSpecific( deltaTime, mWorldMap, mRemotePlayers, mEnergyCells );


		// Enemies
		if( mEnemyListSynced )
		{
			for ( size_t i = 0; i < MAX_NR_OF_ENEMIES; i++ )
			{
				if( mEnemies[i]->IsSynced() )
				{
					mEnemies[i]->Update( deltaTime );

					if( mEnemies[i]->IsAlive() )
					{
						mRadarObjects[nrOfRadarObj].mRadarObjectPos = mEnemies[i]->GetPosition();
						mRadarObjects[nrOfRadarObj++].mType = RADAR_TYPE::HOSTILE;
					}
				}
			}
		}

		/// Spore particles
		RenderManager::GetInstance()->RequestParticleSystem( 997, Spores, XMFLOAT3( mPlayer->GetPlayerPosition().x, mPlayer->GetPlayerPosition().y + 2.5f, mPlayer->GetPlayerPosition().z ), XMFLOAT3( 0.0f, 1.0f, 0.0f ) );

		if( mPlayer->Upgradable() < 1 && mGui->UpgradePlayerWindowIsActive() )
		{
			mGui->DeActivateUpgradePlayerWindow();
		
			if( !mGui->UpgradeShipWindowIsActive() && !mGui->InGameWindowIsActive() )
			{
				mPlayer->UnLock();
			}
		}

		for( int i = 0; i < SHIP_AMOUNT; i++ )
		{
			if( mShips[i] )
			{
				mShips[i]->Update( deltaTime );
				mRadarObjects[nrOfRadarObj].mRadarObjectPos = mShips[i]->GetPos();
				if( i == FRIEND_SHIP )
				{
					mRadarObjects[nrOfRadarObj++].mType = RADAR_TYPE::SHIP_FRIENDLY;
				}
				else
				{
					mRadarObjects[nrOfRadarObj++].mType = RADAR_TYPE::SHIP_HOSTILE;
				}
			}
		}

		//No need to update the first energy cell since it's not supposed to be active
		for( int i = 1; i < MAX_ENERGY_CELLS; i++ )
		{
			if( !mEnergyCells[i]->GetPickedUp() )
			{
				mRadarObjects[nrOfRadarObj].mRadarObjectPos = mEnergyCells[i]->GetPosition();
				mRadarObjects[nrOfRadarObj++].mType			= RADAR_TYPE::OBJECTIVE;
			}
			else
			{
				if( mEnergyCells[i]->GetOwnerID() == mPlayer->GetID() )
				{
					mEnergyCells[i]->SetPosition( mPlayer->GetPosition() );
				}
				else
				{
					for( auto& p : mRemotePlayers )
					{
						if( mEnergyCells[i]->GetOwnerID() == p->GetID() )
						{
							mEnergyCells[i]->SetPosition( p->GetPosition() );
						}
					}
				}
			}
			mEnergyCells[i]->Update( deltaTime );
		}

		CheckProjectileCollision();
		UpdateProjectiles( deltaTime );

		//GUI UPDATE ANYTHING RELATED TO IT NEEDS TO PUT ABOVE THIS COMMENT
		////////////////////////////////////////////////////////////////////////////////////////////
		guiUpdate.mRadarObjects	= mRadarObjects;
		guiUpdate.mNrOfObjects	= nrOfRadarObj;
		guiUpdate.mPlayerPos	= mPlayer->GetPlayerPosition();	

		guiUpdate.mPlayerHP		= (float)( mPlayer->GetHP() / mPlayer->GetMaxHP() );
		guiUpdate.mPlayerXP		= mPlayer->GetXPToNext();
		guiUpdate.mLevel		= mPlayer->Upgradable();
	
		guiUpdate.deltaTime = deltaTime;

		if( mGui->CheckWin() )
		{
			guiUpdate.mEndGame = true;
			guiUpdate.mWonGame = true;
			mEndGame = true;

			IEventPtr E1( new Event_Client_Win( mPlayer->GetTeam() ) );
			Client::GetInstance()->SendEvent( E1 );
		}
		else if( mShips[FRIEND_SHIP] && mShips[FRIEND_SHIP]->CheckLose() )
		{
			guiUpdate.mEndGame = true;
			guiUpdate.mWonGame = false;
			mEndGame = true;

			UINT winTeam = 0;
			if( mPlayer->GetTeam() == 1 )
			{
				winTeam = 2;
			}
			else
			{
				winTeam = 1;
			}
			IEventPtr E1( new Event_Client_Win( winTeam ) );
			Client::GetInstance()->SendEvent( E1 );
		}
		else if( !mEndGame )
		{
			guiUpdate.mEndGame = mEndGame;
			guiUpdate.mWonGame = mWonGame;
		}
		else
		{
			guiUpdate.mEndGame = mEndGame;
			guiUpdate.mWonGame = mWonGame;
		}

		mGui->Update( guiUpdate );
		////////////////////////////////////////////////////////////////////////////////////////////


		while( !mPlayer->gEventList.empty() )
		{
			Client::GetInstance()->SendEvent( mPlayer->gEventList.back() );
			mPlayer->gEventList.pop_back();
		}
		while( !mGui->gEventList.empty() )
		{
			Client::GetInstance()->SendEvent( mGui->gEventList.back() );
			mGui->gEventList.pop_back();
		}
	}

	return S_OK;
}

HRESULT PlayState::Render( float deltaTime )
{
	mPlayer->Render( 0.0f, 1 );

	mWorldMap->Render( deltaTime , mPlayer );

	for ( size_t i = 0; i < mRemotePlayers.size(); i++)
	{
		if ( mRemotePlayers.at(i) && CullEntity( mRemotePlayers.at(i)->GetPosition() ) )
		{
			//RenderManager::GetInstance()->AddCircleToList( mRemotePlayers.at(i)->GetBoundingCircle()->center, XMFLOAT3( 0,1,1), mRemotePlayers.at(i)->GetBoundingCircle()->radius );
			mRemotePlayers.at(i)->Render();
		}
	}

	RenderProjectiles();

	// Enemies
  	if( mEnemyListSynced )
	{
		for ( size_t i = 0; i < MAX_NR_OF_ENEMIES; i++ )
		{
			if( mEnemies[i]->IsSynced() && CullEntity( mEnemies[i]->GetPosition() ) )
			{
				//RenderManager::GetInstance()->AddCircleToList( mEnemies[i]->GetBoundingCircle()->center, XMFLOAT3( 1,0,0), mEnemies[i]->GetBoundingCircle()->radius );
				mEnemies[i]->Render();
			}
		}
	}

	/*for (size_t i = 0; i < MAX_NR_OF_ENEMY_SPAWNERS; i++)
	{
		RenderManager::GetInstance()->AddObject3dToList( mSpawnModel, mSpawners[i] );
	}*/

	mGui->Render();

	for( int i = 1; i < MAX_ENERGY_CELLS; i++ )
	{
		if( !mEnergyCells[i]->GetSecured() && CullEntity( mEnergyCells[i]->GetPosition() ) )
		{
			mEnergyCells[i]->Render();
		}
	}

	//RENDER DEVTEXT
	std::string textToWrite = "FPS\t" + std::to_string( (int)mFPS ) + "\nRemotePlayers\t" + std::to_string( mRemotePlayers.size() ) + "\nActiveProjectiles\t" + std::to_string( mNrOfActiveProjectiles );
	mFont.WriteText( textToWrite, 40.0f, 200.0f, 2.0f );

	XMFLOAT4X4 identity;
	XMStoreFloat4x4( &identity, XMMatrixIdentity() );

	for( int i = 0; i < SHIP_AMOUNT; i++ )
	{
		if( mShips[i] && CullEntity( mShips[i]->GetPos() ) )
		{
			mShips[i]->Render( 0.0f, identity );
		}
	}
	
	if( mShips[FRIEND_SHIP] && mShips[FRIEND_SHIP]->Intersect( mPlayer->GetBoundingCircle() ) )
	{
		WriteInteractionText( 
			"Press E to open or close the upgrade menu!", 
			(float)( Input::GetInstance()->mScreenWidth * 0.5f ),
			(float)( Input::GetInstance()->mScreenHeight * 0.10f ), 
			2.0f,
			COLOR_CYAN );
	}

	RenderManager::GetInstance()->Render();

	return S_OK;
}

void PlayState::OnEnter()
{
	SetCursor( mSight );
	mActive = true;
	// Send Game Started event to server
	IEventPtr E1( new Event_Game_Started() );
	EventManager::GetInstance()->QueueEvent( E1 );

	SoundBufferHandler::GetInstance()->LoopStream( mLobbyMusic );

	mGui->SetTeamID( mPlayer->GetTeam() );
	IEventPtr spawnPos( new Event_Request_Player_Spawn_Position( mPlayer->GetID(), mPlayer->GetTeam() ) );
	EventManager::GetInstance()->QueueEvent( spawnPos );
}

void PlayState::OnExit()
{
	Reset();
	SoundBufferHandler::GetInstance()->StopLoopStream( mLobbyMusic );
	// Send Game Started event to server
	IEventPtr E1( new Event_Game_Ended() );
	EventManager::GetInstance()->QueueEvent( E1 );
}

void PlayState::Reset()
{
	mPlayer->Reset();

	for( size_t i = 0; i < MAX_PROJECTILES; i++ )
		mProjectiles[i]->Reset();

	mNrOfActiveProjectiles = 0;

	mEnemyListSynced		= false;
	mServerInitialized		= false;

	for( auto& rp : mRemotePlayers )
	{
		rp->Release();
		SAFE_DELETE( rp );
	}
	mRemotePlayers.clear();

	for( int i = 0; i < SHIP_AMOUNT; i++ )
	{
		SAFE_RELEASE_DELETE( mShips[i] );
	}

	for( int i = 1; i < MAX_ENERGY_CELLS; i++ )
	{
		mEnergyCells[i]->Reset();
	}

	RenderManager::GetInstance()->mParticleManager->Reset();

	mGui->Reset();

	mEndGame	= false;
	mWonGame	= false;
	mActive		= false;

}

HRESULT PlayState::Initialize()
{
	BaseState::Initialize();
	mStateType = PLAY_STATE;
	
	mSight = (HCURSOR)LoadImage( NULL, L"../Content/Assets/GUI/crosshair.ico", IMAGE_ICON, 32, 32, LR_LOADFROMFILE );

	//AssetID model		= 0;
	//AssetID loader	= 0;

	/*Graphics::GetInstance()->LoadSkeletonAsset( "../Content/Assets/Enemies/Blowuposaur/Animations/", "blowuposaurSkel.Skel", loader );
	Graphics::GetInstance()->LoadAnimated3dAsset( "../Content/Assets/Enemies/Blowuposaur/", "blowuposaur.apfs", loader, model );
	Graphics::GetInstance()->LoadAnimationAsset( "../Content/Assets/Enemies/Blowuposaur/Animations/", "blowuposaurIdle.PaMan", loader );*/

	//RenderManager::GetInstance()->AnimationInitialize( mTestAnimation, model, loader );
	//Graphics::GetInstance()->LoadStatic3dAsset( "../Content/Assets/ForestAssets/Trees/", "tree5.pfs", mTestStaticAsset );

	mPlayer = new Player();
	mPlayer->Initialize();

	mWorldMap = new Map();

	mWorldMap->Initialize( 25 );

	IEventPtr E1( new Event_Load_Level("../Content/Assets/Nodes/hardMap.xml" ) ); 

	EventManager::GetInstance()->TriggerEvent( E1 );

	//Fill up on Projectiles, test values
	mProjectiles	= new Projectile*[MAX_PROJECTILES];
	for ( size_t i = 0; i < MAX_PROJECTILES; i++ )
	{
		mProjectiles[i] = new Projectile();
		mProjectiles[i]->Initialize();
	}

	mNrOfActiveProjectiles	= 0;

	EventManager::GetInstance()->AddListener( &PlayState::EventListener, this, Event_Local_Joined::GUID );
	EventManager::GetInstance()->AddListener( &PlayState::EventListener, this, Event_Remote_Joined::GUID );
	EventManager::GetInstance()->AddListener( &PlayState::EventListener, this, Event_Remote_Left::GUID );
	EventManager::GetInstance()->AddListener( &PlayState::EventListener, this, Event_Remote_Damaged::GUID );
	EventManager::GetInstance()->AddListener( &PlayState::EventListener, this, Event_Remote_Removed_Projectile::GUID );
	EventManager::GetInstance()->AddListener( &PlayState::EventListener, this, Event_Remote_Fired_Projectile::GUID );
	EventManager::GetInstance()->AddListener( &PlayState::EventListener, this, Event_Server_Create_Enemy::GUID );
	EventManager::GetInstance()->AddListener( &PlayState::EventListener, this, Event_Server_Enemies_Created::GUID );
	EventManager::GetInstance()->AddListener( &PlayState::EventListener, this, Event_Connect_Server_Success::GUID );
	EventManager::GetInstance()->AddListener( &PlayState::EventListener, this, Event_Server_Sync_Spawn::GUID );
	EventManager::GetInstance()->AddListener( &PlayState::EventListener, this, Event_Server_Update_Enemy::GUID );
	EventManager::GetInstance()->AddListener( &PlayState::EventListener, this, Event_Remote_Set_Enemy_State::GUID );
	EventManager::GetInstance()->AddListener( &PlayState::EventListener, this, Event_Server_Spawn_Ship::GUID );
	EventManager::GetInstance()->AddListener( &PlayState::EventListener, this, Event_Server_Sync_Enemy_State::GUID ); 
	EventManager::GetInstance()->AddListener( &PlayState::EventListener, this, Event_Remote_Win::GUID );
	EventManager::GetInstance()->AddListener( &PlayState::EventListener, this, Event_Server_Turret_Fired_Projectile::GUID );
	EventManager::GetInstance()->AddListener( &PlayState::EventListener, this, Event_Server_Sync_Energy_Cell::GUID ); 

	EventManager::GetInstance()->AddListener( &PlayState::EventListener, this, Event_Server_XP::GUID ); 

	EventManager::GetInstance()->AddListener( &PlayState::EventListener, this, Event_Trigger_Client_Fired_Projectile::GUID );
	EventManager::GetInstance()->AddListener( &PlayState::EventListener, this, Event_Trigger_Client_Update::GUID );
	EventManager::GetInstance()->AddListener( &PlayState::EventListener, this, Event_Unlock_Player::GUID );
	EventManager::GetInstance()->AddListener( &PlayState::EventListener, this, Event_Reset_Game::GUID );

	EventManager::GetInstance()->AddListener(&PlayState::EventListener, this, Event_Server_Switch_Team::GUID );
	EventManager::GetInstance()->AddListener(&PlayState::EventListener, this, Event_Server_Enemy_Fired_Projectile::GUID);


	mFont.Initialize( "../Content/Assets/GUI/Fonts/final_font/" );

	// Enemies
	mEnemyAnimationManager = new EnemyAnimationManager();
	mEnemyAnimationManager->Initialize();

	mEnemies	= new RemoteEnemy*[MAX_NR_OF_ENEMIES];
	for ( size_t i = 0; i < MAX_NR_OF_ENEMIES; i++ )
	{
		mEnemies[i] = new RemoteEnemy();
		mEnemies[i]->Initialize( i, mEnemyAnimationManager->GetModel( Standard ), mEnemyAnimationManager->GetAnimation( Standard, Idle ) );
	}

	Graphics::GetInstance()->LoadStatic3dAsset( "../Content/Assets/Nests/", "nest_2.pfs", mSpawnModel );
	mSpawners	= new XMFLOAT3[MAX_NR_OF_ENEMY_SPAWNERS];

	//Victory condition, maybe should be set from outside?
	mNeededEnergyCells = 6;
	///////////////////////////////////////////////////////

	mGui = new Gui();
	mGui->Initialize( mNeededEnergyCells );

	//Energy cells
	mEnergyCells = new EnergyCell*[MAX_ENERGY_CELLS];
	for ( size_t i = 0; i < MAX_ENERGY_CELLS; i++ )
	{
		mEnergyCells[i] = new EnergyCell();
		mEnergyCells[i]->Initialize( DirectX::XMFLOAT3( 0.0f, 0.0f, 0.0f ) );
	}

	//TestSound
	mMiniGun			= SoundBufferHandler::GetInstance()->Load3DBuffer( "../Content/Assets/Sound/minigun.wav", 500, 80 );
	mShotGun			= SoundBufferHandler::GetInstance()->Load3DBuffer( "../Content/Assets/Sound/shotgun.wav", 500 );
	mExplosion			= SoundBufferHandler::GetInstance()->Load3DBuffer( "../Content/Assets/Sound/explosion.wav", 250 );
	mSniper				= SoundBufferHandler::GetInstance()->Load3DBuffer( "../Content/Assets/Sound/railgun.wav", 500 );
	mLevelUp			= SoundBufferHandler::GetInstance()->Load3DBuffer( "../Content/Assets/Sound/level up.wav", 10 );
	mLobbyMusic			= SoundBufferHandler::GetInstance()->LoadStreamBuffer( "../Content/Assets/Sound/ambientInGame.wav", 0 );

	Pathfinder::GetInstance()->Initialize( mWorldMap );

	mEndGame	= false;
	mWonGame	= false;
	mActive		= false;

	int worldDim = mWorldMap->GetMapWidth() * NODE_DIM;

	int worldOffset = mWorldMap->GetMapHalfWidth() * NODE_DIM;

	RenderManager::GetInstance()->RequestParticleSystem( 3333, Fire_Flies, XMFLOAT3 ( 0.0f, 2.0f, 0.0f ), XMFLOAT3( 0.0f, 0.1f, 0.0f ) );	//---id, effect, position, direction

	for (int i = 0; i < 100; i++)
	{
		int randX = rand() % worldDim;
		int randZ = rand() % worldDim;
		
		XMFLOAT3 randPos = XMFLOAT3( (float)( randX - worldOffset ), 2.0f, (float)( randZ - worldOffset ) );

		if( Pathfinder::GetInstance()->IsOnNavMesh( randPos ) )
		{
			RenderManager::GetInstance()->RequestParticleSystem( i, Fire_Flies, randPos, XMFLOAT3( 0.0f, 0.1f, 0.0f ) );	//---id, effect, position, direction			
		}
	}	

	return S_OK;
}

void PlayState::Release()
{	
	Pathfinder::GetInstance()->Release();
	mWorldMap->Release();
	SAFE_DELETE( mWorldMap );

	mPlayer->Release();
	SAFE_DELETE(mPlayer);

	for( auto& rp : mRemotePlayers )
	{
		rp->Release();
		SAFE_DELETE( rp );
	}

	mRemotePlayers.clear();

	for ( size_t i = 0; i < MAX_PROJECTILES; i++ )
	{
		mProjectiles[i]->Release();
		SAFE_DELETE( mProjectiles[i] );
	}
	delete [] mProjectiles;

	// Enemies
	SAFE_DELETE( mEnemyAnimationManager );

	for ( size_t i = 0; i < MAX_NR_OF_ENEMIES; i++ )
		SAFE_RELEASE_DELETE( mEnemies[i] );

	delete [] mEnemies;

	delete [] mSpawners;

	mFont.Release();

	mGui->Release();
	SAFE_DELETE( mGui );

	for( int i = 0; i < SHIP_AMOUNT; i++ )
	{
		if( mShips[i] )
			mShips[i]->Release();
		SAFE_DELETE( mShips[i] );
	}
	if( mShips )
		delete[] mShips;

	//Energy cells
	for( int i = 0; i < MAX_ENERGY_CELLS; i++ )
	{
		mEnergyCells[i]->Release();
		SAFE_DELETE( mEnergyCells[i] );
	}

	delete [] mEnergyCells;
}

PlayState::PlayState()
{
	mFPS					= 0.0f;
	mPlayer					= nullptr;
	mRemotePlayers			= std::vector<RemotePlayer*>( 0 );
	mRemotePlayers.reserve(MAX_REMOTE_PLAYERS);
	mEnemyAnimationManager	= nullptr;
	mFrameCounter			= 0;
	mProjectiles			= nullptr;
	mNrOfActiveProjectiles	= 0;
	mEnemies				= nullptr;
	mEnemyListSynced		= false;
	mServerInitialized		= false;
	mGui					= nullptr;
	mShips					= new ClientShip*[SHIP_AMOUNT];
	for( int i = 0; i < SHIP_AMOUNT; i++ )
	{
		mShips[i] = nullptr;
	}
}

PlayState::~PlayState()
{
}
