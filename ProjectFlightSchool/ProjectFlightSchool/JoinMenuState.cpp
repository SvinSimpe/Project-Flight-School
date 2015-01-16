#include "JoinMenuState.h"

void JoinMenuState::HandleInput()
{
	if( mButtons[BACK].LeftMousePressed() )
	{
		IEventPtr E1( new Event_Change_State( START_MENU_STATE ) );
		EventManager::GetInstance()->QueueEvent( E1 );
	}
}

HRESULT JoinMenuState::Update( float deltaTime )
{
	BaseMenuState::Update( deltaTime );
	for( int i = 0; i < BUTTON_AMOUNT; i++ )
	{
		mButtons[i].Update( deltaTime );
	}
	return S_OK;
}

HRESULT JoinMenuState::Render()
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

void JoinMenuState::OnEnter()
{
}

void JoinMenuState::OnExit()
{
	for( int i = 0; i < BUTTON_AMOUNT; i++ )
	{
		mButtons[i].SetExitCooldown();
	}
}

void JoinMenuState::Reset()
{
}

HRESULT JoinMenuState::Initialize()
{
	BaseMenuState::Initialize();
	mStateType = JOIN_MENU_STATE;

	std::string texts[] = { "Back" };

	float x	= (float)Input::GetInstance()->mScreenWidth  * 0.75f;
	float y	= (float)Input::GetInstance()->mScreenHeight * 0.75f;
	float w	= 200.0f;
	float h	= 200.0f;
	for( int i = 0; i < BUTTON_AMOUNT; i++ )
	{
		mButtons[i].Initialize( (UINT)x, (UINT)y, (UINT)w, (UINT)h );

		if( texts[i] == "Back" )
		{
			mTexts[i].Initialize( "../Content/Assets/Textures/Menu/Back.png", (UINT)x, (UINT)y, (UINT)w, (UINT)h );
		}
		else
		{
			mTexts[i].Initialize( "../Content/Assets/Textures/Menu/Join_Menu_Text/" + texts[i] + ".png", (UINT)x, (UINT)y, (UINT)w, (UINT)h );
		}
		x += 200;
	}

	return S_OK;
}

void JoinMenuState::Release()
{
	BaseMenuState::Release();
	for( int i = 0; i < BUTTON_AMOUNT; i++ )
	{
		mButtons[i].Release();
		mTexts[i].Release();
	}
}

JoinMenuState::JoinMenuState() : BaseMenuState()
{
}

JoinMenuState::~JoinMenuState()
{
}
