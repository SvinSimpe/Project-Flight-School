#ifndef CLIENT_H
#define CLIENT_H

#include "Connection.h"

class Client
{
private:
	NetworkEventForwarder*	mNEF;
protected:
public:
	static ClientSocketManager*	mSocketManager;

private:
	void			InitForwardingEvents();
protected:
public:
	void			Initialize( std::string ip, unsigned int port );
	void			Release();
					Client();
	virtual			~Client();
};
#endif