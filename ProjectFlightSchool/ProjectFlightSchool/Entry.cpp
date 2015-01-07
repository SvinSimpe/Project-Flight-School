#include "Entry.h"

bool Entry::IsPressed()
{
	POINT mousePos = Input::GetInstance()->mCurrentMousePos;
	bool leftDown = Input::GetInstance()->mCurrentFrame[KEYS::KEYS_MOUSE_LEFT];

	if( leftDown && mousePos.x >= mUpperLeft.x && mousePos.y >= mUpperLeft.y && 
		mousePos.x <= mLowerRight.x && mousePos.y <= mLowerRight.y )
	{
		return true;
	}
	return false;
}

void Entry::Render()
{
}

bool Entry::Update( float deltaTime )
{
	if(IsPressed())
	{
		printf("%f: Hit!\n", deltaTime);
	}
	return true;
}

bool Entry::Initialize( UINT x, UINT y, UINT width, UINT height )
{
	mUpperLeft.x	= x;
	mUpperLeft.y	= y;
	mLowerRight.x	= x + width;
	mLowerRight.y	= y + height;
	printf( "UpperLeft: %d, %d\nLowerRight: %d, %d\n", mUpperLeft.x, mUpperLeft.y, mLowerRight.x, mLowerRight.y );
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
}


Entry::~Entry()
{
}
