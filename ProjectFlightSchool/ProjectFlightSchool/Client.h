#ifndef CLIENT_H
#define CLIENT_H

#include "Connection.h"
#include <iostream>

class Client // The class used by clients to connect to the server
{
	private:
		int				mResult;
		addrinfo*		mAddrResult;
		SOCKET			mServerSocket;
		Connection*		mConn;

	protected:

	public:

	private:
		bool	MsgLoop();
		bool	ReceiveLoop();
		bool	HandleMsg( char* msg );

	protected:

	public:
		bool	Connect();
		bool	Run();
		bool	Initialize( const char* ip, const char* port ); // Sets up and connects to the server
		void	Release();
				Client();
		virtual	~Client();
};
#endif
