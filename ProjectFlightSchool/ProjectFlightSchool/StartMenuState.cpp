#include "StartMenuState.h"

///////////////////////////////////////////////////////////////////////////////
//									PRIVATE
///////////////////////////////////////////////////////////////////////////////

void StartMenuState::HandleInput()
{
	if( mStartServerClient.LeftMousePressed() )
	{
		IEventPtr E1( new Event_Start_Server() );
		EventManager::GetInstance()->QueueEvent( E1 );

		IEventPtr E2( new Event_Change_State( PLAY_STATE ) );
		EventManager::GetInstance()->QueueEvent( E2 );
	}
	else if( mStartClient.LeftMousePressed() )
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
	return S_OK;
}

HRESULT StartMenuState::Render()
{
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
	mStartServerClient.Initialize( 0, 0, Input::GetInstance()->mScreenWidth/2, Input::GetInstance()->mScreenHeight/2 );
	mStartClient.Initialize( Input::GetInstance()->mScreenWidth/2, Input::GetInstance()->mScreenHeight/2, Input::GetInstance()->mScreenWidth/2, Input::GetInstance()->mScreenHeight/2 );
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