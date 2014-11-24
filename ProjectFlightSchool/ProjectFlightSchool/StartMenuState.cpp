#include "StartMenuState.h"

///////////////////////////////////////////////////////////////////////////////
//									PRIVATE
///////////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////////
//									PUBLIC
///////////////////////////////////////////////////////////////////////////////

HRESULT StartMenuState::Update( float deltaTime )
{
	return S_OK;
}

HRESULT StartMenuState::Render()
{
	return S_OK;
}

HRESULT StartMenuState::Initialize()
{
	mStateType		= STATE_TYPE_START_MENU;
	return S_OK;
}

void StartMenuState::OnEnter()
{
}

void StartMenuState::OnExit()
{
}

void StartMenuState::Reset()
{
}

void StartMenuState::Release()
{
}

StartMenuState::StartMenuState()
{
}

StartMenuState::~StartMenuState()
{
}