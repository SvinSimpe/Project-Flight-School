#include "JoinMenuState.h"

void JoinMenuState::HandleInput()
{
	if( mBackButton.LeftMousePressed() )
	{
		IEventPtr E1( new Event_Change_State( START_MENU_STATE ) );
		EventManager::GetInstance()->QueueEvent( E1 );
	}
	else if( Input::GetInstance()->mCurrentFrame.at(KEYS::KEYS_ENTER) )
	{
		std::string ip		= mIPBox.GetText();
		std::string port	= mPortBox.GetText();

		IEventPtr E1( new Event_Start_Client( ip, port ) );
		EventManager::GetInstance()->QueueEvent( E1 );

		IEventPtr E2( new Event_Change_State( PLAY_STATE ) );
		EventManager::GetInstance()->QueueEvent( E2 );
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
	mBackButton.Update( deltaTime );
	mIPBox.Update( deltaTime );
	mPortBox.Update( deltaTime );
	mText.Update( deltaTime );

	return S_OK;
}

HRESULT JoinMenuState::Render()
{
	BaseMenuState::Render();
	mBackButton.Render();
	mIPBox.Render();
	mPortBox.Render();
	mText.Render();

	RenderManager::GetInstance()->Render();
	return S_OK;
}

void JoinMenuState::OnEnter()
{
}

void JoinMenuState::OnExit()
{
	mBackButton.SetExitCooldown();
}

void JoinMenuState::Reset()
{
}

HRESULT JoinMenuState::Initialize()
{
	BaseMenuState::Initialize();
	mStateType = JOIN_MENU_STATE;

	float x	= (float)Input::GetInstance()->mScreenWidth  * 0.75f;
	float y	= (float)Input::GetInstance()->mScreenHeight * 0.75f;
	float w	= 200.0f;
	float h	= 200.0f;
	mBackButton.Initialize( (UINT)x, (UINT)y, (UINT)w, (UINT)h );
	mText.Initialize( "../Content/Assets/Textures/Menu/Back.png", (UINT)x, (UINT)y, (UINT)w, (UINT)h );

	x = (float)Input::GetInstance()->mScreenWidth  * 0.10f;
	y = (float)Input::GetInstance()->mScreenHeight * 0.80f;
	w = 640.0f/2;
	h = 177.0f/2;

	mIPBox.Initialize( "localhost", "IP", (UINT)x, (UINT)y, (UINT)w, (UINT)h);

	x += w + 20;

	mPortBox.Initialize( "27015", "Port", (UINT)x, (UINT)y, (UINT)w, (UINT)h);

	return S_OK;
}

void JoinMenuState::Release()
{
	BaseMenuState::Release();
	mBackButton.Release();
	mIPBox.Release();
	mPortBox.Release();
	mText.Release();
}

JoinMenuState::JoinMenuState() : BaseMenuState()
{
}

JoinMenuState::~JoinMenuState()
{
}
