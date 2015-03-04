#include "EndGameWindow.h"

void EndGameWindow::Update( float deltaTime, bool wonGame )
{
	mEndGameButton.Update( deltaTime );

	if( mEndGameButton.IsPressed() )
	{
		IEventPtr E1( new Event_Reset_Game() );
		EventManager::GetInstance()->QueueEvent( E1 );
	}

	mWonGame = wonGame;
}

void EndGameWindow::Render()
{
	if( mWonGame )
	{
		RenderManager::GetInstance()->AddObject2dToList( mWinScreen, XMFLOAT2( 0.0f, 0.0f ), XMFLOAT2( (float)Input::GetInstance()->mScreenWidth, (float)Input::GetInstance()->mScreenHeight ) );
	}
	else
	{
		RenderManager::GetInstance()->AddObject2dToList( mLoseScreen, XMFLOAT2( 0.0f, 0.0f ), XMFLOAT2( (float)Input::GetInstance()->mScreenWidth, (float)Input::GetInstance()->mScreenHeight ) );
	}

	mEndGameButton.Render( mFont );
}

HRESULT	EndGameWindow::Initialize()
{
	HRESULT hr = S_OK;
	hr = Graphics::GetInstance()->LoadStatic2dAsset( "../Content/Assets/GUI/EndGame/winScreen.dds", mWinScreen );
	if( FAILED( hr ) )
	{
		return hr;
	}

	hr = Graphics::GetInstance()->LoadStatic2dAsset( "../Content/Assets/GUI/EndGame/loseScreen.dds", mLoseScreen );
	if( FAILED( hr ) )
	{
		return hr;
	}

	mFont.Initialize( "../Content/Assets/GUI/Fonts/final_font/" );

	mEndGameButton.mButton.Initialize( "../Content/Assets/Textures/Menu/ingameMenu/ingame_menuButton.dds", 720.0f, 780.0f, 496.0f, 124.0f );
	
	mEndGameButton.mButtonTextScale = 5.7f;
	
	float pos = (float)( Input::GetInstance()->mScreenWidth / 2.0f ) - mFont.GetMiddleXPoint( "END GAME", mEndGameButton.mButtonTextScale ) + 8.0f; //8 because the button isn't in the middle
	mEndGameButton.mButtonTextPosition = XMFLOAT2( pos, mEndGameButton.mButton.GetPosition().y + 28.0f );

	mWonGame = false; //Default init will be false and then changed to either true/false when this event occurs
	

	return hr;
}

void EndGameWindow::Release()
{
	mEndGameButton.mButton.Release();

	mFont.Release();
}

EndGameWindow::EndGameWindow()
{
	mWinScreen	= (UINT)-1;
	mLoseScreen = (UINT)-1;
	mWonGame	= false;
}

EndGameWindow::~EndGameWindow()
{

}
