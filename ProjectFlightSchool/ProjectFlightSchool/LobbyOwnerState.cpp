#include "LobbyOwnerState.h"

void LobbyOwnerState::HandleInput()
{
	if( mStartButton.LeftMousePressed() )
	{
		// check here if teams are even

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

		if( allReady )
		{
			IEventPtr E1( new Event_Client_Lobby_Finished() );
			Client::GetInstance()->SendEvent( E1 );
		}
	}
}

HRESULT LobbyOwnerState::Update( float deltaTime )
{
	HRESULT hr = LobbyState::Update( deltaTime );

	mStartButton.Update( deltaTime );
	
	HandleInput();

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
		
		mFont.WriteText( textToWrite, p->button.GetPosition().x + 20.0f, p->button.GetPosition().y + 15.0f, 3.0f );
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
	mStartButton.Render();

	if( mLoadOutMenu.IsActive() )
	{
		mLoadOutMenu.Render();
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
}

HRESULT LobbyOwnerState::Initialize()
{
	mStartButton.Initialize( "../Content/Assets/Textures/Menu/lobby_loadout_menu/startText.dds", 1665.0f, 760.0f, 200.0f, 200.0f );
	mWarningTexts[EVEN_TEAMS].text = "Teams has to be even before starting game!";
	mWarningTexts[ALL_READY].text = "Every player has to be ready before starting game!";
	mWarningTexts[FULL_GAME].text = "Server has to be full before starting game!";

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
}

LobbyOwnerState::~LobbyOwnerState()
{

}