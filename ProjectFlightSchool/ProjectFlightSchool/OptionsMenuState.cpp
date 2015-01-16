#include "OptionsMenuState.h"

void OptionsMenuState::HandleInput()
{
}

HRESULT OptionsMenuState::Update( float deltaTime )
{
	BaseMenuState::Update( deltaTime );
	return S_OK;
}

HRESULT OptionsMenuState::Render()
{
	Graphics::GetInstance()->BeginScene();
	BaseMenuState::Render();
	Graphics::GetInstance()->EndScene();
	return S_OK;
}

void OptionsMenuState::OnEnter()
{
}

void OptionsMenuState::OnExit()
{
}

void OptionsMenuState::Reset()
{
}

HRESULT OptionsMenuState::Initialize()
{
	BaseMenuState::Initialize();
	mStateType = OPTIONS_MENU_STATE;
	return S_OK;
}

void OptionsMenuState::Release()
{
}

OptionsMenuState::OptionsMenuState() : BaseMenuState()
{
}

OptionsMenuState::~OptionsMenuState()
{
}
