#ifndef TURRET_H
#define TURRET_H

#include "Player.h"
#include <math.h>

#define SHOOTCOOLDOWN 0.1f
#define PI 3.14159265f
struct Upper
{
	AssetID		model	= CUBE_PLACEHOLDER;
	XMFLOAT3	pos		= XMFLOAT3( 0.0f, 0.0f, 0.0f );
	XMFLOAT3	dir		= XMFLOAT3( 0.0f, 0.0f, 0.0f );
};

struct Lower
{
	AssetID		model	= CUBE_PLACEHOLDER;
	XMFLOAT3	pos		= XMFLOAT3( 0.0f, 0.0f, 0.0f );
	XMFLOAT3	dir		= XMFLOAT3( 0.0f, 0.0f, 0.0f );
};

class Turret
{
	// Variables
	private:
		int					mID;
		Upper*				mUpperBody;
		Lower*				mLowerBody;
		float				mRange;
		LoadOut*			mLoadOut;
		float				mShootTimer;
		bool				mRotating;
		BoundingCircle*		mBoundingCircle;
		RemotePlayer*		mTarget;
	protected:
	public:

	// Functions
	private:
		void				Fire();
		void				TrackTarget();
	protected:
	public:
		BoundingCircle*		GetBoundingCircle() const;
		void				SetTarget( RemotePlayer* obj );
		HRESULT				Update( float deltaTime );
		void				Render();
		void				Initialize();
		void				Release();
							Turret();
		virtual				~Turret();
};
#endif