#ifndef CONNECTION_H
#define CONNECTION_H

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#undef WIN32_LEAN_AND_MEAN
#endif

#include <Winsock2.h>
#pragma comment( lib, "Ws2_32.lib" )

#include <winsock2.h>
#include <ws2tcpip.h>
#include <IPHlpApi.h>
#include <stdio.h>
#include <thread>
#include <string>
#include "Package.h"
#include "Structs.h"

#define DEFAULT_BUFLEN	512
#define DEFAULT_PORT	"1337"
#define DEFAULT_IP		"localhost"

struct Pack
{
	int	x;
	int	y;
};

class Connection
	{
	// Members
	private:
		int		mResult;
		char*	mRecvBuf;
		int		mRecvBufLen;

	protected:

	public:

		// Template functions
	private:
		template <typename T>
		void	Pack( T body, int index );

	protected:
	public:
		template <typename T>
		bool	SendMsg( SOCKET &to, T body );
		template <typename T>
		bool	ReceiveMsg(SOCKET &from, Package<T> &p);

		// Functions
	private:

	protected:

	public:
		char*	ReceiveMsg( SOCKET &from);
		void	DisconnectSocket( SOCKET &socket );
		bool	Initialize();
		void	Release();
				Connection();
		virtual	~Connection();
};

template <typename T>
void Connection::Pack( T body, int index )
{
	Package p			= Package();
	p.head.index		= index;
	p.head.contentType	= ContentType::MESSAGE;
	p.head.contentSize	= DEFAULT_BUFLEN;
	p.body.content			= (char*)&body;

	mRecvBuf = (char*)&p;
}

template <typename T>
bool Connection::SendMsg( SOCKET &to, T body )
{
	Package<T> p;
	p.head.index = 0;
	p.head.contentType = ContentType::MESSAGE;
	p.head.contentSize = sizeof(body) + 12;
	p.body.content = (T)body;

	mResult = send( to, (char*)&p, sizeof(p), 0 );
	if ( mResult == SOCKET_ERROR )
	{
		printf( "sendf failed when sending to %d with error: %d\n", to, WSAGetLastError() );
		DisconnectSocket( to );
		return false;
	}
	return true;
}

template <typename T>
bool Connection::ReceiveMsg(SOCKET &from, Package<T> &p)
{
	mResult = recv(from, (char*)&p, sizeof(p), 0);
	if ( mResult < 0 )
	{
		printf( "recv failed when receiving from %d with error: %d\n", from, WSAGetLastError() );
		DisconnectSocket( from );
		return false;
	}
	return true;
}
#endif