#include "PlayState.h"

///////////////////////////////////////////////////////////////////////////////
//									PRIVATE
///////////////////////////////////////////////////////////////////////////////

void PlayState::EventListener( IEventPtr newEvent )
{
	if ( newEvent->GetEventType() == Event_Local_Player_Joined::GUID ) // Add a remote player to the list when they connect
	{
		std::shared_ptr<Event_Local_Player_Joined> data = std::static_pointer_cast<Event_Local_Player_Joined>( newEvent );
		if ( mPlayer != nullptr )
		{
			mPlayer->SetID( data->ID() );
			mPlayer->SetTeam( data->Team(), mTeams[data->Team()] );
			mPlayer->SetColor( mColorIDs[mCurrentColor] );
			mCurrentColor++;
		}
	}

	else if ( newEvent->GetEventType() == Event_Remote_Player_Joined::GUID ) // Add a remote player to the list when they connect
	{
		std::shared_ptr<Event_Remote_Player_Joined> data = std::static_pointer_cast<Event_Remote_Player_Joined>( newEvent );
		mRemotePlayers.push_back( new RemotePlayer() );
		mRemotePlayers.at(mRemotePlayers.size() - 1)->Initialize();
		mRemotePlayers.at(mRemotePlayers.size() - 1)->RemoteInit( data->ID(), data->Team(), mTeams[data->Team()], mColorIDs[mCurrentColor] );
		mCurrentColor++;
		printf( "Number of other players online: %d.\n", mRemotePlayers.size() );

		///TEST
		mAllPlayers.push_back( ( mRemotePlayers.at( mRemotePlayers.size()-1 ) ) );
	}
	else if ( newEvent->GetEventType() == Event_Remote_Player_Left::GUID ) // Remove a remote player from the list when they disconnect
	{
		std::shared_ptr<Event_Remote_Player_Left> data = std::static_pointer_cast<Event_Remote_Player_Left>( newEvent );
		for( unsigned int i = 0; i < mRemotePlayers.size(); i++ )
		{
			if( !mRemotePlayers.at(i) )
			{
				continue;
			}
			else if( data->ID() == mRemotePlayers.at(i)->GetID() )
			{
				mRemotePlayers.at(i)->Release();
				std::swap( mRemotePlayers.at(i), mRemotePlayers.at(mRemotePlayers.size() - 1) );
				mRemotePlayers.pop_back();
				break;
			}
		}
		printf( "Number of other players online: %d.\n", mRemotePlayers.size() );
	}
	else if( newEvent->GetEventType() == Event_Remote_Player_Died::GUID )
	{
		// Kill remote player
 		std::shared_ptr<Event_Remote_Player_Died> data = std::static_pointer_cast<Event_Remote_Player_Died>( newEvent );
		for ( unsigned int i = 0; i < mRemotePlayers.size(); i++ )
		{
			if ( !mRemotePlayers.at(i) )
			{
				continue;
			}
			else if ( data->ID() == mRemotePlayers.at(i)->GetID() )
			{
				mRemotePlayers.at(i)->Die();
				printf( "Killed by: %d\n", data->KillerID() );
				// Debug
				OutputDebugString( L"> A Remote player has died." );

				//break;
			}
			else if( data->KillerID() == mRemotePlayers.at(i)->GetID() )
			{
				printf( "Murderer is: %d\n", data->KillerID() );
				mRemotePlayers.at(i)->CountUpKills();
			}
		}
		if( data->KillerID() == mPlayer->GetID() )
		{
			printf( "The murderer is you player: %d\n", data->KillerID() );
			mPlayer->CountUpKills();
		}
	}
	else if ( newEvent->GetEventType() == Event_Remote_Player_Damaged::GUID )
	{
		// Damage remote player
		std::shared_ptr<Event_Remote_Player_Damaged> data = std::static_pointer_cast<Event_Remote_Player_Damaged>(newEvent);
		HandleRemoteProjectileHit( data->ID(), data->ProjectileID() );
	}
	else if ( newEvent->GetEventType() == Event_Remote_Player_Spawned::GUID )
	{
		std::shared_ptr<Event_Remote_Player_Spawned> data = std::static_pointer_cast<Event_Remote_Player_Spawned>( newEvent );

		for ( size_t i = 0; i < mRemotePlayers.size(); i++ )
		{
			if( mRemotePlayers.at(i)->GetID() == data->ID() )
			{
				mRemotePlayers.at(i)->Spawn();
			}
		}
	}
	else if ( newEvent->GetEventType() == Event_Remote_Projectile_Fired::GUID )
	{
		// Fire projectile
		std::shared_ptr<Event_Remote_Projectile_Fired> data = std::static_pointer_cast<Event_Remote_Projectile_Fired>(newEvent);
		FireProjectile( data->ID(), data->ProjectileID(), data->BodyPos(), data->Direction() );
	}
	else if ( newEvent->GetEventType() == Event_Remote_Player_Melee_Hit::GUID )
	{
		// Melee Hit
		std::shared_ptr<Event_Remote_Player_Melee_Hit> data = std::static_pointer_cast<Event_Remote_Player_Melee_Hit>(newEvent);
		HandleRemoteMeleeHit( data->ID(), data->Damage(), data->KnockBack(), data->Direction() );
	}
	else if ( newEvent->GetEventType() == Event_Remote_Player_Update_HP::GUID )
	{
		std::shared_ptr<Event_Remote_Player_Update_HP> data = std::static_pointer_cast<Event_Remote_Player_Update_HP>( newEvent );

		for ( size_t i = 0; i < mRemotePlayers.size(); i++ )
		{
			if( mRemotePlayers.at(i)->GetID() == data->ID() )
			{
				mRemotePlayers.at(i)->SetHP( data->HP() );
			}
		}
	}
	else if ( newEvent->GetEventType() == Event_Sync_Enemy::GUID )
	{
		std::shared_ptr<Event_Sync_Enemy> data = std::static_pointer_cast<Event_Sync_Enemy>( newEvent );
		SyncEnemy( data->ID(), data->Model(), data->Animation(), data->HP(), data->IsAlive(), data->Position(), data->Direction() );
	}
	else if ( newEvent->GetEventType() == Event_Enemy_List_Synced::GUID )
	{
		mEnemyListSynced = true;
	}
	else if ( newEvent->GetEventType() == Event_Server_Initialized::GUID )
	{
		mServerInitialized = true;
	}
}

