#include "CreateMenuState.h"

void CreateMenuState::HandleInput()
{
}

HRESULT CreateMenuState::Update( float deltaTime )
{
	BaseMenuState::Update( deltaTime );
	return S_OK;
}

HRESULT CreateMenuState::Render()
{
	return S_OK;
}

void CreateMenuState::OnEnter()
{
}

void CreateMenuState::OnExit()
{
}

void CreateMenuState::Reset()
{
}

HRESULT CreateMenuState::Initialize()
{
	return S_OK;
}

void CreateMenuState::Release()
{
}

CreateMenuState::CreateMenuState() : BaseMenuState()
{
}

CreateMenuState::~CreateMenuState()
{
}