#include "BaseMenuState.h"

HRESULT BaseMenuState::Update( float deltaTime )
{
	HandleInput();
	return S_OK;
}

HRESULT BaseMenuState::Render()
{
	RenderManager::GetInstance()->AddObject2dToList( mBackground, DirectX::XMFLOAT2( 0.0f, 0.0f ), DirectX::XMFLOAT2( (float)Input::GetInstance()->mScreenWidth, (float)Input::GetInstance()->mScreenHeight ));
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
