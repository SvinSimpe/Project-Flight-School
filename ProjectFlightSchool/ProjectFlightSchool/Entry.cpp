#include "Entry.h"

bool Entry::Initialize( float x, float y, float width, float height )
{
	mUpperLeft.x	= (UINT)x;
	mUpperLeft.y	= (UINT)y;
	mLowerRight.x	= (UINT)x + (UINT)width;
	mLowerRight.y	= (UINT)y + (UINT)height;
	mWidth			= width;
	mHeight			= height;
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
	mWidth			= 0.0f;
	mHeight			= 0.0f;
}


Entry::~Entry()
{
}
