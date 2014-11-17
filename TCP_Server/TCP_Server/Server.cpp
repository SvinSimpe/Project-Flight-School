#include "Server.h"

Server* Server::mInstance = nullptr;

Server::Server() : Connection()
{
	mHints.ai_flags = AI_PASSIVE; // The exception mentioned in Connection.cpp
	mListenSocket = INVALID_SOCKET;
	mSendResult = 0;
}

Server::~Server()
{
}

Server* Server::GetInstance()
{
	if (!mInstance)
	{
		mInstance = new Server();
	}
	return mInstance;
}

int Server::Initialize( const char* port )
{
	Connection::Initialize( port );

	/* Thread code left here for future use
	for (int i(0); i < 5; i++){
		mThreads.push_back(std::thread(&Server::Test, this));
	}
	for (auto& t : mThreads) t.join();
	*/

	mListenSocket = socket( mAddrResult->ai_family, mAddrResult->ai_socktype, mAddrResult->ai_protocol ); // Create socket
	if ( mListenSocket == INVALID_SOCKET )
	{
		printf( "SERVER|socket failed with error: %d\n", WSAGetLastError() );
		freeaddrinfo(mAddrResult);
		WSACleanup();
		return 1;
	}

	mResult = bind( mListenSocket, mAddrResult->ai_addr, (int)mAddrResult->ai_addrlen ); // Setup TCP listening socket
	if ( mResult == SOCKET_ERROR )
	{
		printf( "SERVER|bind failed with error: %d\n", WSAGetLastError() );
		freeaddrinfo(mAddrResult);
		closesocket(mListenSocket);
		WSACleanup();
		return 1;
	}
	freeaddrinfo( mAddrResult ); // Not required anymore

	mResult = listen( mListenSocket, SOMAXCONN ); // Listen
	if ( mResult == SOCKET_ERROR )
	{
		printf( "SERVER|listen failed with error: %d\n", WSAGetLastError() );
		closesocket(mListenSocket);
		WSACleanup();
		return 1;
	}

	return 0;
}

int Server::AcceptConnection()
{
	mSocket = accept( mListenSocket, nullptr, nullptr );
	if ( mSocket == INVALID_SOCKET )
	{
		printf( "SERVER|accept failed with error: %d\n", WSAGetLastError() );
		closesocket(mListenSocket);
		WSACleanup();
		return 1;
	}
	closesocket( mListenSocket );
	printf("SERVER|%d connected.\n", mSocket);
	return 0;
}

int Server::MessageLoop()
{
	do{
		mResult = recv( mSocket, mRecvBuf, mRecvBufLen, 0 ); // If connection isn't closed, program will wait here until message is received
		if ( mResult > 0 )
		{
			//printf( "SERVER|Bytes received: %d\n", mResult );
			printf("SERVER|%d:", mSocket);
			PrintMsg();

			mSendResult = SendMsg(mRecvBuf, mResult);
			if (mSendResult == -1)
			{
				Release();
				return 1;
			}
			//printf("SERVER|Bytes sent: %d\n", mSendResult);
		}
		else if (mResult == 0)
		{
			printf("SERVER|Connection closing... \n");
		}
		else
		{
			printf("SERVER|recv failed with error: %d\n", WSAGetLastError());
			Release();
			return 1;
		}

	} while (mResult > 0);

	return 0;
}

void Server::Release()
{
	Connection::Release();
	if (mInstance){
		delete mInstance;
	}
}