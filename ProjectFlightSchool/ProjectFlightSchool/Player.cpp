#include "Player.h"
#include "HelperFunctions.h"
#include "Map.h"
#include "Pathfinder.h"

/////Private

void Player::EventListener( IEventPtr newEvent )
{
	if ( newEvent->GetEventType() == Event_Remote_Died::GUID )
	{
		// Kill remote player
		std::shared_ptr<Event_Remote_Died> data = std::static_pointer_cast<Event_Remote_Died>( newEvent );
		if ( data->KillerID() == mID )
		{
			CountUpKills();
		}
	}
	else if ( newEvent->GetEventType() == Event_Remote_Attempt_Revive::GUID )
	{
		std::shared_ptr<Event_Remote_Attempt_Revive> data = std::static_pointer_cast<Event_Remote_Attempt_Revive>( newEvent) ;
		HandleRevive( data->DeltaTime() );
	}
	else if ( newEvent->GetEventType() == Event_Server_Enemy_Attack_Player::GUID )
	{
		std::shared_ptr<Event_Server_Enemy_Attack_Player> data = std::static_pointer_cast<Event_Server_Enemy_Attack_Player>( newEvent );
		if ( mID == data->PlayerID() )
			TakeDamage( data->Damage(), 0);
	}
	else if ( newEvent->GetEventType() == Event_Remote_Melee_Hit::GUID )
	{
		// Melee Hit
		std::shared_ptr<Event_Remote_Melee_Hit> data = std::static_pointer_cast<Event_Remote_Melee_Hit>( newEvent );
		if ( mID == data->ID() )
		{
			XMFLOAT3 direction = data->Direction();
			direction.x *= data->KnockBack();
			direction.z *= data->KnockBack();
			AddImpuls( direction );
			TakeDamage( data->Damage(), 0);
		}
	}
	else if ( newEvent->GetEventType() == Event_Create_Player_Name::GUID )
	{
		std::shared_ptr<Event_Create_Player_Name> data = std::static_pointer_cast<Event_Create_Player_Name>( newEvent );
		mPlayerName = data->PlayerName();
	}
	else if( newEvent->GetEventType() == Event_Upgrade_Player::GUID )
	{
		std::shared_ptr<Event_Upgrade_Player> data = std::static_pointer_cast<Event_Upgrade_Player>( newEvent );
		if( data->Speed() != 0 )
		{
			UpgradeLegs();
			mCurrentUpgrades--;
		}
		else if( data->Health() != 0 )
		{
			UpgradeBody();
			mCurrentUpgrades--;
		}
		else if( data->Melee() != 0 )
		{
			UpgradeMelee();
			mCurrentUpgrades--;
		}
		else if( data->Range() != 0 )
		{
			UpgradeRange();
			mCurrentUpgrades--;
		}
	}
	else if( newEvent->GetEventType() == Event_New_Player_Spawn_Position::GUID )
	{
		std::shared_ptr<Event_New_Player_Spawn_Position> data = std::static_pointer_cast<Event_New_Player_Spawn_Position>( newEvent );
		if( data->PlayerID() == mID )
		{
			mSpawnPosition	= XMFLOAT3( data->SpawnPosition().x, 0.0f, data->SpawnPosition().y );
			mCameraPosition	= XMFLOAT3( data->SpawnPosition().x, CAMERA_Y, data->SpawnPosition().y + CAMERA_Z );
			if( IsAlive() )
			{
				RemotePlayer::Spawn();
			}
		}
	}
	else if( newEvent->GetEventType() == Event_Server_Switch_Team::GUID )
	{
		std::shared_ptr<Event_Server_Switch_Team> data = std::static_pointer_cast<Event_Server_Switch_Team>( newEvent );
		if( data->ID() == mID )
		{
			mTeam = data->TeamID();
		}
	}
	else if( newEvent->GetEventType() == Event_Change_Weapon::GUID )
	{
		std::shared_ptr<Event_Change_Weapon> data = std::static_pointer_cast<Event_Change_Weapon>( newEvent );

		if( (WeaponType)data->Weapon() == MINIGUN || (WeaponType)data->Weapon() == SHOTGUN || (WeaponType)data->Weapon() == GRENADELAUNCHER || (WeaponType)data->Weapon() == SNIPER )
		{
			delete mLoadOut->rangedWeapon;
			mLoadOut->rangedWeapon	= new RangedInfo( (WeaponType)data->Weapon() );	
			RenderManager::GetInstance()->AnimationInitialize( mArms.rightArm, mWeaponModels[mLoadOut->rangedWeapon->weaponType], mWeaponAnimations[mLoadOut->rangedWeapon->weaponType][IDLE] );
		}
		else if( (WeaponType)data->Weapon() == CLAYMORE || (WeaponType)data->Weapon() == HAMMER || (WeaponType)data->Weapon() == BLOWTORCH || (WeaponType)data->Weapon() == SAW )
		{
			if( mLoadOut->meleeWeapon->boundingCircle )
				delete mLoadOut->meleeWeapon->boundingCircle;
			delete mLoadOut->meleeWeapon;
			mLoadOut->meleeWeapon	= new MeleeInfo( (WeaponType)data->Weapon() );
			RenderManager::GetInstance()->AnimationInitialize( mArms.leftArm, mWeaponModels[mLoadOut->meleeWeapon->weaponType], mWeaponAnimations[mLoadOut->meleeWeapon->weaponType][IDLE] );
		}
	}
	else if( newEvent->GetEventType() == Event_Server_Lobby_Finished::GUID )
	{
		IEventPtr E1( new Event_Client_Change_Weapon( mLoadOut->meleeWeapon->weaponType, mID ) );
		QueueEvent( E1 );
		IEventPtr E2( new Event_Client_Change_Weapon( mLoadOut->rangedWeapon->weaponType, mID ) );
		QueueEvent( E2 );
	}
}

void Player::HandleInput( float deltaTime, std::vector<RemotePlayer*> remotePlayers )
{
	if( Input::GetInstance()->IsKeyDown( KEYS::KEYS_F ) && mCloseToPlayer )
	{
		mAcceleration = XMFLOAT3( 0.0f, 0.0f, 0.0f );
		for( auto rp : remotePlayers )
		{
			if ( rp->IsDown() )
			{
				if ( mBoundingCircleAura->Intersect( rp->GetBoundingCircleAura() ) )
				{
					ReviveRemotePlayer( rp->GetID(), deltaTime );
				}
			}
		}
	}
	else
	{
		mAcceleration = XMFLOAT3(0.0f, 0.0f, 0.0f);

		if (Input::GetInstance()->IsKeyDown(KEYS::KEYS_W) && !Input::GetInstance()->IsKeyDown(KEYS::KEYS_S))
		{
			mFollowPath = false;
			mAcceleration.z = mMaxAcceleration;
		}

		if (Input::GetInstance()->IsKeyDown(KEYS::KEYS_A) && !Input::GetInstance()->IsKeyDown(KEYS::KEYS_D))
		{
			mFollowPath = false;
			mAcceleration.x = -mMaxAcceleration;
		}

		if (Input::GetInstance()->IsKeyDown(KEYS::KEYS_S) && !Input::GetInstance()->IsKeyDown(KEYS::KEYS_W))
		{
			mFollowPath = false;
			mAcceleration.z = -mMaxAcceleration;
		}

		if (Input::GetInstance()->IsKeyDown(KEYS::KEYS_D) && !Input::GetInstance()->IsKeyDown(KEYS::KEYS_A))
		{
			mFollowPath = false;
			mAcceleration.x = mMaxAcceleration;
		}
		if ( Input::GetInstance()->IsKeyDown(KEYS::KEYS_H) )
		{
			mLowerBody.position = XMFLOAT3( 0, 0, 0 );
		}

		//Normalize acceleration 
		XMVECTOR normalizer = XMVector3Length( XMLoadFloat3( &mAcceleration ) );
		if (XMVectorGetX(normalizer) > mMaxAcceleration)
		{
			normalizer = XMVector3Normalize( XMLoadFloat3( &mAcceleration ) );
			normalizer *= mMaxAcceleration;
			XMStoreFloat3( &mAcceleration, normalizer );
		}

		//Dashing
		if( Input::GetInstance()->IsKeyPressed( KEYS::KEYS_SPACE ) && mDashCoolDown <= 0.0f )
		{
			mDashCoolDown = DASH_COOLDOWN;
			mDashVelocity = DASH_VELOCITY;
			RenderManager::GetInstance()->AnimationStartNew( mLowerBody.playerModel[TEAM_ARRAY_ID], mAnimations[LEGS_DASH][TEAM_ARRAY_ID] );

			XMVECTOR normalizer = XMVector3Length( XMLoadFloat3( &mVelocity ) );
			mCurrentVelocity = XMVectorGetX( normalizer );
			normalizer	 = XMVector3Normalize( XMLoadFloat3( &mVelocity ) );
			normalizer	*= mMaxVelocity;
			XMStoreFloat3( &mVelocity, normalizer );

			IEventPtr E1( new Event_Client_Dash( mID ) );
			EventManager::GetInstance()->QueueEvent( E1 );
		}


		//== Calculate upper body rotation ==
		XMVECTOR rayOrigin = XMVECTOR( Input::GetInstance()->mCurrentNDCMousePos );
		XMVECTOR rayDir = rayOrigin;

		XMFLOAT3 unPack;
		XMStoreFloat3( &unPack, rayOrigin );
		rayDir = XMVectorSet( unPack.x, unPack.y, 1.0f, 1.0f );

		XMMATRIX viewInverse;
		Graphics::GetInstance()->GetInverseViewMatrix( viewInverse );

		XMMATRIX projectionInverse;
		Graphics::GetInstance()->GetInverseProjectionMatrix( projectionInverse );

		XMMATRIX combinedInverse = XMMatrixMultiply( projectionInverse, viewInverse );

		XMVECTOR rayPosInWorld = XMVector3TransformCoord( rayOrigin, combinedInverse );
		XMVECTOR rayDirInWorld = XMVector3TransformCoord( rayDir, combinedInverse );
		rayDirInWorld = XMVector3Normalize( rayDirInWorld - rayPosInWorld );

		float t = 0.0f;
		XMVECTOR planeNormal = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);
		XMVECTOR result = -( XMVector3Dot( rayPosInWorld, planeNormal ) ) / ( XMVector3Dot( rayDirInWorld, planeNormal ) );
		XMStoreFloat( &t, result );
		XMVECTOR intersection = XMVectorAdd( rayPosInWorld, rayDirInWorld * t );

		XMVECTOR playerToCursor = XMVectorSubtract( intersection, XMLoadFloat3( &XMFLOAT3( mLowerBody.position.x, 1.0f, mLowerBody.position.z ) ) );
		XMStoreFloat3( &unPack, playerToCursor );
		XMStoreFloat3( &mPlayerToCursor, XMLoadFloat3( &mPlayerToCursor ) * 0.95f + ( XMVectorSet( unPack.x, 0.0f, unPack.z, 0.0f ) * 0.1f ) * 0.05f );
		playerToCursor = XMVector3Normalize( XMVectorSet( unPack.x, 0.0f, unPack.z, 0.0f ) );
		XMStoreFloat3( &mUpperBody.direction, playerToCursor );

		//Cap the mPlayerToCursor vector
		normalizer = XMVector3Length( XMLoadFloat3( &mPlayerToCursor ) );
		float currCamera = XMVectorGetX( normalizer );
		if(  currCamera > CAMERA_CAP )
		{
			normalizer	 = XMVector3Normalize( XMLoadFloat3( &mPlayerToCursor ) );
			normalizer	*= CAMERA_CAP;
			XMStoreFloat3( &mPlayerToCursor, normalizer );
		}

		if( Input::GetInstance()->IsKeyDown(KEYS::KEYS_MOUSE_LEFT) )
		{
			XMStoreFloat3( &mPick, intersection );
		}
	}

	// UNCOMMENT THIS TO USE CLICK TO MOVE
	//if( Input::GetInstance()->IsKeyDown(KEYS::KEYS_MOUSE_LEFT) )
	//{
	//	XMStoreFloat3( &mPick, intersection );
	//}

	//== Weapon handling ==
	mWeaponCoolDown -= deltaTime;
	mMeleeCoolDown -= deltaTime;

	if( Input::GetInstance()->IsKeyDown( KEYS::KEYS_MOUSE_LEFT ) && mWeaponCoolDown <= 0.0f )
	{
		mSlowDown -= mLoadOut->rangedWeapon->slowDown;
		if( mSlowDown < 0.3f )
		{
			mSlowDown = 0.3f;
		}
		RenderManager::GetInstance()->AnimationStartNew( mArms.rightArm, mWeaponAnimations[mLoadOut->rangedWeapon->weaponType][ATTACK] );
		mWeaponCoolDown = mLoadOut->rangedWeapon->attackRate;
		Fire();
	}

	if( Input::GetInstance()->IsKeyDown( KEYS::KEYS_MOUSE_RIGHT ) )
	{
		if( mLoadOut->meleeWeapon->weaponType == SAW )
		{
			mHasMeleeStarted = true;
		}
		else if( mMeleeCoolDown <= 0.0f )
		{
			mSlowDown -= mLoadOut->meleeWeapon->slowDown;
			if( mSlowDown < 0.3f )
			{
				mSlowDown = 0.3f;
			}
			mHasMeleeStarted = true;
		}
	}

	if( mHasMeleeStarted )
		Melee( deltaTime );
	else if( mLoadOut->meleeWeapon->weaponType == BLOWTORCH  )
		BlowtorchIdle();

	//Minigun behaviour
	if( mLoadOut->rangedWeapon->weaponType == MINIGUN )
	{
		if( !mWeaponOverheated )
		{
			if( mTimeSinceLastShot > 0.0f )
			{
				mTimeSinceLastShot -= deltaTime;
			}
			else if( mLoadOut->rangedWeapon->overheat > 0.0f )
			{
				mLoadOut->rangedWeapon->overheat -= MINIGUN_OVERHEAT;
			}
		}
	}
}

