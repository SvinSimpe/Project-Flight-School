#include "Network.h"

bool Network::Initialize( UINT port )
{
	mPort = port;
	return true;
}

Network::Network()
{
	mPort = 0;
}

Network::~Network()
{
}