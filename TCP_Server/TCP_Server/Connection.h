#pragma once

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif

#include <windows.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <IPHlpApi.h>
#include <stdio.h>

#pragma comment(lib, "Ws2_32.lib")

#define DEFAULT_BUFLEN 512

// Abstract base class for both client and server
class Connection
{
	private:

	protected:
		int SendMsg(char* msg, int msgLen);				// Used for sending messages
		void PrintMsg();								// Prints received message

	public:
		virtual int	Initialize( const char* port );		// Initializes WSA and address info
		virtual int MessageLoop() = 0;					// Abstract function that will handle message receiving/sending
		int CloseConnection();							// Closes the current socket-connection
		virtual void Release();							// Frees memory
		Connection();
		virtual ~Connection();

	private:
		WSADATA		mWSAD;								// The WSA-variable

	protected:
		SOCKET		mSocket;							// The socket-variable 
		addrinfo*	mAddrResult;						// Used for address-stuff
		addrinfo	mHints;								// Variable used for retreiving address
		char*		mRecvBuf;							// Array storing the received message
		int			mRecvBufLen;						// The length of above array
		int			mResult;							// Stores return-results of various functions

	public:
};

