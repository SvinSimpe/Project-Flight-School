#include "MultiplayerMenuState.h"

void MultiplayerMenuState::HandleInput()
{
	if( mPortBox.LeftMousePressed() )
	{
		mPortBox.SwitchActive( true );
	}
	else if( mButtons[TWO_VS_TWO].LeftMousePressed() )
	{	
		std::string port = mPortBox.GetText();

		IEventPtr E1( new Event_Start_Server( port ) );
		EventManager::GetInstance()->QueueEvent( E1 );
	}
	else if( mButtons[THREE_VS_THREE].LeftMousePressed() )
	{
		std::string port = mPortBox.GetText();

		IEventPtr E1( new Event_Start_Server( port ) );
		EventManager::GetInstance()->QueueEvent( E1 );
	}
	else if( mButtons[FOUR_VS_FOUR].LeftMousePressed() )
	{
		std::string port = mPortBox.GetText();

		IEventPtr E1( new Event_Start_Server( port ) );
		EventManager::GetInstance()->QueueEvent( E1 );
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
	mPortBox.Update( deltaTime );
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
	mPortBox.Render();
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
	mPortBox.Release();

	float x = (float)Input::GetInstance()->mScreenWidth  * 0.10f;
	float y = (float)Input::GetInstance()->mScreenHeight * 0.80f;
	float w = 640.0f/2;
	float h = 177.0f/2;

	mPortBox.Initialize( "27015", "Port", x, y, w, h );
}

HRESULT MultiplayerMenuState::Initialize()
{
	BaseMenuState::Initialize();
	mStateType = MULTI_MENU_STATE;

	std::string texts[] = { "2vs2", "3vs3", "4vs4", "Back" };

	float x = (float)Input::GetInstance()->mScreenWidth  * 0.10f;
	float y = (float)Input::GetInstance()->mScreenHeight * 0.80f;
	float w = 640.0f/2;
	float h = 177.0f/2;

	mPortBox.Initialize( "27015", "Port", x, y, w, h );
	x += w;

	y	= (float)Input::GetInstance()->mScreenHeight * 0.75f;
	w	= 200.0f;
	h	= 200.0f;

	for( int i = 0; i < BUTTON_AMOUNT; i++ )
	{
		mButtons[i].Initialize( x, y, w, h );
		if( texts[i] == "Back" )
		{
			mTexts[i].Initialize( "../Content/Assets/Textures/Menu/Back.png", x, y, w, h );
		}
		else
		{
			mTexts[i].Initialize( "../Content/Assets/Textures/Menu/Multi_Menu_Text/" + texts[i] + ".png", x, y, w, h );
		}
		x += w;
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