void Player::HandleSpawn( float deltaTime )
{
	mPlayerDownSparksTimer -= deltaTime;

	// Spawn smoke and electricity after ground impact
	if ( mTimeTillSpawn < 8.6f )
	{
		XMFLOAT3 newPos;
		float randY = (float)( rand() % 8 ) * 0.1f;

		XMStoreFloat3( &newPos, XMVector3TransformCoord( XMVectorSet( 0.0f, randY, 0.0f, 1.0f ), XMLoadFloat4x4( &mLowerBody.rootMatrix ) ) );

		RenderManager::GetInstance()->RequestParticleSystem( mID, FireSmoke, XMFLOAT3( newPos.x, newPos.y - 0.3f, newPos.z ), XMFLOAT3( 0.0f, -0.1f , 0.0f ) );

		if( mPlayerDownSparksTimer < 0.0f )
		{
			RenderManager::GetInstance()->RequestParticleSystem( mID, Spark_Electric, newPos, mUpperBody.direction );
			SoundBufferHandler::GetInstance()->Play3D( mPlayerDeath , newPos );
			mPlayerDownSparksTimer = (float)( rand() % 6 + 4 ) * 0.1f;
		}
	}

	// Spawn fire at death
	else if (  9.55f <= mTimeTillSpawn && mTimeTillSpawn <= 10.0f )
	{
		XMFLOAT3 newPos;
		XMFLOAT3 inverseDir;

		XMStoreFloat3( &newPos, XMVector3TransformCoord( XMVectorZero(), XMLoadFloat4x4( &mLowerBody.rootMatrix ) ) );
		XMStoreFloat3( &inverseDir, -XMLoadFloat3( &mUpperBody.direction ) );

		RenderManager::GetInstance()->RequestParticleSystem( mID, FIRE, newPos, inverseDir );
		RenderManager::GetInstance()->RequestParticleSystem( mID, FireSmoke, XMFLOAT3( newPos.x, newPos.y + 1, newPos.z ), inverseDir );
	}
	
	if( mTimeTillSpawn <= 0.0f )
	{
		Spawn();
		UnLock();
		IEventPtr E1( new Event_Client_Spawned( mID ) );
		QueueEvent( E1 );
	}
	else
	{
		mTimeTillSpawn -= deltaTime;
	}
}

void Player::HandleDeath( float deltaTime )
{
	mPlayerDownSparksTimer -= deltaTime;

	if ( mPlayerDownSparksTimer < 0.0f )
	{
		XMFLOAT3 newPos;
		XMFLOAT3 inverseDir;
		float randY = (float)( rand() % 8 ) * 0.1f;

		XMStoreFloat3( &newPos, XMVector3TransformCoord( XMVectorSet( 0.0f, randY, 0.0f, 1.0f ), XMLoadFloat4x4( &mLowerBody.rootMatrix ) ) );

		RenderManager::GetInstance()->RequestParticleSystem( mID, Spark_Electric, newPos, mUpperBody.direction );

		XMStoreFloat3( &newPos, XMVector3TransformCoord( XMVectorZero(), XMLoadFloat4x4( &mLowerBody.rootMatrix ) ) );
		XMStoreFloat3( &inverseDir, -XMLoadFloat3( &mUpperBody.direction ) );
		
		RenderManager::GetInstance()->RequestParticleSystem( mID, Spark_Robot, newPos, inverseDir );

		mPlayerDownSparksTimer = (float)( rand() % 3 + 1 ) * 0.1f;
	}

	if( mTimeTillDeath <= 0.0f )
	{
		Lock();
		Die();
		BroadcastDeath( mLastKiller );
	}
	else
	{
		mTimeTillDeath -= deltaTime;
	}
}

void Player::Move( float deltaTime )
{
	mVelocity.x += mAcceleration.x * deltaTime - mVelocity.x * VELOCITY_FALLOFF * deltaTime;
	mVelocity.y = 0.0f;
	mVelocity.z += mAcceleration.z * deltaTime - mVelocity.z * VELOCITY_FALLOFF * deltaTime;

	XMVECTOR normalizer = XMVector3Length( XMLoadFloat3( &mVelocity ) );
	mCurrentVelocity = XMVectorGetX( normalizer );
	if(  mCurrentVelocity > mMaxVelocity )
	{
		normalizer	 = XMVector3Normalize( XMLoadFloat3( &mVelocity ) );
		normalizer	*= mMaxVelocity;
		XMStoreFloat3( &mVelocity, normalizer );
	}

	if( mCurrentVelocity > 0.05f )
	{
		mLowerBody.direction.x	= mVelocity.x;
		mLowerBody.direction.y	= 0.0f;
		mLowerBody.direction.z	= mVelocity.z;
		normalizer				= XMVector3Normalize( XMLoadFloat3( &mLowerBody.direction ) );
		XMStoreFloat3( &mLowerBody.direction, normalizer );
	}
}

void Player::Ding()
{
	AddXP( mNextLevelXP );
}

void Player::AddXP( float XP )
{
	//Check if maxlevel
	if( mCurrentLevel > MAX_PLAYER_LEVEL )
	{
		mXP = 0;
		return;
	}

	mXP += XP;
	while( ( mXP / mNextLevelXP ) >= 1 )
	{
		mCurrentUpgrades++;
		mXP -= mNextLevelXP;
		mNextLevelXP *= powf( 1.009f, (float) ( mCurrentLevel + 1.0f ) );
		mNextLevelXP += 5.0f;
		mCurrentLevel++;
		if( mCurrentLevel > MAX_PLAYER_LEVEL )
		{
			int modUpg = ( mCurrentUpgrades + MAX_PLAYER_LEVEL ) % MAX_PLAYER_LEVEL;
			mCurrentUpgrades -= modUpg;
			mXP = 0;
			break;
		}
	}
}

void Player::PickUpEnergyCell( EnergyCell** energyCells )
{
	for( UINT i = 0; i < MAX_ENERGY_CELLS; i++ )
	{
		if( !energyCells[i]->GetPickedUp() && mEnergyCellID == (UINT)-1 && energyCells[i]->GetActive() )
		{
			if( energyCells[i]->GetPickUpRadius()->Intersect( mBoundingCircle ) )
			{
				energyCells[i]->SetOwnerID( mID );
				energyCells[i]->SetPickedUp( true );
				energyCells[i]->SetSecured( false );
				energyCells[i]->SetPosition( mLowerBody.position );
				mEnergyCellID = i;

				IEventPtr E1( new Event_Client_Sync_Energy_Cell( i, mID, mLowerBody.position, energyCells[mEnergyCellID]->GetPickedUp(), energyCells[mEnergyCellID]->GetActive() ) );
				QueueEvent( E1 );
			}
		}
	}
}

void Player::DropEnergyCell( EnergyCell** energyCells )
{
	if( mEnergyCellID != (UINT)-1 )
	{
		energyCells[mEnergyCellID]->SetPosition( mLowerBody.position );
		energyCells[mEnergyCellID]->SetOwnerID( (UINT)-1 );
		energyCells[mEnergyCellID]->SetPickedUp( false );
		energyCells[mEnergyCellID]->SetSecured( false );
	

		IEventPtr E1( new Event_Client_Sync_Energy_Cell( mEnergyCellID, (UINT)-1, mLowerBody.position, energyCells[mEnergyCellID]->GetPickedUp(), energyCells[mEnergyCellID]->GetActive() ) );
		QueueEvent( E1 );

		IEventPtr E2( new Event_Client_Dropped_Energy_Cell() );
		QueueEvent( E2 );

		mEnergyCellID	= (UINT)-1;
		mPickUpCooldown	= 3.0f;
	}
}

