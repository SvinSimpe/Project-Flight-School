#ifndef CONNECTION_H
#define CONNECTION_H

#pragma comment( lib, "Ws2_32.lib" )

#include <Winsock2.h>
#include <ws2tcpip.h>
#include <IPHlpApi.h>
#include <stdio.h>
#include <thread>
#include "Package.h"
#include "Structs.h"

#define DEFAULT_BUFLEN	512

class Connection
	{
	// Members
	private:
		int		mResult;
	protected:
	public:

	// Template functions
	private:
	protected:
	public:
		template <typename T>
		bool	SendPkg( SOCKET &to, int eventIndex, Net_Event type, T body );
		template <typename T>
		bool	ReceivePkg( SOCKET &from, Package<T> &p );

	// Functions
	private:
	protected:
	public:
		void	DisconnectSocket( SOCKET &socket );
		bool	Initialize();
		void	Release();
				Connection();
		virtual	~Connection();
};

template <typename T>
bool Connection::SendPkg( SOCKET &to, int eventIndex, Net_Event type, T body )
{
	if ( to == INVALID_SOCKET )
		return false;

	Package<T> p;
	p.head.index		= eventIndex;
	p.head.eventType	= type;
	p.head.contentSize	= sizeof( body );
	p.body.content		= body;

	mResult = send( to, (char*)&p, sizeof( p ), 0 );
	if ( mResult == SOCKET_ERROR )
	{
		printf( "sendf failed when sending to %d with error: %d\n", to, WSAGetLastError() );
		DisconnectSocket( to );
		return false;
	}
	return true;
}

template <typename T>
bool Connection::ReceivePkg( SOCKET &from, Package<T> &p )
{
	if ( from == INVALID_SOCKET )
		return false;

	mResult = recv( from, (char*)&p, DEFAULT_BUFLEN;, 0 );
	if ( mResult < 0 )
	{
		printf( "recv failed when receiving from %d with error: %d\n", from, WSAGetLastError() );
		DisconnectSocket( from );
		return false;
	}
	return true;
}
#endif