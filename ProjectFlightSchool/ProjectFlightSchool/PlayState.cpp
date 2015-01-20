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
					mPlayer->TakeDamage( 10, shooter );
				}
			}
		}
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
	mAnimationTime	+= deltaTime * 0.2f;

	mTurret.Update( deltaTime );
	if( mTurret.GetBoundingCircle()->Intersect( mPlayer->GetBoundingCircle() ) )
	{
		mTurret.SetTarget( mPlayer );
	}
	else
	{
		mTurret.SetTarget( nullptr );
	}

	return S_OK;
}

HRESULT PlayState::Render()
{

	RenderManager::GetInstance()->AddObject3dToList( mPlaneAsset, DirectX::XMFLOAT3( 0.0f, 0.0f, 0.0f ) );
	RenderManager::GetInstance()->AddObject3dToList( mTestAsset, DirectX::XMFLOAT3( 10.0f, 0.0f, 10.0f ) );
	RenderManager::GetInstance()->AddObject3dToList( mNest1Asset, DirectX::XMFLOAT3( 8.0f, 0.0f, 0.0f ) );
	RenderManager::GetInstance()->AddObject3dToList( mTree1Asset, DirectX::XMFLOAT3( 12.0f, 0.0f, 0.0f ) );

	for( int i = 0; i < 6; i++ )
	{
		RenderManager::GetInstance()->AddObject3dToList( mStoneAssets[i], DirectX::XMFLOAT3( (float)i*4.0f, 0.0f, -4.0f ) );
	}
	
	for(int i = 0; i < animTestNr; i++)
		RenderManager::GetInstance()->AddAnim3dToList( mTestAnimation[i], mTestAnimationAnimation[i], &mAnimationTime, DirectX::XMFLOAT3( -5.0f, ((i * 1) + 0.0f), 0.0f ) );



	mPlayer->Render( 0.0f, 1 );
	//mWorldMap->Render( 0.0f );
	for ( size_t i = 0; i < mRemotePlayers.size(); i++)
	{
		if ( mRemotePlayers.at(i) )
		{
			mRemotePlayers.at(i)->Render( 0.0f, i + 2 );
		}
	}

	RenderProjectiles();

	//mFont.WriteText( "HELLO WORLD!\nTIM IS AWESOME!\nTABBING\tIS\tCOOL!\n#YOLO@SWAG.COM", 0.0f, 0.0f, 1.0f );

	mTurret.Render();

	RenderManager::GetInstance()->Render();

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
	AssetID skel  =0;

	for(int i = 0; i < animTestNr; i++)
	{
		Graphics::GetInstance()->LoadSkeletonAsset( "../Content/Assets/Raptor/Animations/", "raptor.Skel", skeleton );
		Graphics::GetInstance()->LoadAnimated3dAsset( "../Content/Assets/Raptor/", "scaledScene.apfs", skeleton, mTestAnimation[i] );
		Graphics::GetInstance()->LoadAnimationAsset( "../Content/Assets/Raptor/Animations/", "raptor_death.PaMan", mTestAnimationAnimation[i] );
	}

	Graphics::GetInstance()->LoadStatic3dAsset( "../Content/Assets/Nests/", "nest_1.pfs", mNest1Asset );
	Graphics::GetInstance()->LoadStatic3dAsset( "../Content/Assets/Stones/", "stone_1.pfs", mStoneAssets[0] );
	Graphics::GetInstance()->LoadStatic3dAsset( "../Content/Assets/Stones/", "stone_2.pfs", mStoneAssets[1] );
	Graphics::GetInstance()->LoadStatic3dAsset( "../Content/Assets/Stones/", "stone_3.pfs", mStoneAssets[2] );
	Graphics::GetInstance()->LoadStatic3dAsset( "../Content/Assets/Stones/", "stone_4.pfs", mStoneAssets[3] );
	Graphics::GetInstance()->LoadStatic3dAsset( "../Content/Assets/Stones/", "stone_5.pfs", mStoneAssets[4] );
	Graphics::GetInstance()->LoadStatic3dAsset( "../Content/Assets/Stones/", "stone_6.pfs", mStoneAssets[5] );

	Graphics::GetInstance()->LoadStatic3dAsset( "../Content/Assets/Tree/", "tree1.pfs", mTree1Asset );

	Graphics::GetInstance()->LoadStatic2dAsset( "../Content/Assets/Textures/burger.png", mTest2dAsset );

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
	EventManager::GetInstance()->AddListener( &PlayState::EventListener, this, Event_Remote_Player_Update_HP::GUID );

	mFont.Initialize( "../Content/Assets/Fonts/final_font/" );

	//TEST
	mAllPlayers.push_back( mPlayer );
	mTurret.Initialize();

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
	mTurret.Release();

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