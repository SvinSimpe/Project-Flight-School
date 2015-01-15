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
			mPlayer->SetTeam( data->Team() );
		}
	}

	else if ( newEvent->GetEventType() == Event_Remote_Player_Joined::GUID ) // Add a remote player to the list when they connect
	{
		std::shared_ptr<Event_Remote_Player_Joined> data = std::static_pointer_cast<Event_Remote_Player_Joined>( newEvent );
		mRemotePlayers.push_back( new RemotePlayer() );
		mRemotePlayers.at(mRemotePlayers.size() - 1)->Initialize();
		mRemotePlayers.at(mRemotePlayers.size() - 1)->RemoteInit( data->ID(), data->Team() );
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

				// Debug
				OutputDebugString( L"> A Remote player has died." );

				break;
			}
		}
	}
	else if ( newEvent->GetEventType() == Event_Remote_Player_Damaged::GUID )
	{
		// Damage remote player
		std::shared_ptr<Event_Remote_Player_Damaged> data = std::static_pointer_cast<Event_Remote_Player_Damaged>(newEvent);
		for ( unsigned int i = 0; i < mRemotePlayers.size(); i++ )
		{
			if ( !mRemotePlayers.at(i) )
			{
				continue;
			}

			HandleRemoteProjectileHit( data->ID(), data->ProjectileID() );

			//else if ( data->ID() == mRemotePlayers.at(i)->GetID() )
			//{
			//	// Damage player
			//	HandleRemoteProjectileHit( data->ID(), data->ProjectileID() );

			//	// Debug
			//	OutputDebugString( L"> A Remote player has taken damage." );

			//	break;
			//}
		}
	}
	else if ( newEvent->GetEventType() == Event_Remote_Projectile_Fired::GUID )
	{
		// Fire projectile
		std::shared_ptr<Event_Remote_Projectile_Fired> data = std::static_pointer_cast<Event_Remote_Projectile_Fired>(newEvent);
		FireProjectile( data->ID(), data->ProjectileID(), data->BodyPos(), data->Direction() );
	}
}

// Tell server that local  player has taken damage
void PlayState::BroadcastDamage( unsigned int playerID, unsigned int projectileID )
{
	IEventPtr dmgEv(new Event_Player_Damaged( playerID, projectileID ) );
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
					//mPlayer->SetIsMovable( false );
					OutputDebugStringA( "\nPLAYER COLLISION" );

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
							//Add broadcast
							
						}			
					}
				}	
			}
		}
	}
}

void PlayState::CheckMeeleCollision()
{
	XMVECTOR aimingDirection = XMLoadFloat3( &mPlayer->GetUpperBodyDirection() );
	aimingDirection = XMVector4Normalize( aimingDirection );

	XMVECTOR meeleLengthVector = ( XMLoadFloat3( &mPlayer->GetPosition() ) -  ( aimingDirection * 2 ) );
}

void PlayState::HandleRemoteProjectileHit( unsigned int id, unsigned int projectileID )
{
	for (size_t i = 0; i < MAX_PROJECTILES; i++)
	{
		if( mProjectiles[i]->GetID() == projectileID )
			mProjectiles[i]->Reset();

	}
}

///////////////////////////////////////////////////////////////////////////////
//									PUBLIC
///////////////////////////////////////////////////////////////////////////////

HRESULT PlayState::Update( float deltaTime )
{
	OutputDebugStringA( "\n-------------------" );


	if( mFrameCounter >= COLLISION_CHECK_OFFSET )
	{
		CheckPlayerCollision();
		CheckProjectileCollision();
		mFrameCounter = 0;
	}
	else
		mFrameCounter++;

	HandleDeveloperCameraInput();
	mPlayer->Update( deltaTime );

	UpdateProjectiles( deltaTime );
	mAnimationTime	+= deltaTime;
	mRobotTime		+= deltaTime;

	return S_OK;
}

HRESULT PlayState::Render()
{
	Graphics::GetInstance()->BeginScene();

	RenderManager::GetInstance()->AddObject3dToList( mPlaneAsset, DirectX::XMFLOAT3( 0.0f, 0.0f, 0.0f ) );
	RenderManager::GetInstance()->AddObject3dToList( mTestAsset, DirectX::XMFLOAT3( 4.0f, 0.0f, 0.0f ) );
	RenderManager::GetInstance()->AddObject3dToList( mNest1Asset, DirectX::XMFLOAT3( 8.0f, 0.0f, 0.0f ) );
	RenderManager::GetInstance()->AddObject3dToList( mTree1Asset, DirectX::XMFLOAT3( 12.0f, 0.0f, 0.0f ) );

	for( int i = 0; i < 6; i++ )
	{
		RenderManager::GetInstance()->AddObject3dToList( mStoneAssets[i], DirectX::XMFLOAT3( (float)i*4.0f, 0.0f, -4.0f ) );
	}

	RenderManager::GetInstance()->AddAnim3dToList( mTestAnimation, mTestAnimationAnimation, &mAnimationTime );

	Graphics::GetInstance()->RenderAnimated3dAsset( mTestRobot, mTestRobotAni, mRobotTime, 4.0f, 0.0f, 4.0f );

	mPlayer->Render( 0.0f );
	//mWorldMap->Render( 0.0f );
	for( auto& rp : mRemotePlayers )
	{
		if( rp )
			rp->Render( 0.0f );
	}

	RenderManager::GetInstance()->AddObject2dToList( mTest2dAsset, DirectX::XMFLOAT2( 300.0f, 300.0f ), DirectX::XMFLOAT2( 100.0f, 100.0f ) );
	RenderProjectiles();

	DirectX::XMFLOAT3 x = { 0.0f, 0.1f, 3.0f };
	DirectX::XMFLOAT3 y = { 3.0f, 0.1f, 0.0f };

	RenderManager::GetInstance()->AddPlaneToList( mTest2dAsset, x, y );
	RenderManager::GetInstance()->AddObject2dToList( mTest2dAsset, DirectX::XMFLOAT2( 500.0f, 500.0f ), DirectX::XMFLOAT2( 50.0f, 50.0f ) );

	RenderManager::GetInstance()->Render();
	mFont.WriteText( "HELLO WORLD!\nTIM IS AWESOME!\nTABBING\tIS\tCOOL!\n#YOLO@SWAG.COM", 0.0f, 0.0f, 1.0f );
	Graphics::GetInstance()->EndScene();

	return S_OK;
}

