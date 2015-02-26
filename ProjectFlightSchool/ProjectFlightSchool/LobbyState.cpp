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
			player->ID		= data->ID();
			player->team		= data->TeamID();
			player->name		= data->Name();
			XMFLOAT2 pos	= XMFLOAT2( 70.0f, 130.0f );
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
		std::shared_ptr<Event_Server_Lobby_Finished> data = std::static_pointer_cast<Event_Server_Lobby_Finished>( newEvent );
		IEventPtr E1( new Event_Change_State( PLAY_STATE ) );
		EventManager::GetInstance()->QueueEvent( E1 );
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

	if( mStartButton.LeftMousePressed() )
	{
		IEventPtr E1( new Event_Client_Lobby_Finished() );
		Client::GetInstance()->SendEvent( E1 );
	}
	else if( mBackButton.LeftMousePressed() )
	{
		// DO SOME STUFF HERE FOR LOGIC AND COOLNESS AND SCIENCE
	}
}

HRESULT LobbyState::Update( float deltaTime )
{
	HRESULT hr = S_OK;

	for( size_t i = 0; i < mPlayers.size(); i++ )
	{
		mPlayers[i]->button.Update( deltaTime );
	}

	mStartButton.Update( deltaTime );
	mBackButton.Update( deltaTime );
	
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

	mStartButton.Render();
	mBackButton.Render();

	for( auto p : mPlayers )
	{
		p->button.Render();
	}
	RenderManager::GetInstance()->AddObject2dToList( mCursor, XMFLOAT2( (float)Input::GetInstance()->mCurrentMousePos.x, (float)Input::GetInstance()->mCurrentMousePos.y ), DirectX::XMFLOAT2( 20.0f, 20.0f ) );

	RenderManager::GetInstance()->Render();

	return hr;
}

void LobbyState::OnEnter()
{
	Reset();
}

void LobbyState::OnExit()
{
	mStartButton.SetExitCooldown();
	for( size_t i = 0; i < mPlayers.size(); i++ )
	{
		mPlayers[i]->button.SetExitCooldown();
	}
}

void LobbyState::Reset()
{
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

	float x = ( (float)Input::GetInstance()->mScreenWidth * 0.9f ) - 400.0f;
	float y = ( (float)Input::GetInstance()->mScreenHeight * 0.9f ) - 200.0f;
	float w = 200.0f;
	float h = 200.0f;

	mStartButton.Initialize( "../Content/Assets/Textures/Menu/Create_Menu_Text/MultiPlayer.png", x, y, w, h );

	x += 250.0f;

	mBackButton.Initialize( "../Content/Assets/Textures/Menu/Back.png", x, y, w, h );

	return hr;
}

void LobbyState::Release()
{
	mFont.Release();
	mStartButton.Release();
	mBackButton.Release();
	for( size_t i = 0; i < mPlayers.size(); i++ )
	{
		mPlayers[i]->button.Release();
		SAFE_DELETE( mPlayers[i] );
	}
	mPlayers.clear();
}

LobbyState::LobbyState()
{
	mPlayers = std::vector<LobbyPlayer*>( 0 );
}

LobbyState::~LobbyState()
{

}