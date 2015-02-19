#include "Button.h"

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
	if( Input::GetInstance()->IsKeyPressed( KEYS::KEYS_MOUSE_LEFT ) && mClickTimer <= 0.0f ) // Checks if left button is clicked and if the cooldown has run out
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
	if( Input::GetInstance()->IsKeyPressed( KEYS::KEYS_MOUSE_RIGHT ) && mClickTimer <= 0.0f )
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
	Image::Render();
}

bool Button::Update( float deltaTime )
{
	Image::Update( deltaTime );
	if( mClickTimer >= 0.0f )
	{
		mClickTimer -= deltaTime;
	}

	return true;
}

void Button::Initialize( std::string imgPath, float x, float y, float width, float height )
{
	Image::Initialize( imgPath, x, y, width, height );
}

void Button::Release()
{
	Image::Release();
}

Button::Button()
{
	mClickTimer = 0.1f;
}

Button::~Button()
{
}

XMFLOAT2 Button::GetPosition() const
{
	return XMFLOAT2( (float)mUpperLeft.x, (float)mUpperLeft.y );
}

void Button::SetPosition( XMFLOAT2 pos )
{
	mUpperLeft.x = (LONG)pos.x;
	mUpperLeft.y = (LONG)pos.y;
}