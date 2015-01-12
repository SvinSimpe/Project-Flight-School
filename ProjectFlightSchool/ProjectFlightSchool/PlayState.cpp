#include "PlayState.h"

///////////////////////////////////////////////////////////////////////////////
//									PRIVATE
///////////////////////////////////////////////////////////////////////////////

void PlayState::EventListener( IEventPtr newEvent )
{
	if ( newEvent->GetEventType() == Event_Remote_Player_Joined::GUID ) // Add a remote player to the list when they connect
	{
		std::shared_ptr<Event_Remote_Player_Joined> data = std::static_pointer_cast<Event_Remote_Player_Joined>( newEvent );
		mRemotePlayers.push_back( new RemotePlayer() );
		mRemotePlayers.at(mRemotePlayers.size() - 1)->Initialize();
		mRemotePlayers.at(mRemotePlayers.size() - 1)->RemoteInit( data->ID() );
		printf( "Number of other players online: %d.\n", mRemotePlayers.size() );
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
		std::shared_ptr<Event_Remote_Player_Died> data = std::static_pointer_cast<Event_Remote_Player_Died>(newEvent);
		for ( unsigned int i = 0; i < mRemotePlayers.size(); i++ )
		{
			if ( !mRemotePlayers.at(i) )
			{
				continue;
			}
			else if ( data->ID() == mRemotePlayers.at(i)->GetID() )
			{
				// Damage player

				// Debug
				OutputDebugString( L"> A Remote player has taken damage." );

				break;
			}
		}
	}
	// Handle spawn logic here
}