void PlayState::SyncEnemy( unsigned int id, unsigned int model, unsigned int animation, float hp, bool alive, XMFLOAT3 position, XMFLOAT3 direction )
{
	mEnemyListSynced = false;
	mEnemies[id]->SetID( id );
	mEnemies[id]->SetModelID( model );
	mEnemies[id]->SetAnimation( animation );
	mEnemies[id]->SetHP( hp );
	mEnemies[id]->SetIsAlive( alive );
	mEnemies[id]->SetPosition( position );
	mEnemies[id]->SetDirection( direction );

	if( id == (MAX_NR_OF_ENEMIES-1) )
		mEnemyListSynced = true;
}

// Tell server that local  player has taken damage
void PlayState::BroadcastDamage( unsigned int playerID, unsigned int projectileID )
{
	IEventPtr dmgEv( new Event_Player_Damaged( playerID, projectileID ) );
	EventManager::GetInstance()->QueueEvent( dmgEv );
}

void PlayState::BroadcastMeleeDamage( unsigned playerID, float damage, float knockBack, XMFLOAT3 direction )
{
	IEventPtr dmgEv( new Event_Player_Melee_Hit( playerID, damage, knockBack, direction ) );
	EventManager::GetInstance()->QueueEvent( dmgEv );
}

void PlayState::FireProjectile( unsigned int id, unsigned int projectileID, XMFLOAT3 position, XMFLOAT3 direction )
{
	mProjectiles[mNrOfProjectilesFired % MAX_PROJECTILES]->SetDirection( id, projectileID, position, direction );
	//mProjectiles[mNrOfProjectilesFired % MAX_PROJECTILES]->SetIsActive( true );
	mNrOfProjectilesFired++;
}

