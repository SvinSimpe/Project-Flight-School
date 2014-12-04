#ifndef CLIENT_H
#define CLIENT_H

#include "Connection.h"
#include "Package.h"
#include <iostream>

class Client // The class used by clients to connect to the server
{
	// Members
	private:
		int				mResult;
		addrinfo*		mAddrResult;
		SOCKET			mServerSocket;
		Connection*		mConn;

	protected:

	public:

		// Template functions
	private:
		template <typename T>
		void HandlePkg( Package<T> p );
	protected:
	public:
		
		// Functions
	private:
		bool	PkgLoop();
		bool	ReceiveLoop();

	protected:

	public:
		bool	Connect();
		bool	Run();
		bool	Initialize( const char* ip, const char* port ); // Sets up and connects to the server
		void	Release();
				Client();
		virtual	~Client();
};

template <typename T>
void Client::HandlePkg( Package<T> p )
{
	switch (p.head.eventType)
	{
	case Net_Event::QUIT:
	{
		printf("Disconnected from server.\n");
		mConn->DisconnectSocket(mServerSocket);
	}
		break;
	default:
	{
	}
		break;
	}
}
#endif
