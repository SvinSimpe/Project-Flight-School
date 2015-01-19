#include "JoinMenuState.h"

void JoinMenuState::HandleInput()
{
	if( mButtons[BACK].LeftMousePressed() )
	{
		IEventPtr E1( new Event_Change_State( START_MENU_STATE ) );
		EventManager::GetInstance()->QueueEvent( E1 );
	}
	else if( mButtons[JOIN].LeftMousePressed() )
	{
		std::string ip		= mIPBox.GetText();
		std::string port	= mPortBox.GetText();

		IEventPtr E1( new Event_Start_Client( ip, port ) );
		EventManager::GetInstance()->QueueEvent( E1 );
	}
	else if( mIPBox.LeftMousePressed() )
	{
		mIPBox.SwitchActive( true );
		mPortBox.SwitchActive( false );
	}
	else if( mPortBox.LeftMousePressed() )
	{
		mIPBox.SwitchActive( false );
		mPortBox.SwitchActive( true );
	}
}

HRESULT JoinMenuState::Update( float deltaTime )
{
	BaseMenuState::Update( deltaTime );
	for( int i = 0; i < BUTTON_AMOUNT; i++ )
	{
		mButtons[i].Update( deltaTime );
		mText[i].Update( deltaTime );
	}
	mIPBox.Update( deltaTime );
	mPortBox.Update( deltaTime );

	return S_OK;
}

HRESULT JoinMenuState::Render()
{
	BaseMenuState::Render();
	for( int i = 0; i < BUTTON_AMOUNT; i++ )
	{
		mButtons[i].Render();
		mText[i].Render();
	}
	mIPBox.Render();
	mPortBox.Render();

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
	mIPBox.Release();
	mPortBox.Release();

	float x = (float)Input::GetInstance()->mScreenWidth  * 0.10f;
	float y = (float)Input::GetInstance()->mScreenHeight * 0.80f;
	float w = 640.0f/2;
	float h = 177.0f/2;

	mIPBox.Initialize( "localhost", "IP", x, y, w, h );

	x += w + 20;

	mPortBox.Initialize( "27015", "Port", x, y, w, h );
}

HRESULT JoinMenuState::Initialize()
{
	BaseMenuState::Initialize();
	mStateType = JOIN_MENU_STATE;

	float x	= (float)Input::GetInstance()->mScreenWidth  * 0.65f;
	float y	= (float)Input::GetInstance()->mScreenHeight * 0.75f;
	float w	= 200.0f;
	float h	= 200.0f;
	mButtons[JOIN].Initialize( x, y, w, h );
	mText[JOIN].Initialize( "../Content/Assets/Textures/Menu/Join_Menu_Text/Join.png", x, y, w, h );

	x += w + 20;

	mButtons[BACK].Initialize( x, y, w, h );
	mText[BACK].Initialize( "../Content/Assets/Textures/Menu/Back.png", x, y, w, h );

	x = (float)Input::GetInstance()->mScreenWidth  * 0.10f;
	y = (float)Input::GetInstance()->mScreenHeight * 0.80f;
	w = 640.0f/2;
	h = 177.0f/2;

	mIPBox.Initialize( "localhost", "IP", x, y, w, h );

	x += w + 20;

	mPortBox.Initialize( "27015", "Port", x, y, w, h );

	return S_OK;
}

void JoinMenuState::Release()
{
	BaseMenuState::Release();
	for( int i = 0; i < BUTTON_AMOUNT; i++ )
	{
		mButtons[i].Release();
		mText[i].Release();
	}
	mIPBox.Release();
	mPortBox.Release();
}

JoinMenuState::JoinMenuState() : BaseMenuState()
{
}

JoinMenuState::~JoinMenuState()
{
}
