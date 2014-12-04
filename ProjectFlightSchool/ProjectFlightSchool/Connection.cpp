#include "Connection.h"

void Connection::DisconnectSocket( SOCKET &socket )
{
	closesocket( socket );
	socket = INVALID_SOCKET;
}

bool Connection::Initialize()
{
	mRecvBuf = new char[mRecvBufLen];
	for ( int i = 0; i < mRecvBufLen; i++ )
	{
		mRecvBuf[i] = '\0';
	}
	return true;
}

void Connection::Release()
{
}

Connection::Connection()
{
	mResult		= 0;
	mRecvBuf	= nullptr;
	mRecvBufLen = DEFAULT_BUFLEN;
}

Connection::~Connection()
{
	if ( mRecvBuf )
		delete[] mRecvBuf;
}
