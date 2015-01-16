#include "CreateMenuState.h"

void CreateMenuState::HandleInput()
{
	if( mButtons[SINGLEPLAYER].LeftMousePressed() )
	{
		IEventPtr E1( new Event_Change_State( SINGLE_MENU_STATE ) );
		EventManager::GetInstance()->QueueEvent( E1 );
	}
	else if( mButtons[MULTIPLAYER].LeftMousePressed() )
	{
		IEventPtr E1( new Event_Change_State( MULTI_MENU_STATE ) );
		EventManager::GetInstance()->QueueEvent( E1 );
	}
	else if( mButtons[BACK].LeftMousePressed() )
	{
		IEventPtr E1( new Event_Change_State( START_MENU_STATE ) );
		EventManager::GetInstance()->QueueEvent( E1 );
	}
}

HRESULT CreateMenuState::Update( float deltaTime )
{
	BaseMenuState::Update( deltaTime );
	for( int i = 0; i < BUTTON_AMOUNT; i++ )
	{
		mButtons[i].Update( deltaTime );
	}
	return S_OK;
}

HRESULT CreateMenuState::Render()
{
	Graphics::GetInstance()->BeginScene();
	BaseMenuState::Render();

	for( int i = 0; i < BUTTON_AMOUNT; i++ )
	{
		mButtons[i].Render();
		mTexts[i].Render();
	}

	Graphics::GetInstance()->EndScene();

	return S_OK;
}

void CreateMenuState::OnEnter()
{
}

void CreateMenuState::OnExit()
{
	for( int i = 0; i < BUTTON_AMOUNT; i++ )
	{
		mButtons[i].SetExitCooldown();
	}
}

void CreateMenuState::Reset()
{
}

HRESULT CreateMenuState::Initialize()
{
	BaseMenuState::Initialize();
	mStateType = CREATE_MENU_STATE;

	std::string texts[] = { "SinglePlayer", "MultiPlayer", "Back" };

	float x	= (float)Input::GetInstance()->mScreenWidth  * 0.30f;
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
			mTexts[i].Initialize( "../Content/Assets/Textures/Menu/Create_Menu_Text/" + texts[i] + ".png", (UINT)x, (UINT)y, (UINT)w, (UINT)h );
		}
		x += 200;
	}
	return S_OK;
}

void CreateMenuState::Release()
{
	BaseMenuState::Release();
	for( int i = 0; i < BUTTON_AMOUNT; i++ )
	{
		mButtons[i].SetExitCooldown();
	}
}

CreateMenuState::CreateMenuState() : BaseMenuState()
{
}

CreateMenuState::~CreateMenuState()
{
}
