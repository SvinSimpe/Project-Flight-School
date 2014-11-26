#include "Connection.h"

bool Connection::SendMsg( SOCKET &to, char* msg )
{
	Message body;
	body.msg = "Fisk";
	msg = Pack(body, 0);
	mResult = send( to, msg, sizeof(Package), 0 );
	if ( mResult == SOCKET_ERROR )
	{
		printf( "send failed when sending to %d with error: %d\n", to, WSAGetLastError() );
		DisconnectSocket( to );
		return false;
	}
	delete msg;
	return true;
}

Package Connection::Unpack(char* package)
{
	Package p;
	CharPtrToStruct(&p, package);
	return p;
}

bool Connection::DisconnectSocket( SOCKET &socket )
{
	bool xXxTinaReTurnerxXx = true;
	closesocket( socket );
	socket = INVALID_SOCKET;
	return xXxTinaReTurnerxXx;
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
	mRecvBufLen = DEFAULT_BUFLEN;
}

Connection::~Connection()
{
}
