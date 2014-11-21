#include "Connection.h"

bool Connection::Initialize()
{
	return true;
}

bool Connection::SendMsg( SOCKET &to, char* msg )
{
	int msgLen = (int)strlen( msg );
	mResult = send( to, msg, msgLen, 0 );
	if ( mResult == SOCKET_ERROR )
	{
		printf( "send failed when sending to %d with error: %d\n", to, WSAGetLastError() );
		DisconnectSocket( to );
		return false;
	}
	return true;
}

char* Connection::ReceiveMsg( SOCKET &from )
{
	char* recvBuf = new char[DEFAULT_BUFLEN];

	mResult = recv( from, recvBuf, mRecvBufLen, 0 );
	if ( mResult < 0 )
	{
		printf( "recv failed when receiving from %d with error: %d\n", from, WSAGetLastError() );
		DisconnectSocket( from );
		if (recvBuf)
			delete[] recvBuf;
		return nullptr;
	}

	char* msg = new char[mResult + 1];
	memset( msg, '\0', mResult + 1 );
	memcpy( msg, recvBuf, mResult );

	if ( recvBuf )
		delete[] recvBuf;
	return msg;
}

void Connection::PrintMsg( char* msg )
{
	int msgLen = (int)strlen( msg );
	for ( int i(0); i < msgLen; i++ )
	{
		printf( "%c", msg[i] );
	}
	printf( "\n" );
}

bool Connection::DisconnectSocket( SOCKET &socket )
{
	bool xXxTinaReTurnerxXx = true;
	closesocket( socket );
	socket = INVALID_SOCKET;
	return xXxTinaReTurnerxXx;
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
