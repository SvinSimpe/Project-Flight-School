#include "LobbyState.h"

void LobbyState::EventListener( IEventPtr  newEvent )
{
	if ( newEvent->GetEventType() == Event_Server_Initialize_LobbyPlayer::GUID ) // Add a remote player to the list when they connect
	{
		std::shared_ptr<Event_Server_Initialize_LobbyPlayer> data = std::static_pointer_cast<Event_Server_Initialize_LobbyPlayer>( newEvent );
		bool add = true;
		for( auto p : mPlayers )
		{
			if( p->ID == data->ID() )
			{
				add = false;
			}
		}

		if( add )
		{
			LobbyPlayer* player = new LobbyPlayer;
			player->ID			= data->ID();
			player->team		= data->TeamID();
			player->name		= data->Name();
			XMFLOAT2 pos		= XMFLOAT2( 70.0f, 130.0f );
			if( mPlayers.size() > 0 )
			{
				pos = XMFLOAT2( mPlayers[mPlayers.size() - 1]->button.GetPosition().x, mPlayers[mPlayers.size() - 1]->button.GetPosition().y + 30 );
			}
			player->button.Initialize( "../Content/Assets/Textures/Menu/Create_Menu_Text/MultiPlayer.png", pos.x, pos.y, 20.0f, 20.0f );
			mPlayers.push_back( player );
			printf( "Lobby:: Ny Spelare: %d, blev lag %d\n", player->ID, player->team );
		}
	}
	else if( newEvent->GetEventType() == Event_Server_Switch_Team::GUID )
	{
		std::shared_ptr<Event_Server_Switch_Team> data = std::static_pointer_cast<Event_Server_Switch_Team>( newEvent );
		for( size_t i = 0; i < mPlayers.size(); i++ )
		{
			if( data->ID() == mPlayers[i]->ID )
			{
				mPlayers[i]->team = data->TeamID();
				printf( "Lobby:: Spelare: %d, blev lag %d\n", mPlayers[i]->ID, mPlayers[i]->team );
			}
		}
	}
	else if( newEvent->GetEventType() == Event_Server_Lobby_Finished::GUID )
	{
		if( mActive )
		{
			std::shared_ptr<Event_Server_Lobby_Finished> data = std::static_pointer_cast<Event_Server_Lobby_Finished>( newEvent );
			IEventPtr E1( new Event_Change_State( PLAY_STATE ) );
			EventManager::GetInstance()->QueueEvent( E1 );
		}
	}
	else if( newEvent->GetEventType() == Event_Remote_Left::GUID )
	{
		std::shared_ptr<Event_Remote_Left> data = std::static_pointer_cast<Event_Remote_Left>( newEvent );
		for( size_t i = 0; i < mPlayers.size(); i++ )
		{
			if( mPlayers[i]->ID == data->ID() )
			{
				mPlayers.at(i)->button.Release();
				SAFE_DELETE( mPlayers.at(i) );
				std::swap( mPlayers.at(i), mPlayers.at(mPlayers.size() - 1) );
				mPlayers.pop_back();
			}
		}
	}
	else if ( newEvent->GetEventType() == Event_Connect_Server_Success::GUID )
	{
		IEventPtr E1( new Event_Change_State( LOBBY_OWNER_STATE ) );
		EventManager::GetInstance()->QueueEvent( E1 );
	}
}

void LobbyState::HandleInput()
{
	for( size_t i = 0; i < mPlayers.size(); i++ )
	{
		if( mPlayers[i]->button.LeftMousePressed() )
		{
			if( mPlayers[i]->team == 1 )
			{
				mPlayers[i]->team = 2;
			}
			else
			{
				mPlayers[i]->team = 1;
			}
			IEventPtr E1( new Event_Client_Switch_Team( mPlayers[i]->ID, mPlayers[i]->team ) );
			Client::GetInstance()->SendEvent( E1 );
		}
	}
}

HRESULT LobbyState::Update( float deltaTime )
{
	HRESULT hr = S_OK;

	for( size_t i = 0; i < mPlayers.size(); i++ )
	{
		mPlayers[i]->button.Update( deltaTime );
	}
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
		textToWrite = p->name;
		
		mFont.WriteText( textToWrite, p->button.GetPosition().x + 30.0f, p->button.GetPosition().y, 2 );
	}

	textToWrite = "Team\n";
	mFont.WriteText( textToWrite, 360.0f, 100.0f, 2 );
	for( auto p : mPlayers )
	{
		textToWrite = std::to_string( p->team );
		
		mFont.WriteText( textToWrite, 360.0f, p->button.GetPosition().y, 2 );
	}

	for( auto p : mPlayers )
	{
		p->button.Render();
	}

	RenderManager::GetInstance()->Render();

	return hr;
}

void LobbyState::OnEnter()
{
	Reset();
	SetCursor( mCursor );
	mActive = true;
}

void LobbyState::OnExit()
{
	mActive = false;
	for( size_t i = 0; i < mPlayers.size(); i++ )
	{
		mPlayers[i]->button.SetExitCooldown();
	}
}

void LobbyState::Reset()
{
	mActive = false;
	for( size_t i = 0; i < mPlayers.size(); i++ )
	{
		mPlayers[i]->button.Release();
		SAFE_DELETE( mPlayers[i] );
	}
	mPlayers.clear();
}

HRESULT LobbyState::Initialize()
{
	BaseState::Initialize();
	HRESULT hr = S_OK;

	mFont.Initialize( "../Content/Assets/GUI/Fonts/final_font/" );

	mStateType = LOBBY_STATE;
	EventManager::GetInstance()->AddListener( &LobbyState::EventListener, this, Event_Server_Initialize_LobbyPlayer::GUID );
	EventManager::GetInstance()->AddListener( &LobbyState::EventListener, this, Event_Server_Switch_Team::GUID );
	EventManager::GetInstance()->AddListener( &LobbyState::EventListener, this, Event_Server_Lobby_Finished::GUID );
	EventManager::GetInstance()->AddListener( &LobbyState::EventListener, this, Event_Remote_Left::GUID );
	EventManager::GetInstance()->AddListener( &LobbyState::EventListener, this, Event_Connect_Server_Success::GUID );

	return hr;
}

void LobbyState::Release()
{
	mFont.Release();
	for( size_t i = 0; i < mPlayers.size(); i++ )
	{
		mPlayers[i]->button.Release();
		SAFE_DELETE( mPlayers[i] );
	}
	mPlayers.clear();
}

LobbyState::LobbyState()
{
	mActive = false;
	mPlayers = std::vector<LobbyPlayer*>( 0 );
}

LobbyState::~LobbyState()
{

}