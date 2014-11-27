#include "Connection.h"

bool Connection::SendMsg( SOCKET &to, char* body )
{
	Package p;
	p.head.index = 0;
	p.head.contentType = ContentType::MESSAGE;
	p.head.contentSize = sizeof(body);
	p.body.content = (char*)body;

	mResult = send( to, (char*)&p, sizeof(p), 0 );
	if ( mResult == SOCKET_ERROR )
	{
		printf( "send failed when sending to %d with error: %d\n", to, WSAGetLastError() );
		DisconnectSocket( to );
		return false;
	}
	return true;
}

Package Connection::Unpack(char* package)
{
	Package p;
	//CharPtrToStruct(&p, package);
	return p;
}

char* Connection::Pack(char* body, int index)
{
	char* result = (char*)malloc(sizeof(Package));
	Package p;

	p.head.index = index;
	p.head.contentType = ContentType::MESSAGE;
	p.head.contentSize = sizeof(Message);
	p.body.content = body;
	//StructToCharPtr(&p, result);

	return result;
}

char* Connection::ReceiveMsg(SOCKET from)
{
	mResult = recv(from, mRecvBuf, mRecvBufLen, 0);
	if (mResult < 0)
	{
		printf("recv failed when receiving from %d with error: %d\n", from, WSAGetLastError());
		DisconnectSocket(from);
		char* result = "Failed!";
		return result;
	}

	Package* p = new Package();
	CharPtrToStruct(p, mRecvBuf, mResult);
	//memcpy(p, mRecvBuf, mResult);

	StructToCharPtr(p->body.content, mRecvBuf, sizeof(p->body.content));
	//memcpy(mRecvBuf, p->body.content, sizeof(p->body.content));

	delete p;
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
	return true;
}

void Connection::Release()
{
	if (mRecvBuf)
		delete[] mRecvBuf;
}

Connection::Connection()
{
	mResult		= 0;
	mRecvBuf	= nullptr;
	mRecvBufLen = DEFAULT_BUFLEN;
}

Connection::~Connection()
{
}