void Player::GiveEnergyCellToShip( EnergyCell** energyCells, UINT shipID, DirectX::XMFLOAT3 shipPos )
{
	if( mEnergyCellID != (UINT)-1 )
	{
		energyCells[mEnergyCellID]->SetPosition( shipPos );
		energyCells[mEnergyCellID]->SetOwnerID( shipID );
		energyCells[mEnergyCellID]->SetPickedUp( true );
		energyCells[mEnergyCellID]->SetSecured( true );

		IEventPtr E1( new Event_Client_Sync_Energy_Cell( mEnergyCellID, shipID, shipPos, energyCells[mEnergyCellID]->GetPickedUp(), energyCells[mEnergyCellID]->GetActive() ) );
		QueueEvent( E1 );

		mEnergyCellID	= (UINT)-1;
		mPickUpCooldown = 3.0f;
	}
}

HRESULT Player::UpdateSpecific( float deltaTime, Map* worldMap, std::vector<RemotePlayer*> remotePlayers, EnergyCell** energyCells )
{

	// Update water status	
	mSlowDown += deltaTime / 5;
	if( mSlowDown > 1.0f )
	{
		mSlowDown = 1.0f;
	}
	mDashVelocity -= deltaTime * 0.5f;
	mDashVelocity = max( 1.0f, mDashVelocity );
	mDashCoolDown -= deltaTime;
	mDashCoolDown = max( 0.0f, mDashCoolDown );

	mIsInWater	= mLowerBody.position.y < -0.7f ? true : false;
	
	XMFLOAT3 testPosition	= mLowerBody.position;
	XMFLOAT3 normal			= XMFLOAT3( 0.0f, 1.0f, 0.0f );

	mCurrentTravelVelocity.x = mVelocity.x * deltaTime * mUpgrades.runSpeedFactor * mSlowDown * mDashVelocity;
	mCurrentTravelVelocity.z = mVelocity.z * deltaTime * mUpgrades.runSpeedFactor * mSlowDown * mDashVelocity;

	testPosition.x += mCurrentTravelVelocity.x;
	testPosition.z += mCurrentTravelVelocity.z;
	testPosition.y = worldMap->GetHeight( testPosition );

	bool collisionTest = worldMap->PlayerVsMap( testPosition, normal );
	if( !collisionTest )
	{
		mLowerBody.position.x = testPosition.x;
		mLowerBody.position.y = testPosition.y;
		mLowerBody.position.z = testPosition.z;

		if( mIsInWater && mIsAlive && !mIsDown )
		{
			WriteInteractionText( 
				"Get out of the water or die!", 
				(float)( Input::GetInstance()->mScreenWidth * 0.5f ),
				(float)( Input::GetInstance()->mScreenHeight * 0.25f ), 
				3.0f,
				COLOR_CYAN );
			XMStoreFloat3( &mVelocity, XMLoadFloat3( &mVelocity ) * ( 1.0f - deltaTime * 10.0f ) );
			mWaterDamageTime += deltaTime;
			if( mWaterDamageTime > WATER_DAMAGE_TIME )
			{
				mWaterDamageTime = 0.0f;
				TakeDamage( WATER_DAMAGE, 0 );
			}
		}
	}
	else
	{
		XMVECTOR loadVel		= XMLoadFloat3( &mVelocity );
		XMVECTOR loadNorm		= XMLoadFloat3( &XMFLOAT3( normal.x, normal.y, normal.z ) );
		XMVECTOR loadNormNorm	= XMLoadFloat3( &XMFLOAT3( -normal.z, -normal.y, normal.x ) );
		XMStoreFloat3( &mVelocity, loadNormNorm * XMVectorGetX( XMVector3Dot( loadVel, loadNormNorm ) ) + loadNorm * deltaTime * 20.0f );
	}

	//Draw goal arrow
	float circleRadius = 2.0f;
	if( energyCells[1]->GetActive() )
	{
		XMVECTOR playerGoal = XMLoadFloat3( &XMFLOAT3( mPlayerGoal.x, mLowerBody.position.y + 1.0f, mPlayerGoal.z ) );
		XMVECTOR playerPos = XMLoadFloat3( &XMFLOAT3( mLowerBody.position.x, mLowerBody.position.y + 1.0f, mLowerBody.position.z ) );
		XMVECTOR goalDir = playerGoal - playerPos;
		goalDir = XMVector3Normalize( goalDir );
	
		float rot = -atan2f( XMVectorGetZ( goalDir ), XMVectorGetX( goalDir ) );
	
		XMMATRIX rotMatrix = XMMatrixRotationY( rot );
		XMMATRIX trans = XMMatrixTranslationFromVector( playerPos + ( goalDir * circleRadius ) );
		XMFLOAT4X4 world;
		XMStoreFloat4x4( &world, ( rotMatrix * trans ) );

		if( energyCells[1]->GetOwnerID() != mID )
		{
			mPlayerGoal = energyCells[1]->GetPosition();
		}
		else
		{
			mPlayerGoal = mShipPos;
		}
		RenderManager::GetInstance()->AddObject3dToList( mCellArrow, world );
	}

	Update( deltaTime, remotePlayers, energyCells );
	return S_OK;
}

void Player::GoDown( int shooter )
{
	Lock();
	RemotePlayer::GoDown();
	mTimeTillRevive	= mReviveTime;
	mLastKiller		= shooter;
	IEventPtr E1( new Event_Client_Down( mID ) );
	QueueEvent( E1 );
}

void Player::GoUp()
{
	RemotePlayer::GoUp();
	UnLock();
	IEventPtr E1( new Event_Client_Up( mID ) );
	QueueEvent( E1 );
}

void Player::ReviveRemotePlayer( int remotePlayerID, float deltaTime )
{
	IEventPtr E1( new Event_Client_Attempt_Revive( mID, remotePlayerID, deltaTime ) );
	QueueEvent( E1 );
}

void Player::BroadcastDeath( unsigned int shooter )
{
	IEventPtr E1( new Event_Client_Died( mID, shooter ) );
	QueueEvent( E1 );
}

void Player::Revive()
{
	GoUp();
}

void Player::Die()
{
	// Queue spawn position request
	IEventPtr E1( new Event_Request_Player_Spawn_Position( mID, mTeam ) );
	EventManager::GetInstance()->QueueEvent( E1 );

	RemotePlayer::Die();
}

void Player::Fire()
{
	//Offset calcs
	XMFLOAT3 weaponOffsets =	mLoadOut->rangedWeapon->weaponType == MINIGUN			? MINIGUN_OFFSETS : 
								mLoadOut->rangedWeapon->weaponType == SHOTGUN			? SHOTGUN_OFFSETS :
								mLoadOut->rangedWeapon->weaponType == GRENADELAUNCHER	? GL_OFFSETS :
								mLoadOut->rangedWeapon->weaponType == SNIPER			? SNIPER_OFFSETS : XMFLOAT3( 0.0f, 0.0f, 1.0f );

	XMVECTOR position	= XMLoadFloat3( &mLowerBody.position );
	XMVECTOR direction	= XMLoadFloat3( &mUpperBody.direction );
	XMVECTOR offset		= XMLoadFloat3( &XMFLOAT3( mLowerBody.position.x, mLowerBody.position.y + weaponOffsets.z, mLowerBody.position.z ) );
	
	offset += XMVector3Normalize( XMVector3Cross( XMLoadFloat3( &XMFLOAT3( 0.0f, 1.0f, 0.0f ) ), direction ) ) * weaponOffsets.y;
	offset += direction * weaponOffsets.x;

	XMFLOAT3 loadDir;
	XMStoreFloat3( &loadDir, offset );

	if( mLoadOut->rangedWeapon->weaponType == SHOTGUN )
	{
		FireShotgun( &loadDir );
	}
	else if( mLoadOut->rangedWeapon->weaponType == MINIGUN )
	{
		FireMinigun( &loadDir );	
	}
	else if( mLoadOut->rangedWeapon->weaponType == GRENADELAUNCHER )
	{
		FireGrenadeLauncher( &loadDir );
	}
	else
	{
		// Set random spread
		if( mLoadOut->rangedWeapon->spread != 0.0f )
		{
			float directionOffset	=  (float)( rand() % 100 ) * 0.001f - mLoadOut->rangedWeapon->spread;
			mFireDirection			= XMFLOAT3( mUpperBody.direction.x + directionOffset, mUpperBody.direction.y, mUpperBody.direction.z + directionOffset );
			IEventPtr E1( new Event_Trigger_Client_Fired_Projectile( mID, loadDir, mFireDirection, mLoadOut->rangedWeapon->GetRandomProjectileSpeed(), mLoadOut->rangedWeapon->range, mLoadOut->rangedWeapon->damage, (int)mLoadOut->rangedWeapon->weaponType ) );
			EventManager::GetInstance()->QueueEvent( E1 );
		}
		else
		{
			IEventPtr E1( new Event_Trigger_Client_Fired_Projectile( mID, loadDir, mUpperBody.direction, mLoadOut->rangedWeapon->GetRandomProjectileSpeed(), mLoadOut->rangedWeapon->range, mLoadOut->rangedWeapon->damage, (int)mLoadOut->rangedWeapon->weaponType ) );
			EventManager::GetInstance()->QueueEvent( E1 );
		}
	}
}

void Player::Melee( float deltaTime )
{
	MeleeInfo* currWeapon = mLoadOut->meleeWeapon;
	if( currWeapon->weaponType == HAMMER )
	{
		HammerMelee( deltaTime );
	}
	else if( currWeapon->weaponType == BLOWTORCH )
	{
		BlowtorchMelee( deltaTime );
	}
	else if ( currWeapon->weaponType == CLAYMORE )
	{
		ClaymoreMelee( deltaTime );
	}
	else if( currWeapon->weaponType == SAW )
	{
		SawMelee( deltaTime );
	}
}

