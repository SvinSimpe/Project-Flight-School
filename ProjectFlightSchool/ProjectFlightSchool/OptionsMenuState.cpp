#include "OptionsMenuState.h"

void OptionsMenuState::HandleInput()
{
	if( !mInOptions )
	{
		if( mButtons.at(BACK)->LeftMousePressed() )
		{
			IEventPtr E1( new Event_Change_State( START_MENU_STATE ) );
			EventManager::GetInstance()->QueueEvent( E1 );
		}
		if( mButtons.at(FULLSCREEN)->LeftMousePressed() )
		{
			IEventPtr E1( new Event_Toggle_Fullscreen() );
			EventManager::GetInstance()->QueueEvent( E1 );
		}
		if( mButtons.at(SOUND)->LeftMousePressed() )
		{
			if( SoundBufferHandler::GetInstance()->SoundIsOn() )
			{
				SoundBufferHandler::GetInstance()->SoundOff();
			}
			else
			{
				SoundBufferHandler::GetInstance()->SoundOn();
			}
		}
		if( mButtons.at(HELP)->LeftMousePressed() )
		{
			mInOptions = true;
		}
	}
	else
	{
		if( mInOptionsBackground.LeftMousePressed() )
		{
			mInOptions = false;
		}
	}
}

HRESULT OptionsMenuState::Update( float deltaTime )
{
	BaseMenuState::Update( deltaTime );

	for( int i = 0; i < BUTTON_AMOUNT; i++ )
	{
		mButtons.at(i)->Update( deltaTime );
	}

	mInOptionsBackground.Update( deltaTime );

	return S_OK;
}

HRESULT OptionsMenuState::Render( float deltaTime )
{
	BaseMenuState::Render();

	for( int i = 0; i < BUTTON_AMOUNT; i++ )
	{
		mButtons.at(i)->Render();
	}
	
	if( mInOptions )
	{
		mInOptionsBackground.Render();
	}

	RenderManager::GetInstance()->Render();
	return S_OK;
}

void OptionsMenuState::OnEnter()
{
	BaseMenuState::OnEnter();
}

void OptionsMenuState::OnExit()
{
	BaseMenuState::OnExit();
}

void OptionsMenuState::Reset()
{
}

HRESULT OptionsMenuState::Initialize()
{
	BaseMenuState::Initialize();
	mStateType = OPTIONS_MENU_STATE;
	mButtons.reserve( BUTTON_AMOUNT );

	std::string texts[] = { "textControls", "textFullScreenOnOff", "textSoundOnOff", "textBack" }; //First Sound should be switched to Help

	float x	= (float)Input::GetInstance()->mScreenWidth  * 0.35f;
	float y	= (float)Input::GetInstance()->mScreenHeight * 0.9f;
	float w	= 200.0f;
	float h	= 200.0f;
	for( int i = 0; i < BUTTON_AMOUNT; i++ )
	{
		mButtons.push_back( new MovingButton() );
		mButtons.at(i)->Initialize( "../Content/Assets/Textures/Menu/Options_Menu_Text/" + texts[i] + ".dds", x - w * 0.5f, y - h * 0.5f, w, h );
		x += (float)Input::GetInstance()->mScreenWidth  * 0.1f;
	}

	mInOptionsBackground.Initialize( "../Content/Assets/Textures/Menu/helpOverlay_menu.dds", 0.0f, 0.0f, Input::GetInstance()->mScreenWidth, Input::GetInstance()->mScreenHeight );

	return S_OK;
}

void OptionsMenuState::Release()
{
	BaseMenuState::Release();
}

OptionsMenuState::OptionsMenuState() : BaseMenuState()
{
	bool mInOptions = false;
}

OptionsMenuState::~OptionsMenuState()
{
}
