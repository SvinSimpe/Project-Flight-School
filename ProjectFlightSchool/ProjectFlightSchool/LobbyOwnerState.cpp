#include "LobbyOwnerState.h"

void LobbyOwnerState::EventListener( IEventPtr eventPtr )
{
	if( eventPtr->GetEventType() == Event_Start_Server::GUID )
	{
		std::shared_ptr<Event_Start_Server> data = std::static_pointer_cast<Event_Start_Server>( eventPtr );
		mMaxPlayers = data->MaxPlayers();
		std::ostringstream out;
		out << "Max players: " << mMaxPlayers << "\n";
		OutputDebugStringA( out.str().c_str() );
	}
}

void LobbyOwnerState::ManageStartButton()
{
	// check here if all players are ready
	bool allReady = true;
	for( size_t i = 0; i < mPlayers.size(); i++ )
	{
		if( !mPlayers[i]->isReady )
		{
			allReady = false;
			mWarningTexts[ALL_READY].timer = MAX_TEXT_TIME;
			break;
		}
	}

	// check here if game is full
	//bool gameFull = mPlayers.size() == mMaxPlayers ? true : false;

	bool gameFull = true; // dev stuff yo
	if( !gameFull )
	{
		mWarningTexts[FULL_GAME].timer = MAX_TEXT_TIME;
	}
	else
	{
		// check here if teams are even
		std::vector<LobbyPlayer*> team1 = std::vector<LobbyPlayer*>();
		std::vector<LobbyPlayer*> team2 = std::vector<LobbyPlayer*>();
		for( auto& p : mPlayers )
		{
			if( p->team == 1 )
			{
				team1.push_back( p );
			}
			else if( p->team == 2 )
			{
				team2.push_back( p );
			}
		}
		if( team1.size() > team2.size() )
		{
			BalanceTeams( team1, team2 );
			mWarningTexts[BALANCE_TEAMS].timer = MAX_TEXT_TIME;
		}
		else if( team2.size() > team1.size() )
		{
			BalanceTeams( team2, team1 );
			mWarningTexts[BALANCE_TEAMS].timer = MAX_TEXT_TIME;
		}
		team1.clear();
		team2.clear();
	}

	if( allReady 
		&& gameFull
		&& !mGameCountdownStarted )
	{
		IEventPtr E1( new Event_Host_Start_Game_Countdown() );
		Client::GetInstance()->SendEvent( E1 );
	}
}

void LobbyOwnerState::BalanceTeams( std::vector<LobbyPlayer*> biggerTeam, std::vector<LobbyPlayer*> smallerTeam )
{
	size_t bSize = biggerTeam.size();
	size_t sSize = smallerTeam.size();
	while( bSize != sSize )
	{
		LobbyPlayer* toMove = biggerTeam.back();
		bSize--;
		sSize++;
		for( auto& p : mPlayers )
		{
			if( p == toMove )
			{
				if( p->team == 1 )
				{
					p->team = 2;
				}
				else if( p->team == 2 )
				{
					p->team = 1;
				}
				IEventPtr E1( new Event_Client_Switch_Team( p->ID, p->team ) );
				Client::GetInstance()->SendEvent( E1 );
				break;
			}
		}
		if( bSize <= 1 ) // won't be needed in the finished game
		{
			break;
		}
	}
}

void LobbyOwnerState::HandleInput()
{
	if( mStartButton.LeftMousePressed() )
	{
		ManageStartButton();
	}
}

HRESULT LobbyOwnerState::Update( float deltaTime )
{
	HRESULT hr = LobbyState::Update( deltaTime );

	mStartButton.Update( deltaTime );
	
	HandleInput();

	if( mGameCountdown < 0.0f && mGameCountdownStarted )
	{
		IEventPtr E1( new Event_Client_Lobby_Finished() );
		Client::GetInstance()->SendEvent( E1 );
	}

	return hr;
}

