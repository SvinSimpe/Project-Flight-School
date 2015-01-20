#include "StateMachine.h"

///////////////////////////////////////////////////////////////////////////////
//									PRIVATE
///////////////////////////////////////////////////////////////////////////////

void StateMachine::EventListener( IEventPtr newEvent )
{
	if ( newEvent->GetEventType() == Event_Change_State::GUID )
	{
		std::shared_ptr<Event_Change_State> data = std::static_pointer_cast<Event_Change_State>( newEvent );
		int eventID = data->EventID();
		if( eventID >= 0 && eventID < NR_OF_STATES )
		{
			ChangeState( eventID );
		}
	}
	else if( newEvent->GetEventType() == Event_Connection_Failed::GUID )
	{
		std::shared_ptr<Event_Connection_Failed> data = std::static_pointer_cast<Event_Connection_Failed>( newEvent );
		std::string msg = data->FailMessage();

		std::cout << msg << std::endl;
		mStates[mCurrentState]->Reset();
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

	for( int i = 0; i < NR_OF_STATES; i++ )
	{
		mStates[i] = nullptr;
	}

	mStates[START_MENU_STATE]		= new StartMenuState();
	mStates[CREATE_MENU_STATE]		= new CreateMenuState();
	mStates[JOIN_MENU_STATE]		= new JoinMenuState();
	mStates[MULTI_MENU_STATE]		= new MultiplayerMenuState();
	mStates[OPTIONS_MENU_STATE]		= new OptionsMenuState();
	mStates[SINGLE_MENU_STATE]		= new SingleplayerMenuState();
	mStates[PLAY_STATE]				= new PlayState();
	mCurrentState					= START_MENU_STATE;

	for( int i = 0; i < NR_OF_STATES; i++ )
	{
		mStates[i]->Initialize();
	}

	EventManager::GetInstance()->AddListener( &StateMachine::EventListener, this, Event_Change_State::GUID );
	EventManager::GetInstance()->AddListener( &StateMachine::EventListener, this, Event_Connection_Failed::GUID );

	return S_OK;
}

void StateMachine::Release()
{
	mCurrentState	= 0;
	for( int i = 0; i < NR_OF_STATES; i++ )
	{
		mStates[i]->Release();
		SAFE_DELETE( mStates[i] );
	}
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