#include "InGameWindow.h"

void InGameWindow::Activate()
{
	mIsActive = true;
}

void InGameWindow::DeActivate()
{
	mIsActive	= false;
	mInOptions	= false;
}

void InGameWindow::Update( float deltaTime )
{
	if( !mInOptions )
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
		{
			DeActivate();
			break;
		}
		case 1:
		{
			mInOptions	= true;
			break;
		}
		case 2:
		{
			DeActivate();
			IEventPtr E1( new Event_Reset_Game() );
			EventManager::GetInstance()->QueueEvent( E1 );
			break;
		}
		case 3:
		{
			DeActivate();
			IEventPtr E1( new Event_Exit_Game() );
			EventManager::GetInstance()->QueueEvent( E1 );
			break;
		}
		default:
			break;
		}
	}
	else
	{
		for( int i = 0; i < NR_OF_OPTION_BUTTONS; i++ )
		{
			mOptionButtons[i].Update( deltaTime );
		}

		int pressed = -1;

		for( int i = 0; i < NR_OF_OPTION_BUTTONS; i++ )
		{
			if( mOptionButtons[i].Pressed() )
			{
				pressed = i;
			}
		}

		switch ( pressed )
		{
		case 0:
		{
			mInOptions	= false;
			break;
		}
		case 1:
		{
			if( SoundBufferHandler::GetInstance()->SoundIsOn() )
				SoundBufferHandler::GetInstance()->SoundOff();

			else
				SoundBufferHandler::GetInstance()->SoundOn();

			break;
		}
		case 2:
		{
			IEventPtr E1( new Event_Toggle_Fullscreen() );
			EventManager::GetInstance()->QueueEvent( E1 );
			break;
		}
		default:
			break;
		}
	}
}

void InGameWindow::Render()
{
	RenderManager::GetInstance()->AddObject2dToList( mBackground, XMFLOAT2( 0.0f, 0.0f ), XMFLOAT2( (float)Input::GetInstance()->mScreenWidth, (float)Input::GetInstance()->mScreenHeight ) );

	if( !mInOptions )
	{
		mFont.WriteText( "Game Pause", ( Input::GetInstance()->mScreenWidth / 2 ) - mFont.GetMiddleXPoint( "Game Pause", 7.0f ), 210.0f, 7.0f );

		for( int i = 0; i < NR_OF_BUTTONS; i++ )
		{
			mButtons[i].Render( mFont );
		}
	}
	else
	{
		mFont.WriteText( "Options", ( Input::GetInstance()->mScreenWidth / 2 ) - mFont.GetMiddleXPoint( "Options", 7.0f ), 270.0f, 7.0f );

		for( int i = 0; i < NR_OF_OPTION_BUTTONS; i++ )
		{
			mOptionButtons[i].Render( mFont );
		}
	}
}

void InGameWindow::Release()
{
	for( int i = 0; i < NR_OF_BUTTONS; i++ )
	{
		mButtons[i].button.Release();
	}
	for( int i = 0; i < NR_OF_OPTION_BUTTONS; i++ )
	{
		mOptionButtons[i].button.Release();
	}
	mFont.Release();
}

HRESULT InGameWindow::Initialize()
{
	mFont.Initialize( "../Content/Assets/GUI/Fonts/final_font/" );
	mIsActive	= false;
	mInOptions	= false;

	HRESULT result;

	result = Graphics::GetInstance()->LoadStatic2dAsset( "../Content/Assets/Textures/Menu/ingameMenu/ingame_menu.dds", mBackground );

	float offsetY = 0.0f;
	for( int i = 0; i < NR_OF_BUTTONS; i++ )
	{
		mButtons[i].button.Initialize( "../Content/Assets/Textures/Menu/ingameMenu/ingame_menuButton.dds", 720.0f, 330.0f + offsetY, 496.0f, 124.0f );
		offsetY += 124.0f;
	}

	mButtons[0].text = "Resume";
	mButtons[1].text = "Options";
	mButtons[2].text = "Exit To Menu";
	mButtons[3].text = "Quit";

	mButtons[0].scale = 5.7f;
	mButtons[1].scale = 5.7f;
	mButtons[2].scale = 4.5f;
	mButtons[3].scale = 5.7f;

	float pos = (float)( Input::GetInstance()->mScreenWidth / 2.0f ) - mFont.GetMiddleXPoint( mButtons[0].text, mButtons[0].scale ) + 8.0f; //8 because the button isn't in the middle
	mButtons[0].textPos = XMFLOAT2( pos, mButtons[0].button.GetPosition().y + 28.0f );
	pos = (float)( Input::GetInstance()->mScreenWidth / 2.0f ) - mFont.GetMiddleXPoint( mButtons[1].text, mButtons[1].scale ) + 8.0f;
	mButtons[1].textPos = XMFLOAT2( pos, mButtons[1].button.GetPosition().y + 28.0f );
	pos = (float)( Input::GetInstance()->mScreenWidth / 2.0f ) - mFont.GetMiddleXPoint( mButtons[2].text, mButtons[2].scale ) + 8.0f;
	mButtons[2].textPos = XMFLOAT2( pos, mButtons[2].button.GetPosition().y + 35.0f );
	pos = (float)( Input::GetInstance()->mScreenWidth / 2.0f ) - mFont.GetMiddleXPoint( mButtons[3].text, mButtons[3].scale ) + 8.0f;
	mButtons[3].textPos = XMFLOAT2( pos, mButtons[3].button.GetPosition().y + 28.0f );



	offsetY = 0.0f;
	for( int i = 0; i < NR_OF_OPTION_BUTTONS; i++ )
	{
		mOptionButtons[i].button.Initialize( "../Content/Assets/Textures/Menu/ingameMenu/ingame_menuButton.dds", 720.0f, 380.0f + offsetY, 496.0f, 124.0f );
		offsetY += 124.0f;
	}

	mOptionButtons[0].text = "Back";
	mOptionButtons[1].text = "Sound";
	mOptionButtons[2].text = "Fullscreen";

	mOptionButtons[0].scale = 5.7f;
	mOptionButtons[1].scale = 5.7f;
	mOptionButtons[2].scale = 5.7f;

	pos = (float)( Input::GetInstance()->mScreenWidth / 2.0f ) - mFont.GetMiddleXPoint( mOptionButtons[0].text, mOptionButtons[0].scale ) + 8.0f; //8 because the button isn't in the middle
	mOptionButtons[0].textPos = XMFLOAT2( pos, mOptionButtons[0].button.GetPosition().y + 28.0f );
	pos = (float)( Input::GetInstance()->mScreenWidth / 2.0f ) - mFont.GetMiddleXPoint( mOptionButtons[1].text, mOptionButtons[1].scale ) + 8.0f;
	mOptionButtons[1].textPos = XMFLOAT2( pos, mOptionButtons[1].button.GetPosition().y + 28.0f );
	pos = (float)( Input::GetInstance()->mScreenWidth / 2.0f ) - mFont.GetMiddleXPoint( mOptionButtons[2].text, mOptionButtons[2].scale ) + 8.0f;
	mOptionButtons[2].textPos = XMFLOAT2( pos, mOptionButtons[2].button.GetPosition().y + 35.0f );

	return result;
}

InGameWindow::InGameWindow()
{
	mIsActive	= false;
	mInOptions	= false;
	mBackground	= 0;
}

InGameWindow::~InGameWindow()
{

}

bool InGameWindow::IsActive() const
{
	return mIsActive;
}