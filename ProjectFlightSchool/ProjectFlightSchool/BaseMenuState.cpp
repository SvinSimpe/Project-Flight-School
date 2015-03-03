#include "BaseMenuState.h"

HRESULT BaseMenuState::Update( float deltaTime )
{
	SetCursor( mCursor );
	HandleInput();

	return S_OK;
}

HRESULT BaseMenuState::Render()
{
	RenderManager::GetInstance()->AddObject2dToList( mBackground, DirectX::XMFLOAT2( 0.0f, 0.0f ), DirectX::XMFLOAT2( (float)Input::GetInstance()->mScreenWidth, (float)Input::GetInstance()->mScreenHeight ) );
	return S_OK;
}

void BaseMenuState::OnEnter()
{
	SetCursor( mCursor );
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
	BaseState::Initialize();
	Graphics::GetInstance()->LoadStatic2dAsset( "../Content/Assets/Textures/Menu/lobby_loadout_menu/menuBackground.dds", mBackground );
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
