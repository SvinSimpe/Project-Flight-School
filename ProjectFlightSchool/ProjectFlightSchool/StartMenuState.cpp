#include "StartMenuState.h"

///////////////////////////////////////////////////////////////////////////////
//									PRIVATE
///////////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////////
//									PUBLIC
///////////////////////////////////////////////////////////////////////////////

HRESULT StartMenuState::Update( float deltaTime )
{
	if( Input::GetInstance()->mCurrentFrame.at(KEYS::KEYS_SPACE) )
	{
		IEventPtr E1( new Event_Change_State( PLAY_STATE ) );
		EventManager::GetInstance()->QueueEvent( E1 );
	}
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