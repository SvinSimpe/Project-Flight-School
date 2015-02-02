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
	void			InitForwardingEvents();
protected:
public:
	void			ForwardEvent( IEventPtr ePtr );
	void			Initialize( unsigned int port );
	void			Release();
					Server();
	virtual			~Server();
};
#endif