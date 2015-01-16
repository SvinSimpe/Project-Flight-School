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
	Graphics::GetInstance()->BeginScene();
	BaseMenuState::Render();
	Graphics::GetInstance()->EndScene();
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
	BaseMenuState::Initialize();
	mStateType = MULTI_MENU_STATE;
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
