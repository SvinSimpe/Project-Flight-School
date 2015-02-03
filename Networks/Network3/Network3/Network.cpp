#include "Network.h"

bool Network::Initialize( UINT port )
{
	mPort = port;
	return true;
}

void Network::Release()
{
	SAFE_DELETE( mNEF );
}

Network::Network()
{
	mNEF = nullptr;
}

Network::~Network()
{
}
