#include "MultiplayerMenuState.h"

void MultiplayerMenuState::HandleInput()
{
	if( mPortBox.LeftMousePressed() )
	{
		mPortBox.SwitchActive( true );
	}
	else if( mButtons.at(TWO_VS_TWO)->LeftMousePressed() )
	{	
		// WARNING, THE STRINGSTREAM HERE MIGHT FUCK SHIT UP
		std::string port = mPortBox.GetText();

		IEventPtr E1( new Event_Start_Server( port ) );
		EventManager::GetInstance()->QueueEvent( E1 );

		IEventPtr E2( new Event_Start_Client( "localhost", port ) );
		EventManager::GetInstance()->QueueEvent( E2 );
	}
	else if( mButtons.at(THREE_VS_THREE)->LeftMousePressed() )
	{
		std::string port = mPortBox.GetText();

		IEventPtr E1( new Event_Start_Server( port ) );
		EventManager::GetInstance()->QueueEvent( E1 );
		
		IEventPtr E2( new Event_Start_Client( "localhost", port ) );
		EventManager::GetInstance()->QueueEvent( E2 );
	}
	else if( mButtons.at(FOUR_VS_FOUR)->LeftMousePressed() )
	{
		std::string port = mPortBox.GetText();

		IEventPtr E1( new Event_Start_Server( port ) );
		EventManager::GetInstance()->QueueEvent( E1 );
				
		IEventPtr E2( new Event_Start_Client( "localhost", port ) );
		EventManager::GetInstance()->QueueEvent( E2 );
	}
	else if( mButtons.at(BACK)->LeftMousePressed() )
	{
		IEventPtr E1( new Event_Change_State( CREATE_MENU_STATE ) );
		EventManager::GetInstance()->QueueEvent( E1 );
	}
}

HRESULT MultiplayerMenuState::Update( float deltaTime )
{
	BaseMenuState::Update( deltaTime );
	for( int i = 0; i < BUTTON_AMOUNT; i++ )
	{
		mButtons.at(i)->Update( deltaTime );
	}
	mPortBox.Update( deltaTime );
	return S_OK;
}

HRESULT MultiplayerMenuState::Render()
{
	BaseMenuState::Render();

	for( int i = 0; i < BUTTON_AMOUNT; i++ )
	{
		mButtons.at(i)->Render();
	}
	mPortBox.Render();
	RenderManager::GetInstance()->AddObject2dToList( mCursor, XMFLOAT2( (float)Input::GetInstance()->mCurrentMousePos.x, (float)Input::GetInstance()->mCurrentMousePos.y ), DirectX::XMFLOAT2( 20.0f, 20.0f ) );
	RenderManager::GetInstance()->Render();
	return S_OK;
}

void MultiplayerMenuState::OnEnter()
{
}

void MultiplayerMenuState::OnExit()
{
	BaseMenuState::OnExit();
}

void MultiplayerMenuState::Reset()
{
	mPortBox.Release();

	float x = (float)Input::GetInstance()->mScreenWidth  * 0.2f;
	float y = (float)Input::GetInstance()->mScreenHeight * 0.9f;
	float w = 640.0f * 0.5f;
	float h = 177.0f * 0.5f;

	mPortBox.Initialize( "27015", "Port", x - w * 0.5f, y - h * 0.5f, w, h );
}

HRESULT MultiplayerMenuState::Initialize()
{
	BaseMenuState::Initialize();
	mStateType = MULTI_MENU_STATE;
	mButtons.reserve( BUTTON_AMOUNT );

	std::string texts[] = { "2vs2", "3vs3", "4vs4", "Back" };

	float x = (float)Input::GetInstance()->mScreenWidth  * 0.1f;
	float y = (float)Input::GetInstance()->mScreenHeight * 0.9f;
	float w = 640.0f/2;
	float h = 177.0f/2;

	mPortBox.Initialize( "27015", "Port", x - w * 0.5f, y - h * 0.5f, w, h );

	x	= (float)Input::GetInstance()->mScreenWidth  * 0.35f;
	y	= (float)Input::GetInstance()->mScreenHeight * 0.9f;
	w	= 200.0f;
	h	= 200.0f;

	for( int i = 0; i < BUTTON_AMOUNT; i++ )
	{
		mButtons.push_back( new MovingButton() );
		if( texts[i] == "Back" )
		{
			mButtons.at(i)->Initialize( "../Content/Assets/Textures/Menu/Back.png", x - w * 0.5f, y - h * 0.5f, w, h );
		}
		else
		{
			mButtons.at(i)->Initialize( "../Content/Assets/Textures/Menu/Multi_Menu_Text/" + texts[i] + ".png", x - w * 0.5f, y - h * 0.5f, w, h );
		}
		x += (float)Input::GetInstance()->mScreenWidth  * 0.1f;
	}

	return S_OK;
}

void MultiplayerMenuState::Release()
{
	BaseMenuState::Release();
	mPortBox.Release();
}

MultiplayerMenuState::MultiplayerMenuState() : BaseMenuState()
{
}

MultiplayerMenuState::~MultiplayerMenuState()
{
}
