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

#define DEFAULT_BUFLEN 512
#define DEFAULT_PORT "27015"
#define DEFAULT_IP "192.168.1.61"

class Connection
{
	private:
		int mResult;
		int mRecvBufLen;

	protected:

	public:

	private:

	protected:

	public:
		bool	SendMsg( SOCKET &to, char* msg );
		char*	ReceiveMsg( SOCKET &from );
		bool	DisconnectSocket( SOCKET &socket );
		bool	Initialize();
		void	Release();
				Connection();
		virtual	~Connection();
};
#endif