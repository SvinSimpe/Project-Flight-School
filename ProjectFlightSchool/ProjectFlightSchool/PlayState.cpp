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
	mAnimationTime += deltaTime * 2.0f;
	return S_OK;
}

HRESULT PlayState::Render()
{
	Graphics::GetInstance()->BeginScene();

	//Graphics::GetInstance()->RenderStatic3dAsset( mPlaneAsset );
	mPlayer->Render( 0.0f );
	Graphics::GetInstance()->RenderStatic3dAsset( mCubeAsset, mTest2dTexture );
	Graphics::GetInstance()->RenderAnimated3dAsset( mTestAnimation, mTestAnimationAnimation, mAnimationTime );
	mPlayer->Render( 0.0f );
	mWorldMap->Render( 0.0f );
	for( auto& rp : mRemotePlayers )
	{
		if( rp )
			rp->Render( 0.0f );
	}

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

	//Graphics::GetInstance()->LoadStatic3dAsset( "CUBE", mCubeAsset );
	Graphics::GetInstance()->LoadStatic3dAsset( "PLANE", mPlaneAsset );
	Graphics::GetInstance()->LoadStatic3dAsset( "../Content/Assets/bin/aggro_test_utan_Anim.pfs", mTestAsset );

	Graphics::GetInstance()->LoadStatic2dAsset( "../Content/Assets/Textures/burger.png", mTest2dTexture );
	Graphics::GetInstance()->LoadSkeletonAsset( "../Content/Assets/Animations/skelTest/", "raptor.Skel", mTestSkeleton );
	Graphics::GetInstance()->LoadAnimated3dAsset( "../Content/Assets/Animations/skelTest/scaledScenetest.apfs", mTestSkeleton, mTestAnimation );
	Graphics::GetInstance()->LoadAnimationAsset( "../Content/Assets/Animations/skelTest/", "raptor_idle.PaMan", mTestAnimationAnimation );

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