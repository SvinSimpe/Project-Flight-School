#include "OptionsMenuState.h"

void OptionsMenuState::HandleInput()
{
	if( mButtons[BACK].LeftMousePressed() )
	{
		IEventPtr E1( new Event_Change_State( START_MENU_STATE ) );
		EventManager::GetInstance()->QueueEvent( E1 );
	}
}

HRESULT OptionsMenuState::Update( float deltaTime )
{
	BaseMenuState::Update( deltaTime );

	for( int i = 0; i < BUTTON_AMOUNT; i++ )
	{
		mButtons[i].Update( deltaTime );
	}
	return S_OK;
}

HRESULT OptionsMenuState::Render()
{
	BaseMenuState::Render();

	for( int i = 0; i < BUTTON_AMOUNT; i++ )
	{
		mButtons[i].Render();
		mTexts[i].Render();
	}

	RenderManager::GetInstance()->Render();
	return S_OK;
}

void OptionsMenuState::OnEnter()
{
}

void OptionsMenuState::OnExit()
{
	for( int i = 0; i < BUTTON_AMOUNT; i++ )
	{
		mButtons[i].SetExitCooldown();
	}
}

void OptionsMenuState::Reset()
{
}

HRESULT OptionsMenuState::Initialize()
{
	BaseMenuState::Initialize();
	mStateType = OPTIONS_MENU_STATE;

	std::string texts[] = { "Fullscreen", "Sound", "Back" };

	float x	= (float)Input::GetInstance()->mScreenWidth  * 0.30f;
	float y	= (float)Input::GetInstance()->mScreenHeight * 0.75f;
	float w	= 200.0f;
	float h	= 200.0f;
	for( int i = 0; i < BUTTON_AMOUNT; i++ )
	{
		mButtons[i].Initialize( x, y, w, h );
		if( texts[i] == "Back" )
		{
			mTexts[i].Initialize( "../Content/Assets/Textures/Menu/Back.png", x, y, w, h );
		}
		else
		{
			mTexts[i].Initialize( "../Content/Assets/Textures/Menu/Options_Menu_Text/" + texts[i] + ".png", x, y, w, h );
		}
		x += 200;
	}
	return S_OK;
}

void OptionsMenuState::Release()
{
	BaseMenuState::Release();
	for( int i = 0; i < BUTTON_AMOUNT; i++ )
	{
		mButtons[i].Release();
		mTexts[i].Release();
	}
}

OptionsMenuState::OptionsMenuState() : BaseMenuState()
{
}

OptionsMenuState::~OptionsMenuState()
{
}
