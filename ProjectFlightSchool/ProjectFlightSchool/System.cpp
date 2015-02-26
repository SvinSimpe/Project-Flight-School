#include "System.h"
#include <sstream>
///////////////////////////////////////////////////////////////////////////////
//									PRIVATE
///////////////////////////////////////////////////////////////////////////////

void System::EventListener( IEventPtr newEvent )
{
	if ( newEvent->GetEventType() == Event_Toggle_Fullscreen::GUID ) // Add a remote player to the list when they connect
	{
		Graphics::GetInstance()->ToggleFullscreen();
		mFullscreen = !mFullscreen;

		InitializeFile initFile;
		initFile.fullscreen = mFullscreen;
		std::ofstream out( "init.bin", std::ios::out | std::ios::binary );
		out.write( (char*)&initFile, sizeof( InitializeFile ) );
		out.close();
	}
	else if( newEvent->GetEventType() == Event_Exit_Game::GUID )
	{
		PostQuitMessage( 0 );
	}
}

LRESULT CALLBACK System::WndProc( HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam )
{
	PAINTSTRUCT paintStruct;
	HDC			hdc;
	
	switch( message )
	{
		case WM_PAINT:
			hdc = BeginPaint( hWnd, &paintStruct );
			EndPaint( hWnd, &paintStruct );
			break;

		case WM_DESTROY:
			PostQuitMessage( 0 );
			break;

		case WM_KILLFOCUS:
			Input::GetInstance()->LostFocus();
			break;

		case WM_INPUT:
			Input::GetInstance()->RawRead( lParam ); //Returns a bool vector that represents all button presses
			break;

		default:
			return DefWindowProc( hWnd, message, wParam, lParam );
	}

	return 0;
}

HRESULT	System::Update( float deltaTime )
{
	mGame->Update( deltaTime );
	Input::GetInstance()->Update();
	return S_OK;
}

HRESULT	System::Render()
{
	mGame->Render();

	return S_OK;
}

///////////////////////////////////////////////////////////////////////////////
//									PUBLIC
///////////////////////////////////////////////////////////////////////////////

//System loop, reads windows messages and runs the application.
int	System::Run()
{
	MSG message			= { 0 };

	while( WM_QUIT != message.message )
	{
		if( PeekMessage( &message, nullptr, 0, 0, PM_REMOVE) )
		{
			TranslateMessage( &message );
			DispatchMessage( &message );
		}
		else
		{
			float deltaTime	= mTimer->GetDeltaTime();
			//float fps		= mTimer->GetFPS();

			if( deltaTime > DELTA_TIME_CAP )
				deltaTime = DELTA_TIME_CAP;

			Update( deltaTime );

			Render();
		}
	}
	return (int)message.wParam;
}

//Initializes the window and sub-applications. 
HRESULT System::Initialize( HINSTANCE hInstance, int nCmdShow )
{
	InitializeFile initFile;
	ZeroMemory( &initFile, sizeof( InitializeFile ) );
	std::ifstream in( "init.bin", std::ios::in | std::ios::binary );
	if( in )
	{
		in.read( (char*)&initFile, sizeof( InitializeFile ) );
	}
	else
	{
		initFile.fullscreen = false;
		std::ofstream out( "init.bin", std::ios::out | std::ios::binary );
		out.write( (char*)&initFile, sizeof( InitializeFile ) );
		out.close();
	}
	in.close();

	mFullscreen		= initFile.fullscreen;
	mScreenWidth	= 1920;
	mScreenHeight	= 1080;

	/////////////////////////////
	// Initialize windows-window
	/////////////////////////////
	WNDCLASS wc;
	wc.style			= CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc		= WndProc;
	wc.cbClsExtra		= 0;
	wc.cbWndExtra		= 0;
	wc.hInstance		= hInstance;
	wc.hIcon			= 0;
	wc.hCursor			= 0;
	wc.hbrBackground	= (HBRUSH)( COLOR_WINDOW + 1 );
	wc.lpszMenuName		= nullptr;
	wc.lpszClassName	= L"Project-Flight-School";

	if( !RegisterClass( &wc ) )
		return E_FAIL;

	mHInstance	= hInstance;
	RECT rect	= { 0, 0, mScreenWidth, mScreenHeight };
	AdjustWindowRect( &rect, WS_OVERLAPPEDWINDOW, false );

	int dwStyle = ( WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX | WS_MAXIMIZEBOX );

	if( !( mHWnd = CreateWindow( L"Project-Flight-School",
								L"Project-Flight-School",
								dwStyle,
								CW_USEDEFAULT,
								CW_USEDEFAULT,
								rect.right - rect.left,
								rect.bottom - rect.top,
								nullptr,
								nullptr,
								hInstance,
								nullptr ) ) )
		return E_FAIL;

	ShowWindow( mHWnd, nCmdShow );
	ShowCursor( true );

	///////////////////////////////
	// Initialize sub-applications
	///////////////////////////////
	HRESULT hr = CoInitializeEx( NULL, 0 );
    if ( FAILED( hr ) )
    {
      MessageBox( NULL, L"CoInitializeEx failed", L"Error", MB_OK );
    }

	Client::GetInstance()->Initialize();

	Graphics::GetInstance()->Initialize( mHWnd, mScreenWidth, mScreenHeight, mFullscreen );

	EventManager::GetInstance();
	EventManager::GetInstance()->AddListener( &System::EventListener, this, Event_Exit_Game::GUID );
	EventManager::GetInstance()->AddListener( &System::EventListener, this, Event_Toggle_Fullscreen::GUID );

	Input::GetInstance()->Initialize( mScreenWidth, mScreenHeight, mHWnd );

	RenderManager::GetInstance()->Initialize();
	//TestSound
	SoundBufferHandler::GetInstance()->Initialize();

	mGame = new Game();
	mGame->Initialize();

	mTimer = new Timer();
	mTimer->Initialize();

	CoUninitialize();
	
	return S_OK;
}

//Release all data used.
void System::Release()
{
	SAFE_RELEASE_DELETE( mGame );

	SAFE_RELEASE_DELETE( mTimer );

	Client::GetInstance()->Release();
	EventManager::GetInstance()->Release();
	Input::GetInstance()->Release();
	RenderManager::GetInstance()->Release();
	SoundBufferHandler::GetInstance()->Release();
	Graphics::GetInstance()->Release();
}

System::System()
{
	mHInstance		= 0;
	mHWnd			= 0;
	mScreenWidth	= 0;
	mScreenHeight	= 0;
	mGame			= nullptr;
	mTimer			= nullptr;
}

System::~System()
{
}