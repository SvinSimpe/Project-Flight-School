#include "Head.h"

bool Head::Initialize(int socketID, Operation operation, int size)
{
	socketID		= mSocketID;
	mOperation		= operation;
	mContentSize	= size;

	return true;
}

void Head::Release()
{

}

Head::Head()
{
	mSocketID = -1;
	mOperation = Operation::NONE;
	mContentSize = 0;
}

Head::~Head()
{
}