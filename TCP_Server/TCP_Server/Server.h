#pragma once
#include "Connection.h"
#include <vector>
#include <iostream>

// The server class handles connecting clients
class Server : public Connection
{
private:
	Server();
	~Server();

protected:

public:
	static Server* GetInstance();			// Returns the singleton-object
	int Initialize( const char* port );		// Sets up the listensocket
	int AcceptConnection();					// Waits for a client to connect
	int MessageLoop();						// Handles receiving and echoing messages
	void Release();

private:
	static Server* mInstance;				// The static singleton-object
	SOCKET mListenSocket;					// The listensocket (duh)
	int mSendResult;						// Stores length of sent message

protected:

public:
};

