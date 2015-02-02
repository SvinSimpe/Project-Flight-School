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
protected:
public:
	void			DoSelect( int pauseMicroSecs, bool handleInput = true );
	void			Initialize( std::string ip, unsigned int port );
	void			Release();
					Client();
	virtual			~Client();
};
#endif