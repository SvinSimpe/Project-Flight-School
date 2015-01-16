#include "Entry.h"

bool Entry::MouseInbounds()
{
	POINT mousePos = Input::GetInstance()->mCurrentMousePos;

	if( mousePos.x >= mUpperLeft.x && mousePos.y >= mUpperLeft.y && 
		mousePos.x <= mLowerRight.x && mousePos.y <= mLowerRight.y )
	{
		return true;
	}
	return false;
}

bool Entry::LeftMousePressed()
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

bool Entry::RightMousePressed()
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

void Entry::SetExitCooldown()
{
	mCurrentCD = 0.1f;
}

bool Entry::Update( float deltaTime )
{
	if( mCurrentCD >= 0.0f )
	{
		mCurrentCD -= deltaTime;
	}
	return true;
}

bool Entry::Initialize( UINT x, UINT y, UINT width, UINT height )
{
	mUpperLeft.x	= x;
	mUpperLeft.y	= y;
	mLowerRight.x	= x + width;
	mLowerRight.y	= y + height;
	mWidth			= (float)width;
	mHeight			= (float)height;
	return true;
}

void Entry::Release()
{
}

Entry::Entry()
{
	mUpperLeft.x	= 0;
	mUpperLeft.y	= 0;
	mLowerRight.x	= 0;
	mLowerRight.y	= 0;
	mCurrentCD		= 0.1f;
}


Entry::~Entry()
{
}