void Player::FireShotgun( XMFLOAT3* spawnPoint )
{
	// Fire shotgun
	//XMFLOAT3 shotDir = XMFLOAT3( 0.0f, 0.0f, 0.0f );
	float directionOffsetX;
	float directionOffsetZ;
	switch( mLoadOut->rangedWeapon->level )
	{
		case 5:
		{
				//// projectile 6
				directionOffsetX	=  (float)(( rand() % 100 - 50 ) * 0.001f) * mLoadOut->rangedWeapon->spread;// - mLoadOut->rangedWeapon->spread;
				directionOffsetZ	=  (float)(( rand() % 100 - 50 ) * 0.001f) * mLoadOut->rangedWeapon->spread;// - mLoadOut->rangedWeapon->spread;
				mFireDirection			= XMFLOAT3( mUpperBody.direction.x + directionOffsetX, mUpperBody.direction.y, mUpperBody.direction.z + directionOffsetZ );// + sin(directionOffset) );
				EventManager::GetInstance()->QueueEvent( (IEventPtr)new Event_Trigger_Client_Fired_Projectile( mID, *spawnPoint, mFireDirection, mLoadOut->rangedWeapon->projectileSpeed, mLoadOut->rangedWeapon->range, mLoadOut->rangedWeapon->damage, (int)mLoadOut->rangedWeapon->weaponType ) );

				// projectile 7
				directionOffsetX	=  (float)(( rand() % 100 - 50 ) * 0.001f) * mLoadOut->rangedWeapon->spread;// - mLoadOut->rangedWeapon->spread;
				directionOffsetZ	=  (float)(( rand() % 100 - 50 ) * 0.001f) * mLoadOut->rangedWeapon->spread;// - mLoadOut->rangedWeapon->spread;
				mFireDirection			= XMFLOAT3( mUpperBody.direction.x + directionOffsetX, mUpperBody.direction.y, mUpperBody.direction.z + directionOffsetZ );// + sin(directionOffset) );
				EventManager::GetInstance()->QueueEvent( (IEventPtr)new Event_Trigger_Client_Fired_Projectile( mID, *spawnPoint, mFireDirection, mLoadOut->rangedWeapon->projectileSpeed, mLoadOut->rangedWeapon->range, mLoadOut->rangedWeapon->damage, (int)mLoadOut->rangedWeapon->weaponType ) );
		}


		case 4:
		{
				directionOffsetX	=  (float)(( rand() % 100 - 50 ) * 0.001f) * mLoadOut->rangedWeapon->spread;// - mLoadOut->rangedWeapon->spread;
				directionOffsetZ	=  (float)(( rand() % 100 - 50 ) * 0.001f) * mLoadOut->rangedWeapon->spread;// - mLoadOut->rangedWeapon->spread;
				mFireDirection			= XMFLOAT3( mUpperBody.direction.x + directionOffsetX, mUpperBody.direction.y, mUpperBody.direction.z + directionOffsetZ );// + sin(directionOffset) );
				EventManager::GetInstance()->QueueEvent( (IEventPtr)new Event_Trigger_Client_Fired_Projectile( mID, *spawnPoint, mFireDirection, mLoadOut->rangedWeapon->projectileSpeed, mLoadOut->rangedWeapon->range, mLoadOut->rangedWeapon->damage, (int)mLoadOut->rangedWeapon->weaponType ) );
		}

		case 3:
		{
				//// projectile 4
				directionOffsetX	=  (float)(( rand() % 100 - 50 ) * 0.001f) * mLoadOut->rangedWeapon->spread;// - mLoadOut->rangedWeapon->spread;
				directionOffsetZ	=  (float)(( rand() % 100 - 50 ) * 0.001f) * mLoadOut->rangedWeapon->spread;// - mLoadOut->rangedWeapon->spread;
				mFireDirection			= XMFLOAT3( mUpperBody.direction.x + directionOffsetX, mUpperBody.direction.y, mUpperBody.direction.z + directionOffsetZ );// + sin(directionOffset) );
				EventManager::GetInstance()->QueueEvent( (IEventPtr)new Event_Trigger_Client_Fired_Projectile( mID, *spawnPoint, mFireDirection, mLoadOut->rangedWeapon->projectileSpeed, mLoadOut->rangedWeapon->range, mLoadOut->rangedWeapon->damage, (int)mLoadOut->rangedWeapon->weaponType ) );

				// projectile 5
				directionOffsetX	=  (float)(( rand() % 100 - 50 ) * 0.001f) * mLoadOut->rangedWeapon->spread;// - mLoadOut->rangedWeapon->spread;
				directionOffsetZ	=  (float)(( rand() % 100 - 50 ) * 0.001f) * mLoadOut->rangedWeapon->spread;// - mLoadOut->rangedWeapon->spread;
				mFireDirection			= XMFLOAT3( mUpperBody.direction.x + directionOffsetX, mUpperBody.direction.y, mUpperBody.direction.z + directionOffsetZ );// + sin(directionOffset) );
				EventManager::GetInstance()->QueueEvent( (IEventPtr)new Event_Trigger_Client_Fired_Projectile( mID, *spawnPoint, mFireDirection, mLoadOut->rangedWeapon->projectileSpeed, mLoadOut->rangedWeapon->range, mLoadOut->rangedWeapon->damage, (int)mLoadOut->rangedWeapon->weaponType ) );
		}

		case 2:
		{
				directionOffsetX	=  (float)(( rand() % 100 - 50 ) * 0.001f) * mLoadOut->rangedWeapon->spread;// - mLoadOut->rangedWeapon->spread;
				directionOffsetZ	=  (float)(( rand() % 100 - 50 ) * 0.001f) * mLoadOut->rangedWeapon->spread;// - mLoadOut->rangedWeapon->spread;
				mFireDirection			= XMFLOAT3( mUpperBody.direction.x + directionOffsetX, mUpperBody.direction.y, mUpperBody.direction.z + directionOffsetZ );// + sin(directionOffset) );
				EventManager::GetInstance()->QueueEvent( (IEventPtr)new Event_Trigger_Client_Fired_Projectile( mID, *spawnPoint, mFireDirection, mLoadOut->rangedWeapon->projectileSpeed, mLoadOut->rangedWeapon->range, mLoadOut->rangedWeapon->damage, (int)mLoadOut->rangedWeapon->weaponType ) );
		}

		case 1:
		{
				// middle projectile
				directionOffsetX	=  (float)(( rand() % 100 - 50 ) * 0.001f) * mLoadOut->rangedWeapon->spread;// - mLoadOut->rangedWeapon->spread;
				directionOffsetZ	=  (float)(( rand() % 100 - 50 ) * 0.001f) * mLoadOut->rangedWeapon->spread;// - mLoadOut->rangedWeapon->spread;
				mFireDirection			= XMFLOAT3( mUpperBody.direction.x + directionOffsetX, mUpperBody.direction.y, mUpperBody.direction.z + directionOffsetZ );// + sin(directionOffset) );
				EventManager::GetInstance()->QueueEvent( (IEventPtr)new Event_Trigger_Client_Fired_Projectile( mID, *spawnPoint, mFireDirection, mLoadOut->rangedWeapon->projectileSpeed, mLoadOut->rangedWeapon->range, mLoadOut->rangedWeapon->damage, (int)mLoadOut->rangedWeapon->weaponType ) );

				//// projectile 2
				directionOffsetX	=  (float)(( rand() % 100 - 50 ) * 0.001f) * mLoadOut->rangedWeapon->spread;// - mLoadOut->rangedWeapon->spread;
				directionOffsetZ	=  (float)(( rand() % 100 - 50 ) * 0.001f) * mLoadOut->rangedWeapon->spread;// - mLoadOut->rangedWeapon->spread;
				mFireDirection			= XMFLOAT3( mUpperBody.direction.x + directionOffsetX, mUpperBody.direction.y, mUpperBody.direction.z + directionOffsetZ );// + sin(directionOffset) );
				EventManager::GetInstance()->QueueEvent( (IEventPtr)new Event_Trigger_Client_Fired_Projectile( mID, *spawnPoint, mFireDirection, mLoadOut->rangedWeapon->projectileSpeed, mLoadOut->rangedWeapon->range, mLoadOut->rangedWeapon->damage, (int)mLoadOut->rangedWeapon->weaponType ) );

				// projectile 3
				directionOffsetX	=  (float)(( rand() % 100 - 50 ) * 0.001f) * mLoadOut->rangedWeapon->spread;// - mLoadOut->rangedWeapon->spread;
				directionOffsetZ	=  (float)(( rand() % 100 - 50 ) * 0.001f) * mLoadOut->rangedWeapon->spread;// - mLoadOut->rangedWeapon->spread;
				mFireDirection			= XMFLOAT3( mUpperBody.direction.x + directionOffsetX, mUpperBody.direction.y, mUpperBody.direction.z + directionOffsetZ );// + sin(directionOffset) );
				EventManager::GetInstance()->QueueEvent( (IEventPtr)new Event_Trigger_Client_Fired_Projectile( mID, *spawnPoint, mFireDirection, mLoadOut->rangedWeapon->projectileSpeed, mLoadOut->rangedWeapon->range, mLoadOut->rangedWeapon->damage, (int)mLoadOut->rangedWeapon->weaponType ) );
		}
	}
}

