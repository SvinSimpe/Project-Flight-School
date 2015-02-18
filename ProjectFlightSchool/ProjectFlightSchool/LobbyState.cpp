#include "LobbyState.h"

void LobbyState::EventListener( IEventPtr  newEvent )
{
	if ( newEvent->GetEventType() == Event_Server_Initialize_LobbyPlayer::GUID ) // Add a remote player to the list when they connect
	{
		std::shared_ptr<Event_Server_Initialize_LobbyPlayer> data = std::static_pointer_cast<Event_Server_Initialize_LobbyPlayer>( newEvent );
		LobbyPlayer player;
		player.ID	= data->ID();
		player.team = data->TeamID();
		player.name	= data->Name();
		mPlayers.push_back( player );
	}
}

HRESULT LobbyState::Update( float deltaTime )
{
	HRESULT hr = S_OK;

	//Sleep( 50 );

	//IEventPtr E1( new Event_Change_State( PLAY_STATE ) );
	//EventManager::GetInstance()->QueueEvent( E1 );

	return hr;
}

HRESULT LobbyState::Render()
{
	HRESULT hr = S_OK;

	std::string textToWrite = "Name\n";

	for( auto p : mPlayers )
	{
		textToWrite += p.name;
	}

	mFont.WriteText( textToWrite, 100, 100, 2 );

	textToWrite = "Team\n";
	for( auto p : mPlayers )
	{
		textToWrite += std::to_string( p.team );
	}

	mFont.WriteText( textToWrite, 400, 100, 2 );

	RenderManager::GetInstance()->Render();

	return hr;
}

void LobbyState::OnEnter()
{

}

void LobbyState::OnExit()
{

}

void LobbyState::Reset()
{
	mPlayers.clear();
}

HRESULT LobbyState::Initialize()
{
	HRESULT hr = S_OK;

	mFont.Initialize( "../Content/Assets/GUI/Fonts/final_font/" );

	mStateType = LOBBY_STATE;
	EventManager::GetInstance()->AddListener( &LobbyState::EventListener, this, Event_Server_Initialize_LobbyPlayer::GUID );

	return hr;
}

void LobbyState::Release()
{
	mPlayers.clear();
}

LobbyState::LobbyState()
{
	mPlayers = std::vector<LobbyPlayer>( 0 );
}

LobbyState::~LobbyState()
{

}