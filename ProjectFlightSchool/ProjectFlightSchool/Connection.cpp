#include "Connection.h"

bool Connection::SendMsg( SOCKET &to, char* msg )
{
	Pack test;
	test.x	= 3;
	test.y	= 5;
	msg = (char*)malloc( sizeof(Pack) );
	memcpy( msg, &test, sizeof(test) );
	printf( "Printar struct send: %d, %d\n", test.x, test.y );

	mResult = send( to, msg, sizeof(Pack), 0 );
	if ( mResult == SOCKET_ERROR )
	{
		printf( "send failed when sending to %d with error: %d\n", to, WSAGetLastError() );
		DisconnectSocket( to );
		return false;
	}
	delete msg;
	return true;
}

char* Connection::ReceiveMsg( SOCKET &from )
{
	char* recvBuf	= (char*)malloc( sizeof(Pack) );
	mResult			= recv( from, recvBuf, mRecvBufLen, 0 );
	if ( mResult < 0 )
	{
		printf( "recv failed when receiving from %d with error: %d\n", from, WSAGetLastError() );
		DisconnectSocket( from );
		if (recvBuf)
			delete[] recvBuf;
		return nullptr;
	}

	Pack test;
	memcpy( &test, recvBuf, sizeof(Pack) );
	printf( "Printar struct recv: %d, %d\n", test.x, test.y );

	//If you receive something else than a struct. Can be deleted when package struct is in place.
	/*char* msg = new char[mResult + 1];
	memset( msg, '\0', mResult + 1 );
	memcpy( msg, recvBuf, mResult );*/

	if ( recvBuf )
		delete[] recvBuf;
	return "Hej!";
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
