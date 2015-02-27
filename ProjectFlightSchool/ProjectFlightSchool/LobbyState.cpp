#include "LobbyState.h"

void LobbyState::EventListener( IEventPtr  newEvent )
{
	if ( newEvent->GetEventType() == Event_Server_Initialize_LobbyPlayer::GUID && mActive ) // Add a remote player to the list when they connect
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
			XMFLOAT2 pos		= XMFLOAT2( mTeamOneXPos, 720.0f );
			if( player->team == 2 )
			{
				pos.x = mTeamTwoXPos;
			}

			for( auto p : mPlayers )
			{
				if( p->team == player->team )
				{
					pos = XMFLOAT2( p->button.GetPosition().x, p->button.GetPosition().y + 70 );
				}
			}
			player->button.Initialize( "../Content/Assets/Textures/Menu/lobby_loadout_menu/lobbyNameFrame.dds", pos.x, pos.y, player->size.x, player->size.y );
			mPlayers.push_back( player );
			printf( "Lobby:: Ny Spelare: %d, blev lag %d\n", player->ID, player->team );
		}
	}
	else if( newEvent->GetEventType() == Event_Server_Switch_Team::GUID && mActive )
	{
		std::shared_ptr<Event_Server_Switch_Team> data = std::static_pointer_cast<Event_Server_Switch_Team>( newEvent );
		for( size_t i = 0; i < mPlayers.size(); i++ )
		{
			if( data->ID() == mPlayers[i]->ID )
			{
				mPlayers[i]->team = data->TeamID();
				printf( "Lobby:: Spelare: %d, blev lag %d\n", mPlayers[i]->ID, mPlayers[i]->team );
				XMFLOAT2 pos		= XMFLOAT2( mTeamOneXPos, 720.0f );
				if( mPlayers[i]->team == 2 )
				{
					pos.x = mTeamTwoXPos;
				}

				for( auto p : mPlayers )
				{
					if( p->team == mPlayers[i]->team && p->ID != mPlayers[i]->ID )
					{
						if( p->button.GetPosition().y >= pos.y )
						{
							pos = XMFLOAT2( p->button.GetPosition().x, p->button.GetPosition().y + 70 );
						}
					}
				}
				mPlayers[i]->button.SetPosition( pos, mPlayers[i]->size );

				std::vector<LobbyPlayer*>playersTemp = std::vector<LobbyPlayer*>( 0 );

				for( auto p : mPlayers )
				{
					playersTemp.push_back( p );
					if( p->team != mPlayers[i]->team )
					{
						XMFLOAT2 pos = XMFLOAT2( mTeamOneXPos, 720.0f );
						if( p->team == 2 )
						{
							pos.x = mTeamTwoXPos;
						}
						for( auto pl : playersTemp )
						{
							if( pl->team == p->team && pl->ID != p->ID )
							{
								pos = XMFLOAT2( pl->button.GetPosition().x, pl->button.GetPosition().y + 70 );
							}
						}
						p->button.SetPosition( pos, p->size );
					}
				}
			}
		}
	}
	else if( newEvent->GetEventType() == Event_Server_Lobby_Finished::GUID && mActive )
	{
		std::shared_ptr<Event_Server_Lobby_Finished> data = std::static_pointer_cast<Event_Server_Lobby_Finished>( newEvent );
		IEventPtr E1( new Event_Change_State( PLAY_STATE ) );
		EventManager::GetInstance()->QueueEvent( E1 );
	}
	else if( newEvent->GetEventType() == Event_Remote_Left::GUID && mActive )
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
	if( mBackButton.LeftMousePressed() )
	{
		IEventPtr E1( new Event_Reset_Game() );
		EventManager::GetInstance()->QueueEvent( E1 );
	}
}

HRESULT LobbyState::Update( float deltaTime )
{
	HRESULT hr = S_OK;

	IEventPtr E1( new Event_Client_Reach_Server() ); //Check so that the connection isn't broken.
	Client::GetInstance()->SendEvent( E1 );

	for( size_t i = 0; i < mPlayers.size(); i++ )
	{
		mPlayers[i]->button.Update( deltaTime );
	}
	mBackButton.Update( deltaTime );

	HandleInput();

	return hr;
}

HRESULT LobbyState::Render()
{
	HRESULT hr = S_OK;
	
	RenderManager::GetInstance()->AddObject2dToList( mBackground, XMFLOAT2( 0.0f, 0.0f ), XMFLOAT2( (float)Input::GetInstance()->mScreenWidth, (float)Input::GetInstance()->mScreenHeight ) );
	
	for( auto p : mPlayers )
	{
		p->button.Render();
	}
	
	std::string textToWrite = "";

	for( auto p : mPlayers )
	{
		textToWrite = p->name;
		
		mFont.WriteText( textToWrite, p->button.GetPosition().x + 20.0f, p->button.GetPosition().y + 15.0f, 3.0f );
	}

	mBackButton.Render();
	
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
	mBackButton.SetExitCooldown();
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
	
	Graphics::GetInstance()->LoadStatic2dAsset( "../Content/Assets/Textures/Menu/lobby_loadout_menu/lobbyNameFrame.dds", mBackground ); //Laddar in bilden till knapparna så att deras initialize bara får en int och inte laddar nnya bilder.
	Graphics::GetInstance()->LoadStatic2dAsset( "../Content/Assets/Textures/Menu/lobby_loadout_menu/lobby_menu.dds", mBackground );

	mStateType = LOBBY_STATE;
	EventManager::GetInstance()->AddListener( &LobbyState::EventListener, this, Event_Server_Initialize_LobbyPlayer::GUID );
	EventManager::GetInstance()->AddListener( &LobbyState::EventListener, this, Event_Server_Switch_Team::GUID );
	EventManager::GetInstance()->AddListener( &LobbyState::EventListener, this, Event_Server_Lobby_Finished::GUID );
	EventManager::GetInstance()->AddListener( &LobbyState::EventListener, this, Event_Remote_Left::GUID );
	EventManager::GetInstance()->AddListener( &LobbyState::EventListener, this, Event_Connect_Server_Success::GUID );
	
	mBackButton.Initialize( "../Content/Assets/Textures/Menu/Back.png", 70.0f, 760.0f, 200.0f, 200.0f );

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
	mBackButton.Release();
}

LobbyState::LobbyState()
{
	mActive			= false;
	mPlayers		= std::vector<LobbyPlayer*>( 0 );
	mTeamOneXPos	= 415.0f;
	mTeamTwoXPos	= 1190.0f;
}

LobbyState::~LobbyState()
{

}