#ifndef NETWORK_H
#define NETWORK_H

#include "Connection.h"
#include <iostream>

// Base class for both server and client
class Network
{
	private:
	protected:
		struct Enemy
		{
			UINT id;
			UINT state;
			UINT type;
			XMFLOAT3 pos;
			XMFLOAT3 dir;
			bool isAlive;

			Enemy()
			{
				id = (UINT)-1;
				state = (UINT)-1;
				type = (UINT)-1;
				pos = XMFLOAT3( 0.0f, 0.0f, 0.0f );
				dir = XMFLOAT3( 0.0f, 0.0f, 0.0f );
				isAlive = false;
			}

			Enemy( UINT id, UINT state, UINT type, XMFLOAT3 pos, XMFLOAT3 dir )
			{
				this->id = id;
				this->state = state;
				this->type = type;
				this->pos = pos;
				this->dir = dir;
				isAlive = true;
			}
		};
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