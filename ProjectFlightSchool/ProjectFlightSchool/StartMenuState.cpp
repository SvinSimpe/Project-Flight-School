#include "StartMenuState.h"

///////////////////////////////////////////////////////////////////////////////
//									PRIVATE
///////////////////////////////////////////////////////////////////////////////

void StartMenuState::HandleInput()
{
	if( mButtons[CREATE].LeftMousePressed() )
	{
		IEventPtr E1( new Event_Start_Server() );
		EventManager::GetInstance()->QueueEvent( E1 );

		IEventPtr E2( new Event_Change_State( CREATE_MENU_STATE ) );
		EventManager::GetInstance()->QueueEvent( E2 );
	}
	else if( mButtons[JOIN].LeftMousePressed() )
	{
		IEventPtr E1( new Event_Start_Client() );
		EventManager::GetInstance()->QueueEvent( E1 );

		IEventPtr E2( new Event_Change_State( PLAY_STATE ) );
		EventManager::GetInstance()->QueueEvent( E2 );
	}
}


///////////////////////////////////////////////////////////////////////////////
//									PUBLIC
///////////////////////////////////////////////////////////////////////////////

HRESULT StartMenuState::Update( float deltaTime )
{
	BaseMenuState::Update( deltaTime );
	for(auto& it : mButtons)
	{
		it.Update( deltaTime );
	}
	return S_OK;
}

HRESULT StartMenuState::Render()
{
	Graphics::GetInstance()->BeginScene();
	BaseMenuState::Render();

	for(auto& it : mButtons)
	{
		it.Render();
	}
	for(auto& it : mTexts)
	{
		it.Render();
	}

	Graphics::GetInstance()->EndScene();
	return S_OK;
}

void StartMenuState::OnEnter()
{
}

void StartMenuState::OnExit()
{
}

void StartMenuState::Reset()
{
}

HRESULT StartMenuState::Initialize()
{
	BaseMenuState::Initialize();
	mStateType		= START_MENU_STATE;

	std::string texts[] = { "Create", "Join", "Options", "Exit" };

	UINT x	= (UINT)Input::GetInstance()->mScreenWidth  * 0.20;
	UINT y	= (UINT)Input::GetInstance()->mScreenHeight * 0.75;
	UINT w	= 200;
	UINT h	= 200;
	for(int i = 0; i < 4; i++)
	{
		mButtons[i].Initialize(x, y, w, h);
		mTexts[i].Initialize("../Content/Assets/Textures/Menu/Start_Menu_Text/" + texts[i] + ".png", x, y, w, h);
		x += 200;
	}

	return S_OK;
}

void StartMenuState::Release()
{
	BaseMenuState::Release();
}

StartMenuState::StartMenuState() : BaseMenuState()
{
}

StartMenuState::~StartMenuState()
{
}