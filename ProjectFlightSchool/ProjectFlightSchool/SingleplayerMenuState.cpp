#include "SingleplayerMenuState.h"

void SingleplayerMenuState::HandleInput()
{
	if( mButtons[EASY].LeftMousePressed() )
	{
		IEventPtr E1( new Event_Change_State( PLAY_STATE ) );
		EventManager::GetInstance()->QueueEvent( E1 );

		IEventPtr E2( new Event_Start_Server( "27015" ) );
		EventManager::GetInstance()->QueueEvent( E2 );
	}
	else if( mButtons[MEDIUM].LeftMousePressed() )
	{
		IEventPtr E1( new Event_Change_State( PLAY_STATE ) );
		EventManager::GetInstance()->QueueEvent( E1 );

		IEventPtr E2( new Event_Start_Server( "27015" ) );
		EventManager::GetInstance()->QueueEvent( E2 );
	}
	else if( mButtons[HARD].LeftMousePressed() )
	{
		IEventPtr E1( new Event_Change_State( PLAY_STATE ) );
		EventManager::GetInstance()->QueueEvent( E1 );

		IEventPtr E2( new Event_Start_Server( "27015" ) );
		EventManager::GetInstance()->QueueEvent( E2 );
	}
	else if( mButtons[BACK].LeftMousePressed() )
	{
		IEventPtr E1( new Event_Change_State( CREATE_MENU_STATE ) );
		EventManager::GetInstance()->QueueEvent( E1 );
	}
}

HRESULT SingleplayerMenuState::Update( float deltaTime )
{
	BaseMenuState::Update( deltaTime );
	for( int i = 0; i < BUTTON_AMOUNT; i++ )
	{
		mButtons[i].Update( deltaTime );
	}
	return S_OK;
}

HRESULT SingleplayerMenuState::Render()
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

void SingleplayerMenuState::OnEnter()
{
}

void SingleplayerMenuState::OnExit()
{
	for( int i = 0; i < BUTTON_AMOUNT; i++ )
	{
		mButtons[i].SetExitCooldown();
	}
}

void SingleplayerMenuState::Reset()
{
}

HRESULT SingleplayerMenuState::Initialize()
{
	BaseMenuState::Initialize();
	mStateType = SINGLE_MENU_STATE;

	std::string texts[] = { "Easy", "Medium", "Hard", "Back" };

	float x	= (float)Input::GetInstance()->mScreenWidth  * 0.20f;
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
			mTexts[i].Initialize( "../Content/Assets/Textures/Menu/Single_Menu_Text/" + texts[i] + ".png", x, y, w, h );
		}
		x += 200;
	}
	return S_OK;
}

void SingleplayerMenuState::Release()
{
	BaseMenuState::Release();
	for( int i = 0; i < BUTTON_AMOUNT; i++ )
	{
		mButtons[i].Release();
		mTexts[i].Release();
	}
}

SingleplayerMenuState::SingleplayerMenuState() : BaseMenuState()
{
}

SingleplayerMenuState::~SingleplayerMenuState()
{
}
