#include "Connection.h"

void Connection::DisconnectSocket( SOCKET &socket )
{
	closesocket( socket );
	socket = INVALID_SOCKET;
}

bool Connection::Initialize()
{
	return true;
}

void Connection::Release()
{
}

Connection::Connection()
{
	mResult		= 0;
}

Connection::~Connection()
{
}
