#ifndef REMOTEPLAYER_H
#define REMOTEPLAYER_H

#include "Graphics.h"
#include "EventManager.h"
#include "Events.h"
#include "Input.h"
#include "Player.h"

class RemotePlayer
{
	// Member variables
	private:
		int			mID;
		UpperBody	mUpperBody;
		LowerBody	mLowerBody;
	protected:
	public:

	// Member functions
	private:
		void RemoteUpdate( IEventPtr newEvent );
		void LookAt( float rotation );

	protected:
	public:
		HRESULT Render( float deltaTime );
		int GetID() const;
		HRESULT Initialize( unsigned int id );
		void Release();
		RemotePlayer();
		~RemotePlayer();
};

#endif

