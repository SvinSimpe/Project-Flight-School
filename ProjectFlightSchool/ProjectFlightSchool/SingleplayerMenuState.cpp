#include "SingleplayerMenuState.h"

void SingleplayerMenuState::HandleInput()
{
}

HRESULT SingleplayerMenuState::Update( float deltaTime )
{
	BaseMenuState::Update( deltaTime );
	return S_OK;
}

HRESULT SingleplayerMenuState::Render()
{
	return S_OK;
}

void SingleplayerMenuState::OnEnter()
{
}

void SingleplayerMenuState::OnExit()
{
}

void SingleplayerMenuState::Reset()
{
}

HRESULT SingleplayerMenuState::Initialize()
{
	return S_OK;
}

void SingleplayerMenuState::Release()
{
}

SingleplayerMenuState::SingleplayerMenuState() : BaseMenuState()
{
}

SingleplayerMenuState::~SingleplayerMenuState()
{
}
