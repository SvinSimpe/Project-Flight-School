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
}


Entry::~Entry()
{
}
