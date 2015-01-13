#ifndef _PROJECTILE_H_
#define _PROJECTILE_H_

#include "EventManager.h"
#include "Events.h"
#include <Graphics.h>
#include "BoundingGeometry.h"

class Projectile
{
	private:
		XMFLOAT3	mPosition;
		XMFLOAT3	mDirection;
		float		mSpeed;
		bool		mIsActive;
		float		mRotation;
		float		mLifeTime;

		BoundingCircle*	mBoundingCircle;

	public:
		HRESULT Update( float deltaTime );
		HRESULT Render();

		void	SetDirection( XMFLOAT3 startPosition, XMFLOAT3 direction );
		void	SetIsActive( bool isActive );
		bool	IsActive() const;
		void	Reset();
		BoundingCircle*	GetBoundingCircle() const;

		HRESULT Initialize();
		void	Release();

				Projectile();
				~Projectile();
};
#endif