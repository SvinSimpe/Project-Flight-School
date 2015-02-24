#include "InGameWindow.h"

void InGameWindow::Activate()
{
	mIsActive = true;
}

void InGameWindow::DeActivate()
{
	mIsActive = false;
}

void InGameWindow::Update( float deltaTime )
{
	for( int i = 0; i < NR_OF_BUTTONS; i++ )
	{
		mButtons[i].Update( deltaTime );
	}

	int pressed = -1;

	for( int i = 0; i < NR_OF_BUTTONS; i++ )
	{
		if( mButtons[i].Pressed() )
		{
			pressed = i;
		}
	}

	switch ( pressed )
	{
	case 0:
		break;
	case 1:
		break;
	case 2:
		break;
	default:
		break;
	}
}

void InGameWindow::Render()
{
	RenderManager::GetInstance()->AddObject2dToList( mBackground, XMFLOAT2( 0.0f, 0.0f ), XMFLOAT2( (float)Input::GetInstance()->mScreenWidth, (float)Input::GetInstance()->mScreenHeight ) );

	mFont.WriteText( "Game Pause", ( Input::GetInstance()->mScreenWidth / 2 ) - mFont.GetMiddleXPoint( "Game Pause", 7.0f ), 210.0f, 7.0f );

	for( int i = 0; i < NR_OF_BUTTONS; i++ )
	{
		mButtons[i].Render( mFont );
	}
}

void InGameWindow::Release()
{
	for( int i = 0; i < NR_OF_BUTTONS; i++ )
	{
		mButtons[i].button.Release();
	}
	mFont.Release();
}

HRESULT InGameWindow::Initialize()
{
	mFont.Initialize( "../Content/Assets/GUI/Fonts/final_font/" );
	mIsActive	= false;

	HRESULT result;

	result = Graphics::GetInstance()->LoadStatic2dAsset( "../Content/Assets/Textures/Menu/ingameMenu/ingame_menu.dds", mBackground );

	float offsetY = 0.0f;
	for( int i = 0; i < NR_OF_BUTTONS; i++ )
	{
		mButtons[i].button.Initialize( "../Content/Assets/Textures/Menu/ingameMenu/ingame_menuButton.dds", 720.0f, 330.0f + offsetY, 496.0f, 124.0f );
		offsetY += 124.0f;
	}

	mButtons[0].text = "Resume";
	mButtons[1].text = "Exit To Menu";
	mButtons[2].text = "Quit";

	mButtons[0].scale = 5.7f;
	mButtons[1].scale = 4.5f;
	mButtons[2].scale = 5.7f;

	float pos = (float)( Input::GetInstance()->mScreenWidth / 2.0f ) - mFont.GetMiddleXPoint( mButtons[0].text, mButtons[0].scale ) + 8.0f; //8 because the button isn't in the middle
	mButtons[0].textPos = XMFLOAT2( pos, mButtons[0].button.GetPosition().y + 28.0f );
	pos = (float)( Input::GetInstance()->mScreenWidth / 2.0f ) - mFont.GetMiddleXPoint( mButtons[1].text, mButtons[1].scale ) + 8.0f;
	mButtons[1].textPos = XMFLOAT2( pos, mButtons[1].button.GetPosition().y + 35.0f );
	pos = (float)( Input::GetInstance()->mScreenWidth / 2.0f ) - mFont.GetMiddleXPoint( mButtons[2].text, mButtons[2].scale ) + 8.0f;
	mButtons[2].textPos = XMFLOAT2( pos, mButtons[2].button.GetPosition().y + 28.0f );

	return result;
}

InGameWindow::InGameWindow()
{

}

InGameWindow::~InGameWindow()
{

}

bool InGameWindow::IsActive() const
{
	return mIsActive;
}