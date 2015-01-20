#include "Entry.h"

bool Entry::Initialize( float x, float y, float width, float height )
{
	mUpperLeft.x	= (UINT)x;
	mUpperLeft.y	= (UINT)y;
	mLowerRight.x	= (UINT)x + width;
	mLowerRight.y	= (UINT)y + height;
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
}


Entry::~Entry()
{
}