void Player::FireMinigun( XMFLOAT3* projectileOffset )
{
	if( mLoadOut->rangedWeapon->overheat <= 100 )
	{
		IEventPtr E2( new Event_Client_Attack( mID, RIGHT_ARM_ID, mWeaponAnimations[mLoadOut->rangedWeapon->weaponType][ATTACK] ) );
		QueueEvent( E2 );
		float directionOffsetX	=  (float)(( rand() % 100 - 50 ) * 0.001f) * mLoadOut->rangedWeapon->spread;// - mLoadOut->rangedWeapon->spread;
		float directionOffsetZ	=  (float)(( rand() % 100 - 50 ) * 0.001f) * mLoadOut->rangedWeapon->spread;// - mLoadOut->rangedWeapon->spread;
		mFireDirection			= XMFLOAT3( mUpperBody.direction.x + directionOffsetX, mUpperBody.direction.y, mUpperBody.direction.z + directionOffsetZ );// + sin(directionOffset) );

		XMFLOAT3 transFloat;
		transFloat.x = projectileOffset->x + ( mUpperBody.direction.x * 0.4f );
		transFloat.y = projectileOffset->y;
		transFloat.z = projectileOffset->z + ( mUpperBody.direction.z * 0.4f );


		IEventPtr E1( new Event_Trigger_Client_Fired_Projectile( mID, transFloat, mFireDirection, mLoadOut->rangedWeapon->GetRandomProjectileSpeed(), mLoadOut->rangedWeapon->range, mLoadOut->rangedWeapon->damage, (int)mLoadOut->rangedWeapon->weaponType ) );
		EventManager::GetInstance()->QueueEvent( E1 );

		mLoadOut->rangedWeapon->overheat += MINIGUN_OVERHEAT;
		mTimeSinceLastShot = MINIGUN_OVERHEAT_CD;

		if( mWeaponOverheated )
		{
			mWeaponOverheated = false;
		}
		//XMFLOAT3 transFloat;
		//transFloat.x = projectileOffset->x + ( mUpperBody.direction.x * 0.4f );
		//transFloat.y = projectileOffset->y;
		//transFloat.z = projectileOffset->z + ( mUpperBody.direction.z * 0.4f );
		//RenderManager::GetInstance()->RequestParticleSystem( mID, MuzzleFlash, transFloat, mUpperBody.direction, mVelocity );
		//transFloat.x = projectileOffset->x + ( mUpperBody.direction.x * 0.7 );
		//transFloat.z = projectileOffset->z + ( mUpperBody.direction.z * 0.7 );
		//RenderManager::GetInstance()->RequestParticleSystem( mID, Spark, transFloat, mUpperBody.direction, mVelocity );	

	}
	else
	{
		mWeaponCoolDown						= MINIGUN_OVERHEAT_CD;
		mLoadOut->rangedWeapon->overheat	= 0.0f;
		mTimeSinceLastShot					= 0.0f;
		mWeaponOverheated					= true;
		SoundBufferHandler::GetInstance()->Play3D( mMiniGunOverheat , GetPosition() );

		XMFLOAT3 weaponOffsets = MINIGUN_OFFSETS;

		XMVECTOR position	= XMLoadFloat3( &mLowerBody.position );
		XMVECTOR direction	= XMLoadFloat3( &mUpperBody.direction );
		XMVECTOR offset		= XMLoadFloat3( &XMFLOAT3( mLowerBody.position.x, weaponOffsets.z, mLowerBody.position.z ) );
	
		offset += XMVector3Normalize( XMVector3Cross( XMLoadFloat3( &XMFLOAT3( 0.0f, 1.0f, 0.0f ) ), direction ) ) * weaponOffsets.y;
		offset += direction * weaponOffsets.x;

		XMFLOAT3 loadDir;
		XMStoreFloat3( &loadDir, offset );

		IEventPtr E2( new Event_Client_Request_ParticleSystem( mID, (int)NormalSmoke, loadDir, DirectX::XMFLOAT3( 0.0f, 1.0f, 0.0f ), mVelocity ) );
		QueueEvent( E2 );

		if( mWeaponCoolDown <= 0.0f )
		{
			mWeaponOverheated = false;
		}
	}
}

void Player::FireGrenadeLauncher( XMFLOAT3* projectileOffset )
{
	float elevation = CalculateLaunchAngle();

	mFireDirection			= XMFLOAT3( mUpperBody.direction.x, elevation, mUpperBody.direction.z );
	SoundBufferHandler::GetInstance()->Play3D( mGrenadeLauncher , GetPosition() );
	IEventPtr E1( new Event_Trigger_Client_Fired_Projectile( mID, *projectileOffset, mFireDirection, mLoadOut->rangedWeapon->projectileSpeed, mLoadOut->rangedWeapon->range, mLoadOut->rangedWeapon->damage, (int)mLoadOut->rangedWeapon->weaponType ) );
	EventManager::GetInstance()->QueueEvent( E1 );
}

void Player::HammerMelee( float deltaTime )
{
	if( mMeleeCoolDown <= 0.0f )
	{
		IEventPtr E1( new Event_Client_Attack( mID, LEFT_ARM_ID, mWeaponAnimations[mLoadOut->meleeWeapon->weaponType][ATTACK] ) );
		QueueEvent( E1 );
		mLeftArmAnimationCompleted	= false;
		RenderManager::GetInstance()->AnimationStartNew( mArms.leftArm, mWeaponAnimations[mLoadOut->meleeWeapon->weaponType][ATTACK] );
		mMeleeCoolDown				= mLoadOut->meleeWeapon->attackRate;
	}
	else
	{
		if( mHasMeleeStarted )
			mTimeTillattack -= deltaTime;

		if( mTimeTillattack <= 0.0f && mHasMeleeStarted )
		{
			mIsMeleeing			= true;
			SoundBufferHandler::GetInstance()->Play3D( mHammerSound , GetPosition() );
			mTimeTillattack		= mLoadOut->meleeWeapon->timeTillAttack;
			mHasMeleeStarted	= false;
			IEventPtr E1( new Event_Client_Request_ParticleSystem( mID, (int)Hammer_Effect, XMFLOAT3( mLoadOut->meleeWeapon->boundingCircle->center.x, 0.3f, mLoadOut->meleeWeapon->boundingCircle->center.z ) , XMFLOAT3( 1.0f, 0.0f, 1.0f ), mVelocity ) );
			QueueEvent( E1 );
		}
	}
}

void Player::BlowtorchMelee( float deltaTime )
{
		XMFLOAT3 weaponOffsets = BLOWTORCH_OFFSETS;

		weaponOffsets = XMFLOAT3( weaponOffsets.x + 1.25f, weaponOffsets.y, weaponOffsets.z );
		XMVECTOR position	= XMLoadFloat3( &mLowerBody.position );
		XMVECTOR direction	= XMLoadFloat3( &mUpperBody.direction );
		XMVECTOR offset		= XMLoadFloat3( &XMFLOAT3( mLowerBody.position.x, mLowerBody.position.y + weaponOffsets.z, mLowerBody.position.z ) );
	
		offset += XMVector3Normalize( XMVector3Cross( XMLoadFloat3( &XMFLOAT3( 0.0f, 1.0f, 0.0f ) ), direction ) ) * weaponOffsets.y;
		offset += direction * weaponOffsets.x;

		XMFLOAT3 loadDir;
		XMStoreFloat3( &loadDir, offset );
		if( mHasMeleeStarted )
		{
			mTimeTillattack -= deltaTime;
			IEventPtr E1( new Event_Client_Request_ParticleSystem( mID, (int)BlowTorchFire, loadDir, mUpperBody.direction, mVelocity ) );
			QueueEvent( E1 );
			SoundBufferHandler::GetInstance()->Play3D( mBlowTorch , GetPosition() );
			if( mTimeTillattack <= 0.0f )
			{
				mTimeTillattack		= mLoadOut->meleeWeapon->timeTillAttack;
				mIsMeleeing			= true;
				mHasMeleeStarted	= false;
			}
			else
			{
				mIsMeleeing			= false;
			}
		}
}

void Player::BlowtorchIdle()
{
	XMFLOAT3 weaponOffsets = BLOWTORCH_OFFSETS;

	weaponOffsets = XMFLOAT3( weaponOffsets.x + 1.25f, weaponOffsets.y, weaponOffsets.z );
	XMVECTOR position	= XMLoadFloat3( &mLowerBody.position );
	XMVECTOR direction	= XMLoadFloat3( &mUpperBody.direction );
	XMVECTOR offset		= XMLoadFloat3( &XMFLOAT3( mLowerBody.position.x, mLowerBody.position.y + weaponOffsets.z, mLowerBody.position.z ) );
	
	offset += XMVector3Normalize( XMVector3Cross( XMLoadFloat3( &XMFLOAT3( 0.0f, 1.0f, 0.0f ) ), direction ) ) * weaponOffsets.y;
	offset += direction * weaponOffsets.x;

	XMFLOAT3 loadDir;
	XMStoreFloat3( &loadDir, offset );

	IEventPtr E1( new Event_Client_Request_ParticleSystem( mID, (int)BlowTorchIdle, loadDir, mUpperBody.direction, mVelocity ) );
	QueueEvent( E1 );
}

void Player::ClaymoreMelee( float deltaTime )
{
	if( mMeleeCoolDown <= 0.0f )
	{
		IEventPtr E1( new Event_Client_Attack( mID, LEFT_ARM_ID, mWeaponAnimations[mLoadOut->meleeWeapon->weaponType][ATTACK] ) );
		QueueEvent( E1 );
		mLeftArmAnimationCompleted	= false;
		RenderManager::GetInstance()->AnimationStartNew( mArms.leftArm, mWeaponAnimations[mLoadOut->meleeWeapon->weaponType][ATTACK] );
		mMeleeCoolDown				= mLoadOut->meleeWeapon->attackRate;
	}
	else
	{
		if( mHasMeleeStarted )
			mTimeTillattack -= deltaTime;

		if( mTimeTillattack <= 0.0f && mHasMeleeStarted )
		{
			mIsMeleeing			= true;
			//RenderManager::GetInstance()->RequestParticleSystem( mID, Hammer_Effect, XMFLOAT3( mLoadOut->meleeWeapon->boundingCircle->center.x, 0.3f, mLoadOut->meleeWeapon->boundingCircle->center.z ) , XMFLOAT3( 1.0f, 0.0f, 1.0f ) );
			mTimeTillattack		= mLoadOut->meleeWeapon->timeTillAttack;
			mHasMeleeStarted	= false;
		}
	}
}

void Player::SawMelee( float deltaTime )
{
	if( mMeleeCoolDown <= 0.0f )
	{
		IEventPtr E1( new Event_Client_Attack( mID, LEFT_ARM_ID, mWeaponAnimations[mLoadOut->meleeWeapon->weaponType][ATTACK] ) );
		QueueEvent( E1 );
		mLeftArmAnimationCompleted	= false;
		RenderManager::GetInstance()->AnimationStartNew( mArms.leftArm, mWeaponAnimations[mLoadOut->meleeWeapon->weaponType][ATTACK] );
		mMeleeCoolDown				= mLoadOut->meleeWeapon->attackRate;
	}
	else
	{
		if( mHasMeleeStarted )
			mTimeTillattack -= deltaTime;

		if( mTimeTillattack <= 0.0f && mHasMeleeStarted )
		{
			mIsMeleeing			= true;
			mTimeTillattack		= mLoadOut->meleeWeapon->timeTillAttack;
			mHasMeleeStarted	= false;
		}
	}
}

float Player::CalculateLaunchAngle()
{
	float distance	= XMVectorGetX( XMVector3Length( XMLoadFloat3( &mPick ) - XMLoadFloat3( &mLowerBody.position ) ) );
	float speed		= 0.0f;

	if( distance < 3.0f )
		distance = 3.0f;

	if( distance > mLoadOut->rangedWeapon->range )
		distance = mLoadOut->rangedWeapon->range;
	
	speed = mLoadOut->rangedWeapon->projectileSpeed;

	float x = 0.6f * ( 25.0f * distance ) / ( speed* speed );
	
	if( x > 1.0f )
		x = 1.0f;
	
	float result = asin( x );

	if( distance > 7.0f && distance < 10.0f )
		result -= 0.10f;

	if( distance >= 3.0f && distance <= 7.0f )
		result -= 0.15f;
		
	return result;
}

void Player::AddImpuls( XMFLOAT3 impuls )
{
	if ( !mLock )
	{
		mVelocity.x += impuls.x;
		mVelocity.z += impuls.z;
	}
}

void Player::QueueEvent( IEventPtr ptr )
{
	gEventList.push_front( ptr );
}

