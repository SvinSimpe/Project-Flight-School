#include "BaseMenuState.h"

HRESULT BaseMenuState::Update( float deltaTime )
{
	HandleInput();
	return S_OK;
}

HRESULT BaseMenuState::Render()
{
	Graphics::GetInstance()->Render2dAsset( mBackground, 0.0f, 0.0f, (float)Input::GetInstance()->mScreenWidth, (float)Input::GetInstance()->mScreenHeight );
	return S_OK;
}

HRESULT BaseMenuState::Initialize()
{
	Graphics::GetInstance()->LoadStatic2dAsset( "../Content/Assets/Textures/Menu/Menu_Background.jpg", mBackground );
	return S_OK;
}

void BaseMenuState::Release()
{
}

BaseMenuState::BaseMenuState()
{
}


BaseMenuState::~BaseMenuState()
{
}
