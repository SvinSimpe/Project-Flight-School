#include "Connection.h"

bool Connection::SendMsg( SOCKET &to, char* body )
{
	char* msg;
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

char* Connection::Pack(char* body, int index)
{
	char* result = (char*)malloc(sizeof(Package));
	Package p;

	p.head.index = index;
	p.head.contentType = ContentType::MESSAGE;
	p.head.contentSize = sizeof(Message);
	p.body.content = body;
	/*p.body.content = (char*)malloc(sizeof(Message));
	StructToCharPtr(&body, p.body.content);*/
	//p.body.content		= body;
	//StructToCharPtr( &body, p.body.content );
	StructToCharPtr(&p, result);

	return result;
}

char* Connection::ReceiveMsg(SOCKET from)
{
	char* recvBuf = (char*)malloc(sizeof(Package));
	mResult = recv(from, recvBuf, mRecvBufLen, 0);
	if (mResult < 0)
	{
		printf("recv failed when receiving from %d with error: %d\n", from, WSAGetLastError());
		DisconnectSocket(from);
		if (recvBuf)
			free(recvBuf);
		char* result = "Failed!";
		return result;
	}

	Package p = Unpack(recvBuf);

	//switch (p.head.contentType)
	//{
	//case ContentType::MESSAGE:
	//	Message temp;
	//	if (sizeof(temp) == p.head.contentSize)
	//	{
	//		printf("Yes mothafucka!\n");
	//	}
	//	CharPtrToStruct(&temp, p.body.content);
	//	result = &temp;
	//	break;
	//};
	//StructToCharPtr(&temp, p.body.content);
	char* result = p.body.content;

	/**result = (T)malloc(p.head.contentSize);
	CharPtrToStruct(*result, p.body.content);*/
	if (recvBuf)
		free(recvBuf);
	return result;
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
