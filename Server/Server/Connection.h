#pragma once

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif

#include <windows.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <IPHlpApi.h>
#include <stdio.h>
#include <thread>

#pragma comment(lib, "Ws2_32.lib")

#define DEFAULT_BUFLEN 512

#define DEFAULT_PORT "27015"
#define DEFAULT_IP "127.0.0.1"

class Connection
{
	private:

	protected:

	public:
		bool Initialize();
		bool SendMsg( SOCKET &to, char* msg );
		char* ReceiveMsg( SOCKET &from );
		void PrintMsg( char* msg );
		bool DisconnectSocket( SOCKET &socket );
		void Release();
		Connection();
		~Connection();

	private:
		int mResult;
		int mRecvBufLen;

	protected:

	public:
};