void PlayState::UpdateProjectiles( float deltaTime )
{
	if( mNrOfProjectilesFired != 0 )
	{
		for ( size_t i = 0; i < MAX_PROJECTILES; i++ )
		{
			if( mProjectiles[i]->IsActive() )
				mProjectiles[i]->Update( deltaTime );
		}
	}
}

void PlayState::RenderProjectiles()
{
  	if( mNrOfProjectilesFired != 0 )
	{
		for ( size_t i = 0; i < MAX_PROJECTILES; i++ )
		{
			if( mProjectiles[i]->IsActive() )
				mProjectiles[i]->Render();
		}
	}
}

void PlayState::HandleDeveloperCameraInput()
{
	// TOGGLE CAM
	if( Input::GetInstance()->mCurrentFrame.at( KEYS::KEYS_RCTRL ) )
		Graphics::GetInstance()->ChangeCamera();
	// ZOOM IN
	if( Input::GetInstance()->mCurrentFrame.at( KEYS::KEYS_DOWN ) )
		Graphics::GetInstance()->ZoomOutDeveloperCamera();
	// ZOOM OUT
	if( Input::GetInstance()->mCurrentFrame.at( KEYS::KEYS_UP) )
		Graphics::GetInstance()->ZoomInDeveloperCamera();
}

void PlayState::CheckPlayerCollision()
{	
	if( mRemotePlayers.size() > 0 )
	{
		if( mRemotePlayers.at(0)->GetID() != 0 )
		{
			for (size_t i = 0; i < mRemotePlayers.size(); i++)
			{
				//if( mPlayer->GetBoundingBox()->Intersect( mRemotePlayers.at(i)->GetBoundingBox() ) )		// BoundingBox
				if( mPlayer->GetBoundingCircle()->Intersect( mRemotePlayers.at(i)->GetBoundingCircle() ) )	// BoundingCircle
				{
					//Direction
					XMVECTOR remoteToPlayerVec	= ( XMLoadFloat3( &mPlayer->GetBoundingCircle()->center ) - 
													XMLoadFloat3( &mRemotePlayers.at(i)->GetBoundingCircle()->center ) );

					//Normalize direction
					remoteToPlayerVec = XMVector4Normalize( remoteToPlayerVec );

					//Length of new vector
					float vectorLength = ( mRemotePlayers.at(i)->GetBoundingCircle()->radius + mPlayer->GetBoundingCircle()->radius ) + 0.0001f;
					
					//New position of player			 
					XMVECTOR playerPosition = XMLoadFloat3( &mRemotePlayers.at(i)->GetBoundingCircle()->center ) + remoteToPlayerVec * vectorLength;
					mPlayer->SetPosition( playerPosition );	
				}
			}	
		}
	}
}

void PlayState::CheckProjectileCollision()
{
	if( mRemotePlayers.size() > 0 )
	{
		for (size_t i = 0; i < mRemotePlayers.size(); i++)
		{
			for (size_t j = 0; j < MAX_PROJECTILES; j++)
			{
				if ( mProjectiles[j]->IsActive() )
				{
					if( mProjectiles[j]->GetPlayerID() == mPlayer->GetID() )
					{
						if( mProjectiles[j]->GetBoundingCircle()->Intersect( mRemotePlayers[i]->GetBoundingCircle() ) ) //Player hit remote
						{
							BroadcastDamage( mRemotePlayers[i]->GetID(), mProjectiles[j]->GetID() );
						}			
					}
				}	
			}
		}
	}

	for( size_t i = 0; i < MAX_PROJECTILES; i++ )
	{
		if( mProjectiles[i]->IsActive() )
		{
			if( mProjectiles[i]->GetBoundingCircle()->Intersect( mShip.GetHitBox() ))
			{
				mShip.TakeDamage( 1.0f );
			}
		}
	}
}

void PlayState::CheckMeeleCollision()
{
	for ( size_t i = 0; i < mRemotePlayers.size(); i++ )
	{
		//Check intersection with melee circle & remotePlayer
		if( mPlayer->GetLoadOut()->meleeWeapon->boundingCircle->Intersect( mRemotePlayers.at(i)->GetBoundingCircle() ) )
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
}

void PlayState::HandleRemoteProjectileHit( unsigned int id, unsigned int projectileID )
{
	unsigned int shooter = 0;
	for ( size_t i = 0; i < MAX_PROJECTILES; i++ )
	{
		if( mProjectiles[i]->GetID() == projectileID )
		{
				shooter = mProjectiles[i]->GetPlayerID();
				mProjectiles[i]->Reset();
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
					mPlayer->TakeDamage( mRemotePlayers.at(i)->GetLoadOut()->rangedWeapon->damage, shooter );
				}
			}
		}
	}
}

