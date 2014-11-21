#pragma once
#include <vector>
#include "Connection.h"

#define ServerObj Server::GetInstance()

class Server
{
	private:
		Server();
		~Server();

	protected:

	public:
		static Server* GetInstance();
		bool Initialize( const char* port );
		bool Connect();
		bool AcceptConnection();
		bool Update();
		bool RecvLoop( int index );
		void HandleMsg( SOCKET &socket, char* msg );
		void Release();

	private:
		static Server* mInstance;
		int mResult;
		addrinfo* mAddrResult;
		SOCKET mListenSocket;
		std::vector<SOCKET> mClientSockets;
		Connection* mConn;

	protected:

	public:
};

