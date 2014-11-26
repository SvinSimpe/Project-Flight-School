#ifndef CONNECTION_H
#define CONNECTION_H

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#undef WIN32_LEAN_AND_MEAN
#endif

#include <Winsock2.h>
#pragma comment(lib, "Ws2_32.lib")

#include <winsock2.h>
#include <ws2tcpip.h>
#include <IPHlpApi.h>
#include <stdio.h>
#include <thread>
#include <string>
#include "Package.h"
#include "Structs.h"

#define DEFAULT_BUFLEN 512
#define DEFAULT_PORT "27015"
#define DEFAULT_IP "localhost"

struct Pack
{
	int	x;
	int	y;
};

class Connection
	{
	// Members
	private:
		int mResult;
		int mRecvBufLen;

	protected:

	public:

		// Template functions
	private:
	protected:
	public:
		template <typename T>
		char*	Pack( T body, int index );
		template <typename T>
		void	ReceiveMsg( SOCKET &from, T** result );
		template <typename T>
		void	StructToCharPtr( T* inStruct, char* result );
		template <typename T>
		void	CharPtrToStruct( T* result, char* inChar );

		// Functions
	private:

	protected:

	public:
		bool	SendMsg( SOCKET &to, char* msg );
		Package		Unpack(char* package);
		char*	ReceiveMsg( SOCKET &from );
		bool	DisconnectSocket( SOCKET &socket );
		bool	Initialize();
		void	Release();
				Connection();
		virtual	~Connection();
};

template <typename T>
char* Connection::Pack( T body, int index )
{
	char* result = ( char* )malloc( sizeof( Package ) );
	Package p;

	p.head.index		= index;
	p.head.contentType	= ContentType::MESSAGE;
	p.head.contentSize	= sizeof( body );
	p.body.content		= ( char* )malloc( sizeof ( body ) );
	StructToCharPtr(&body, p.body.content);
	//p.body.content		= body;
	//StructToCharPtr( &body, p.body.content );
	StructToCharPtr( &p, result );

	return result;
}

template<typename T>
void Connection::ReceiveMsg(SOCKET &from, T** result)
{
	char* recvBuf = (char*)malloc(sizeof(Package));
	mResult = recv(from, recvBuf, mRecvBufLen, 0);
	if (mResult < 0)
	{
		printf("recv failed when receiving from %d with error: %d\n", from, WSAGetLastError());
		DisconnectSocket(from);
		if (recvBuf)
			free(recvBuf);
		return;
	}

	Package p = Unpack(recvBuf);

	*result = (T*)malloc(p.head.contentSize);
	CharPtrToStruct(*result, p.body.content);
	if (recvBuf)
		free(recvBuf);
}

template <typename T>
void Connection::StructToCharPtr( T* inStruct, char* result )
{
	memcpy( result, inStruct, sizeof( *inStruct ) );
}

template <typename T>
void Connection::CharPtrToStruct( T* result, char* inChar )
{
	memcpy( result, inChar, sizeof( *result ) );
}


#endif