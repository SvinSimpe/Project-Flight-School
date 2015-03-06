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
			mSpawnPosition = XMFLOAT3( data->SpawnPosition().x, 0.0f, data->SpawnPosition().y );
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
		if( mFollowPath && !currentPath.empty() )
		{
			if( currStep != currentPath.end() )
			{
				mAcceleration = XMFLOAT3( ( (*currStep).x - mLowerBody.position.x ) * mMaxAcceleration, 0, ( (*currStep).y - mLowerBody.position.z ) * mMaxAcceleration );
				XMFLOAT3 step = XMFLOAT3( (*currStep).x, 0, (*currStep).y );
				if( HelperFunctions::Dist3Squared( step, mLowerBody.position  ) < 1.0f )
				{
					currStep = currentPath.erase(currStep);		
				}
			}
		}

		//Normalize acceleration 
		XMVECTOR normalizer = XMVector3Length( XMLoadFloat3( &mAcceleration ) );
		if (XMVectorGetX(normalizer) > mMaxAcceleration)
		{
			normalizer = XMVector3Normalize( XMLoadFloat3( &mAcceleration ) );
			normalizer *= mMaxAcceleration;
			XMStoreFloat3( &mAcceleration, normalizer );
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
		playerToCursor = XMVector3Normalize( XMVectorSet( unPack.x, 0.0f, unPack.z, 0.0f ) );
		XMStoreFloat3( &mUpperBody.direction, playerToCursor );


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
		mWeaponCoolDown = mLoadOut->rangedWeapon->attackRate;
		Fire();

		RenderManager::GetInstance()->AnimationStartNew( mArms.rightArm, mWeaponAnimations[mLoadOut->rangedWeapon->weaponType][ATTACK] );
		mRightArmAnimationCompleted	= false;
		IEventPtr E2( new Event_Client_Attack( mID, RIGHT_ARM_ID, mWeaponAnimations[mLoadOut->rangedWeapon->weaponType][ATTACK] ) );
		QueueEvent( E2 );
		
		//mWeaponCoolDown = 2.0f;
	}

	if( Input::GetInstance()->IsKeyDown( KEYS::KEYS_MOUSE_RIGHT ) && mMeleeCoolDown <= 0.0f && !mHasMeleeStarted )
	{
		//RenderManager::GetInstance()->AnimationStartNew( mArms.leftArm, mWeaponAnimations[mLoadOut->meleeWeapon->weaponType][ATTACK] );
		mLeftArmAnimationCompleted = false;
		RenderManager::GetInstance()->AnimationStartNew( mArms.leftArm, mWeaponAnimations[mLoadOut->meleeWeapon->weaponType][ATTACK] );
		IEventPtr E1( new Event_Client_Attack( mID, LEFT_ARM_ID, mWeaponAnimations[mLoadOut->meleeWeapon->weaponType][ATTACK] ) );
		QueueEvent( E1 );
		mHasMeleeStarted	= true;
		mMeleeCoolDown		= mLoadOut->meleeWeapon->attackRate;
	}

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
		else
		{
			XMFLOAT3 weaponOffsets = MINIGUN_OFFSETS;

			XMVECTOR position	= XMLoadFloat3( &mLowerBody.position );
			XMVECTOR direction	= XMLoadFloat3( &mUpperBody.direction );
			XMVECTOR offset		= XMLoadFloat3( &XMFLOAT3( mLowerBody.position.x, weaponOffsets.z, mLowerBody.position.z ) );
	
			offset += XMVector3Normalize( XMVector3Cross( XMLoadFloat3( &XMFLOAT3( 0.0f, 1.0f, 0.0f ) ), direction ) ) * weaponOffsets.y;
			offset += direction * weaponOffsets.x;

			XMFLOAT3 loadDir;
			XMStoreFloat3( &loadDir, offset );

			RenderManager::GetInstance()->mParticleManager->RequestParticleSystem( mID, NormalSmoke, loadDir, DirectX::XMFLOAT3( 0.0f, 1.0f, 0.0f ) ); 

			if( mWeaponCoolDown <= 0.0f )
			{
				mWeaponOverheated = false;
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
		XMStoreFloat3( &newPos, XMVector3TransformCoord( XMVectorZero(), XMLoadFloat4x4( &mLowerBody.rootMatrix ) ) );

		RenderManager::GetInstance()->RequestParticleSystem( mID, FireSmoke, XMFLOAT3( newPos.x, newPos.y - 0.3f, newPos.z ), XMFLOAT3( 0.0f, -0.1f , 0.0f ) );

		if( mPlayerDownSparksTimer < 0.0f )
		{
			RenderManager::GetInstance()->RequestParticleSystem( mID, Spark_Electric, newPos, mUpperBody.direction );
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

		XMStoreFloat3( &newPos, XMVector3TransformCoord( XMVectorZero(), XMLoadFloat4x4( &mLowerBody.rootMatrix ) ) );
		XMStoreFloat3( &inverseDir, -XMLoadFloat3( &mUpperBody.direction ) );

		RenderManager::GetInstance()->RequestParticleSystem( mID, Spark_Electric, newPos, mUpperBody.direction );
		RenderManager::GetInstance()->RequestParticleSystem( mID, Spark, newPos, inverseDir );

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

void Player::AddXP( int XP )
{
	mXP += XP;
	while( ( mXP / mNextLevelXP ) >= 1 )
	{
		mCurrentUpgrades++;
		mXP -= mNextLevelXP;
	}
}

void Player::PickUpEnergyCell( EnergyCell** energyCells )
{
	for( UINT i = 0; i < MAX_ENERGY_CELLS; i++ )
		{
		if( !energyCells[i]->GetPickedUp() && mEnergyCellID == (UINT)-1 )
			{
				if( energyCells[i]->GetPickUpRadius()->Intersect( mBoundingCircle ) )
				{
					energyCells[i]->SetOwnerID( mID );
					energyCells[i]->SetPickedUp( true );
					energyCells[i]->SetPosition( mLowerBody.position );
					mEnergyCellID = i;

					IEventPtr E1( new Event_Client_Sync_Energy_Cell( i, mID, mLowerBody.position, true ) );
					QueueEvent( E1 );

					IEventPtr reg( new Event_Add_Point_Light( mEnergyCellLight ) );
					EventManager::GetInstance()->QueueEvent( reg );
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
	

		IEventPtr E1( new Event_Client_Sync_Energy_Cell( mEnergyCellID, (UINT)-1, mLowerBody.position, false ) );
		QueueEvent( E1 );

		IEventPtr reg( new Event_Remove_Point_Light( mEnergyCellLight ) );
		EventManager::GetInstance()->QueueEvent( reg );

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
	

		IEventPtr E1( new Event_Client_Sync_Energy_Cell( mEnergyCellID, shipID, shipPos, true ) );
		QueueEvent( E1 );

		IEventPtr reg( new Event_Remove_Point_Light( mEnergyCellLight ) );
		EventManager::GetInstance()->QueueEvent( reg );

		mEnergyCellID	= (UINT)-1;
		mPickUpCooldown = 3.0f;
	}
}

HRESULT Player::UpdateSpecific( float deltaTime, Map* worldMap, std::vector<RemotePlayer*> remotePlayers, EnergyCell** energyCells )
{
	//Comment in for click-to-move
	//if( Input::GetInstance()->IsKeyDown(KEYS::KEYS_MOUSE_LEFT) )
	//{
	//	mFollowPath = false;
	//	Pathfinder::GetInstance()->RequestPath( currentPath1, mLowerBody.position, mPick  );
	//	Pathfinder::GetInstance()->CalculateSubPath( currentPath1 );
	//	currentPath = currentPath1->TotalPath();
	//	currStep = currentPath.begin();
	//}

	// Update water status	
	mIsInWater	= mLowerBody.position.y < -0.7f ? true : false;
	
	XMFLOAT3 testPosition	= mLowerBody.position;
	XMFLOAT3 normal			= XMFLOAT3( 0.0f, 1.0f, 0.0f );
	testPosition.x += mVelocity.x * deltaTime * ( 0.8f + (float)mUpgrades.legs / 5.0f );
	testPosition.z += mVelocity.z * deltaTime * ( 0.8f + (float)mUpgrades.legs / 5.0f );
	testPosition.y = worldMap->GetHeight( testPosition );

	bool collisionTest = worldMap->PlayerVsMap( testPosition, normal );
	if( !collisionTest )
	{
		mLowerBody.position.x = testPosition.x;
		mLowerBody.position.y = testPosition.y;
		mLowerBody.position.z = testPosition.z;

		if( mIsInWater )
		{
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

void Player::FireShotgun( XMFLOAT3* spawnPoint )
{
	// Fire shotgun
	XMFLOAT3 shotDir = XMFLOAT3( 0.0f, 0.0f, 0.0f );
	switch( mLoadOut->rangedWeapon->level )
	{
		case 5:
		{
				//// projectile 6
				XMStoreFloat3( &shotDir, XMVector3TransformNormal( XMLoadFloat3( &mUpperBody.direction ), XMMatrixRotationY( XMConvertToRadians( 15.0f ) ) ) );
				EventManager::GetInstance()->QueueEvent( (IEventPtr)new Event_Trigger_Client_Fired_Projectile( mID, *spawnPoint, shotDir, mLoadOut->rangedWeapon->projectileSpeed, mLoadOut->rangedWeapon->range, mLoadOut->rangedWeapon->damage, (int)mLoadOut->rangedWeapon->weaponType ) );

				// projectile 7
				XMStoreFloat3( &shotDir, XMVector3TransformNormal( XMLoadFloat3( &mUpperBody.direction ), XMMatrixRotationY( XMConvertToRadians( -15.0f ) ) ) );
				EventManager::GetInstance()->QueueEvent( (IEventPtr)new Event_Trigger_Client_Fired_Projectile( mID, *spawnPoint, shotDir, mLoadOut->rangedWeapon->projectileSpeed, mLoadOut->rangedWeapon->range, mLoadOut->rangedWeapon->damage, (int)mLoadOut->rangedWeapon->weaponType ) );
		}


		case 4:
		{
		}

		case 3:
		{
				//// projectile 4
				XMStoreFloat3( &shotDir, XMVector3TransformNormal( XMLoadFloat3( &mUpperBody.direction ), XMMatrixRotationY( XMConvertToRadians( 10.0f ) ) ) );
				EventManager::GetInstance()->QueueEvent( (IEventPtr)new Event_Trigger_Client_Fired_Projectile( mID, *spawnPoint, shotDir, mLoadOut->rangedWeapon->projectileSpeed, mLoadOut->rangedWeapon->range, mLoadOut->rangedWeapon->damage, (int)mLoadOut->rangedWeapon->weaponType ) );

				// projectile 5
				XMStoreFloat3( &shotDir, XMVector3TransformNormal( XMLoadFloat3( &mUpperBody.direction ), XMMatrixRotationY( XMConvertToRadians( -10.0f ) ) ) );
				EventManager::GetInstance()->QueueEvent( (IEventPtr)new Event_Trigger_Client_Fired_Projectile( mID, *spawnPoint, shotDir, mLoadOut->rangedWeapon->projectileSpeed, mLoadOut->rangedWeapon->range, mLoadOut->rangedWeapon->damage, (int)mLoadOut->rangedWeapon->weaponType ) );
		}

		case 2:
		{
		}

		case 1:
		{
				// middle projectile
				EventManager::GetInstance()->QueueEvent( (IEventPtr)new Event_Trigger_Client_Fired_Projectile( mID, *spawnPoint, mUpperBody.direction, mLoadOut->rangedWeapon->projectileSpeed, mLoadOut->rangedWeapon->range, mLoadOut->rangedWeapon->damage, (int)mLoadOut->rangedWeapon->weaponType ) );

				//// projectile 2
				XMStoreFloat3( &shotDir, XMVector3TransformNormal( XMLoadFloat3( &mUpperBody.direction ), XMMatrixRotationY( XMConvertToRadians( 5.0f ) ) ) );
				EventManager::GetInstance()->QueueEvent( (IEventPtr)new Event_Trigger_Client_Fired_Projectile( mID, *spawnPoint, shotDir, mLoadOut->rangedWeapon->projectileSpeed, mLoadOut->rangedWeapon->range, mLoadOut->rangedWeapon->damage, (int)mLoadOut->rangedWeapon->weaponType ) );

				// projectile 3
				XMStoreFloat3( &shotDir, XMVector3TransformNormal( XMLoadFloat3( &mUpperBody.direction ), XMMatrixRotationY( XMConvertToRadians( -5.0f ) ) ) );
				EventManager::GetInstance()->QueueEvent( (IEventPtr)new Event_Trigger_Client_Fired_Projectile( mID, *spawnPoint, shotDir, mLoadOut->rangedWeapon->projectileSpeed, mLoadOut->rangedWeapon->range, mLoadOut->rangedWeapon->damage, (int)mLoadOut->rangedWeapon->weaponType ) );
		}
	}
}

void Player::FireMinigun( XMFLOAT3* projectileOffset )
{
	if( mLoadOut->rangedWeapon->overheat <= 100 )
	{
		float directionOffset	=  (float)( rand() % 100 ) * 0.001f - mLoadOut->rangedWeapon->spread;
		mFireDirection			= XMFLOAT3( mUpperBody.direction.x + directionOffset, mUpperBody.direction.y, mUpperBody.direction.z + directionOffset );
		IEventPtr E1( new Event_Trigger_Client_Fired_Projectile( mID, *projectileOffset, mFireDirection, mLoadOut->rangedWeapon->GetRandomProjectileSpeed(), mLoadOut->rangedWeapon->range, mLoadOut->rangedWeapon->damage, (int)mLoadOut->rangedWeapon->weaponType ) );
		EventManager::GetInstance()->QueueEvent( E1 );

		mLoadOut->rangedWeapon->overheat += MINIGUN_OVERHEAT;
		mTimeSinceLastShot = MINIGUN_OVERHEAT_CD;

		if( mWeaponOverheated )
		{
			mWeaponOverheated = false;
		}
	}
	else
	{
		mWeaponCoolDown						= MINIGUN_OVERHEAT_CD;
		mLoadOut->rangedWeapon->overheat	= 0.0f;
		mTimeSinceLastShot					= 0.0f;
		mWeaponOverheated					= true;
	}
}

void Player::FireGrenadeLauncher( XMFLOAT3* projectileOffset )
{
	float elevation = CalculateLaunchAngle();
	//if( elevation <= 0.0f )
	//	elevation *= -1.0f;

	mFireDirection			= XMFLOAT3( mUpperBody.direction.x, elevation, mUpperBody.direction.z );
	IEventPtr E1( new Event_Trigger_Client_Fired_Projectile( mID, *projectileOffset, mFireDirection, mLoadOut->rangedWeapon->projectileSpeed, mLoadOut->rangedWeapon->range, mLoadOut->rangedWeapon->damage, (int)mLoadOut->rangedWeapon->weaponType ) );
	EventManager::GetInstance()->QueueEvent( E1 );
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

void Player::UpgradeBody()
{
	mUpgrades.body++;
}

void Player::UpgradeLegs()
{
	mUpgrades.legs++;
}

void Player::UpgradeMelee()
{
	mLoadOut->meleeWeapon->LevelUp();
	mUpgrades.melee++;
}

void Player::UpgradeRange()
{
	mLoadOut->rangedWeapon->LevelUp();
	mUpgrades.range++;
}

void Player::QueueEvent( IEventPtr ptr )
{
	gEventList.push_front( ptr );
}

/////Public

void Player::TakeDamage( float damage, unsigned int shooter )
{
	if( mIsBuffed )
	{
		float moddedDmg = damage * mBuffMod;
		damage -= moddedDmg;
	}
	mCurrentHp -= damage / (float)mUpgrades.body;
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
	//mTimeSinceLastShot			= 0.0f;
	//mWeaponCoolDown				= 0;
	//mMeleeCoolDown				= 0;
	//mTimeTillattack				= mLoadOut->meleeWeapon->timeTillAttack;
	//mIsMeleeing					= false;
	//mHasMeleeStarted			= false;
	//mLock						= false;
	//mCloseToPlayer				= false;

	//mMaxVelocity				= 7.7f;
	//mVelocity					= XMFLOAT3( 0.0f, 0.0f, 0.0f );
	//mCurrentVelocity			= 0.0f;
	//mMaxAcceleration			= 20.0f;;
	//mAcceleration				= XMFLOAT3( 0.0f, 0.0f, 0.0f );

	//mIsBuffed					= false;
	//mBuffMod					= 1; // Modifies the damage a player takes by a percentage, should only range between 0 and 1

	//mTimeTillSpawn				= mSpawnTime;
	//mTimeTillDeath				= mDeathTime;
	//mTimeTillRevive				= mReviveTime;
	//mLastKiller					= 0;

	//mLowerBody.position				= XMFLOAT3( 3.0f, 0.0f, 6.0f );
	//
	//mIsAlive				= true;
	//mIsDown					= false;
	//mMaxHp					= 100.0f;
	//mCurrentHp				= mMaxHp;
	//mNrOfDeaths				= 0;
	//mNrOfKills				= 0;
	//mID						= -1;
	//mTeam					= 1;
	//mEnergyCellID			= (UINT)-1;
	//mPickUpCooldown			= 0.0f;

	//mLeftArmAnimationCompleted	= false;
	//mRightArmAnimationCompleted	= false;

	//mUpperBody.direction	= XMFLOAT3( 0.0f, 0.0f, 0.0f );
	//mLowerBody.direction	= XMFLOAT3( 0.0f, 0.0f, 0.0f );

	//RenderManager::GetInstance()->AnimationReset( mLowerBody.playerModel[TEAM_ARRAY_ID], mAnimations[PLAYER_ANIMATION::LEGS_IDLE][TEAM_ARRAY_ID] );
	//RenderManager::GetInstance()->AnimationReset( mArms.leftArm, mWeaponAnimations[mLoadOut->meleeWeapon->weaponType][WEAPON_ANIMATION::IDLE] );
	//RenderManager::GetInstance()->AnimationReset( mArms.rightArm, mWeaponAnimations[mLoadOut->rangedWeapon->weaponType][WEAPON_ANIMATION::IDLE] );

	Release();
	Initialize();
}

HRESULT Player::Update( float deltaTime, std::vector<RemotePlayer*> remotePlayers, EnergyCell** energyCells )
{
	//Keep the rootMatrix for legs updated.
	XMFLOAT4X4 upperBody = Graphics::GetInstance()->GetRootMatrix( mLowerBody.playerModel[TEAM_ARRAY_ID] );
	XMMATRIX loadedMat = XMLoadFloat4x4( &upperBody );
	XMMATRIX translate = XMMatrixTranslation( mLowerBody.position.x, mLowerBody.position.y, mLowerBody.position.z );

	float yaw = -atan2f( mUpperBody.direction.z, mUpperBody.direction.x );
	XMMATRIX rotate	= XMMatrixRotationRollPitchYaw( 0.0f, yaw, 0.0 );

	XMMATRIX transformation = loadedMat * rotate * translate;

	XMStoreFloat4x4( &mLowerBody.rootMatrix, transformation );

	mCloseToPlayer = false;
	for( auto rp : remotePlayers )
	{
		if( rp->IsAlive() && mBoundingCircle->Intersect( rp->GetBoundingCircleAura() ) )
		{
			mCloseToPlayer = true;
		}
	}

	//Temp visual aid to know that you're close to another player. Used to know if you can revive.
	if( mCloseToPlayer )
	{
		mPointLight->colorAndRadius = DirectX::XMFLOAT4( 0.6f, 0.8f, 0.6f, 30.0f );
	}
	else
	{
		mPointLight->colorAndRadius = DirectX::XMFLOAT4( 0.8f, 0.8f, 0.8f, 10.0f );
	}

	if ( !mLock )
	{
		HandleInput( deltaTime, remotePlayers );
		Move( deltaTime );
	}

	// Mele attack
	//mLoadOut->meleeWeapon->boundingCircle->center = GetPosition() + ( GetDirection() * mLoadOut->meleeWeapon->radius );

	if( mHasMeleeStarted )
		mTimeTillattack -= deltaTime;

	if( mTimeTillattack <= 0.0f && mHasMeleeStarted )
	{
		mIsMeleeing						= true;
		//mLeftArmAnimationCompleted		= false;
		if ( mLoadOut->meleeWeapon->weaponType == HAMMER )
		{
			RenderManager::GetInstance()->RequestParticleSystem( mID, Hammer_Effect, XMFLOAT3( mLoadOut->meleeWeapon->boundingCircle->center.x, 0.3f, mLoadOut->meleeWeapon->boundingCircle->center.z ) , XMFLOAT3( 1.0f, 0.0f, 1.0f ) );
		}
		mTimeTillattack		= mLoadOut->meleeWeapon->timeTillAttack;
		mHasMeleeStarted	= false;
	}

	if( mLoadOut->meleeWeapon->weaponType == BLOWTORCH )
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
			RenderManager::GetInstance()->RequestParticleSystem( mID, BlowTorchFire, loadDir, mUpperBody.direction );
		else
			RenderManager::GetInstance()->RequestParticleSystem( mID, BlowTorchIdle, loadDir, mUpperBody.direction );
	}
	// If player is alive, update position. If hp <= 0 kill player
	if( !mIsDown )
	{
		if( mIsAlive )
		{
			//////////////////////////////////
			//ANIMATIONS
			float currentVelocity = XMVectorGetX( XMVector3Length( XMLoadFloat3( &mVelocity ) ) );

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

			RenderManager::GetInstance()->AnimationUpdate( mLowerBody.playerModel[TEAM_ARRAY_ID], 
				mLowerBody.playerModel[TEAM_ARRAY_ID].mNextAnimation == mAnimations[PLAYER_ANIMATION::LEGS_WALK][TEAM_ARRAY_ID] ? deltaTime * currentVelocity / 1.1f : deltaTime );

			if( mLeftArmAnimationCompleted && mArms.leftArm.mNextAnimation != mWeaponAnimations[mLoadOut->meleeWeapon->weaponType][IDLE] )
				RenderManager::GetInstance()->AnimationStartNew( mArms.leftArm, mWeaponAnimations[mLoadOut->meleeWeapon->weaponType][IDLE] );

			if( mRightArmAnimationCompleted && mArms.rightArm.mNextAnimation != mWeaponAnimations[mLoadOut->rangedWeapon->weaponType][IDLE] )
				RenderManager::GetInstance()->AnimationStartNew( mArms.rightArm, mWeaponAnimations[mLoadOut->rangedWeapon->weaponType][IDLE] );

			RenderManager::GetInstance()->AnimationUpdate( mArms.leftArm, deltaTime );
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

			if( mEnergyCellID != (UINT)-1 )
			{
				mEnergyCellLight->position = DirectX::XMFLOAT4( mLowerBody.position.x, 4.0f, mLowerBody.position.z, 0.0f );
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
	cameraPosition.x = mLowerBody.position.x;
	cameraPosition.y = mLowerBody.position.y + 20.0f;
	cameraPosition.z = mLowerBody.position.z - 12.0f;

	Graphics::GetInstance()->SetEyePosition( CAMERAS_MAIN, cameraPosition );
	Graphics::GetInstance()->SetFocus( CAMERAS_MAIN, mLowerBody.position );

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
	mPointLight->position = DirectX::XMFLOAT4( mLowerBody.position.x, mLowerBody.position.y + 1.0f, mLowerBody.position.z, 0.0f );

	//== Event to sync player with server ==

	IEventPtr E1( new Event_Trigger_Client_Update( mID, mLowerBody.position, mVelocity, mUpperBody.direction, mPlayerName, mIsBuffed, mIsAlive ) );
	EventManager::GetInstance()->QueueEvent( E1 );
	return S_OK;
}

HRESULT Player::Render( float deltaTime, int position )
{
	if( !mIsAlive )
	{
        std::string textToWrite = std::to_string( (int)mTimeTillSpawn );
		mFont.WriteText( textToWrite, (float)Input::GetInstance()->mScreenWidth/2, (float)Input::GetInstance()->mScreenHeight/2, 7.8f );
	}

	if( !currentPath.empty() )
	{
		for( UINT i = 0; i < currentPath.size() - 1; i++ )
		{
			XMFLOAT3 start = XMFLOAT3( currentPath[i].x, 0, currentPath[i].y );
			XMFLOAT3 end = XMFLOAT3( currentPath[i + 1].x, 0, currentPath[i + 1].y );
			RenderManager::GetInstance()->AddLineToList( start, end );
		}
	}

	if( mIsOutSideZone )
	{
        std::string textToWrite = "Robot losing connection get back!\n" + std::to_string( (int)mLeavingAreaTime );
		float offset = mFont.GetMiddleXPoint( "Robot losing connection get back!", 3.8f );
		mFont.WriteText( textToWrite, (float)Input::GetInstance()->mScreenWidth/2 - offset, (float)Input::GetInstance()->mScreenHeight/4, 3.8f, COLOR_RED );
	}

	RemotePlayer::Render();
	//---------------------------DEBUG RENDERING----------------------------
	//MeleeInfo* currWeapon = mLoadOut->meleeWeapon;
	//RenderManager::GetInstance()->AddCircleToList( currWeapon->boundingCircle->center, DirectX::XMFLOAT3( 0, 1, 1 ), currWeapon->boundingCircle->radius );
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
	mEnergyCellLight	= nullptr;

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
	mAcceleration		= XMFLOAT3( 0.0f, 0.0f, 0.0f );
	mFireDirection		= XMFLOAT3( 0.0f, 0.0f, 0.0f );
	mIsOutSideZone		= false;
	mIsInWater			= false;
	mHasMeleeStarted	= false;
	mXP					= 0;
	mNextLevelXP		= 0;
	
	mSpawnTime				= 0.0f;
	mTimeTillSpawn			= 0.0f;
	mDeathTime				= 0.0f;
	mTimeTillDeath			= 0.0f;
	mReviveTime				= 0.0f;
	mTimeTillRevive			= 0.0f;
	mLeavingAreaTime		= 0.0f;
	mWaterDamageTime		= 0.0f;
	mLastKiller				= 0;


	gEventList				= std::list<IEventPtr>(); 



	RemotePlayer::Initialize();

	mFollowPath = false;

	srand( (unsigned int)time( NULL ) );

	mPlayerName = "";
	
	////////////
	// Light
	////////////
	mPointLight				= new PointLight;
	mPointLight->position	= DirectX::XMFLOAT4( mLowerBody.position.x, mLowerBody.position.y, mLowerBody.position.z, 0.0f );
	IEventPtr reg( new Event_Add_Point_Light( mPointLight ) );
	EventManager::GetInstance()->QueueEvent( reg );

	mPointLight->colorAndRadius		= DirectX::XMFLOAT4( 0.0f, 0.0f, 1.0f, 5.0f );

	mEnergyCellLight					= new PointLight;
	mEnergyCellLight->position			= DirectX::XMFLOAT4( 0.0f, 0.0f, 0.0f, 0.0f );
	mEnergyCellLight->colorAndRadius	= DirectX::XMFLOAT4( 2.0f, 3.0f, 8.0f, 1.0f );

	mMaxVelocity		= 7.7f;
	mCurrentVelocity	= 0.0f;
	mMaxAcceleration	= 20.0f;
	mAcceleration		= XMFLOAT3( 0.0f, 0.0f, 0.0f );
	mVelocity			= XMFLOAT3( 0.0f, 0.0f, 0.0f );

	mBuffMod			= 0.5f;

	mNextLevelXP		= 10;
	mCurrentUpgrades	= 0;

	mReviveTime			= 2.0f;
	mTimeTillRevive		= mReviveTime;

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

	mEnergyCellID	= (UINT)-1;
	mPickUpCooldown = 0.0f;

	currentPath1 = new Path();
	
	return S_OK;
}

void Player::Release()
{
	RemotePlayer::Release();

	IEventPtr reg( new Event_Remove_Point_Light( mPointLight ) );
	EventManager::GetInstance()->QueueEvent( reg );
	SAFE_DELETE( mPointLight );
	SAFE_DELETE( currentPath1 );
	SAFE_DELETE( mEnergyCellLight );

}

Player::Player()
	:RemotePlayer()
{
	mPointLight			= nullptr;
	mEnergyCellLight	= nullptr;

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
	mAcceleration		= XMFLOAT3( 0.0f, 0.0f, 0.0f );
	mFireDirection		= XMFLOAT3( 0.0f, 0.0f, 0.0f );
	mIsOutSideZone		= false;
	mIsInWater			= false;
	mHasMeleeStarted	= false;
	mXP					= 0;
	mNextLevelXP		= 0;
	
	mSpawnTime				= 0.0f;
	mTimeTillSpawn			= 0.0f;
	mDeathTime				= 0.0f;
	mTimeTillDeath			= 0.0f;
	mReviveTime				= 0.0f;
	mTimeTillRevive			= 0.0f;
	mLeavingAreaTime		= 0.0f;
	mWaterDamageTime		= 0.0f;
	mLastKiller				= 0;

	gEventList				= std::list<IEventPtr>();
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

UINT Player::GetEnergyCellID() const
{
	return mEnergyCellID;
}

float Player::GetXPToNext() const
{
	return (float)( (float)mXP / (float)mNextLevelXP );
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

void Player::SetEnergyCellID( UINT energyCellID )
{
	mEnergyCellID = energyCellID;
}

