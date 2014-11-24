#include "Package.h"

bool Package::Initialize(Head head, Body body)
{
	mHead = head;
	mBody = body;

	return true;
}

void Package::Release()
{

}

Package::Package()
{
	mHead = Head();
	mBody = Body();
}

Package::~Package()
{
}