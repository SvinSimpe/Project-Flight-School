#include "PlayState.h"

///////////////////////////////////////////////////////////////////////////////
//									PRIVATE
///////////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////////
//									PUBLIC
///////////////////////////////////////////////////////////////////////////////

HRESULT PlayState::Update( float deltaTime )
{
	return S_OK;
}

HRESULT PlayState::Render()
{
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
	return S_OK;
}

void PlayState::Release()
{
}

PlayState::PlayState()
{
}

PlayState::~PlayState()
{
}