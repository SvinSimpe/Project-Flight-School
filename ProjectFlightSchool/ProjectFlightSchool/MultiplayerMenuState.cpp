#include "MultiplayerMenuState.h"

void MultiplayerMenuState::HandleInput()
{
}

HRESULT MultiplayerMenuState::Update( float deltaTime )
{
	BaseMenuState::Update( deltaTime );
	return S_OK;
}

HRESULT MultiplayerMenuState::Render()
{
	return S_OK;
}

void MultiplayerMenuState::OnEnter()
{
}

void MultiplayerMenuState::OnExit()
{
}

void MultiplayerMenuState::Reset()
{
}

HRESULT MultiplayerMenuState::Initialize()
{
	return S_OK;
}

void MultiplayerMenuState::Release()
{
}

MultiplayerMenuState::MultiplayerMenuState() : BaseMenuState()
{
}

MultiplayerMenuState::~MultiplayerMenuState()
{
}
