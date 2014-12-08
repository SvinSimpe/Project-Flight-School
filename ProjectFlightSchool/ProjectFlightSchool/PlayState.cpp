#include "PlayState.h"

///////////////////////////////////////////////////////////////////////////////
//									PRIVATE
///////////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////////
//									PUBLIC
///////////////////////////////////////////////////////////////////////////////

HRESULT PlayState::Update( float deltaTime )
{
	mPlayer->Update( deltaTime );
	return S_OK;
}

HRESULT PlayState::Render()
{
	Graphics::GetInstance()->BeginScene();
	Graphics::GetInstance()->RenderStatic3dAsset( mPlaneAsset );
	Graphics::GetInstance()->RenderStatic3dAsset( mCubeAsset, 5.0f, 1.0f, 10.0f );
	//Graphics::GetInstance()->RenderStatic3dAsset( mTestAsset );
	mPlayer->Render( 0.0f );
	mRemotePlayer->Render(0.0f);
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
	mStateType		= STATE_TYPE_PLAY;

	Graphics::GetInstance()->LoadStatic3dAsset( "CUBE", mCubeAsset );
	Graphics::GetInstance()->LoadStatic3dAsset( "PLANE", mPlaneAsset );
	Graphics::GetInstance()->LoadStatic3dAsset( "../Content/Assets/bin/cubeandsphere.peniz", mTestAsset );
	
	mPlayer = new Player();
	mPlayer->Initialize();

	mRemotePlayer = new RemotePlayer();
	mRemotePlayer->Initialize();

	return S_OK;
}

void PlayState::Release()
{
	mPlayer->Release();
}

PlayState::PlayState()
{
}

PlayState::~PlayState()
{
}