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
		char* mRecvBuf;
		int mRecvBufLen;

	protected:

	public:

		// Template functions
	private:
		template <typename T>
		Package	Pack( T body, int index );

	protected:
	public:
		template <typename T>
		bool	SendMsg( SOCKET &to, T body );
		template <typename T>
		void	StructToCharPtr( T* inStruct, char* result, int size );
		template <typename T>
		void	CharPtrToStruct( T* result, char* inChar, int size );

		// Functions
	private:

	protected:

	public:
		char*	ReceiveMsg( SOCKET &from);
		bool	DisconnectSocket( SOCKET &socket );
		bool	Initialize();
		void	Release();
				Connection();
		virtual	~Connection();
};

template <typename T>
Package Connection::Pack(T body, int index)
{
	Package p = Package();
	p.head.index = index;
	p.head.contentType = ContentType::MESSAGE;
	p.head.contentSize = DEFAULT_BUFLEN;
	p.body.content = (char*)body;

	return p;
}

template <typename T>
bool Connection::SendMsg(SOCKET &to, T body)
{
	Package p = Pack(body, 0);
	mResult = send(to, (char*)&p, sizeof(p), 0);
	if (mResult == SOCKET_ERROR)
	{
		printf("sendf failed when sending to %d with error: %d\n", to, WSAGetLastError());
		DisconnectSocket(to);
		return false;
	}
	return true;
}

template <typename T>
void Connection::StructToCharPtr( T* inStruct, char* result, int size)
{
	memcpy(result, inStruct, size);
}

template <typename T>
void Connection::CharPtrToStruct( T* result, char* inChar, int size)
{
	memcpy(result, inChar, size);
}
#endif