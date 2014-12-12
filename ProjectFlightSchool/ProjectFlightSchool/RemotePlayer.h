#ifndef REMOTEPLAYER_H
#define REMOTEPLAYER_H

#include "Graphics.h"
#include "EventManager.h"
#include "Events.h"
#include "Input.h"
#include "Player.h"

class RemotePlayer
{
private:
	int			mID;
	UpperBody	mUpperBody;
	LowerBody	mLowerBody;

private:
	void RemoteUpdate( IEventPtr newEvent );
	void LookAt( float rotation );

public:
	HRESULT Render( float deltaTime );

	HRESULT Initialize( unsigned int id );
	void Release();
	RemotePlayer();
	~RemotePlayer();
};

#endif