void PlayState::HandleRemoteMeleeHit( unsigned int id, float damage, float knockBack, XMFLOAT3 direction )
{
	if( id == mPlayer->GetID() )
	{
		direction.x *= knockBack;
		direction.z *= knockBack;
		mPlayer->AddImpuls( direction );
		mPlayer->TakeDamage( damage, 0 );
	}

	for ( size_t i = 0; i < mRemotePlayers.size(); i++ )
	{
		if( mRemotePlayers.at(i)->GetID() == id )
		{
			direction.x *= ( knockBack * 5.0f ); // 1 knock back == 5
			direction.z *= ( knockBack * 5.0f );
			mRemotePlayers.at(i)->AddImpuls( direction );
			mRemotePlayers.at(i)->TakeDamage( damage );
		}
	}
}

///////////////////////////////////////////////////////////////////////////////
//									PUBLIC
///////////////////////////////////////////////////////////////////////////////

HRESULT PlayState::Update( float deltaTime )
{
	if( Input::GetInstance()->mCurrentFrame.at( KEYS::KEYS_SPACE ) )
		mAnimationTime = 1.0f / 60.0f;

	if( mFrameCounter >= COLLISION_CHECK_OFFSET )
	{
		CheckPlayerCollision();
		CheckProjectileCollision();

		if( mPlayer->GetIsMeleeing() )
		{
			CheckMeeleCollision();
			mPlayer->SetIsMeleeing( false );
		}

		mFrameCounter = 0;
	}
	else
		mFrameCounter++;

	HandleDeveloperCameraInput();
	mPlayer->Update( deltaTime );

	UpdateProjectiles( deltaTime );
	mAnimationTime	+= deltaTime;

	mShip.BuffPlayer( mPlayer );
	mShip.PickTurretTarget( mAllPlayers );
	mShip.Update( deltaTime );
	if( mEnemyListSynced )
	{
		for ( size_t i = 0; i < MAX_NR_OF_ENEMIES; i++ )
		{
			if( mEnemies[i]->IsAlive() )
				mEnemies[i]->Update( deltaTime );
		}
	}


	return S_OK;
}

HRESULT PlayState::Render()
{
	RenderManager::GetInstance()->AddObject3dToList( mPlaneAsset, DirectX::XMFLOAT3( 0.0f, 0.0f, 0.0f ) );
	
	RenderManager::GetInstance()->AddAnim3dToList( mTestAnimation, mTestAnimationAnimation, &mAnimationTime, ANIMATION_PLAY_ONCE, DirectX::XMFLOAT3( 0.0f, 0.0f, 0.0f ) );

	mPlayer->Render( 0.0f, 1 );

	mWorldMap->Render( 0.0f , mPlayer );

	for ( size_t i = 0; i < mRemotePlayers.size(); i++)
	{
		if ( mRemotePlayers.at(i) )
		{
			mRemotePlayers.at(i)->Render( 0.0f, i + 2 );
		}
	}

	RenderProjectiles();

	//mFont.WriteText( "HELLO WORLD!\nTIM IS AWESOME!\nTABBING\tIS\tCOOL!\n#YOLO@SWAG.COM", 0.0f, 0.0f, 1.0f );


	// Enemies
  	if( mEnemyListSynced )
	{
		for ( size_t i = 0; i < MAX_NR_OF_ENEMIES; i++ )
		{
			if( mEnemies[i]->IsAlive() )
				mEnemies[i]->Render();
		}
	}

	mShip.Render();


	RenderManager::GetInstance()->Render();

	return S_OK;
}

void PlayState::OnEnter()
{
	// Send Game Started event to server
	IEventPtr E1( new Event_Game_Started() );
	EventManager::GetInstance()->QueueEvent( E1 );
}