void Player::UpgradeBody()
{
	mUpgrades.currentBodyLevel++;
	mUpgrades.damageTakenPercentage	-= 0.05f;
	//mCurrentHp = mMaxHp = 100.0f + ( ( mUpgrades.currentBodyLevel - 1 ) * 20.0f ) + ( pow( (float)( mUpgrades.currentBodyLevel - 1 ), 2 ) * 5.0f );
}

void Player::UpgradeLegs()
{
	mUpgrades.currentLegsLevel++;
	mUpgrades.runSpeedFactor = 0.7f + ( (float)mUpgrades.currentLegsLevel *  0.1f );
}

void Player::UpgradeMelee()
{
	mLoadOut->meleeWeapon->LevelUp();
}

void Player::UpgradeRange()
{
	mLoadOut->rangedWeapon->LevelUp();
}

void Player::WriteInteractionText( std::string text, float xPos, float yPos, float scale, XMFLOAT4 color )
{
	float offset = mFont.GetMiddleXPoint( text, scale );
	mFont.WriteText( text, xPos - offset, yPos, scale, color );
}

/////Public

void Player::TakeDamage( float damage, unsigned int shooter )
{
	//if( mIsBuffed )
	//{
	//	float moddedDmg = damage * mBuffMod;
	//	damage -= moddedDmg;
	//}
	mCurrentHp -= ( damage * mUpgrades.damageTakenPercentage );
	if( mCurrentHp < 0.0f )
	{
		mCurrentHp = 0.0f;
	}
	IEventPtr E1( new Event_Client_Update_HP( mID, mCurrentHp ) );
	QueueEvent( E1 );
	if ( !mIsDown && mIsAlive && mCurrentHp <= 0.0f )
	{
		GoDown( shooter );
	}
}

void Player::HandleRevive(float deltaTime)
{
	if (mTimeTillRevive <= 0.0f)
	{
		GoUp();
	}
	else
	{
		mTimeTillRevive -= deltaTime;
	}
}

void Player::Lock()
{
	mVelocity = XMFLOAT3( 0.0f, 0.0f, 0.0f );
	mLock = true;
}

void Player::UnLock()
{
	if (!mIsDown && mIsAlive)
	{
		mLock = false;
	}
}

void Player::Reset()
{
	mTimeSinceLastShot			= 0.0f;
	mWeaponCoolDown				= 0;
	mMeleeCoolDown				= 0;
	mTimeTillattack				= mLoadOut->meleeWeapon->timeTillAttack;
	mIsMeleeing					= false;
	mHasMeleeStarted			= false;
	mLock						= false;
	mCloseToPlayer				= false;

	mMaxVelocity				= 7.7f;
	mVelocity					= XMFLOAT3( 0.0f, 0.0f, 0.0f );
	mCurrentVelocity			= 0.0f;
	mMaxAcceleration			= 20.0f;
	mAcceleration				= XMFLOAT3( 0.0f, 0.0f, 0.0f );
	mDashVelocity				= 1.0f;

	mIsBuffed					= false;
	mLifeRegenerationAmount		= 1.0f; 
	mLifeRegenerationTimer		= 1.4f;

	mTimeTillSpawn				= mSpawnTime;
	mTimeTillDeath				= mDeathTime;
	mTimeTillRevive				= mReviveTime;
	mLastKiller					= 0;

	mLowerBody.position				= XMFLOAT3( 3.0f, 0.0f, 6.0f );
	
	mIsAlive				= true;
	mIsDown					= false;
	mMaxHp					= 100.0f;
	mCurrentHp				= mMaxHp;
	mNrOfDeaths				= 0;
	mNrOfKills				= 0;
	mID						= -1;
	mTeam					= 1;
	mEnergyCellID			= (UINT)-1;
	mPickUpCooldown			= 0.0f;

	mLeftArmAnimationCompleted	= false;
	mRightArmAnimationCompleted	= false;

	mUpperBody.direction	= XMFLOAT3( 0.0f, 0.0f, 0.0f );
	mLowerBody.direction	= XMFLOAT3( 0.0f, 0.0f, 0.0f );

	RenderManager::GetInstance()->AnimationReset( mLowerBody.playerModel[TEAM_ARRAY_ID], mAnimations[PLAYER_ANIMATION::LEGS_IDLE][TEAM_ARRAY_ID] );
	RenderManager::GetInstance()->AnimationReset( mArms.leftArm, mWeaponAnimations[mLoadOut->meleeWeapon->weaponType][WEAPON_ANIMATION::IDLE] );
	RenderManager::GetInstance()->AnimationReset( mArms.rightArm, mWeaponAnimations[mLoadOut->rangedWeapon->weaponType][WEAPON_ANIMATION::IDLE] );

	mUpgrades.currentBodyLevel	= 1;
	mUpgrades.damageTakenPercentage = 1.0f;
	mUpgrades.currentLegsLevel = 1;
	mUpgrades.runSpeedFactor = 0.7f;

	mXP					= 0.0f;
	mNextLevelXP		= 20.0f;
	mCurrentLevel		= 0;
	mCurrentUpgrades	= 0;
}