// Tell server that local  player has taken damage
void PlayState::BroadcastDamage()
{
	IEventPtr dmgEv(new Event_Player_Damaged( mPlayer->GetID()) );
	EventManager::GetInstance()->QueueEvent( dmgEv );
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

void PlayState::CheckCollision()
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
					OutputDebugStringA( "\nCOLLISION" );

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

///////////////////////////////////////////////////////////////////////////////
//									PUBLIC
///////////////////////////////////////////////////////////////////////////////

HRESULT PlayState::Update( float deltaTime )
{
	OutputDebugStringA( "\n-------------------" );


	if( mFrameCounter >= COLLISION_CHECK_OFFSET )
	{
		CheckCollision();
		mFrameCounter = 0;
	}
	else
		mFrameCounter++;

	HandleDeveloperCameraInput();
	mPlayer->Update( deltaTime );
	mAnimationTime += deltaTime;

	return S_OK;
}

HRESULT PlayState::Render()
{
	Graphics::GetInstance()->BeginScene();
	Graphics::GetInstance()->RenderStatic3dAsset( mPlaneAsset, 0.0f, 0.0f, 0.0f );
	Graphics::GetInstance()->RenderStatic3dAsset( mTestAsset, DirectX::XMFLOAT3( 15.0f, 1.0f, 15.0f ), DirectX::XMFLOAT3( 0.0f, 1.0f, 0.0f ) );
	Graphics::GetInstance()->RenderStatic3dAsset( mNest1Asset, 8.0f, 0.0f, 0.0f );
	Graphics::GetInstance()->RenderStatic3dAsset( mTree1Asset, 12.0f, 0.0f, 0.0f );

	for( int i = 0; i < 6; i++ )
	{
		Graphics::GetInstance()->RenderStatic3dAsset( mStoneAssets[i], (float)i*4.0f, 0.0f, -4.0f );
	}

	//Graphics::GetInstance()->RenderAnimated3dAsset( mTestAnimation, mTestAnimationAnimation, mAnimationTime );

	int w = 10;
	int h = 10;
	for( int i = 0; i < w * h; i++ )
		Graphics::GetInstance()->RenderAnimated3dAsset( mTestAnimation, mTestAnimationAnimation, mAnimationTime, (float)-(i%w)  * 4.0f, 0.0f, (float)(i/h) * 4.0f );

	mPlayer->Render( 0.0f );
	//mWorldMap->Render( 0.0f );
	for( auto& rp : mRemotePlayers )
	{
		if( rp )
			rp->Render( 0.0f );
	}

	Graphics::GetInstance()->Render2dAsset( mTest2dAsset, 300, 300, 100, 100 );

	mFont.WriteText( "HELLO WORLD!\nTIM IS AWESOME!\nTABBING\tIS\tCOOL!\n#YOLO@SWAG.COM", 0.0f, 0.0f, 1.0f );

	DirectX::XMFLOAT3 x = { 0.0f, 0.1f, 3.0f };
	DirectX::XMFLOAT3 y = { 3.0f, 0.1f, 0.0f };
	Graphics::GetInstance()->RenderPlane2dAsset( mTest2dAsset,  x, y );
	Graphics::GetInstance()->Render2dAsset( mTest2dAsset, 500, 500, 50, 50 );
	
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

	Graphics::GetInstance()->LoadSkeletonAsset( "../Content/Assets/Raptor/Animations/", "raptor.Skel", mTestSkeleton );
	Graphics::GetInstance()->LoadAnimated3dAsset( "../Content/Assets/Raptor/", "scaledScene.apfs", mTestSkeleton, mTestAnimation );
	Graphics::GetInstance()->LoadAnimationAsset( "../Content/Assets/Raptor/Animations/", "raptor_death.PaMan", mTestAnimationAnimation );

	Graphics::GetInstance()->LoadStatic3dAsset( "../Content/Assets/Nests/", "nest_1.pfs", mNest1Asset );
	Graphics::GetInstance()->LoadStatic3dAsset( "../Content/Assets/Stones/", "stone_1.pfs", mStoneAssets[0] );
	Graphics::GetInstance()->LoadStatic3dAsset( "../Content/Assets/Stones/", "stone_2.pfs", mStoneAssets[1] );
	Graphics::GetInstance()->LoadStatic3dAsset( "../Content/Assets/Stones/", "stone_3.pfs", mStoneAssets[2] );
	Graphics::GetInstance()->LoadStatic3dAsset( "../Content/Assets/Stones/", "stone_4.pfs", mStoneAssets[3] );
	Graphics::GetInstance()->LoadStatic3dAsset( "../Content/Assets/Stones/", "stone_5.pfs", mStoneAssets[4] );
	Graphics::GetInstance()->LoadStatic3dAsset( "../Content/Assets/Stones/", "stone_6.pfs", mStoneAssets[5] );

	Graphics::GetInstance()->LoadStatic3dAsset( "../Content/Assets/Tree/", "tree1.pfs", mTree1Asset );

	Graphics::GetInstance()->LoadStatic2dAsset( "../Content/Assets/Textures/burger.png", mTest2dAsset );

	mAnimationTime = 1.0f;

	mPlayer = new Player();
	mPlayer->Initialize();

	//mWorldMap = new Map();
	//mWorldMap->Initialize( 8.0f, 24 );

	EventManager::GetInstance()->AddListener( &PlayState::EventListener, this, Event_Remote_Player_Joined::GUID );
	EventManager::GetInstance()->AddListener( &PlayState::EventListener, this, Event_Remote_Player_Left::GUID );
	EventManager::GetInstance()->AddListener( &PlayState::EventListener, this, Event_Remote_Player_Died::GUID );
	EventManager::GetInstance()->AddListener( &PlayState::EventListener, this, Event_Remote_Player_Damaged::GUID );
	EventManager::GetInstance()->AddListener( &PlayState::EventListener, this, Event_Remote_Player_Spawned::GUID );

	mFont.Initialize( "../Content/Assets/Fonts/mv_boli_26_red/" );

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
}

PlayState::PlayState()
{
	mRemotePlayers	= std::vector<RemotePlayer*>( 0 );
	mRemotePlayers.reserve(MAX_REMOTE_PLAYERS);
	mFrameCounter	= 0;
}

PlayState::~PlayState()
{
}