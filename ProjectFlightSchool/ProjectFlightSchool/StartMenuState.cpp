#include "StartMenuState.h"

///////////////////////////////////////////////////////////////////////////////
//									PRIVATE
///////////////////////////////////////////////////////////////////////////////

void StartMenuState::HandleInput()
{
	if( mServerClientButton.LeftMousePressed() )
	{
		IEventPtr E1( new Event_Start_Server() );
		EventManager::GetInstance()->QueueEvent( E1 );

		IEventPtr E2( new Event_Change_State( PLAY_STATE ) );
		EventManager::GetInstance()->QueueEvent( E2 );
	}
	else if( mClientButton.LeftMousePressed() )
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
	HandleInput();
	mServerClientButton.Update( deltaTime );
	mClientButton.Update( deltaTime );
	return S_OK;
}

HRESULT StartMenuState::Render()
{
	Graphics::GetInstance()->BeginScene();

	mServerClientButton.Render();
	mClientButton.Render();

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
	mStateType		= START_MENU_STATE;
	mServerClientButton.Initialize( "../Content/Assets/Textures/ServerButton.png", 0, 0, Input::GetInstance()->mScreenWidth/2, Input::GetInstance()->mScreenHeight/2 );
	mClientButton.Initialize( "../Content/Assets/Textures/ClientButton.png", Input::GetInstance()->mScreenWidth/2, Input::GetInstance()->mScreenHeight/2, Input::GetInstance()->mScreenWidth/2, Input::GetInstance()->mScreenHeight/2 );
	return S_OK;
}

void StartMenuState::Release()
{
}

StartMenuState::StartMenuState()
{
}

StartMenuState::~StartMenuState()
{
}