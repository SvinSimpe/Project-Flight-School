#include "JoinMenuState.h"

void JoinMenuState::HandleInput()
{
	if( mButtons[BACK].LeftMousePressed() )
	{
		IEventPtr E1( new Event_Change_State( START_MENU_STATE ) );
		EventManager::GetInstance()->QueueEvent( E1 );
	}
	else if(Input::GetInstance()->mCurrentFrame.at(KEYS::KEYS_ENTER))
	{
		IEventPtr E1( new Event_Change_State( PLAY_STATE ) );
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

	float x	= (float)Input::GetInstance()->mScreenWidth  * 0.75f;
	float y	= (float)Input::GetInstance()->mScreenHeight * 0.75f;
	float w	= 200.0f;
	float h	= 200.0f;
	mButtons[BACK].Initialize( (UINT)x, (UINT)y, (UINT)w, (UINT)h );
	mTexts[BACK].Initialize( "../Content/Assets/Textures/Menu/Back.png", (UINT)x, (UINT)y, (UINT)w, (UINT)h );

	x = (float)Input::GetInstance()->mScreenWidth  * 0.75f;
	y = (float)Input::GetInstance()->mScreenHeight * 0.75f;
	w = 640.0f;
	h = 177.0f;

	mIPBox.Initialize((UINT)x, (UINT)y, (UINT)w, (UINT)h);
	mTexts[1].Initialize( "../Content/Assets/Textures/Menu/Join_Menu_Text/IP.png", (UINT)x, (UINT)y, (UINT)w, (UINT)h );

	x += w + 20.0f;

	mPortBox.Initialize((UINT)x + w + 20, (UINT)y, (UINT)w, (UINT)h);
	mTexts[2].Initialize( "../Content/Assets/Textures/Menu/Join_Menu_Text/Port.png", (UINT)x, (UINT)y, (UINT)w, (UINT)h );

	return S_OK;
}

void JoinMenuState::Release()
{
	BaseMenuState::Release();
	for( int i = 0; i < BUTTON_AMOUNT; i++ )
	{
		mButtons[i].Release();
	}
	mIPBox.Release();
	mPortBox.Release();

	for( int i = 0; i < 3; i++ )
	{
		mTexts[i].Release();
	}
}

JoinMenuState::JoinMenuState() : BaseMenuState()
{
}

JoinMenuState::~JoinMenuState()
{
}
