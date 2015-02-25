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
			SoundBufferHandler::GetInstance()->Play( mSoundAsset );

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
		for( unsigned int i = 0; i < mRemotePlayers.size(); i++ )
		{
			if( !mRemotePlayers.at(i) )
			{
				continue;
			}
			else if( data->ID() == mRemotePlayers.at(i)->GetID() )
			{
				mRemotePlayers.at(i)->Release();
				SAFE_DELETE( mRemotePlayers.at(i) );
				std::swap( mRemotePlayers.at(i), mRemotePlayers.at(mRemotePlayers.size() - 1) );
				mRemotePlayers.pop_back();
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
		SoundBufferHandler::GetInstance()->Play3D( m3DSoundAsset , data->BodyPos());
		
		// Request Muzzle Flash from Particle Manager
		
		RenderManager::GetInstance()->RequestParticleSystem( data->ID(), MuzzleFlash, data->BodyPos(), data->Direction() );
		RenderManager::GetInstance()->RequestParticleSystem( data->ID(), Smoke_MiniGun, data->BodyPos(), data->Direction() );
		//RenderManager::GetInstance()->RequestParticleSystem( data->ID(), Level_Up, mPlayer->GetBoundingCircle()->center, XMFLOAT3( 0.0f, 1.0f, 0.0f ) ); //both of these calls are needed for levelup effect.
		//RenderManager::GetInstance()->RequestParticleSystem( data->ID(), Level_Inner, mPlayer->GetBoundingCircle()->center, XMFLOAT3( 0.1f, 0.1f, 0.1f ) ); //both of these calls are needed for levelup effect.
		RenderManager::GetInstance()->RequestParticleSystem( data->ID(), Explosion, XMFLOAT3( 5.0f, 0.5f, 0.0f ), XMFLOAT3( 1.0f, 1.0f, 1.0f ) );
		RenderManager::GetInstance()->RequestParticleSystem( data->ID(), ExplosionSmoke, XMFLOAT3( 5.0f, 0.5f, 0.0f ), XMFLOAT3( 1.0f, 1.0f, 1.0f ) );
		//RenderManager::GetInstance()->RequestParticleSystem( 9999, Blood, XMFLOAT3( 2.0f, 3.0f, 0.0f ) , XMFLOAT3( -data->Direction().x, data->Direction().y, -data->Direction().z ) );
		//RenderManager::GetInstance()->RequestParticleSystem( 8999, Spark, XMFLOAT3( -2.0f, 2.0f, 0.0f ), XMFLOAT3( -data->Direction().x, data->Direction().y, -data->Direction().z ) );
	}
	else if ( newEvent->GetEventType() == Event_Server_Create_Enemy::GUID )
	{
		std::shared_ptr<Event_Server_Create_Enemy> data = std::static_pointer_cast<Event_Server_Create_Enemy>( newEvent );
		SyncEnemy( data->ID(), (EnemyState)data->State(), (EnemyType)data->Type(), data->Position(), data->Direction() );
	}
	else if ( newEvent->GetEventType() == Event_Server_Update_Enemy::GUID )
	{
		std::shared_ptr<Event_Server_Update_Enemy> data = std::static_pointer_cast<Event_Server_Update_Enemy>( newEvent );
		UpdateEnemyPosition( data->ID(), data->Position(), data->Direction(), data->IsAlive() );
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
			mFriendShip = new ClientShip();
			mFriendShip->Initialize( data->ID(), data->TeamID(), data->Position(), data->Rotation(), data->Scale() );
		}
		else
		{
			mEnemyShip = new ClientShip();
			mEnemyShip->Initialize( data->ID(), data->TeamID(), data->Position(), data->Rotation(), data->Scale() );
		}
	}
	else if( newEvent->GetEventType() == Event_Remote_Win::GUID )
	{
		std::shared_ptr<Event_Remote_Win> data = std::static_pointer_cast<Event_Remote_Win>( newEvent );
		if( data->Team() == mPlayer->GetTeam() )
		{
			MessageBox( NULL, L"You Won", L"Congratulations", MB_OK );
		}
		else
		{
			MessageBox( NULL, L"You Lost", L"Sorry", MB_OK );
		}
		IEventPtr E1( new Event_Reset_Game() );
		EventManager::GetInstance()->QueueEvent( E1 );
	}
	else if( newEvent->GetEventType() == Event_Server_Turret_Fired_Projectile::GUID )
	{
		// Fire projectile
		std::shared_ptr<Event_Server_Turret_Fired_Projectile> data = std::static_pointer_cast<Event_Server_Turret_Fired_Projectile>(newEvent);
		FireProjectile( data->ID(), data->ProjectileID(), data->TeamID(), data->Position(), data->Direction(), data->Speed(), data->Range(), 1.0f, TURRET ); // Don't know where to get damage from yet

		//TestSound
		SoundBufferHandler::GetInstance()->Play3D( m3DSoundAsset , data->Position());
		
		// Request Muzzle Flash from Particle Manager
		
		RenderManager::GetInstance()->RequestParticleSystem( data->ID(), MuzzleFlash, data->Position(), data->Direction() );
		RenderManager::GetInstance()->RequestParticleSystem( data->ID(), Smoke_MiniGun, data->Position(), data->Direction() );
		//RenderManager::GetInstance()->RequestParticleSystem( 9999, Blood, XMFLOAT3( 2.0f, 3.0f, 0.0f ) , XMFLOAT3( 0.0f, 1.0f, 1.0f ) );
		
	}
	else if( newEvent->GetEventType() == Event_Server_Sync_Energy_Cell::GUID )
	{
		std::shared_ptr<Event_Server_Sync_Energy_Cell> data = std::static_pointer_cast<Event_Server_Sync_Energy_Cell>( newEvent );
		
		mEnergyCells[data->EnergyCellID()]->SetOwnerID( data->OwnerID() );
		mEnergyCells[data->EnergyCellID()]->SetPosition( data->Position() );
		mEnergyCells[data->EnergyCellID()]->SetPickedUp( data->PickedUp() );
	}
	else if( newEvent->GetEventType() == Event_Server_XP::GUID )
	{
		std::shared_ptr<Event_Server_XP> data = std::static_pointer_cast<Event_Server_XP>( newEvent );
		if( data->PlayerID() == mPlayer->GetID() )
		{
			int levelUp = mPlayer->Upgradable();
			mPlayer->AddXP( data->XP() );
			if( mPlayer->Upgradable() != levelUp )
			{
				RenderManager::GetInstance()->RequestParticleSystem( mPlayer->GetID(), Level_Up, mPlayer->GetBoundingCircle()->center, XMFLOAT3( 0.0f, 1.0f, 0.0f ) ); //both of these calls are needed for levelup effect.
				RenderManager::GetInstance()->RequestParticleSystem( mPlayer->GetID(), Level_Inner, mPlayer->GetBoundingCircle()->center, XMFLOAT3( 0.1f, 0.1f, 0.1f ) ); //both of these calls are needed for levelup effect.
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
}

void PlayState::SyncEnemy( unsigned int id, EnemyState state, EnemyType type, XMFLOAT3 position, XMFLOAT3 direction )
{
	mEnemyListSynced = false;
	mEnemies[id]->SetID( id );
	mEnemies[id]->SetEnemyType( type );
	mEnemies[id]->SetModelID( mEnemyAnimationManager->GetModel( mEnemies[id]->GetEnemyType() ), mEnemyAnimationManager->GetDefaultAnimation( mEnemies[id]->GetEnemyType() ) );
	mEnemies[id]->SetAnimation( mEnemyAnimationManager->GetAnimation( mEnemies[id]->GetEnemyType(), state ) );
	mEnemies[id]->SetPosition( position );
	mEnemies[id]->SetDirection( direction );
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
		// Players
		if( mProjectiles[i]->IsActive() ) //&& mRemotePlayers.size() > 0 )
		{
			if( mPlayer->IsAlive() &&
				( mProjectiles[i]->GetPlayerID() == 70 || mProjectiles[i]->GetPlayerID() == 71 ) && 
				mPlayer->GetTeam() != mProjectiles[i]->GetTeamID() &&
				mProjectiles[i]->GetBoundingCircle()->Intersect( mPlayer->GetBoundingCircle() ) )
			{
				mPlayer->TakeDamage( mProjectiles[i]->GetDamage(), mProjectiles[i]->GetPlayerID() );
				RenderManager::GetInstance()->RequestParticleSystem( mPlayer->GetID(), Spark, mProjectiles[i]->GetPosition(), XMFLOAT3( -mProjectiles[i]->GetDirection().x, mProjectiles[i]->GetDirection().y, -mProjectiles[i]->GetDirection().z ) );
			}
			for ( size_t j = 0; j < mRemotePlayers.size(); j++ )
			{
				if( mRemotePlayers[j]->IsAlive() )
				{
					if( mProjectiles[i]->GetPlayerID() == mPlayer->GetID() &&
						mProjectiles[i]->GetBoundingCircle()->Intersect( mRemotePlayers[j]->GetBoundingCircle() ) )
					{
						BroadcastProjectileDamage( mRemotePlayers[j]->GetID(), mProjectiles[i]->GetID() );
						RenderManager::GetInstance()->RequestParticleSystem( mRemotePlayers[j]->GetID(), Spark, mProjectiles[i]->GetPosition(), XMFLOAT3( -mProjectiles[i]->GetDirection().x, mProjectiles[i]->GetDirection().y, -mProjectiles[i]->GetDirection().z ) );
						break;
					}
				}
			}

			// Enemies
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
						if( mProjectiles[i]->GetWeaponType() != SNIPER )
							mProjectiles[i]->Reset();
						break;
					}

					if( mProjectiles[i]->GetPlayerID() == mPlayer->GetID() &&
						mProjectiles[i]->GetBoundingCircle()->Intersect( mEnemies[j]->GetBoundingCircle() ) )
					{
						// hit
						BroadcastEnemyProjectileDamage( mProjectiles[i]->GetPlayerID(), mProjectiles[i]->GetID(), mEnemies[j]->GetID(), mProjectiles[i]->GetDamage() );
						RenderManager::GetInstance()->RequestParticleSystem( mProjectiles[i]->GetPlayerID(), Blood, mProjectiles[i]->GetPosition(), XMFLOAT3( -mProjectiles[i]->GetDirection().x, mProjectiles[i]->GetDirection().y, -mProjectiles[i]->GetDirection().z ) );
						if( mProjectiles[i]->GetWeaponType() != SNIPER )
							mProjectiles[i]->Reset();
						break;
					}
				}
			}
		}
	}
}

