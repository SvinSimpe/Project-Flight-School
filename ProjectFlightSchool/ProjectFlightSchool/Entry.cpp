#include "Entry.h"

bool Entry::LeftMousePressed()
{
	POINT mousePos = Input::GetInstance()->mCurrentMousePos;
	bool leftDown = Input::GetInstance()->mCurrentFrame[KEYS::KEYS_MOUSE_LEFT];

	if( leftDown && mCurrentCD <= 0.0f && mousePos.x >= mUpperLeft.x && mousePos.y >= mUpperLeft.y && 
		mousePos.x <= mLowerRight.x && mousePos.y <= mLowerRight.y )
	{
		mCurrentCD = CLICKCOOLDOWN;
		return true;
	}
	return false;
}

bool Entry::RightMousePressed()
{
	POINT mousePos = Input::GetInstance()->mCurrentMousePos;
	bool rightDown = Input::GetInstance()->mCurrentFrame[KEYS::KEYS_MOUSE_RIGHT];

	if( rightDown && mCurrentCD <= 0.0f && mousePos.x >= mUpperLeft.x && mousePos.y >= mUpperLeft.y && 
		mousePos.x <= mLowerRight.x && mousePos.y <= mLowerRight.y )
	{
		mCurrentCD = CLICKCOOLDOWN;
		return true;
	}
	return false;
}

void Entry::Render()
{
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
	mCurrentCD		= 0.0f;
}


Entry::~Entry()
{
}
