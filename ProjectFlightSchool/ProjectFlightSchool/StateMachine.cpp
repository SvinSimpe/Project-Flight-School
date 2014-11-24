#include "StateMachine.h"

///////////////////////////////////////////////////////////////////////////////
//									PRIVATE
///////////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////////
//									PUBLIC
///////////////////////////////////////////////////////////////////////////////
HRESULT StateMachine::Update( float deltaTime )
{
	mStates[mCurrentState]->Update( deltaTime );
	
	return S_OK;
}

HRESULT StateMachine::Render()
{
	mStates[mCurrentState]->Render();

	return S_OK;
}

HRESULT StateMachine::Initialize()
{
	mStates							= new BaseState*[NR_OF_STATES];
	mStates[START_MENU_STATE]		= new StartMenuState();
	mStates[PLAY_STATE]				= new PlayState();
	mCurrentState					= START_MENU_STATE;

	mStates[START_MENU_STATE]->Initialize();
	mStates[PLAY_STATE]->Initialize();

	return S_OK;
}

// In parameter const int NEW_STATE must be a valid state
// If valid close current state and change to new state
HRESULT StateMachine::ChangeState( const int NEW_STATE )
{
	if( NEW_STATE == START_MENU_STATE || NEW_STATE == NEW_STATE )
	{
		mStates[mCurrentState]->OnExit();
		mCurrentState		= NEW_STATE;
		mStates[mCurrentState]->OnEnter();
	}
	else
	{
		OutputDebugString( L"StateMachine: Failed to change state" );
		return E_FAIL;
	}

	return S_OK;
}

void StateMachine::ResetState( const int STATE )
{
	mStates[STATE]->Reset();
}

void StateMachine::Release()
{
	mCurrentState	= 0;
	delete mStates[START_MENU_STATE];
	delete mStates[PLAY_STATE];
	delete [] mStates;
}

StateMachine::StateMachine()
{
	mStates			= nullptr;
	mCurrentState	= 0;
}

StateMachine::~StateMachine()
{
}