#include "LobbyState.h"

void LobbyState::EventListener( IEventPtr  newEvent )
{
	if ( newEvent->GetEventType() == Event_Server_Initialize_LobbyPlayer::GUID ) // Add a remote player to the list when they connect
	{
		std::shared_ptr<Event_Server_Initialize_LobbyPlayer> data = std::static_pointer_cast<Event_Server_Initialize_LobbyPlayer>( newEvent );
		bool add = true;
		for( auto p : mPlayers )
		{
			if( p.ID == data->ID() )
			{
				add = false;
			}
		}

		if( add )
		{
			LobbyPlayer player;
			player.ID		= data->ID();
			player.team		= data->TeamID();
			player.name		= data->Name();
			XMFLOAT2 pos	= XMFLOAT2( 70.0f, 130.0f );
			if( mPlayers.size() > 0 )
			{
				pos = XMFLOAT2( mPlayers[mPlayers.size() - 1].button.GetPosition().x, mPlayers[mPlayers.size() - 1].button.GetPosition().y + 30 );
			}
			player.button.Initialize( "../Content/Assets/Textures/Menu/Create_Menu_Text/MultiPlayer.png", pos.x, pos.y, 20.0f, 20.0f );
			mPlayers.push_back( player );
		}
	}
	else if( newEvent->GetEventType() == Event_Server_Switch_Team::GUID )
	{
		std::shared_ptr<Event_Server_Switch_Team> data = std::static_pointer_cast<Event_Server_Switch_Team>( newEvent );
		for( size_t i = 0; i < mPlayers.size(); i++ )
		{
			if( data->ID() == mPlayers[i].ID )
			{
				mPlayers[i].team = data->TeamID();
			}
		}
	}
	else if( newEvent->GetEventType() == Event_Server_Lobby_Finished::GUID )
	{
		std::shared_ptr<Event_Server_Lobby_Finished> data = std::static_pointer_cast<Event_Server_Lobby_Finished>( newEvent );
		IEventPtr E1( new Event_Change_State( PLAY_STATE ) );
		EventManager::GetInstance()->QueueEvent( E1 );
	}
}

void LobbyState::HandleInput()
{
	for( size_t i = 0; i < mPlayers.size(); i++ )
	{
		if( mPlayers[i].button.LeftMousePressed() )
		{
			if( mPlayers[i].team == 1 )
			{
				mPlayers[i].team = 2;
			}
			else
			{
				mPlayers[i].team = 1;
			}
			IEventPtr E1( new Event_Client_Switch_Team( mPlayers[i].ID, mPlayers[i].team ) );
			Client::GetInstance()->SendEvent( E1 );
		}
	}

	if( mStartButton.LeftMousePressed() )
	{
		IEventPtr E1( new Event_Client_Lobby_Finished() );
		Client::GetInstance()->SendEvent( E1 );
	}
}

HRESULT LobbyState::Update( float deltaTime )
{
	HRESULT hr = S_OK;

	for( size_t i = 0; i < mPlayers.size(); i++ )
	{
		mPlayers[i].button.Update( deltaTime );
	}

	mStartButton.Update( deltaTime );
	
	HandleInput();

	return hr;
}

HRESULT LobbyState::Render()
{
	HRESULT hr = S_OK;

	std::string textToWrite = "Name\n";
	mFont.WriteText( textToWrite, 100.0f, 100.0f, 2 );

	for( auto p : mPlayers )
	{
		textToWrite = p.name;
		
		mFont.WriteText( textToWrite, p.button.GetPosition().x + 30.0f, p.button.GetPosition().y, 2 );
	}

	textToWrite = "Team\n";
	mFont.WriteText( textToWrite, 360.0f, 100.0f, 2 );
	for( auto p : mPlayers )
	{
		textToWrite = std::to_string( p.team );
		
		mFont.WriteText( textToWrite, 360.0f, p.button.GetPosition().y, 2 );
	}


	mStartButton.Render();

	for( auto p : mPlayers )
	{
		p.button.Render();
	}

	RenderManager::GetInstance()->Render();

	return hr;
}

void LobbyState::OnEnter()
{

}

void LobbyState::OnExit()
{
	mStartButton.SetExitCooldown();
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
	EventManager::GetInstance()->AddListener( &LobbyState::EventListener, this, Event_Server_Switch_Team::GUID );
	EventManager::GetInstance()->AddListener( &LobbyState::EventListener, this, Event_Server_Lobby_Finished::GUID );

	mStartButton.Initialize( "../Content/Assets/Textures/Menu/Create_Menu_Text/MultiPlayer.png", 1600, 700, 200, 200 );

	return hr;
}

void LobbyState::Release()
{
	mStartButton.Release();
	for( size_t i = 0; i < mPlayers.size(); i++ )
	{
		mPlayers[i].button.Release();
	}
	mPlayers.clear();
}

LobbyState::LobbyState()
{
	mPlayers = std::vector<LobbyPlayer>( 0 );
}

LobbyState::~LobbyState()
{

}