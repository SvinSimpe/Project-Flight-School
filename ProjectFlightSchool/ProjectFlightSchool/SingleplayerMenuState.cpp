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
	Graphics::GetInstance()->BeginScene();
	BaseMenuState::Render();
	Graphics::GetInstance()->EndScene();
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
	BaseMenuState::Initialize();
	mStateType = SINGLE_MENU_STATE;
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
