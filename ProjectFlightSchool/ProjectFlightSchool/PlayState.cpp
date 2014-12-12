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
	mAnimationTime += deltaTime / 2.0f;
	return S_OK;
}

HRESULT PlayState::Render()
{
	Graphics::GetInstance()->BeginScene();
	Graphics::GetInstance()->RenderStatic3dAsset( mPlaneAsset );
	//Graphics::GetInstance()->RenderStatic3dAsset( mCubeAsset, 5.0f, 1.0f, 10.0f );
	//Graphics::GetInstance()->RenderStatic3dAsset( mTestAsset );
	Graphics::GetInstance()->RenderAnimated3dAsset( mTestAnimation, mTestAnimationAnimation, mAnimationTime );
	mPlayer->Render( 0.0f );
	mWorldMap->Render( 0.0f );
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

	Graphics::GetInstance()->LoadSkeletonAsset( "../Content/Assets/Animations/", "walk.Skel", mTestSkeleton );
	Graphics::GetInstance()->LoadAnimated3dAsset( "../Content/Assets/bin/maya_testAsset_tree.pfs", mTestSkeleton, mTestAnimation );
	Graphics::GetInstance()->LoadAnimationAsset( "../Content/Assets/Animations/", "walk.PaMan", mTestAnimationAnimation );

	mAnimationTime = 1.0f;

	mPlayer = new Player();
	mPlayer->Initialize();

	mWorldMap = new Map();
	mWorldMap->Initialize( 1 );

	return S_OK;
}

void PlayState::Release()
{
	mPlayer->Release();
	mWorldMap->Release();
}

PlayState::PlayState()
{
}

PlayState::~PlayState()
{
}