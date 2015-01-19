#include "MultiplayerMenuState.h"

void MultiplayerMenuState::HandleInput()
{
	if( mButtons[TWO_VS_TWO].LeftMousePressed() )
	{
		IEventPtr E1( new Event_Change_State( PLAY_STATE ) );
		EventManager::GetInstance()->QueueEvent( E1 );

		IEventPtr E2( new Event_Start_Server( "27015" ) );
		EventManager::GetInstance()->QueueEvent( E2 );
	}
	else if( mButtons[THREE_VS_THREE].LeftMousePressed() )
	{
		IEventPtr E1( new Event_Change_State( PLAY_STATE ) );
		EventManager::GetInstance()->QueueEvent( E1 );

		IEventPtr E2( new Event_Start_Server( "27015" ) );
		EventManager::GetInstance()->QueueEvent( E2 );
	}
	else if( mButtons[FOUR_VS_FOUR].LeftMousePressed() )
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

HRESULT MultiplayerMenuState::Update( float deltaTime )
{
	BaseMenuState::Update( deltaTime );
	for( int i = 0; i < BUTTON_AMOUNT; i++ )
	{
		mButtons[i].Update( deltaTime );
	}
	return S_OK;
}

HRESULT MultiplayerMenuState::Render()
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

void MultiplayerMenuState::OnEnter()
{
}

void MultiplayerMenuState::OnExit()
{
	for( int i = 0; i < BUTTON_AMOUNT; i++ )
	{
		mButtons[i].SetExitCooldown();
	}
}

void MultiplayerMenuState::Reset()
{
}

HRESULT MultiplayerMenuState::Initialize()
{
	BaseMenuState::Initialize();
	mStateType = MULTI_MENU_STATE;

	std::string texts[] = { "2vs2", "3vs3", "4vs4", "Back" };

	float x	= (float)Input::GetInstance()->mScreenWidth  * 0.20f;
	float y	= (float)Input::GetInstance()->mScreenHeight * 0.75f;
	float w	= 200.0f;
	float h	= 200.0f;
	for( int i = 0; i < BUTTON_AMOUNT; i++ )
	{
		mButtons[i].Initialize( (UINT)x, (UINT)y, (UINT)w, (UINT)h );
		if(texts[i] == "Back")
		{
			mTexts[i].Initialize( "../Content/Assets/Textures/Menu/Back.png", (UINT)x, (UINT)y, (UINT)w, (UINT)h );
		}
		else
		{
			mTexts[i].Initialize( "../Content/Assets/Textures/Menu/Multi_Menu_Text/" + texts[i] + ".png", (UINT)x, (UINT)y, (UINT)w, (UINT)h );
		}
		x += 200;
	}

	return S_OK;
}

void MultiplayerMenuState::Release()
{
	BaseMenuState::Release();
	for( int i = 0; i < BUTTON_AMOUNT; i++ )
	{
		mButtons[i].Release();
		mTexts[i].Release();
	}
}

MultiplayerMenuState::MultiplayerMenuState() : BaseMenuState()
{
}

MultiplayerMenuState::~MultiplayerMenuState()
{
}
