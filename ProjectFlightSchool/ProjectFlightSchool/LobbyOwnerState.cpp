#include "LobbyOwnerState.h"

void LobbyOwnerState::HandleInput()
{
	if( mStartButton.LeftMousePressed() )
	{
		IEventPtr E1( new Event_Client_Lobby_Finished() );
		Client::GetInstance()->SendEvent( E1 );
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
}

LobbyOwnerState::~LobbyOwnerState()
{

}