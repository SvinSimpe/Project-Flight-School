#include "StartMenuState.h"

///////////////////////////////////////////////////////////////////////////////
//									PRIVATE
///////////////////////////////////////////////////////////////////////////////

void StartMenuState::HandleInput()
{
	if( mButtons.at(CREATE)->LeftMousePressed() )
	{
		IEventPtr E1( new Event_Change_State( MULTI_MENU_STATE ) );
		EventManager::GetInstance()->QueueEvent( E1 );
	}
	else if( mButtons.at(JOIN)->LeftMousePressed() )
	{
		IEventPtr E1( new Event_Change_State( JOIN_MENU_STATE ) );
		EventManager::GetInstance()->QueueEvent( E1 );
	}
	else if( mButtons.at(OPTIONS)->LeftMousePressed() )
	{
		IEventPtr E1( new Event_Change_State( OPTIONS_MENU_STATE ) );
		EventManager::GetInstance()->QueueEvent( E1 );
	}
	else if( mButtons.at(EXIT)->LeftMousePressed() )
	{
		IEventPtr E1( new Event_Exit_Game() );
		EventManager::GetInstance()->QueueEvent( E1 );
	}
}


///////////////////////////////////////////////////////////////////////////////
//									PUBLIC
///////////////////////////////////////////////////////////////////////////////

HRESULT StartMenuState::Update( float deltaTime )
{
	BaseMenuState::Update( deltaTime );
	for( int i = 0; i < BUTTON_AMOUNT; i++ )
	{
		mButtons.at(i)->Update( deltaTime );
	}
	return S_OK;
}

HRESULT StartMenuState::Render( float deltaTime )
{
	BaseMenuState::Render();

	for( int i = 0; i < BUTTON_AMOUNT; i++ )
	{
		mButtons.at(i)->Render();
	}

	RenderManager::GetInstance()->Render();
	return S_OK;
}

void StartMenuState::OnEnter()
{
	BaseMenuState::OnEnter();
	SoundBufferHandler::GetInstance()->Loop( mStreamSoundAsset );
}

void StartMenuState::OnExit()
{
	BaseMenuState::OnExit();
}

void StartMenuState::Reset()
{
}

HRESULT StartMenuState::Initialize()
{
	BaseMenuState::Initialize();
	mStateType		= START_MENU_STATE;
	mButtons.reserve( BUTTON_AMOUNT );

	std::string texts[] = { "textCreate", "textJoin", "textOptions", "textExit" };

	float x	= (float)Input::GetInstance()->mScreenWidth  * 0.35f;
	float y	= (float)Input::GetInstance()->mScreenHeight * 0.9f;
	float w	= 200.0f;
	float h	= 200.0f;
	for(int i = 0; i < BUTTON_AMOUNT; i++)
	{
		mButtons.push_back( new MovingButton() );
		mButtons.at(i)->Initialize( "../Content/Assets/Textures/Menu/Start_Menu_Text/" + texts[i] + ".dds", x - w * 0.5f , y - h * 0.5f, w, h );
		x += (float)Input::GetInstance()->mScreenWidth * 0.1f;
	}
	mStreamSoundAsset = SoundBufferHandler::GetInstance()->LoadBuffer( "../Content/Assets/Sound/ambient_menu.wav" );

	return S_OK;
}

void StartMenuState::Release()
{
	BaseMenuState::Release();
}

StartMenuState::StartMenuState() : BaseMenuState()
{
}

StartMenuState::~StartMenuState()
{
}
