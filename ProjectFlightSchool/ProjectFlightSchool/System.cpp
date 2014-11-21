#include "System.h"
///////////////////////////////////////////////////////////////////////////////
//									PRIVATE
///////////////////////////////////////////////////////////////////////////////

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

		case WM_KEYDOWN:
			switch( wParam )
			{
				case VK_ESCAPE:
					PostQuitMessage( 0 );
					break;
			}

		default:
			return DefWindowProc( hWnd, message, wParam, lParam );
	}

	return 0;
}

HRESULT	System::Update( float deltaTime )
{
	return S_OK;
}

HRESULT	System::Render()
{
	Graphics::GetInstance()->BeginScene();
	Graphics::GetInstance()->RenderStatic3dAsset( mPlaneAsset );
	Graphics::GetInstance()->RenderStatic3dAsset( mCubeAsset );
	Graphics::GetInstance()->EndScene();
	return S_OK;
}

///////////////////////////////////////////////////////////////////////////////
//									PUBLIC
///////////////////////////////////////////////////////////////////////////////

//System loop, reads windows messages and runs the application.
int	System::Run()
{
	MSG message = { 0 };
	while( WM_QUIT != message.message )
	{
		if( PeekMessage( &message, nullptr, 0, 0, PM_REMOVE) )
		{
			TranslateMessage( &message );
			DispatchMessage( &message );
		}
		else
		{
			Update( 0.0f );
			Render();
		}
	}

	return (int)message.wParam;
}

//Initializes the window and sub-applications. 
HRESULT System::Initialize( HINSTANCE hInstance, int nCmdShow )
{
	mScreenWidth	= 1280;
	mScreenHeight	= 780;

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

	if( !( mHWnd = CreateWindow( L"Project-Flight-School",
								L"Project-Flight-School",
								WS_OVERLAPPEDWINDOW,
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

	Graphics::GetInstance()->Initialize( mHWnd, mScreenWidth, mScreenHeight );

	Graphics::GetInstance()->Graphics::LoadStatic3dAsset( "PLANE", mPlaneAsset );
	Graphics::GetInstance()->Graphics::LoadStatic3dAsset( "CUBE", mCubeAsset );

	return S_OK;
}

//Release all data used.
void System::Release()
{
	Graphics::GetInstance()->Release();
}

System::System()
{
	mHInstance		= 0;
	mHWnd			= 0;
	mScreenWidth	= 0;
	mScreenHeight	= 0;
}

System::~System()
{

}