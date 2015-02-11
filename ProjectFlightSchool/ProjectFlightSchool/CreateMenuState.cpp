#include "CreateMenuState.h"

void CreateMenuState::HandleInput()
{
	if( mButtons.at(SINGLEPLAYER)->LeftMousePressed() )
	{
		IEventPtr E1( new Event_Change_State( SINGLE_MENU_STATE ) );
		EventManager::GetInstance()->QueueEvent( E1 );
		IEventPtr E2( new Event_Create_Player_Name( mNameBox.GetText() ) );
		EventManager::GetInstance()->QueueEvent( E2 );
	}
	else if( mButtons.at(MULTIPLAYER)->LeftMousePressed() )
	{
		IEventPtr E1( new Event_Change_State( MULTI_MENU_STATE ) );
		EventManager::GetInstance()->QueueEvent( E1 );
		IEventPtr E2( new Event_Create_Player_Name( mNameBox.GetText() ) );
		EventManager::GetInstance()->QueueEvent( E2 );
	}
	else if( mButtons.at(BACK)->LeftMousePressed() )
	{
		IEventPtr E1( new Event_Change_State( START_MENU_STATE ) );
		EventManager::GetInstance()->QueueEvent( E1 );
	}
	else if( mNameBox.LeftMousePressed() )
	{
		mNameBox.SwitchActive( true );
	}
}

HRESULT CreateMenuState::Update( float deltaTime )
{
	BaseMenuState::Update( deltaTime );
	for( int i = 0; i < BUTTON_AMOUNT; i++ )
	{
		mButtons.at(i)->Update( deltaTime );
	}

	mNameBox.Update( deltaTime );

	return S_OK;
}

HRESULT CreateMenuState::Render()
{
	BaseMenuState::Render();
	for( int i = 0; i < BUTTON_AMOUNT; i++ )
	{
		mButtons.at(i)->Render();
	}

	mNameBox.Render();

	RenderManager::GetInstance()->Render();
	return S_OK;
}

void CreateMenuState::OnEnter()
{
}

void CreateMenuState::OnExit()
{
	BaseMenuState::OnExit();
}

void CreateMenuState::Reset()
{
}

HRESULT CreateMenuState::Initialize()
{
	BaseMenuState::Initialize();
	mStateType = CREATE_MENU_STATE;
	mButtons.reserve( BUTTON_AMOUNT );

	std::string texts[] = { "SinglePlayer", "MultiPlayer", "Back" };

	float x	= (float)Input::GetInstance()->mScreenWidth  * 0.35f;
	float y	= (float)Input::GetInstance()->mScreenHeight * 0.9f;
	float w	= 200.0f;
	float h	= 200.0f;
	for( int i = 0; i < BUTTON_AMOUNT; i++ )
	{
		mButtons.push_back( new Button() );
		if( texts[i] == "Back" )
		{
			mButtons.at(i)->Initialize( "../Content/Assets/Textures/Menu/Back.png", x - w * 0.5f, y - h * 0.5f, w, h );
		}
		else
		{
			mButtons.at(i)->Initialize( "../Content/Assets/Textures/Menu/Create_Menu_Text/" + texts[i] + ".png", x - w * 0.5f, y - h * 0.5f, w, h );
		}
		x += (float)Input::GetInstance()->mScreenWidth * 0.1f;
	}

	mNameBox.Initialize( "mudkipfucker", "IP", Input::GetInstance()->mScreenWidth * 0.3f - (640.0f * 0.5f) * 0.5f, Input::GetInstance()->mScreenHeight * 0.5f + (177.0f * 0.5f) *0.5f, 640.0f * 0.5f, 177.0f * 0.5f );

	return S_OK;
}

void CreateMenuState::Release()
{
	BaseMenuState::Release();
	mNameBox.Release();
}

CreateMenuState::CreateMenuState() : BaseMenuState()
{
}

CreateMenuState::~CreateMenuState()
{
}
