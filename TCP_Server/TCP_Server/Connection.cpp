#include "Connection.h"

int Connection::SendMsg( char* msg, int msgLen )
{
	int result = send( mSocket, msg, msgLen, 0 ); // Sends the message, returns the length of the sent message
	if ( result == SOCKET_ERROR ) // Handles errors
	{
		printf( "send failed with error: %d\n", WSAGetLastError() );
		closesocket(mSocket);
		WSACleanup();
		result = -1;
	}
	return result;
}

void Connection::PrintMsg()
{
	for (int i(0); i < mResult; i++){
		printf("%c", mRecvBuf[i]);
	}
	printf("\n");
}

int Connection::Initialize( const char* port )
{
	mRecvBuf = new char[mRecvBufLen]; // This could maybe just be an ordinary array, but whatever

	mResult	= WSAStartup( MAKEWORD( 2, 2), &mWSAD ); // Initializes WSA
	if ( mResult != 0 )
	{
		printf( "WSAStartup failed with error: %d\n", mResult );
		return 1;
	}

	mResult	= getaddrinfo( nullptr, port, &mHints, &mAddrResult ); // Gets the address info
	if ( mResult != 0 )
	{
		printf( "getaddrinfo failed with error: %d\n", mResult );
		WSACleanup();
		return 1;
	}

	return 0;
}

int Connection::CloseConnection()
{
	mResult = shutdown( mSocket, SD_SEND ); // Shuts down connection
	if ( mResult == SOCKET_ERROR )
	{
		printf( "shutdown failed with error: %d\n", WSAGetLastError() );
		closesocket( mSocket );
		WSACleanup();
		return 1;
	}
	return 0;
}

void Connection::Release()
{
	closesocket( mSocket );
	WSACleanup();
	delete mRecvBuf;
}

Connection::Connection()
{
	mSocket				= INVALID_SOCKET;
	mAddrResult			= nullptr;

	// This needs to be done in here since both client and server will initialize the same, except for a single exception server-side
	ZeroMemory(&mHints, sizeof(mHints));
	mHints.ai_family	= AF_UNSPEC;
	mHints.ai_socktype	= SOCK_STREAM;
	mHints.ai_protocol	= IPPROTO_TCP;

	mRecvBufLen			= DEFAULT_BUFLEN; // This is currently 512, defined in Connection.h
}

Connection::~Connection()
{
}