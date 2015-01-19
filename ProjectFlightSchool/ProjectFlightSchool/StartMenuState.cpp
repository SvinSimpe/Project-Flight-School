#include "StartMenuState.h"

///////////////////////////////////////////////////////////////////////////////
//									PRIVATE
///////////////////////////////////////////////////////////////////////////////

void StartMenuState::HandleInput()
{
	if( mButtons[CREATE].LeftMousePressed() )
	{
		IEventPtr E1( new Event_Change_State( CREATE_MENU_STATE ) );
		EventManager::GetInstance()->QueueEvent( E1 );
	}
	else if( mButtons[JOIN].LeftMousePressed() )
	{
		IEventPtr E1( new Event_Change_State( JOIN_MENU_STATE ) );
		EventManager::GetInstance()->QueueEvent( E1 );
	}
	else if( mButtons[OPTIONS].LeftMousePressed() )
	{
		IEventPtr E1( new Event_Change_State( OPTIONS_MENU_STATE ) );
		EventManager::GetInstance()->QueueEvent( E1 );
	}
}


///////////////////////////////////////////////////////////////////////////////
//									PUBLIC
///////////////////////////////////////////////////////////////////////////////

HRESULT StartMenuState::Update( float deltaTime )
{
	BaseMenuState::Update( deltaTime );
	for( int i = 0; i < BUTTON_AMOUNT; i++ )
	{
		mButtons[i].Update( deltaTime );
	}
	return S_OK;
}

HRESULT StartMenuState::Render()
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

void StartMenuState::OnEnter()
{
}

void StartMenuState::OnExit()
{
	for( int i = 0; i < BUTTON_AMOUNT; i++ )
	{
		mButtons[i].SetExitCooldown();
	}
}

void StartMenuState::Reset()
{
}

HRESULT StartMenuState::Initialize()
{
	BaseMenuState::Initialize();
	mStateType		= START_MENU_STATE;

	std::string texts[] = { "Create", "Join", "Options", "Exit" };

	float x	= (float)Input::GetInstance()->mScreenWidth  * 0.20f;
	float y	= (float)Input::GetInstance()->mScreenHeight * 0.75f;
	float w	= 200.0f;
	float h	= 200.0f;
	for(int i = 0; i < BUTTON_AMOUNT; i++)
	{
		mButtons[i].Initialize( (UINT)x, (UINT)y, (UINT)w, (UINT)h );
		mTexts[i].Initialize( "../Content/Assets/Textures/Menu/Start_Menu_Text/" + texts[i] + ".png", (UINT)x, (UINT)y, (UINT)w, (UINT)h );
		x += 200;
	}

	return S_OK;
}

void StartMenuState::Release()
{
	BaseMenuState::Release();
	for( int i = 0; i < BUTTON_AMOUNT; i++ )
	{
		mButtons[i].Release();
		mTexts[i].Release();
	}
}

StartMenuState::StartMenuState() : BaseMenuState()
{
}

StartMenuState::~StartMenuState()
{
}
