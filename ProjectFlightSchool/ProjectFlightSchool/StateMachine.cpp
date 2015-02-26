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
	else if( newEvent->GetEventType() == Event_Connect_Client_Fail::GUID )
	{
		std::shared_ptr<Event_Connect_Client_Fail> data = std::static_pointer_cast<Event_Connect_Client_Fail>( newEvent );
		std::string msg = data->ErrorMsg();

		std::cout << msg << std::endl;
		mStates[mCurrentState]->Reset();
	}
	else if( newEvent->GetEventType() == Event_Connect_Server_Fail::GUID )
	{
		std::shared_ptr<Event_Connect_Server_Fail> data = std::static_pointer_cast<Event_Connect_Server_Fail>( newEvent );
		std::string msg = data->ErrorMsg();

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

HRESULT StateMachine::Render( float deltaTime )
{
	mStates[mCurrentState]->Render( deltaTime );

	return S_OK;
}

// In parameter const int NEW_STATE must be a valid state
// If valid close current state and change to new state
HRESULT StateMachine::ChangeState( const int NEW_STATE )
{
	if( NEW_STATE == START_MENU_STATE || NEW_STATE == NEW_STATE )
	{
		if( NEW_STATE == LOBBY_STATE && mStates[mCurrentState]->GetStateType() == LOBBY_OWNER_STATE )
		{
			OutputDebugString( L"Not an appopriate state change" );
		}
		else
		{
			mStates[mCurrentState]->OnExit();
			mCurrentState = NEW_STATE;
			mStates[mCurrentState]->OnEnter();
		}
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
	mStates[JOIN_MENU_STATE]		= new JoinMenuState();
	mStates[MULTI_MENU_STATE]		= new MultiplayerMenuState();
	mStates[OPTIONS_MENU_STATE]		= new OptionsMenuState();
	mStates[PLAY_STATE]				= new PlayState();
	mStates[LOBBY_STATE]			= new LobbyState();
	mStates[LOBBY_OWNER_STATE]		= new LobbyOwnerState();
	mCurrentState					= START_MENU_STATE;

	for( int i = 0; i < NR_OF_STATES; i++ )
	{
		mStates[i]->Initialize();
	}

	EventManager::GetInstance()->AddListener( &StateMachine::EventListener, this, Event_Change_State::GUID );
	EventManager::GetInstance()->AddListener( &StateMachine::EventListener, this, Event_Connect_Client_Fail::GUID );
	EventManager::GetInstance()->AddListener( &StateMachine::EventListener, this, Event_Connect_Server_Fail::GUID );


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