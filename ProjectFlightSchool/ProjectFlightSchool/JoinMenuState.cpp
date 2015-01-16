#include "JoinMenuState.h"

void JoinMenuState::HandleInput()
{
}

HRESULT JoinMenuState::Update( float deltaTime )
{
	BaseMenuState::Update( deltaTime );
	return S_OK;
}

HRESULT JoinMenuState::Render()
{
	Graphics::GetInstance()->BeginScene();
	BaseMenuState::Render();
	Graphics::GetInstance()->EndScene();
	return S_OK;
}

void JoinMenuState::OnEnter()
{
}

void JoinMenuState::OnExit()
{
}

void JoinMenuState::Reset()
{
}

HRESULT JoinMenuState::Initialize()
{
	BaseMenuState::Initialize();
	mStateType = JOIN_MENU_STATE;
	return S_OK;
}

void JoinMenuState::Release()
{
}

JoinMenuState::JoinMenuState() : BaseMenuState()
{
}

JoinMenuState::~JoinMenuState()
{
}
