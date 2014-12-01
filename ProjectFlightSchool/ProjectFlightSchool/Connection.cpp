#include "Connection.h"

char* Connection::ReceiveMsg(SOCKET &from)
{
	mResult = recv(from, mRecvBuf, mRecvBufLen, 0);
	if (mResult < 0)
	{
		printf("recv failed when receiving from %d with error: %d\n", from, WSAGetLastError());
		DisconnectSocket(from);
		char* result = "Failed!";
		return result;
	}

	Package p = Package();
	CharPtrToStruct(&p, mRecvBuf, mResult);
	StructToCharPtr(p.body.content, mRecvBuf, p.head.contentSize);

	return mRecvBuf;
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
	mRecvBuf = new char[mRecvBufLen];
	for (int i(0); i < mRecvBufLen; i++)
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
	if (mRecvBuf)
		delete[] mRecvBuf;
}
