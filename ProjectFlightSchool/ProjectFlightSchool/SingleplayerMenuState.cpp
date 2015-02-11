#include "SingleplayerMenuState.h"

void SingleplayerMenuState::HandleInput()
{
	if( mButtons.at(EASY)->LeftMousePressed() )
	{
		IEventPtr E1( new Event_Change_State( PLAY_STATE ) );
		EventManager::GetInstance()->QueueEvent( E1 );

		IEventPtr E2( new Event_Start_Server( "27015" ) );
		EventManager::GetInstance()->QueueEvent( E2 );
	}
	else if( mButtons.at(MEDIUM)->LeftMousePressed() )
	{
		IEventPtr E1( new Event_Change_State( PLAY_STATE ) );
		EventManager::GetInstance()->QueueEvent( E1 );

		IEventPtr E2( new Event_Start_Server( "27015" ) );
		EventManager::GetInstance()->QueueEvent( E2 );
	}
	else if( mButtons.at(HARD)->LeftMousePressed() )
	{
		IEventPtr E1( new Event_Change_State( PLAY_STATE ) );
		EventManager::GetInstance()->QueueEvent( E1 );

		IEventPtr E2( new Event_Start_Server( "27015" ) );
		EventManager::GetInstance()->QueueEvent( E2 );
	}
	else if( mButtons.at(BACK)->LeftMousePressed() )
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
		mButtons.at(i)->Update( deltaTime );
	}
	return S_OK;
}

HRESULT SingleplayerMenuState::Render()
{
	BaseMenuState::Render();

	for( int i = 0; i < BUTTON_AMOUNT; i++ )
	{
		mButtons.at(i)->Render();
	}

	RenderManager::GetInstance()->Render();
	return S_OK;
}

void SingleplayerMenuState::OnEnter()
{
}

void SingleplayerMenuState::OnExit()
{
	BaseMenuState::OnExit();
}

void SingleplayerMenuState::Reset()
{
}

HRESULT SingleplayerMenuState::Initialize()
{
	BaseMenuState::Initialize();
	mStateType = SINGLE_MENU_STATE;
	mButtons.reserve( BUTTON_AMOUNT );

	std::string texts[] = { "Easy", "Medium", "Hard", "Back" };

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
			mButtons.at(i)->Initialize( "../Content/Assets/Textures/Menu/Single_Menu_Text/" + texts[i] + ".png", x - w * 0.5f, y - h * 0.5f, w, h );
		}
		x += (float)Input::GetInstance()->mScreenWidth  * 0.1f;
	}
	return S_OK;
}

void SingleplayerMenuState::Release()
{
	BaseMenuState::Release();
}

SingleplayerMenuState::SingleplayerMenuState() : BaseMenuState()
{
}

SingleplayerMenuState::~SingleplayerMenuState()
{
}
