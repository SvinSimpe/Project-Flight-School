#include "Client.h"

int Client::Initialize(const char* port)
{
	Connection::Initialize(port);

	// Attempts to connect to an address until one succeeds
	addrinfo* ptr = nullptr;
	for (ptr = mAddrResult; ptr != nullptr; ptr = ptr->ai_next)
	{
		mSocket = socket(ptr->ai_family, ptr->ai_socktype, ptr->ai_protocol);
		if (mSocket == INVALID_SOCKET)
		{
			printf("CLIENT|socket failed with error: %d\n", WSAGetLastError());
			WSACleanup();
			return 1;
		}

		// Connection attempt is done here, if it fails it will continue to the next iteration
		mResult = connect(mSocket, ptr->ai_addr, (int)ptr->ai_addrlen);
		if (mResult == SOCKET_ERROR)
		{
			closesocket(mSocket);
			mSocket = INVALID_SOCKET;
			continue;
		}
		break;
	}
	freeaddrinfo(mAddrResult); // This isn't needed anymore

	// Makes sure a connection is established
	if (mSocket == INVALID_SOCKET)
	{
		printf("CLIENT|Unable to connect to server.\n");
		closesocket(mSocket);
		WSACleanup();
		return 1;
	}
	printf("CLIENT|Socket ID: %d\n", mSocket);

	return 0;
}

int Client::MessageLoop()
{
	do
	{
		std::string msg = "";
		printf("Message: ");
		std::getline(std::cin, msg);
		mSendBuf = (char*)msg.c_str();

		// Sends buffered message
		mResult = SendMsg(mSendBuf, (int)strlen(mSendBuf));
		if (mResult == -1)
		{
			return 1;
		}
		//printf("CLIENT|Bytes sent: %d\n", mResult);

		// Attempts to receive a message
		mResult = recv(mSocket, mRecvBuf, mRecvBufLen, 0);

		if (mResult > 0)
		{
			//printf( "CLIENT|Bytes received: %d\n", mResult );
			//printf("CLIENT|%d:", mSocket);
			//PrintMsg();
		}
		else // Error handling
		{
			printf("CLIENT|recv failed with error: %d\n", WSAGetLastError());
			Release();
			return 1;
		}

		if (msg == "Quit")
		{
			printf("CLIENT|Closing connection...\n");
			CloseConnection();
			break;
		}
	} while (mResult > 0); // Iterates while messages are being sent

	return 0;
}

void Client::Release()
{
	Connection::Release();
}


Client::Client() : Connection()
{
	mSendBuf = "";
}


Client::~Client()
{
}