HRESULT Player::Update( float deltaTime, std::vector<RemotePlayer*> remotePlayers, EnergyCell** energyCells )
{
	//Keep the rootMatrix for legs updated.
	XMFLOAT4X4 upperBody = Graphics::GetInstance()->GetRootMatrix( mLowerBody.playerModel[TEAM_ARRAY_ID] );
	XMMATRIX loadedMat = XMLoadFloat4x4( &upperBody );
	XMMATRIX translate = XMMatrixTranslation( mLowerBody.position.x, mLowerBody.position.y, mLowerBody.position.z );

	float yaw				= -atan2f( mLowerBody.direction.z, mLowerBody.direction.x );
	XMMATRIX legRotation	= XMMatrixRotationRollPitchYaw( 0.0f, yaw, 0.0f );
	XMMATRIX invLegRotation = XMMatrixInverse( nullptr, legRotation );

	yaw						= -atan2f( mUpperBody.direction.z, mUpperBody.direction.x );
	XMMATRIX upperRotation	= XMMatrixRotationRollPitchYaw( 0.0f, yaw, 0.0f );

	XMMATRIX transformation = invLegRotation * upperRotation * loadedMat * legRotation * translate;

	XMStoreFloat4x4( &mLowerBody.rootMatrix, transformation );

	mCloseToPlayer = false;
	for( auto rp : remotePlayers )
	{
		if( rp->IsDown() && mBoundingCircle->Intersect( rp->GetBoundingCircleAura() ) && rp->GetTeam() == mTeam )
		{
			mCloseToPlayer = true;
		}
	}

	////Temp visual aid to know that you're close to another player. Used to know if you can revive.
	//if( mCloseToPlayer )
	//{
	//	mPointLight->colorAndRadius = DirectX::XMFLOAT4( 0.6f, 0.8f, 0.6f, 30.0f );
	//}
	//else
	//{
	//	mPointLight->colorAndRadius = DirectX::XMFLOAT4( 0.8f, 0.8f, 0.8f, 10.0f );
	//}

	if ( !mLock )
	{
		HandleInput( deltaTime, remotePlayers );
		Move( deltaTime );
	}

	// Mele attack
	//mLoadOut->meleeWeapon->boundingCircle->center = GetPosition() + ( GetDirection() * mLoadOut->meleeWeapon->radius );

	// If player is alive, update position. If hp <= 0 kill player
	if( !mIsDown )
	{
		if( mIsAlive )
		{
			//////////////////////////////////
			//ANIMATIONS
			float currentVelocity = XMVectorGetX( XMVector3Length( XMLoadFloat3( &mVelocity ) ) );

			if( mDashVelocity < 1.35f )
			{
				if( currentVelocity < 0.2f )
				{
					if( mLowerBody.playerModel[TEAM_ARRAY_ID].mNextAnimation != mAnimations[PLAYER_ANIMATION::LEGS_IDLE][TEAM_ARRAY_ID] )
					{
						RenderManager::GetInstance()->AnimationStartNew( mLowerBody.playerModel[TEAM_ARRAY_ID], mAnimations[PLAYER_ANIMATION::LEGS_IDLE][TEAM_ARRAY_ID] );
					}
				}
				else
				{
					if(	mLowerBody.playerModel[TEAM_ARRAY_ID].mNextAnimation != mAnimations[PLAYER_ANIMATION::LEGS_WALK][TEAM_ARRAY_ID] )
					{
						RenderManager::GetInstance()->AnimationStartNew( mLowerBody.playerModel[TEAM_ARRAY_ID], mAnimations[PLAYER_ANIMATION::LEGS_WALK][TEAM_ARRAY_ID] );
					}
				}
			}

			RenderManager::GetInstance()->AnimationUpdate( mLowerBody.playerModel[TEAM_ARRAY_ID], 
				mLowerBody.playerModel[TEAM_ARRAY_ID].mNextAnimation == 
				mAnimations[PLAYER_ANIMATION::LEGS_WALK][TEAM_ARRAY_ID] ? deltaTime * currentVelocity / 1.1f * mSlowDown * mUpgrades.runSpeedFactor : deltaTime );

			if( mLeftArmAnimationCompleted && mArms.leftArm.mNextAnimation != mWeaponAnimations[mLoadOut->meleeWeapon->weaponType][IDLE] )
				RenderManager::GetInstance()->AnimationStartNew( mArms.leftArm, mWeaponAnimations[mLoadOut->meleeWeapon->weaponType][IDLE] );

			if( mRightArmAnimationCompleted && mArms.rightArm.mNextAnimation != mWeaponAnimations[mLoadOut->rangedWeapon->weaponType][IDLE] )
				RenderManager::GetInstance()->AnimationStartNew( mArms.rightArm, mWeaponAnimations[mLoadOut->rangedWeapon->weaponType][IDLE] );

			if( mLoadOut->meleeWeapon->weaponType == CLAYMORE )
			{
				RenderManager::GetInstance()->AnimationUpdate( mArms.leftArm, deltaTime * CLAYMORE_SPEED_INCREASE );
			}
			else if( mLoadOut->meleeWeapon->weaponType == HAMMER )
			{
				RenderManager::GetInstance()->AnimationUpdate( mArms.leftArm, deltaTime * HAMMER_SPEED_INCREASE );
			}
			else
			{
				RenderManager::GetInstance()->AnimationUpdate( mArms.leftArm, deltaTime );
			}
			
			RenderManager::GetInstance()->AnimationUpdate( mArms.rightArm, deltaTime );
			////////////////////////////////////
			//ENERGY CELLS
			if( mPickUpCooldown <= 0.0f )
			{
				PickUpEnergyCell( energyCells );
			}
			else
			{
				mPickUpCooldown -= deltaTime;
			}
			//////////////////////////////////////////
			// IF LEAVING AREA
			if( XMVectorGetX( XMVector3LengthSq( XMLoadFloat3( &mLowerBody.position ) ) ) > MAX_ROBOT_RANGE ) // calculated from origo, not intended behaviour i guess.
			{
				if( mIsOutSideZone )
				{
					mLeavingAreaTime -= deltaTime;
					if( mLeavingAreaTime < 0.0f )
					{
						Lock();
						Die();
						BroadcastDeath( 0 );
						mIsOutSideZone = false;
					}
				}
				else
				{
					mIsOutSideZone		= true;
					mLeavingAreaTime	= LEAVING_AREA_TIME;
				}
			}
			else if( mIsOutSideZone )
			{
				mIsOutSideZone = false;
			}

			mPlayerDownSparksTimer -= deltaTime;

			if ( mCurrentHp <= 50 && mPlayerDownSparksTimer < 0.0f )
			{
				// Spawn electricity att upperBody
				XMFLOAT3 newPos;
				float randY = (float)( rand() % 8 ) * 0.1f;

				XMStoreFloat3( &newPos, XMVector3TransformCoord( XMVectorSet( 0.0f, randY, 0.0f, 1.0f ), XMLoadFloat4x4( &mLowerBody.rootMatrix ) ) );

				RenderManager::GetInstance()->RequestParticleSystem( mID, Spark_Electric, newPos, mUpperBody.direction, mVelocity );

				// Spawn spark on lowerBody
				if ( mCurrentHp <= 10 )
				{
					XMFLOAT3 inverseDir;
					randY = (float)( rand() % 8 ) * 0.1f;

					XMStoreFloat3( &inverseDir, -XMLoadFloat3( &mUpperBody.direction ) );
					XMStoreFloat3( &newPos, XMVector3TransformCoord( XMVectorSet( 0.0f, randY, 0.0f, 1.0f ), XMLoadFloat4x4( &mLowerBody.rootMatrix ) ) );
					
					RenderManager::GetInstance()->RequestParticleSystem( mID, Spark_Robot, newPos, XMFLOAT3( inverseDir.x * randY, inverseDir.y, inverseDir.z * randY ), mVelocity );
					SoundBufferHandler::GetInstance()->Play3D( mPlayerDeath , newPos );
				}
				
				float intensity = ( mCurrentHp * 0.1f ) * 0.6f;
				mPlayerDownSparksTimer = (float)( rand() % 3 + 1 ) * 0.1f + intensity;
			}
		}
		else
		{
			if( mLowerBody.playerModel[TEAM_ARRAY_ID].mNextAnimation != mAnimations[PLAYER_ANIMATION::LEGS_DEATH][TEAM_ARRAY_ID] )
			RenderManager::GetInstance()->AnimationStartNew( mLowerBody.playerModel[TEAM_ARRAY_ID], mAnimations[PLAYER_ANIMATION::LEGS_DEATH][TEAM_ARRAY_ID] );
			RenderManager::GetInstance()->AnimationUpdate( mLowerBody.playerModel[TEAM_ARRAY_ID], deltaTime );

			/////////////////////////////////////////////////
			// interpolate upper to face lower direction
			XMVECTOR upLoad		= XMLoadFloat3( &mUpperBody.direction );
			XMVECTOR lowLoad	= XMLoadFloat3( &mLowerBody.direction );
			float change		= min( 1.0f, 3.0f * deltaTime );
			XMStoreFloat3( &mUpperBody.direction, upLoad * ( 1.0f - change ) + lowLoad * change );
			/////////////////////////////////////////////////

			HandleSpawn( deltaTime );
		}
	}
	else
	{
		DropEnergyCell( energyCells );

		if( mLowerBody.playerModel[TEAM_ARRAY_ID].mNextAnimation != mAnimations[PLAYER_ANIMATION::LEGS_DOWN][TEAM_ARRAY_ID] )
			RenderManager::GetInstance()->AnimationStartNew( mLowerBody.playerModel[TEAM_ARRAY_ID], mAnimations[PLAYER_ANIMATION::LEGS_DOWN][TEAM_ARRAY_ID] );
		RenderManager::GetInstance()->AnimationUpdate( mLowerBody.playerModel[TEAM_ARRAY_ID], deltaTime );

		/////////////////////////////////////////////////
		// interpolate upper to face lower direction
		XMVECTOR upLoad		= XMLoadFloat3( &mUpperBody.direction );
		XMVECTOR lowLoad	= XMLoadFloat3( &mLowerBody.direction );
		float change		= min( 1.0f, 3.0f * deltaTime );
		XMStoreFloat3( &mUpperBody.direction, upLoad * ( 1.0f - change ) + lowLoad * change );
		/////////////////////////////////////////////////

		HandleDeath( deltaTime );
	}

	///Lock camera position to player
	XMFLOAT3 cameraPosition;
	cameraPosition.x = mLowerBody.position.x			+ mPlayerToCursor.x;
	cameraPosition.y = mLowerBody.position.y + CAMERA_Y;
	cameraPosition.z = mLowerBody.position.z + CAMERA_Z	+ mPlayerToCursor.z;

	Graphics::GetInstance()->SetEyePosition( CAMERAS_MAIN, cameraPosition );

	cameraPosition.x = mLowerBody.position.x + mPlayerToCursor.x;
	cameraPosition.y = mLowerBody.position.y;
	cameraPosition.z = mLowerBody.position.z + mPlayerToCursor.z;

	Graphics::GetInstance()->SetFocus( CAMERAS_MAIN, cameraPosition );

	//Shadow map camera
	cameraPosition.y = mLowerBody.position.y + 30.0f;
	cameraPosition.z = mLowerBody.position.z;

	Graphics::GetInstance()->SetEyePosition( CAMERAS_SHADOWMAP, cameraPosition );
	Graphics::GetInstance()->SetFocus( CAMERAS_SHADOWMAP, mLowerBody.position );

	//Update Bounding Primitives
	mBoundingBox->position								= mLowerBody.position;
	mBoundingCircle->center								= mLowerBody.position;
	mBoundingCircleAura->center							= mLowerBody.position;

	XMVECTOR position	= XMLoadFloat3( &mLowerBody.position );
	XMVECTOR direction	= XMLoadFloat3( &mUpperBody.direction );
	XMVECTOR offset		= XMLoadFloat3( &XMFLOAT3( mLowerBody.position.x, mLowerBody.position.y + mLoadOut->meleeWeapon->offSet.z, mLowerBody.position.z ) );
	
	offset += XMVector3Normalize( XMVector3Cross( XMLoadFloat3( &XMFLOAT3( 0.0f, 1.0f, 0.0f ) ), direction ) ) * mLoadOut->meleeWeapon->offSet.y;
	offset += direction * ( mLoadOut->meleeWeapon->offSet.x );

	XMFLOAT3 corrPos;
	XMStoreFloat3( &corrPos, offset );

	mLoadOut->meleeWeapon->boundingCircle->center = XMFLOAT3( corrPos.x, 0.0f, corrPos.z );

	//Update Light
	mPointLight->positionAndIntensity = DirectX::XMFLOAT4( mLowerBody.position.x, mLowerBody.position.y + 1.0f, mLowerBody.position.z, 1.0f );

	//== Event to sync player with server ==

	IEventPtr E1( new Event_Trigger_Client_Update( mID, mLowerBody.position, mVelocity, mUpperBody.direction, mPlayerName, mIsBuffed, mIsAlive ) );
	EventManager::GetInstance()->QueueEvent( E1 );


	if( mIsBuffed && mCurrentHp < mMaxHp && mCurrentHp != 0.0f )
	{
		mLifeRegenerationTimer -= deltaTime;

		if( mLifeRegenerationTimer <= 0.0f )
		{
			mCurrentHp	+= mLifeRegenerationAmount;
			
			if( mCurrentHp > mMaxHp )
				mCurrentHp = mMaxHp;

			mLifeRegenerationTimer	= 1.4f - ( (float)( mBufflevel - 1 ) * 0.6f );
		}
	}
	else
		mLifeRegenerationTimer =  1.4f - ( (float)( mBufflevel - 1 ) * 0.6f );

	return S_OK;
}

HRESULT Player::Render( float deltaTime, int position )
{
	if( !mIsAlive )
	{
        std::string textToWrite = std::to_string( (int)( mTimeTillSpawn + 1 ) );
		WriteInteractionText( 
			textToWrite, 
			(float)Input::GetInstance()->mScreenWidth * 0.5f, 
			(float)( Input::GetInstance()->mScreenHeight * 0.5f ), 
			7.8f, 
			COLOR_RED );
	}

	if( mIsOutSideZone )
	{
		WriteInteractionText( 
			"Robot losing connection get back!",
			(float)( Input::GetInstance()->mScreenWidth * 0.5f ),
			(float)( Input::GetInstance()->mScreenHeight * 0.25 ), 
			4.0f,
			COLOR_RED );

		WriteInteractionText( 
			std::to_string( (int)( mLeavingAreaTime + 1 ) ),
			(float)( Input::GetInstance()->mScreenWidth * 0.5f ),
			(float)( Input::GetInstance()->mScreenHeight * 0.25 ) + 40.0f, 
			4.0f,
			COLOR_RED );
	}

	if( mEnergyCellID != (UINT)-1 )
	{
		WriteInteractionText( 
			"Head back to your ship!", 
			(float)( Input::GetInstance()->mScreenWidth * 0.5f ),
			(float)( Input::GetInstance()->mScreenHeight * 0.10f ), 
			2.0f,
			COLOR_CYAN );
	}

	if( mCloseToPlayer )
	{
		WriteInteractionText( 
			"Hold F to revive team mate!", 
			(float)( Input::GetInstance()->mScreenWidth * 0.5f ),
			(float)( Input::GetInstance()->mScreenHeight * 0.25 ) + 25.0f, 
			2.0f,
			COLOR_CYAN );
	}

	RemotePlayer::Render();
	//---------------------------DEBUG RENDERING----------------------------
	//MeleeInfo* currWeapon = mLoadOut->meleeWeapon;
	//RenderManager::GetInstance()->AddCircleToList( currWeapon->boundingCircle->center, DirectX::XMFLOAT3( 0, 2, 1 ), currWeapon->boundingCircle->radius );
	//RenderManager::GetInstance()->AddBoxToList( XMFLOAT3( mPick.x - 0.5f, mPick.y - 0.5f, mPick.z - 0.5f ), XMFLOAT3( mPick.x + 0.5f, mPick.y + 0.5f, mPick.z + 0.5f ) );
	//RenderManager::GetInstance()->AddCircleToList( mLoadOut->meleeWeapon->boundingCircle->center, DirectX::XMFLOAT3(1,1,0), mLoadOut->meleeWeapon->boundingCircle->radius );
	//RenderManager::GetInstance()->AddCircleToList( mBoundingCircle->center, DirectX::XMFLOAT3(0,1,0), mBoundingCircle->radius );
	//if( mHasMeleeStarted )
	//{
	//	
	//	XMVECTOR meeleRadiusVector =  ( XMLoadFloat3( &mUpperBody.direction ) * currWeapon->radius );

	//	float halfRadian = XMConvertToRadians( currWeapon->spread * 18.0f ) * 0.5f;

	//	XMVECTOR leftVector = XMVector3Rotate( meeleRadiusVector, XMQuaternionRotationRollPitchYawFromVector( XMVectorSet( 0, -halfRadian, 0 , 1 ) ) );
	//	XMVECTOR rightVector = XMVector3Rotate( meeleRadiusVector, XMQuaternionRotationRollPitchYawFromVector( XMVectorSet( 0, halfRadian, 0 , 1 ) ) );

	//	XMFLOAT3 leftEnd, rightEnd;
	//	XMFLOAT3 pos = currWeapon->boundingCircle->center;
	//	
	//	XMStoreFloat3( &leftEnd, XMLoadFloat3( &pos ) + leftVector );
	//	XMStoreFloat3( &rightEnd, XMLoadFloat3( &pos ) + rightVector );
	//	RenderManager::GetInstance()->AddCircleToList( currWeapon->boundingCircle->center, DirectX::XMFLOAT3( 0, 1, 1 ), currWeapon->boundingCircle->radius );
	//	RenderManager::GetInstance()->AddLineToList( pos, XMFLOAT3( leftEnd.x, 0.1f, leftEnd.z ) );
	//	RenderManager::GetInstance()->AddLineToList( pos, XMFLOAT3( rightEnd.x, 0.1f, rightEnd.z ) );
	//}
	//---------------------------DEBUG RENDERING----------------------------


	return S_OK;
}

