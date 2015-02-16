#include "BaseMenuState.h"

HRESULT BaseMenuState::Update( float deltaTime )
{
	HandleInput();

	return S_OK;
}

HRESULT BaseMenuState::Render()
{
	RenderManager::GetInstance()->AddObject2dToList( mBackground, DirectX::XMFLOAT2( 0.0f, 0.0f ), DirectX::XMFLOAT2( (float)Input::GetInstance()->mScreenWidth, (float)Input::GetInstance()->mScreenHeight ) );
	float midOffset = mText.GetMiddleXPoint( "Walking Robot With Spear Gun", 4.0f );
	mText.WriteText( "Walking Robot With Spear Gun", Input::GetInstance()->mScreenWidth * 0.50f - midOffset, 25.0f, 4.0f, COLOR_CYAN );
	return S_OK;
}

void BaseMenuState::OnExit()
{
	for( auto& it : mButtons )
	{
		it->SetExitCooldown();
	}
}

HRESULT BaseMenuState::Initialize()
{
	Graphics::GetInstance()->LoadStatic2dAsset( "../Content/Assets/Textures/Menu/Menu_Background.jpg", mBackground );
	mText.Initialize( "../Content/Assets/GUI/Fonts/final_font/" );
	
	return S_OK;
}

void BaseMenuState::Release()
{
	for( auto& it : mButtons )
	{
		it->Release();
		SAFE_DELETE( it );
	}
	mButtons.clear();
	mText.Release();
}

BaseMenuState::BaseMenuState()
{
}


BaseMenuState::~BaseMenuState()
{
}
