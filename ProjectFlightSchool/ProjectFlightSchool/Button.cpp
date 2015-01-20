#include "Button.h"

void Button::SpinCircle()
{
	mFrames[mFrameIndex].active = false;

	mFrameIndex++;
	if( mFrameIndex >= FRAME_AMOUNT )
		mFrameIndex = 0;

	mFrames[mFrameIndex].active = true;

	mSpinTimer = 0.0f;
}

bool Button::MouseInbounds()
{
	POINT mousePos = Input::GetInstance()->mCurrentMousePos;

	if( mousePos.x >= mUpperLeft.x && mousePos.y >= mUpperLeft.y && 
		mousePos.x <= mLowerRight.x && mousePos.y <= mLowerRight.y )
	{
		return true;
	}
	return false;
}

bool Button::LeftMousePressed()
{
	if( Input::GetInstance()->mCurrentFrame.at(KEYS::KEYS_MOUSE_LEFT) && mClickTimer <= 0.0f ) // Checks if left button is clicked and if the cooldown has run out
	{
		if( MouseInbounds() ) // Checks if the mouse is within bounds of the button
		{
			mClickTimer = CLICKCOOLDOWN; // Sets cooldown to given value
			return true;
		}
	}
	return false;
}

bool Button::RightMousePressed()
{
	if( Input::GetInstance()->mCurrentFrame.at(KEYS::KEYS_MOUSE_RIGHT) && mClickTimer <= 0.0f )
	{
		if( MouseInbounds() )
		{
			mClickTimer = CLICKCOOLDOWN;
			return true;
		}
	}
	return false;
}

void Button::SetExitCooldown()
{
	mClickTimer = 0.1f;
}

void Button::Render()
{
	for( auto& it : mFrames )
	{
		if( it.active )
			RenderManager::GetInstance()->AddObject2dToList( it.asset, DirectX::XMFLOAT2( (float)mUpperLeft.x, (float)mUpperLeft.y ), DirectX::XMFLOAT2( mWidth, mHeight ) );
	}
	Image::Render();
}

bool Button::Update( float deltaTime )
{
	Image::Update( deltaTime );
	if( mClickTimer >= 0.0f )
	{
		mClickTimer -= deltaTime;
	}

	mSpinTimer += deltaTime;
	if( mSpinTimer >= SPIN_COOLDOWN )
	{
		SpinCircle();
	}

	return true;
}

void Button::Initialize( std::string imgPath, float x, float y, float width, float height )
{
	Image::Initialize( imgPath, x, y, width, height );

	std::string path = "../Content/Assets/Textures/Menu/Menu_Button/";

	for( int i = 0; i < FRAME_AMOUNT; i++ )
	{
		int index = i + 1;
		std::string itostring = std::to_string( index );
		Graphics::GetInstance()->LoadStatic2dAsset( path + "menuRing." + itostring + ".png", mFrames[i].asset );
	}
}

void Button::Release()
{
	Image::Release();
}

Button::Button()
{
	mSpinTimer	= 0.0f;
	mFrameIndex	= 0;
	mClickTimer = 0.1f;
}

Button::~Button()
{
}