HRESULT Player::Initialize()
{
	mPointLight			= nullptr;

	mWeaponOverheated	= false;
	mTimeSinceLastShot	= 0.0f;
	mWeaponCoolDown		= 0.0f;
	mMeleeCoolDown		= 0.0f;
	mTimeTillattack		= 0.0f;
	mIsMeleeing			= false;
	mLock				= false;
	mCloseToPlayer		= false;

	mMaxVelocity		= 0.0f;
	mCurrentVelocity	= 0.0f;
	mMaxAcceleration	= 0.0f;
	mDashVelocity		= 1.0f;
	mAcceleration		= XMFLOAT3( 0.0f, 0.0f, 0.0f );
	mFireDirection		= XMFLOAT3( 0.0f, 0.0f, 0.0f );
	mIsOutSideZone		= false;
	mIsInWater			= false;
	mHasMeleeStarted	= false;
	mXP					= 0.0f;
	mCurrentLevel		= 0;
	
	mSpawnTime				= 0.0f;
	mTimeTillSpawn			= 0.0f;
	mDeathTime				= 0.0f;
	mTimeTillDeath			= 0.0f;
	mReviveTime				= 0.0f;
	mTimeTillRevive			= 0.0f;
	mLeavingAreaTime		= 0.0f;
	mWaterDamageTime		= 0.0f;
	mLastKiller				= 0;

	mLifeRegenerationAmount	= 0.0f;
	mLifeRegenerationTimer	= 0.0f;
	mBufflevel				= 0;

	mDashCoolDown			= 0.0f;

	gEventList				= std::list<IEventPtr>(); 

	RemotePlayer::Initialize();

	mFollowPath = false;

	srand( (unsigned int)time( NULL ) );

	mPlayerName = "";
	
	////////////
	// Light
	////////////
	mPointLight							= new PointLight;
	mPointLight->positionAndIntensity	= DirectX::XMFLOAT4( mLowerBody.position.x, mLowerBody.position.y, mLowerBody.position.z, 1.0f );
	IEventPtr reg( new Event_Add_Point_Light( mPointLight ) );
	EventManager::GetInstance()->QueueEvent( reg );

	mPointLight->colorAndRadius		= DirectX::XMFLOAT4( 0.8f, 0.8f, 0.8f, 10.0f );

	mMaxVelocity		= 7.7f;
	mCurrentVelocity	= 0.0f;
	mMaxAcceleration	= 20.0f;
	mSlowDown			= 1.0f;
	mAcceleration		= XMFLOAT3( 0.0f, 0.0f, 0.0f );
	mVelocity			= XMFLOAT3( 0.0f, 0.0f, 0.0f );



	mNextLevelXP		= 20.0f;
	mCurrentUpgrades	= 0;

	mReviveTime			= 2.0f;
	mTimeTillRevive		= mReviveTime;

	mMiniGunOverheat	= SoundBufferHandler::GetInstance()->Load3DBuffer( "../Content/Assets/Sound/minigun_Overheat.wav", 10 );
	mHammerSound		= SoundBufferHandler::GetInstance()->Load3DBuffer( "../Content/Assets/Sound/hammer.wav", 10 );
	mSword				= SoundBufferHandler::GetInstance()->Load3DBuffer( "../Content/Assets/Sound/sword.wav", 10 );
	mGrenadeLauncher	= SoundBufferHandler::GetInstance()->Load3DBuffer( "../Content/Assets/Sound/grenadeLauncher.wav", 10 );
	mBlowTorch			= SoundBufferHandler::GetInstance()->Load3DBuffer( "../Content/Assets/Sound/tempBlowT.wav", 1000 );
	mPlayerDeath		= SoundBufferHandler::GetInstance()->Load3DBuffer( "../Content/Assets/Sound/sparksPlayerDeath.wav", 10, 15 );

	EventManager::GetInstance()->AddListener( &Player::EventListener, this, Event_Remote_Died::GUID );
	EventManager::GetInstance()->AddListener( &Player::EventListener, this, Event_Remote_Attempt_Revive::GUID );
	EventManager::GetInstance()->AddListener( &Player::EventListener, this, Event_Server_Enemy_Attack_Player::GUID );
	EventManager::GetInstance()->AddListener( &Player::EventListener, this, Event_Remote_Melee_Hit::GUID );
	EventManager::GetInstance()->AddListener( &Player::EventListener, this, Event_Create_Player_Name::GUID );
	EventManager::GetInstance()->AddListener( &Player::EventListener, this, Event_Server_Change_Buff_State::GUID );
	EventManager::GetInstance()->AddListener( &Player::EventListener, this, Event_Upgrade_Player::GUID );
	EventManager::GetInstance()->AddListener( &Player::EventListener, this, Event_New_Player_Spawn_Position::GUID );
	EventManager::GetInstance()->AddListener( &Player::EventListener, this, Event_Server_Switch_Team::GUID );
	EventManager::GetInstance()->AddListener( &Player::EventListener, this, Event_Change_Weapon::GUID );
	EventManager::GetInstance()->AddListener( &Player::EventListener, this, Event_Server_Lobby_Finished::GUID );
	mTimeTillattack	= mLoadOut->meleeWeapon->timeTillAttack;
	mPick = XMFLOAT3( 0, 0, 0 );

	mPickUpCooldown = 0.0f;

	currentPath1 = new Path();

	mCameraPosition = XMFLOAT3( 0.0f, 0.0f, 0.0f );
	mPlayerToCursor = XMFLOAT3( 0.0f, 0.0f, 0.0f );
	
	return S_OK;
}

void Player::Release()
{
	RemotePlayer::Release();

	IEventPtr reg( new Event_Remove_Point_Light( mPointLight ) );
	EventManager::GetInstance()->QueueEvent( reg );
	SAFE_DELETE( mPointLight );
	SAFE_DELETE( currentPath1 );
}

Player::Player()
	:RemotePlayer()
{
	mPointLight			= nullptr;

	mWeaponOverheated	= false;
	mTimeSinceLastShot	= 0.0f;
	mWeaponCoolDown		= 0.0f;
	mMeleeCoolDown		= 0.0f;
	mTimeTillattack		= 0.0f;
	mIsMeleeing			= false;
	mLock				= false;
	mCloseToPlayer		= false;

	mMaxVelocity		= 0.0f;
	mCurrentVelocity	= 0.0f;
	mMaxAcceleration	= 0.0f;
	mDashVelocity		= 0.0f;
	mSlowDown			= 0.0f;
	mAcceleration		= XMFLOAT3( 0.0f, 0.0f, 0.0f );
	mFireDirection		= XMFLOAT3( 0.0f, 0.0f, 0.0f );
	mIsOutSideZone		= false;
	mIsInWater			= false;
	mHasMeleeStarted	= false;
	mXP					= 0.0f;
	mNextLevelXP		= 0.0f;
	
	mSpawnTime			= 0.0f;
	mTimeTillSpawn		= 0.0f;
	mDeathTime			= 0.0f;
	mTimeTillDeath		= 0.0f;
	mReviveTime			= 0.0f;
	mTimeTillRevive		= 0.0f;
	mLeavingAreaTime	= 0.0f;
	mWaterDamageTime	= 0.0f;
	mLastKiller			= 0;

	mDashCoolDown		= 0.0f;

	gEventList			= std::list<IEventPtr>();

	mUpgrades.currentBodyLevel	= 1;
	mUpgrades.damageTakenPercentage = 1.0f;
	mUpgrades.currentLegsLevel = 1;
	mUpgrades.runSpeedFactor = 0.7f;

	mCameraPosition	= XMFLOAT3( 0.0f, 0.0f, 0.0f );
	mPlayerToCursor = XMFLOAT3( 0.0f, 0.0f, 0.0f );
}

Player::~Player()
{}

/////GetSet

bool Player::GetIsMeleeing() const
{
	return mIsMeleeing;
}

XMFLOAT3 Player::GetPlayerPosition() const
{
	return mLowerBody.position;
}

XMFLOAT3 Player::GetUpperBodyDirection() const
{
	return mUpperBody.direction;
}

float Player::GetXPToNext() const
{
	return mXP / mNextLevelXP;
}

int Player::GetCurrentLevel() const
{
	return mCurrentLevel;
}

int Player::Upgradable() const
{
	return mCurrentUpgrades;
}

void Player::SetIsMeleeing( bool isMeleeing )
{
	mIsMeleeing = isMeleeing;
}

void Player::SetID( unsigned int id )
{
	mID = id;
}

void Player::SetTeam( int team )
{
	mTeam		= team;
}

void Player::SetPosition( XMVECTOR position )
{
	XMStoreFloat3( &mLowerBody.position, position );
}

void Player::SetHomePos( XMFLOAT3 pos )
{
	mShipPos = pos;
}
