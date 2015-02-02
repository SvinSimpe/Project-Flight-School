#ifndef SERVER_H
#define SERVER_H

#include "Connection.h"

class Server
{
private:
	NetworkEventForwarder*	mNEF;
protected:
public:
	static SocketManager*	mSocketManager;

private:
protected:
public:
	void			DoSelect( int pauseMicroSecs, bool handleInput = true );
	void			Initialize( unsigned int port );
	void			Release();
					Server();
	virtual			~Server();
};
#endif