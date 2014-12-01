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
	protected:
	public:
		template <typename T>
		void	StructToCharPtr( T* inStruct, char* result, int size );
		template <typename T>
		void	CharPtrToStruct( T* result, char* inChar, int size );

		// Functions
	private:
		Package	Pack( char* body, int index );

	protected:

	public:
		bool	SendMsg( SOCKET &to, char* body );
		char*	ReceiveMsg( SOCKET &from);
		bool	DisconnectSocket( SOCKET &socket );
		bool	Initialize();
		void	Release();
				Connection();
		virtual	~Connection();
};


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