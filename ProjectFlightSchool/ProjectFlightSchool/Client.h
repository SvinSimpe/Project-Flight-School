#ifndef CLIENT_H
#define CLIENT_H

#include <string>
#include <iostream>

#include "Connection.h"

class Client // The class used by clients to connect to the server
{
	private:

	protected:

	public:
		bool Connect();
		bool Run();
		bool MsgLoop();
		bool ReceiveLoop();
		bool HandleMsg( char* msg );
		bool Initialize( const char* ip, const char* port ); // Sets up and connects to the server
		void Release();
		Client();
		~Client();

	private:
		int mResult;
		addrinfo* mAddrResult;
		SOCKET mServerSocket;
		Connection* mConn;

	protected:

	public:
};
#endif