HRESULT LobbyOwnerState::Render( float deltaTime )
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
			mReadyImg.Render( p->button.GetPosition().x + 265.0f, p->button.GetPosition().y + 7.0f, 50.0f, 50.0f );
			if( p->thisPlayer )
			{
				mReadyImg.Render();
			}
		}
	}

	mBackButton.Render();
	mCheckBox.Render();
	mChooseWeaponButton.Render();
	mChooseWeaponText.Render();
	mStartButton.Render();

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

	for( int i = 0; i < WARNING_AMOUNT; i++ )
	{
		if( mWarningTexts[i].timer > 0.0f )
		{
			float offset = mFont.GetMiddleXPoint( mWarningTexts[i].text, 3.0f ); 
			float textShadowWidth = 1.0f;
			mFont.WriteText( mWarningTexts[i].text, (float)(Input::GetInstance()->mScreenWidth) * 0.5f - offset + textShadowWidth, 570.0f + textShadowWidth + ((LETTER_HEIGHT * 3.0f) * i), 3.0f, DirectX::XMFLOAT4( 0.0f, 0.0f, 0.0f, 1.0f ) );
			mFont.WriteText( mWarningTexts[i].text, (float)(Input::GetInstance()->mScreenWidth) * 0.5f - offset - textShadowWidth, 570.0f + textShadowWidth + ((LETTER_HEIGHT * 3.0f) * i), 3.0f, DirectX::XMFLOAT4( 0.0f, 0.0f, 0.0f, 1.0f ) );
			mFont.WriteText( mWarningTexts[i].text, (float)(Input::GetInstance()->mScreenWidth) * 0.5f - offset + textShadowWidth, 570.0f - textShadowWidth + ((LETTER_HEIGHT * 3.0f) * i), 3.0f, DirectX::XMFLOAT4( 0.0f, 0.0f, 0.0f, 1.0f ) );
			mFont.WriteText( mWarningTexts[i].text, (float)(Input::GetInstance()->mScreenWidth) * 0.5f - offset - textShadowWidth, 570.0f - textShadowWidth + ((LETTER_HEIGHT * 3.0f) * i), 3.0f, DirectX::XMFLOAT4( 0.0f, 0.0f, 0.0f, 1.0f ) );

			mFont.WriteText( mWarningTexts[i].text, (float)(Input::GetInstance()->mScreenWidth) * 0.5f - offset, 570.0f + ((LETTER_HEIGHT * 3.0f) * i), 3.0f, COLOR_ORANGE );
		}
		mWarningTexts[i].timer -= deltaTime;
	}
	mCurrentTextTime -= deltaTime;

	RenderManager::GetInstance()->Render();

	return hr;
}

void LobbyOwnerState::OnExit()
{
	LobbyState::OnExit();
	mStartButton.SetExitCooldown();
	for( int i = 0; i < WARNING_AMOUNT; i++ )
	{
		mWarningTexts[i].timer = -1.0f;
	}
}

HRESULT LobbyOwnerState::Initialize()
{
	mStartButton.Initialize( "../Content/Assets/Textures/Menu/lobby_loadout_menu/startText.dds", 1665.0f, 810.0f, 200.0f, 200.0f );
	mWarningTexts[ALL_READY].text = "Every player has to be ready before starting game!";
	mWarningTexts[FULL_GAME].text = "Server has to be full before starting game!";
	mWarningTexts[BALANCE_TEAMS].text = "Teams balanced";

	EventManager::GetInstance()->AddListener( &LobbyOwnerState::EventListener, this, Event_Start_Server::GUID );

	return LobbyState::Initialize();
}

void LobbyOwnerState::Release()
{
	LobbyState::Release();
	mStartButton.Release();
}

LobbyOwnerState::LobbyOwnerState()
{
	LobbyState::LobbyState();
	mCurrentTextTime	= 0.0f;
	mMaxPlayers			= (UINT)-1;
}

LobbyOwnerState::~LobbyOwnerState()
{

}