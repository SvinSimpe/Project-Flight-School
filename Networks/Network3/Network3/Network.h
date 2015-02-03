#ifndef NETWORK_H
#define NETWORK_H

#include "Connection.h"
#include <iostream>

// Base class for both server and client
class Network
{
	private:
	protected:
		NetworkEventForwarder*	mNEF;
		UINT					mPort;
	public:

	private:
		virtual void	InitEventListening() = 0;
		virtual void	InitForwardingEvents() = 0;
	protected:
	public:
		virtual void	DoSelect( int pauseMicroSecs, bool handleInput = true ) = 0;
		virtual bool	Initialize( UINT port );
		virtual void	Release();
						Network();
		virtual			~Network();
};
#endif