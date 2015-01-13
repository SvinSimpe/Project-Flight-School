#include "StateMachine.h"

///////////////////////////////////////////////////////////////////////////////
//									PRIVATE
///////////////////////////////////////////////////////////////////////////////

void StateMachine::ChangeStateListener( IEventPtr newEvent )
{
	if ( newEvent->GetEventType() == Event_Change_State::GUID )
	{
		std::shared_ptr<Event_Change_State> data = std::static_pointer_cast<Event_Change_State>( newEvent );
		int eventID = data->EventID();
		if( eventID > 0 && eventID < NR_OF_STATES )
		{
			ChangeState( eventID );
		}
	}
}

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

HRESULT StateMachine::Initialize()
{
	mStates							= new BaseState*[NR_OF_STATES];
	mStates[START_MENU_STATE]		= new StartMenuState();
	mStates[PLAY_STATE]				= new PlayState();
	mCurrentState					= START_MENU_STATE;

	mStates[START_MENU_STATE]->Initialize();
	mStates[PLAY_STATE]->Initialize();

	EventManager::GetInstance()->AddListener( &StateMachine::ChangeStateListener, this, Event_Change_State::GUID );

	return S_OK;
}

void StateMachine::Release()
{
	mCurrentState	= 0;
	mStates[START_MENU_STATE]->Release();
	SAFE_DELETE( mStates[START_MENU_STATE] );
	mStates[PLAY_STATE]->Release();
	SAFE_DELETE( mStates[PLAY_STATE] );
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