void PlayState::CheckMeeleCollision()
{
	for ( size_t i = 0; i < mRemotePlayers.size(); i++ )
	{
		//Check intersection with melee circle & remotePlayer
		if( mRemotePlayers[i]->IsAlive() && mPlayer->GetLoadOut()->meleeWeapon->boundingCircle->Intersect( mRemotePlayers.at(i)->GetBoundingCircle() ) )
		{
			XMVECTOR meeleRadiusVector =  ( XMLoadFloat3( &mPlayer->GetUpperBodyDirection() ) * mPlayer->GetLoadOut()->meleeWeapon->radius );

			//Spread to Radians
			float halfRadian = XMConvertToRadians( mPlayer->GetLoadOut()->meleeWeapon->spread * 18.0f ) * 0.5f;

			XMVECTOR playerToRemote = XMLoadFloat3( &mRemotePlayers.at(i)->GetPosition() ) - XMLoadFloat3( &mPlayer->GetPosition() );

			float angleRemoteToAim = 0.0f;
			XMVECTOR playerToCenter = XMLoadFloat3( &mRemotePlayers.at(i)->GetBoundingCircle()->center ) - XMLoadFloat3( &mPlayer->GetPlayerPosition() );
			XMStoreFloat( &angleRemoteToAim, XMVector4AngleBetweenVectors( playerToCenter, meeleRadiusVector ) );

			if( angleRemoteToAim <= halfRadian )
			{
				XMFLOAT3 direction = XMFLOAT3( 0.0f, 0.0f, 0.0f );
				XMStoreFloat3( &direction, XMVector4Normalize( XMLoadFloat3( &mPlayer->GetUpperBodyDirection() ) ) );
				BroadcastMeleeDamage( mRemotePlayers.at(i)->GetID(), mPlayer->GetLoadOut()->meleeWeapon->damage, mPlayer->GetLoadOut()->meleeWeapon->knockBack, direction );
			}
		}
	}

	for ( size_t i = 0; i < MAX_NR_OF_ENEMIES; i++ )
	{
		//Check intersection with melee circle & enemy
		if( mPlayer->GetLoadOut()->meleeWeapon->boundingCircle->Intersect( mEnemies[i]->GetBoundingCircle() ) )
		{
			XMVECTOR meeleRadiusVector =  ( XMLoadFloat3( &mPlayer->GetUpperBodyDirection() ) * mPlayer->GetLoadOut()->meleeWeapon->radius );

			//Spread to Radians
			float halfRadian = XMConvertToRadians( mPlayer->GetLoadOut()->meleeWeapon->spread * 18.0f ) * 0.5f;

			XMVECTOR playerToRemote = XMLoadFloat3( &mEnemies[i]->GetPosition() ) - XMLoadFloat3( &mPlayer->GetPosition() );

			float angleRemoteToAim = 0.0f;
			XMVECTOR playerToCenter = XMLoadFloat3( &mEnemies[i]->GetBoundingCircle()->center ) - XMLoadFloat3( &mPlayer->GetPlayerPosition() );
			XMStoreFloat( &angleRemoteToAim, XMVector4AngleBetweenVectors( playerToCenter, meeleRadiusVector ) );

			if( angleRemoteToAim <= halfRadian )
			{
				XMFLOAT3 direction = XMFLOAT3( 0.0f, 0.0f, 0.0f );
				XMStoreFloat3( &direction, XMVector4Normalize( XMLoadFloat3( &mPlayer->GetUpperBodyDirection() ) ) );
				BroadcastEnemyMeleeDamage(mEnemies[i]->GetID(), mPlayer->GetLoadOut()->meleeWeapon->damage, mPlayer->GetLoadOut()->meleeWeapon->knockBack, direction );
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
	if( Input::GetInstance()->IsKeyDown( KEYS::KEYS_Q ) )
	{
		IEventPtr E1( new Event_Reset_Game() );
		EventManager::GetInstance()->QueueEvent( E1 );
	}
	if( Input::GetInstance()->IsKeyDown( KEYS::KEYS_O ) )
	{
		if( mFriendShip->Intersect( mPlayer->GetBoundingCircle() ) )
		{
			IEventPtr E1( new Event_Client_Win( mPlayer->GetTeam() ) );
			Client::GetInstance()->SendEvent( E1 );
		}
	}
	if( Input::GetInstance()->IsKeyPressed( KEYS::KEYS_U ) )
	{
		if( mFriendShip->Intersect( mPlayer->GetBoundingCircle() ) )
		{
			if( mGui->UpgradeShipWindowIsActive() )
			{
				mPlayer->UnLock();
				mGui->DeActivateUpgradeShipWindow();
			}
			else if( mPlayer->IsAlive() )
			{
				mPlayer->Lock();
				mGui->ActivateUpgradeShipWindow();
			}
		}
	}
	if( Input::GetInstance()->IsKeyPressed( KEYS::KEYS_Y ) )
	{
		if( mFriendShip->Intersect( mPlayer->GetBoundingCircle() ) )
		{
			if( mGui->UpgradePlayerWindowIsActive() )
			{
				mPlayer->UnLock();
				mGui->DeActivateUpgradePlayerWindow();
			}
			else if( mPlayer->IsAlive() && ( mPlayer->Upgradable() >= 1 ) )
			{
				mPlayer->Lock();
				mGui->ActivateUpgradePlayerWindow();
			}
		}
	}
	if( Input::GetInstance()->IsKeyPressed( KEYS::KEYS_ENTER ) )
	{
		if( mGui->InGameWindowIsActive() )
		{
			mPlayer->UnLock();
			mGui->DeActivateInGameWindow();
		}
		else
		{
			mPlayer->Lock();
			mGui->ActivateInGameWindow();
		}
	}
	if( Input::GetInstance()->IsKeyDown( KEYS::KEYS_1 ) )
	{
		RenderManager::GetInstance()->ChangeRasterizerState( CULL_NONE );

		IEventPtr E1( new Event_Client_Change_Ship_Levels( mFriendShip->GetID(), 0, -1, 0, 0 ) );
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

void PlayState::UpdateEnemyPosition( unsigned int id, XMFLOAT3 position, XMFLOAT3 direction, bool isAlive )
{
	mEnemies[id]->SetPosition( position );
	mEnemies[id]->SetDirection( direction );
	mEnemies[id]->SetIsAlive( isAlive );
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

///////////////////////////////////////////////////////////////////////////////
//									PUBLIC
///////////////////////////////////////////////////////////////////////////////

HRESULT PlayState::Update( float deltaTime )
{
	//Fps update
	mFPS = mFPS * 0.1f + 0.9f / deltaTime;
	HandleDeveloperCameraInput();

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
				mRadarObjects[nrOfRadarObj++].mType			= RADAR_TYPE::HOSTILE;
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

	if( mFriendShip )
		guiUpdate.mShipHP	= mFriendShip->PercentHP();
	else
		guiUpdate.mShipHP	= 1.0f;

	if( mPlayer->GetEnergyCellID() != (UINT)-1 )
	{
		if( mFriendShip->Intersect( mPlayer->GetBoundingCircle() ) )
		{
			UINT temp = mPlayer->GetEnergyCellID();
			mPlayer->GiveEnergyCellToShip( mEnergyCells, mFriendShip->GetID(), mFriendShip->GetPos() );
		}
	}
	mPlayer->UpdateSpecific( deltaTime, mWorldMap, mRemotePlayers, mEnergyCells );

	UpdateProjectiles( deltaTime );

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

	///Test fountain particle system
	RenderManager::GetInstance()->RequestParticleSystem( 990, NormalSmoke, XMFLOAT3( 0.0f, 1.0f, 0.0f ), XMFLOAT3( 1.0f, 1.0f, 0.0f ) );

	RenderManager::GetInstance()->RequestParticleSystem( 1889, Fire, XMFLOAT3( 1.0f, 2.0f, 15.0f ), XMFLOAT3( 0.5f, 1.0f, 0.5f ) );	//---------id, effect, position, direction
	RenderManager::GetInstance()->RequestParticleSystem( 1889, FireSmoke, XMFLOAT3( 2.5f, 3.5f, 16.5f ), XMFLOAT3( 1.5f, 2.0f, 1.5f ) );	//---------id, effect, position, direction
	
	RenderManager::GetInstance()->RequestParticleSystem( 997, Test_Fountain, XMFLOAT3( 0.0f, 20.0f, 0.0f ), XMFLOAT3( 0.0f, -1.0f, 0.0f ) );


	if( mPlayer->Upgradable() < 1 )
	{
		mPlayer->UnLock();
		mGui->DeActivateUpgradePlayerWindow();
	}

	if( mFriendShip )
	{
		mFriendShip->Update( deltaTime );
		mRadarObjects[nrOfRadarObj].mRadarObjectPos = mFriendShip->GetPos();
		mRadarObjects[nrOfRadarObj++].mType = RADAR_TYPE::SHIP_FRIENDLY;
	}

	if( mEnemyShip )
	{
		mEnemyShip->Update( deltaTime );
		mRadarObjects[nrOfRadarObj].mRadarObjectPos = mEnemyShip->GetPos();
		mRadarObjects[nrOfRadarObj++].mType = RADAR_TYPE::SHIP_HOSTILE;
	}

	guiUpdate.mRadarObjects	= mRadarObjects;
	guiUpdate.mNrOfObjects	= nrOfRadarObj;
	guiUpdate.mPlayerPos	= mPlayer->GetPlayerPosition();	

	guiUpdate.mPlayerHP		= (float)( mPlayer->GetHP() / mPlayer->GetMaxHP() );
	guiUpdate.mPlayerXP		= mPlayer->GetXPToNext();
	guiUpdate.mPlayerShield	= (float)( mPlayer->GetHP() / mPlayer->GetMaxHP() );
	guiUpdate.mLevel		= mPlayer->Upgradable();
	
	guiUpdate.deltaTime = deltaTime;

	mGui->Update( guiUpdate );

	CheckProjectileCollision();

	return S_OK;
}

HRESULT PlayState::Render()
{
	mPlayer->Render( 0.0f, 1 );

	mWorldMap->Render( 0.0f , mPlayer );

	for ( size_t i = 0; i < mRemotePlayers.size(); i++)
	{
		if ( mRemotePlayers.at(i) && CullEntity( mRemotePlayers.at(i)->GetPosition() ) )
		{
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
				mEnemies[i]->Render();
		}
	}

	for (size_t i = 0; i < MAX_NR_OF_ENEMY_SPAWNERS; i++)
	{
		RenderManager::GetInstance()->AddObject3dToList( mSpawnModel, mSpawners[i] );
	}

	mGui->Render();

	for( int i = 0; i < MAX_ENERGY_CELLS; i++ )
	{
		if( !mEnergyCells[i]->GetPickedUp() && CullEntity( mEnergyCells[i]->GetPosition() ) )
		{
			mEnergyCells[i]->Render();
		}
	}

	//TestUpgradeWindow
	//mWindow.Render();

	//RENDER DEVTEXT
	std::string textToWrite = "FPS\t" + std::to_string( (int)mFPS ) + "\nRemotePlayers\t" + std::to_string( mRemotePlayers.size() ) + "\nActiveProjectiles\t" + std::to_string( mNrOfActiveProjectiles );
	mFont.WriteText( textToWrite, 40.0f, 200.0f, 2.0f );

	XMFLOAT4X4 identity;
	XMStoreFloat4x4( &identity, XMMatrixIdentity() );

	if( mFriendShip && CullEntity( mFriendShip->GetPos() ) )
		mFriendShip->Render( 0.0f, identity );
	if( mEnemyShip && CullEntity( mEnemyShip->GetPos() ) )
		mEnemyShip->Render( 0.0f, identity );

	RenderManager::GetInstance()->Render();

	return S_OK;
}

void PlayState::OnEnter()
{
	// Send Game Started event to server
	IEventPtr E1( new Event_Game_Started() );
	EventManager::GetInstance()->QueueEvent( E1 );

	//SoundBufferHandler::GetInstance()->LoopStream( mStreamSoundAsset );
}

void PlayState::OnExit()
{
	Reset();
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

	SAFE_RELEASE_DELETE( mFriendShip );
	SAFE_RELEASE_DELETE( mEnemyShip );

	RenderManager::GetInstance()->mParticleManager->Reset();
}

HRESULT PlayState::Initialize()
{
	mStateType = PLAY_STATE;


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

	mWorldMap->Initialize( 12 );

	IEventPtr E1( new Event_Load_Level("../Content/Assets/Nodes/HardMap.xml" ) ); 

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

	EventManager::GetInstance()->AddListener( &PlayState::EventListener, this, Event_Remote_Win::GUID );
	EventManager::GetInstance()->AddListener( &PlayState::EventListener, this, Event_Server_XP::GUID ); 

	EventManager::GetInstance()->AddListener( &PlayState::EventListener, this, Event_Trigger_Client_Fired_Projectile::GUID );
	EventManager::GetInstance()->AddListener( &PlayState::EventListener, this, Event_Trigger_Client_Update::GUID );

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

	mGui = new Gui();
	mGui->Initialize();

	//Energy cells
	mEnergyCells = new EnergyCell*[MAX_ENERGY_CELLS];
	for( int i = 0; i < MAX_ENERGY_CELLS; i++ )
	{
		mEnergyCells[i] = new EnergyCell();
		mEnergyCells[i]->Initialize( DirectX::XMFLOAT3( 0.0f, 0.0f, 0.0f ) );
	}

	//TestSound
	m3DSoundAsset	= SoundBufferHandler::GetInstance()->Load3DBuffer( "../Content/Assets/Sound/alert02.wav" );
	mSoundAsset		= SoundBufferHandler::GetInstance()->LoadBuffer( "../Content/Assets/Sound/alert02.wav" );

	//TestSound
	m3DSoundAsset		= SoundBufferHandler::GetInstance()->Load3DBuffer( "../Content/Assets/Sound/alert02.wav" );
	mSoundAsset			= SoundBufferHandler::GetInstance()->LoadBuffer( "../Content/Assets/Sound/alert02.wav" );
	mStreamSoundAsset	= SoundBufferHandler::GetInstance()->LoadStreamBuffer( "../Content/Assets/Sound/Groove 1 Bass.wav" );

	Pathfinder::GetInstance()->Initialize( mWorldMap );

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

	SAFE_RELEASE_DELETE( mFriendShip );
	SAFE_RELEASE_DELETE( mEnemyShip );

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
	mFriendShip				= nullptr;
	mEnemyShip				= nullptr;
}

PlayState::~PlayState()
{
}
