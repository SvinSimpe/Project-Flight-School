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

HRESULT LobbyOwnerState::Render()
{

	mStartButton.Render();
	
	HRESULT hr = LobbyState::Render( 0.0f );
	return hr;
}

void LobbyOwnerState::OnExit()
{
	LobbyState::OnExit();
	mStartButton.SetExitCooldown();
}

HRESULT LobbyOwnerState::Initialize()
{
	mStartButton.Initialize( "../Content/Assets/Textures/Menu/Create_Menu_Text/MultiPlayer.png", 1600, 700, 200, 200 );

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