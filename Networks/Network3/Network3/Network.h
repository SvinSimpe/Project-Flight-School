#ifndef NETWORK_H
#define NETWORK_H

#include "Connection.h"

// Base class for both server and client
class Network
{
	private:
	protected:
		UINT mPort;
	public:

	private:
	protected:
	public:
		virtual void	Update( float deltaTime ) = 0;
		virtual void	DoSelect( int pauseMicroSecs, bool handleInput = true ) = 0;
		virtual bool	Initialize( UINT port );
		virtual void	Release() = 0;
						Network();
		virtual			~Network();
};
#endif