void PlayState::OnEnter()
{
}

void PlayState::OnExit()
{
}

void PlayState::Reset()
{
}

HRESULT PlayState::Initialize()
{
	mStateType = PLAY_STATE;

	Graphics::GetInstance()->LoadStatic3dAsset( "../Content/Assets/Plane/", "plane.pfs", mPlaneAsset );
	Graphics::GetInstance()->LoadStatic3dAsset( "../Content/Assets/Ship/", "crashed_ship.pfs", mTestAsset );

	AssetID skeleton = 0;

	Graphics::GetInstance()->LoadSkeletonAsset( "../Content/Assets/Raptor/Animations/", "raptor.Skel", skeleton );
	Graphics::GetInstance()->LoadAnimated3dAsset( "../Content/Assets/Raptor/", "scaledScene.apfs", skeleton, mTestAnimation );
	Graphics::GetInstance()->LoadAnimationAsset( "../Content/Assets/Raptor/Animations/", "raptor_death.PaMan", mTestAnimationAnimation );

	Graphics::GetInstance()->LoadSkeletonAsset( "../Content/Assets/Robot/Animations/", "robot_legs.Skel", skeleton );
	Graphics::GetInstance()->LoadAnimated3dAsset( "../Content/Assets/Robot/", "walkanimationTest5_1.apfs", skeleton, mTestRobot );
	Graphics::GetInstance()->LoadAnimationAsset( "../Content/Assets/Robot/Animations/", "testLegs.PaMan", mTestRobotAni );

	Graphics::GetInstance()->LoadStatic3dAsset( "../Content/Assets/Nests/", "nest_1.pfs", mNest1Asset );
	Graphics::GetInstance()->LoadStatic3dAsset( "../Content/Assets/Stones/", "stone_1.pfs", mStoneAssets[0] );
	Graphics::GetInstance()->LoadStatic3dAsset( "../Content/Assets/Stones/", "stone_2.pfs", mStoneAssets[1] );
	Graphics::GetInstance()->LoadStatic3dAsset( "../Content/Assets/Stones/", "stone_3.pfs", mStoneAssets[2] );
	Graphics::GetInstance()->LoadStatic3dAsset( "../Content/Assets/Stones/", "stone_4.pfs", mStoneAssets[3] );
	Graphics::GetInstance()->LoadStatic3dAsset( "../Content/Assets/Stones/", "stone_5.pfs", mStoneAssets[4] );
	Graphics::GetInstance()->LoadStatic3dAsset( "../Content/Assets/Stones/", "stone_6.pfs", mStoneAssets[5] );

	Graphics::GetInstance()->LoadStatic3dAsset( "../Content/Assets/Tree/", "tree1.pfs", mTree1Asset );

	Graphics::GetInstance()->LoadStatic2dAsset( "../Content/Assets/Textures/burger.png", mTest2dAsset );

	mAnimationTime	= 1.0f;
	mRobotTime		= 1.0f;

	mPlayer = new Player();
	mPlayer->Initialize();

	//mWorldMap = new Map();
	//mWorldMap->Initialize( 8.0f, 24 );

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

	mFont.Initialize( "../Content/Assets/Fonts/mv_boli_26_red/" );



	//TEST
	mAllPlayers.push_back( mPlayer );

	return S_OK;
}

void PlayState::Release()
{
	mPlayer->Release();
	SAFE_DELETE(mPlayer);

	mWorldMap->Release();
	SAFE_DELETE( mWorldMap );

	for( auto& rp : mRemotePlayers )
	{
		rp->Release();
		SAFE_DELETE( rp );
	}

	mRemotePlayers.clear();

	for ( size_t i = 0; i < MAX_PROJECTILES; i++ )
		SAFE_DELETE( mProjectiles[i] );

	delete [] mProjectiles;
}

PlayState::PlayState()
{
	mPlayer			= nullptr;
	mRemotePlayers	= std::vector<RemotePlayer*>( 0 );
	mRemotePlayers.reserve(MAX_REMOTE_PLAYERS);
	mFrameCounter	= 0;
	mProjectiles	= nullptr;
}

PlayState::~PlayState()
{
}