void PlayState::OnExit()
{
	// Send Game Started event to server
	IEventPtr E1( new Event_Game_Ended() );
	EventManager::GetInstance()->QueueEvent( E1 );
}

void PlayState::Reset()
{
}

HRESULT PlayState::Initialize()
{
	mStateType = PLAY_STATE;

	Graphics::GetInstance()->LoadStatic3dAsset( "../Content/Assets/Plane/", "plane.pfs", mPlaneAsset );

	AssetID skeleton = 0;
	AssetID skel  =0;

	Graphics::GetInstance()->LoadSkeletonAsset( "../Content/Assets/Raptor/Animations/", "raptor.Skel", skeleton );
	Graphics::GetInstance()->LoadAnimated3dAsset( "../Content/Assets/Raptor/", "scaledScene.apfs", skeleton, mTestAnimation );
	Graphics::GetInstance()->LoadAnimationAsset( "../Content/Assets/Raptor/Animations/", "raptorDeath2.PaMan", mTestAnimationAnimation );

	AssetID loader;

	Graphics::GetInstance()->LoadStatic2dAsset( "../Content/Assets/Textures/burger.png", mTest2dAsset );
	for( int i = 1; i < 8; i++ )
	{
		char buffer[50];
		sprintf_s(buffer,"tree%d.pfs",i);
		Graphics::GetInstance()->LoadStatic3dAsset( "../Content/Assets/Tree/", buffer, loader );
	}
	for( int i = 1; i < 6; i++ )
	{
		char buffer[50];
		sprintf_s(buffer,"greaystone%d.pfs",i);
		Graphics::GetInstance()->LoadStatic3dAsset( "../Content/Assets/Stones/", buffer, loader );
	}
	for( int i = 1; i < 7; i++ )
	{
		char buffer[50];
		sprintf_s(buffer,"sandstone%d.pfs",i);
		Graphics::GetInstance()->LoadStatic3dAsset( "../Content/Assets/Stones/", buffer, loader );
	}
	//for( int i = 1; i < 5; i++ )
	//{
	//	char buffer[50];
	//	sprintf_s(buffer,"bush%d.pfs",i);
	//	Graphics::GetInstance()->LoadStatic3dAsset( "../Content/Assets/Bushes/", buffer, mTree1Asset );
	//}
	//for( int i = 1; i < 3; i++ )
	//{
	//	char buffer[50];
	//	sprintf_s(buffer,"plant%d.pfs",i);
	//	Graphics::GetInstance()->LoadStatic3dAsset( "../Content/Assets/Bushes/", buffer, mTree1Asset );
	//}
	Graphics::GetInstance()->LoadStatic3dAsset( "../Content/Assets/Bushes/", "plant1.pfs", loader );
	std::string colorIDFileNames[MAX_REMOTE_PLAYERS] = { "../Content/Assets/Textures/FunnyCircles/BlueID.png", "../Content/Assets/Textures/FunnyCircles/CoralID.png", "../Content/Assets/Textures/FunnyCircles/DarkBlueID.png", "../Content/Assets/Textures/FunnyCircles/DarkGreenID.png", "../Content/Assets/Textures/FunnyCircles/DarkPurpleID.png", "../Content/Assets/Textures/FunnyCircles/GreenID.png", "../Content/Assets/Textures/FunnyCircles/GreyID.png", "../Content/Assets/Textures/FunnyCircles/LightBlueID.png", "../Content/Assets/Textures/FunnyCircles/LightGreenID.png", "../Content/Assets/Textures/FunnyCircles/LightPurpleID.png","../Content/Assets/Textures/FunnyCircles/OrangeID.png", "../Content/Assets/Textures/FunnyCircles/PinkID.png", "../Content/Assets/Textures/FunnyCircles/ScreamBlueID.png", "../Content/Assets/Textures/FunnyCircles/YellowID.png" };

	for( int i=0; i<MAX_REMOTE_PLAYERS; i++ )
	{
		Graphics::GetInstance()->LoadStatic2dAsset( colorIDFileNames[i], mColorIDs[i] );
	}

	Graphics::GetInstance()->LoadStatic2dAsset( "../Content/Assets/Textures/FunnyCircles/WhiteTeam.png", mTeams[0] );
	Graphics::GetInstance()->LoadStatic2dAsset( "../Content/Assets/Textures/FunnyCircles/BlackTeam.png", mTeams[1] );

	mAnimationTime	= 1.0f;

	mPlayer = new Player();
	mPlayer->Initialize();

	mWorldMap = new Map();
	mWorldMap->Initialize( 4 );

	//mMapNodeMan = new MapNodeManager();
	//mMapNodeMan->Initialize( "../Content/Assets/Nodes/gridtest2.lp"  );

	//Fill up on Projectiles, test values
	mProjectiles	= new Projectile*[MAX_PROJECTILES];
	for ( size_t i = 0; i < MAX_PROJECTILES; i++ )
	{
		mProjectiles[i] = new Projectile();
		mProjectiles[i]->Initialize();
	}

	EventManager::GetInstance()->AddListener( &PlayState::EventListener, this, Event_Local_Player_Joined::GUID );
	EventManager::GetInstance()->AddListener( &PlayState::EventListener, this, Event_Remote_Player_Joined::GUID );
	EventManager::GetInstance()->AddListener( &PlayState::EventListener, this, Event_Remote_Player_Left::GUID );
	EventManager::GetInstance()->AddListener( &PlayState::EventListener, this, Event_Remote_Player_Died::GUID );
	EventManager::GetInstance()->AddListener( &PlayState::EventListener, this, Event_Remote_Player_Damaged::GUID );
	EventManager::GetInstance()->AddListener( &PlayState::EventListener, this, Event_Remote_Player_Spawned::GUID );
	EventManager::GetInstance()->AddListener( &PlayState::EventListener, this, Event_Remote_Projectile_Fired::GUID );
	EventManager::GetInstance()->AddListener( &PlayState::EventListener, this, Event_Remote_Player_Melee_Hit::GUID );
	EventManager::GetInstance()->AddListener( &PlayState::EventListener, this, Event_Remote_Player_Update_HP::GUID );
	EventManager::GetInstance()->AddListener( &PlayState::EventListener, this, Event_Sync_Enemy::GUID );
	EventManager::GetInstance()->AddListener( &PlayState::EventListener, this, Event_Enemy_List_Synced::GUID );
	EventManager::GetInstance()->AddListener( &PlayState::EventListener, this, Event_Server_Initialized::GUID );

	mFont.Initialize( "../Content/Assets/Fonts/final_font/" );

	//TEST
	mAllPlayers.push_back( mPlayer );
	mShip.Initialize( 0, XMFLOAT3( 10.0f, 0.0f, 10.0f ), XMFLOAT3( 1.0f, 0.0f, 0.0f ) );

	// Enemies
	mEnemies	= new Enemy*[MAX_NR_OF_ENEMIES];
	for ( size_t i = 0; i < MAX_NR_OF_ENEMIES; i++ )
	{
		mEnemies[i] = new Enemy();
		mEnemies[i]->Initialize( i );
	}

	return S_OK;
}

void PlayState::Release()
{
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
	mShip.Release();

	for ( size_t i = 0; i < MAX_PROJECTILES; i++ )
	{
		mProjectiles[i]->Release();
		SAFE_DELETE( mProjectiles[i] );
	}
	delete [] mProjectiles;

	// Enemies
	for ( size_t i = 0; i < MAX_NR_OF_ENEMIES; i++ )
		SAFE_DELETE( mEnemies[i] );

	delete [] mEnemies;

	mFont.Release();

}

PlayState::PlayState()
{
	mPlayer				= nullptr;
	mRemotePlayers		= std::vector<RemotePlayer*>( 0 );
	mRemotePlayers.reserve(MAX_REMOTE_PLAYERS);
	mFrameCounter		= 0;
	mProjectiles		= nullptr;
	mEnemies			= nullptr;
	mNrOfEnemies		= 0;
	mMaxNrOfEnemies		= 0;
	mEnemyListSynced	= false;
	mServerInitialized  = false;
}

PlayState::~PlayState()
{
	printf("Destructor for %s\n", __FILE__);
}