#ifndef REMOTEPLAYER_H
#define REMOTEPLAYER_H

#include "Graphics.h"
#include "EventManager.h"
#include "Events.h"

struct UpperBody
{
	UINT		playerModel;
	XMFLOAT3	direction;
	XMFLOAT3	position;
};

struct LowerBody
{
	UINT		playerModel;
	XMFLOAT3	direction;
	XMFLOAT3	position;
	float		speed;
};

class RemotePlayer
{
	// Member variables
	private:
	protected:
		int			mID;
		UpperBody	mUpperBody;
		LowerBody	mLowerBody;
	public:

	// Member functions
	private:
		void RemoteUpdate( IEventPtr newEvent );

	protected:
		void LookAt( float rotation );

	public:
		int GetID() const;
		HRESULT Render( float deltaTime );
		HRESULT Initialize( unsigned int id );
		void Release();
		RemotePlayer();
		~RemotePlayer();
};

#endif

