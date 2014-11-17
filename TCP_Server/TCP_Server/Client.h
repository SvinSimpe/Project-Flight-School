#pragma once
#include "Connection.h"
#include <iostream>
#include <string>

// The client part of the program connects to a server
class Client : public Connection
{
	private:

	protected:

	public:
		int Initialize( const char* port );		// Establishes connection between client and server
		int MessageLoop();						// Handles sending and receiving messages
		void Release();
		Client();
		~Client();

	private:
		char* mSendBuf;							// This is where the message to be sent is stored

	protected:

	public:
};

