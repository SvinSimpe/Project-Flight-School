#include "PlayState.h"

///////////////////////////////////////////////////////////////////////////////
//									PRIVATE
///////////////////////////////////////////////////////////////////////////////

void PlayState::RemoteUpdate( IEventPtr newEvent )
{
	if ( newEvent->GetEventType() == Event_Remote_Player_Joined::GUID )
	{
		std::shared_ptr<Event_Remote_Player_Joined> data = std::static_pointer_cast<Event_Remote_Player_Joined>( newEvent );
		mRemotePlayers.push_back( new RemotePlayer() );
		mRemotePlayers.at(mRemotePlayers.size() - 1)->Initialize( data->ID() );
		printf( "Number of other players online: %d.\n", mRemotePlayers.size() );
	}
	else if ( newEvent->GetEventType() == Event_Remote_Player_Left::GUID )
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

///////////////////////////////////////////////////////////////////////////////
//									PUBLIC
///////////////////////////////////////////////////////////////////////////////

HRESULT PlayState::Update( float deltaTime )
{
	HandleDeveloperCameraInput();
	mPlayer->Update( deltaTime );
	mAnimationTime += deltaTime;
	return S_OK;
}

HRESULT PlayState::Render()
{
	Graphics::GetInstance()->BeginScene();
	Graphics::GetInstance()->RenderStatic3dAsset( mPlaneAsset, 0.0f, 0.0f, 0.0f );
	Graphics::GetInstance()->RenderStatic3dAsset( mTestAsset, 4.0f, 0.0f, 0.0f );
	Graphics::GetInstance()->RenderStatic3dAsset( mNest1Asset, 8.0f, 0.0f, 0.0f );
	Graphics::GetInstance()->RenderStatic3dAsset( mTree1Asset, 12.0f, 0.0f, 0.0f );

	for( int i = 0; i < 6; i++ )
	{
		Graphics::GetInstance()->RenderStatic3dAsset( mStoneAssets[i], (float)i*4.0f, 0.0f, -4.0f );
	}

	Graphics::GetInstance()->RenderAnimated3dAsset( mTestAnimation, mTestAnimationAnimation, mAnimationTime );

	mPlayer->Render( 0.0f );
	//mWorldMap->Render( 0.0f );
	for( auto& rp : mRemotePlayers )
	{
		if( rp )
			rp->Render( 0.0f );
	}

	Graphics::GetInstance()->Render2dAsset( mTest2dAsset, 300, 300, 100, 100 );

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
	mStateType = STATE_TYPE_PLAY;

	Graphics::GetInstance()->LoadStatic3dAsset( "../Content/Assets/Plane/", "plane.pfs", mPlaneAsset );
	Graphics::GetInstance()->LoadStatic3dAsset( "../Content/Assets/Log/", "log_1.pfs", mTestAsset );

	Graphics::GetInstance()->LoadSkeletonAsset( "../Content/Assets/Raptor/Animations/", "raptor.Skel", mTestSkeleton );
	Graphics::GetInstance()->LoadAnimated3dAsset( "../Content/Assets/Raptor/", "scaledScene.apfs", mTestSkeleton, mTestAnimation );
	Graphics::GetInstance()->LoadAnimationAsset( "../Content/Assets/Raptor/Animations/", "raptor_run.PaMan", mTestAnimationAnimation );

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

	mWorldMap = new Map();
	mWorldMap->Initialize( 8.0f, 24 );

	EventManager::GetInstance()->AddListener( &PlayState::RemoteUpdate, this, Event_Remote_Player_Joined::GUID );
	EventManager::GetInstance()->AddListener( &PlayState::RemoteUpdate, this, Event_Remote_Player_Left::GUID );

	return S_OK;
}

void PlayState::Release()
{
	mPlayer->Release();
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
	mRemotePlayers = std::vector<RemotePlayer*>( 0 );
	mRemotePlayers.reserve(MAX_REMOTE_PLAYERS);
}

PlayState::~PlayState()
{
}