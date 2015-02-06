#ifndef NETWORK_H
#define NETWORK_H

#include "Connection.h"

// Base class for both server and client
class Network
{
	private:
	protected:
	public:

	private:
		virtual void	StartUp( IEventPtr eventPtr ) = 0;

	protected:

	public:
		virtual void	Update( float deltaTime ) = 0;
		virtual void	DoSelect( int pauseMicroSecs, bool handleInput = true ) = 0;
		virtual bool	Initialize() = 0;
		virtual void	Release() = 0;
						Network();
		virtual			~Network();
};
#endif