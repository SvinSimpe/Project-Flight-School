#ifndef CLIENT_H
#define CLIENT_H

#include "Connection.h"
#include "Package.h"
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
		template <typename T>
		void	StructToCharPtr( T* inStruct, char* result );
		template <typename T>
		void	CharPtrToStruct( T* result, char* inChar );

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
