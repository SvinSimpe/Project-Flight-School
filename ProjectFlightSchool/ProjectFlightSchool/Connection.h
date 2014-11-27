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
		char*	Pack( char* body, int index );
		char*	ReceiveMsg( SOCKET from);
		template <typename T>
		void	StructToCharPtr( T* inStruct, char* result );
		template <typename T>
		void	CharPtrToStruct( T* result, char* inChar );

		// Functions
	private:

	protected:

	public:
		bool	SendMsg( SOCKET &to, char* body );
		Package		Unpack(char* package);
		//char*	ReceiveMsg( SOCKET &from );
		bool	DisconnectSocket( SOCKET &socket );
		bool	Initialize();
		void	Release();
				Connection();
		virtual	~Connection();
};



template <typename T>
void Connection::StructToCharPtr( T* inStruct, char* result )
{
	if (sizeof(T) == sizeof(result))
	{
		int i = 0;
	}
	memcpy( result, inStruct, sizeof( T ) );
}

template <typename T>
void Connection::CharPtrToStruct( T* result, char* inChar )
{
	if (sizeof(result) == sizeof(inChar))
	{
		int i = 0;
	}
	memcpy( result, inChar, sizeof( T ) );
}


#endif