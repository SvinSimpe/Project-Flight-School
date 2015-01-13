#include "Button.h"

bool Button::LeftMousePressed()
{
	if( Input::GetInstance()->mCurrentFrame.at(KEYS::KEYS_MOUSE_LEFT) && mCurrentCD <= 0.0f ) // Checks if left button is clicked and if the cooldown has run out
	{
		if( MouseInbounds() ) // Checks if the mouse is within bounds of the button
		{
			mCurrentCD = CLICKCOOLDOWN; // Sets cooldown to given value
			return true;
		}
	}
	return false;
}

bool Button::RightMousePressed()
{
	if( Input::GetInstance()->mCurrentFrame.at(KEYS::KEYS_MOUSE_RIGHT) && mCurrentCD <= 0.0f )
	{
		if( MouseInbounds() )
		{
			mCurrentCD = CLICKCOOLDOWN;
			return true;
		}
	}
	return false;
}

void Button::Render()
{
	Graphics::GetInstance()->Render2dAsset( m2DTexture, (float)mUpperLeft.x, (float)mUpperLeft.y, mWidth, mHeight );
}

bool Button::Update( float deltaTime )
{
	if( mCurrentCD >= 0.0f )
	{
		mCurrentCD -= deltaTime;
	}
	return true;
}

void Button::Initialize(const char* assetPath, UINT x, UINT y, UINT width, UINT height )
{
	Entry::Initialize(x, y, width, height);
	Graphics::GetInstance()->LoadStatic2dAsset( assetPath, m2DTexture );
}

void Button::Release()
{
}

Button::Button() : Entry()
{
	mCurrentCD = 0.0f;
}

Button::~Button()
{
}
