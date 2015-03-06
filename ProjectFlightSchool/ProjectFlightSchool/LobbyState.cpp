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
			XMFLOAT2 pos		= XMFLOAT2( mTeamOneXPos, 740.0f );
			if( player->team == 2 )
			{
				pos.x = mTeamTwoXPos;
			}

			for( auto p : mPlayers )
			{
				if( p->team == player->team )
				{
					pos = XMFLOAT2( p->button.GetPosition().x, p->button.GetPosition().y + 60 );
				}
			}
			player->button.Initialize( "../Content/Assets/Textures/Menu/lobby_loadout_menu/lobbyNameFrame.dds", pos.x, pos.y, player->size.x, player->size.y );
			mPlayers.push_back( player );
			if( data->ID() == mMyID )
			{
				mPlayers.back()->thisPlayer = true;
			}
			for( size_t i = 0; i < mPlayers.size(); i++ )
			{
				if( mPlayers[i]->thisPlayer )
				{
					IEventPtr E1( new Event_Client_Change_Ready_State( mPlayers[i]->ID, mPlayers[i]->isReady ) );
					Client::GetInstance()->SendEvent( E1 );
					break;
				}
			}
		}
	}
	else if( newEvent->GetEventType() == Event_Server_Switch_Team::GUID && mActive && !mTeamsLocked )
	{
		std::shared_ptr<Event_Server_Switch_Team> data = std::static_pointer_cast<Event_Server_Switch_Team>( newEvent );
		for( size_t i = 0; i < mPlayers.size(); i++ )
		{
			if( data->ID() == mPlayers[i]->ID )
			{
				mPlayers[i]->team = data->TeamID();
				printf( "Lobby:: Spelare: %d, blev lag %d\n", mPlayers[i]->ID, mPlayers[i]->team );
				XMFLOAT2 pos		= XMFLOAT2( mTeamOneXPos, 740.0f );
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
							pos = XMFLOAT2( p->button.GetPosition().x, p->button.GetPosition().y + 60 );
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
						XMFLOAT2 pos = XMFLOAT2( mTeamOneXPos, 740.0f );
						if( p->team == 2 )
						{
							pos.x = mTeamTwoXPos;
						}
						for( auto pl : playersTemp )
						{
							if( pl->team == p->team && pl->ID != p->ID )
							{
								pos = XMFLOAT2( pl->button.GetPosition().x, pl->button.GetPosition().y + 60 );
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
	else if( newEvent->GetEventType() == Event_Local_Joined::GUID )
	{
		std::shared_ptr<Event_Local_Joined> data = std::static_pointer_cast<Event_Local_Joined>( newEvent );
		if( mMyID == (UINT)-1 )
		{
			mMyID = data->ID();
		}
	}
	else if( newEvent->GetEventType() == Event_Server_Change_Ready_State::GUID )
	{
		std::shared_ptr<Event_Server_Change_Ready_State> data = std::static_pointer_cast<Event_Server_Change_Ready_State>( newEvent );
		for( size_t i = 0; i < mPlayers.size(); i++ )
		{
			if( mPlayers[i]->ID == data->ID() )
			{
				mPlayers[i]->isReady = data->IsReady();
			}
		}
	}
	else if( newEvent->GetEventType() == Event_Server_Start_Game_Countdown::GUID )
	{
		StartGameCountdown();
	}
}

void LobbyState::StartGameCountdown()
{
	mGameCountdown = 5.0f;
	mGameCountdownStarted = true;
	mTeamsLocked = true;
}

void LobbyState::HandleInput()
{
	for( size_t i = 0; i < mPlayers.size(); i++ )
	{
		if( mPlayers[i]->thisPlayer && mPlayers[i]->button.LeftMousePressed() )
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
	if( mReadyButton.LeftMousePressed() )
	{
		for( size_t i = 0; i < mPlayers.size(); i++ )
		{
			if( mPlayers[i]->thisPlayer )
			{
				if( mPlayers[i]->isReady )
				{
					mPlayers[i]->isReady = false;
				}
				else
				{
					mPlayers[i]->isReady = true;
				}
				IEventPtr E1( new Event_Client_Change_Ready_State( mPlayers[i]->ID, true ) );
				Client::GetInstance()->SendEvent( E1 );
				break;
			}
		}
	}
	if( mChooseWeaponButton.LeftMousePressed() )
	{
		mLoadOutMenu.Activate();
	}
}

HRESULT LobbyState::Update( float deltaTime )
{
	SetCursor( mCursor );
	HRESULT hr = S_OK;

	IEventPtr E1( new Event_Client_Reach_Server() ); //Check so that the connection isn't broken.
	Client::GetInstance()->SendEvent( E1 );

	for( size_t i = 0; i < mPlayers.size(); i++ )
	{
		mPlayers[i]->button.Update( deltaTime );
	}
	mBackButton.Update( deltaTime );
	mReadyButton.Update( deltaTime );

	if( mLoadOutMenu.IsActive() )
	{
		mLoadOutMenu.Update( deltaTime );
	}
	else
	{
		HandleInput();
	}

	for( size_t i = 0; i < mPlayers.size(); i++ )
	{
		if( mPlayers[i]->thisPlayer )
		{
			if( mPlayers[i]->isReady )
			{
				mReadyButton.SetImage( mReadyImg.GetAssetID() );
			}
			else
			{
				mReadyButton.SetImage( mNotReadyImg.GetAssetID() );
			}
		}
	}

	if( mGameCountdownStarted )
	{
		mGameCountdown -= deltaTime;
	}

	mChooseWeaponButton.Update( deltaTime );

	return hr;
}

HRESULT LobbyState::Render( float deltaTime )
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
		
		mFont.WriteText( textToWrite, p->button.GetPosition().x + 20.0f, p->button.GetPosition().y + 15.0f, 3.0f, COLOR_CYAN );
		if( p->isReady )
		{
			mReadyImg.Render( p->button.GetPosition().x + 275.0f, p->button.GetPosition().y + 5.0f, 50.0f, 50.0f );
		}
		else
		{
			mNotReadyImg.Render( p->button.GetPosition().x + 275.0f, p->button.GetPosition().y + 5.0f, 50.0f, 50.0f );
		}

	}

	mBackButton.Render();
	mReadyButton.Render();
	mChooseWeaponButton.Render();
	mChooseWeaponText.Render();

	if( mLoadOutMenu.IsActive() )
	{
		mLoadOutMenu.Render();
	}

	if( mGameCountdownStarted )
	{
		int secondsLeft = (int) mGameCountdown;
		std::ostringstream out;
		out << secondsLeft + 1;
		float offset = mFont.GetMiddleXPoint( out.str(), 20.0f );
		mFont.WriteText( out.str(), (float)( Input::GetInstance()->mScreenWidth * 0.5f ) - offset, 200.0f, 20.0f, COLOR_ORANGE );
	}
	
	RenderManager::GetInstance()->Render();

	return hr;
}

void LobbyState::OnEnter()
{
	Reset();
	SetCursor( mCursor );
	mActive = true;
	mTeamsLocked = false;
}

void LobbyState::OnExit()
{
	Reset();
	//SoundBufferHandler::GetInstance()->StopLoopStream( mStreamSoundAsset );
	mTeamsLocked = true;
}

void LobbyState::Reset()
{
	mLoadOutMenu.Reset();
	mActive = false;
	mGameCountdown = 0.0f;
	mGameCountdownStarted = false;
	for( size_t i = 0; i < mPlayers.size(); i++ )
	{
		mPlayers[i]->button.Release();
		SAFE_DELETE( mPlayers[i] );
	}
	mBackButton.SetExitCooldown();
	mReadyButton.SetExitCooldown();
	mChooseWeaponButton.SetExitCooldown();
	mPlayers.clear();
	mMyID = (UINT)-1;	
}

HRESULT LobbyState::Initialize()
{
	BaseState::Initialize();
	HRESULT hr = S_OK;

	mFont.Initialize( "../Content/Assets/GUI/Fonts/final_font/" );
	
	Graphics::GetInstance()->LoadStatic2dAsset( "../Content/Assets/Textures/Menu/lobby_loadout_menu/lobbyNameFrame.dds", mBackground ); //Laddar in bilden till knapparna så att deras initialize bara får en int och inte laddar nnya bilder.
	Graphics::GetInstance()->LoadStatic2dAsset( "../Content/Assets/Textures/Menu/lobby_loadout_menu/lobbyMenu.dds", mBackground );

	mStateType = LOBBY_STATE;
	EventManager::GetInstance()->AddListener( &LobbyState::EventListener, this, Event_Server_Initialize_LobbyPlayer::GUID );
	EventManager::GetInstance()->AddListener( &LobbyState::EventListener, this, Event_Server_Switch_Team::GUID );
	EventManager::GetInstance()->AddListener( &LobbyState::EventListener, this, Event_Server_Lobby_Finished::GUID );
	EventManager::GetInstance()->AddListener( &LobbyState::EventListener, this, Event_Remote_Left::GUID );
	EventManager::GetInstance()->AddListener( &LobbyState::EventListener, this, Event_Local_Joined::GUID );
	EventManager::GetInstance()->AddListener( &LobbyState::EventListener, this, Event_Server_Change_Ready_State::GUID );
	EventManager::GetInstance()->AddListener( &LobbyState::EventListener, this, Event_Server_Start_Game_Countdown::GUID );

	mStreamSoundAsset = SoundBufferHandler::GetInstance()->LoadStreamBuffer( "../Content/Assets/Sound/Groove 1 Bass.wav" );
	//float x = ( (float)Input::GetInstance()->mScreenWidth * 0.9f ) - 650.0f;
	//float y = ( (float)Input::GetInstance()->mScreenHeight * 0.9f ) - 200.0f;
	//float w = 200.0f;
	//float h = 200.0f;

	//mBackButton.Initialize( "../Content/Assets/Textures/Menu/lobby_loadout_menu/textBack.dds", x, y, w, h );

	EventManager::GetInstance()->AddListener( &LobbyState::EventListener, this, Event_Connect_Server_Success::GUID );

	mBackButton.Initialize( "../Content/Assets/Textures/Menu/lobby_loadout_menu/textBack.dds", 70.0f, 760.0f, 200.0f, 200.0f );
	
	mReadyButton.Initialize( "", 1665.0f, 550.0f, 200.0, 200.0 );
	mReadyImg.Initialize( "../Content/Assets/Textures/Menu/lobby_loadout_menu/ready.png", 1665.0f, 550.0f, 200.0, 200.0 );
	mNotReadyImg.Initialize( "../Content/Assets/Textures/Menu/lobby_loadout_menu/notReady.png", 1665.0f, 550.0f, 200.0, 200.0 );
	mReadyButton.SetImage( mNotReadyImg.GetAssetID() );

	mLoadOutMenu.Initialize();

	mChooseWeaponButton.Initialize( "../Content/Assets/Textures/Menu/lobby_loadout_menu/changeYourWeaponFrame.dds", 875.0f, 820.0f, 184.0f, 152.0f );
	mChooseWeaponText.Initialize( "../Content/Assets/Textures/Menu/lobby_loadout_menu/textChooseYourWeapon.dds", 875.0f, 820.0f, 184.0f, 152.0f );
	mGameCountdown = 0.0f;

	return hr;
}

void LobbyState::Release()
{
	mFont.Release();
	mBackButton.Release();
	mReadyButton.Release();
	for( size_t i = 0; i < mPlayers.size(); i++ )
	{
		mPlayers[i]->button.Release();
		SAFE_DELETE( mPlayers[i] );
	}
	mPlayers.clear();
	mBackButton.Release();
	mLoadOutMenu.Release();
	mChooseWeaponButton.Release();
	mChooseWeaponText.Release();
}

LobbyState::LobbyState()
{
	mActive			= false;
	mPlayers		= std::vector<LobbyPlayer*>( 0 );
	mTeamOneXPos	= 415.0f;
	mTeamTwoXPos	= 1190.0f;
	mMyID			= (UINT)-1;
	mGameCountdown	= 0.0f;
	mGameCountdownStarted = false;
}

LobbyState::~LobbyState()
{

}