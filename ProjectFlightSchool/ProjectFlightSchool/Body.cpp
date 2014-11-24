#include "Body.h"

bool Body::Initialize(char* content)
{
	mContent = content;
	if (!mContent)
	{
		return false;
	}

	return true;
}

void Body::Release()
{
	if (mContent)
		delete mContent;
}

Body::Body()
{
	mContent = nullptr;
}

Body::~Body()
{

}