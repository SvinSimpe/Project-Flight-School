#ifndef REMOTEPLAYER_H
#define REMOTEPLAYER_H

#include "Graphics.h"
#include "EventManager.h"
#include "Events.h"

struct BoundingBox
{
	XMFLOAT3	position;
	float		width;
	float		height;

	BoundingBox()
	{
		position	= XMFLOAT3( 0.0f, 0.0f, 0.0f );
		width		= 1.0f;
		height		= 1.0f;
	}

	BoundingBox( float width, float height )
	{
		this->width		= width;
		this->height	= height;
	}

	bool Intersect( BoundingBox* inBox ) 
	{
		return ( ( position.x < inBox->position.x + inBox->width  ) && ( position.x + width  > inBox->position.x ) &&
				 ( position.z < inBox->position.z + inBox->height ) && ( position.z + height > inBox->position.z ) );		
	}
};

struct BoundingCircle
{
	XMFLOAT3	center;
	float		radius;

	BoundingCircle()
	{
		center	= XMFLOAT3( 0.0f, 0.0f, 0.0f );
		radius	= 0.0f;
	}

	BoundingCircle( float radius )
	{
		this->radius	= radius;
	}

	bool Intersect( BoundingCircle* inCircle ) const
	{
		return ( pow( center.x - inCircle->center.x, 2 ) + pow( center.z - inCircle->center.z, 2 ) ) < pow( radius + inCircle->radius, 2 );
	}
};


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
		unsigned int	mID;
		UpperBody		mUpperBody;
		LowerBody		mLowerBody;
		BoundingBox*	mBoundingBox;
		BoundingCircle*	mBoundingCircle;

	public:

	// Member functions
	private:
		void			RemoteUpdate( IEventPtr newEvent );

	protected:
		void			LookAt( float rotation );

	public:
		int				GetID() const;
		HRESULT			Render( float deltaTime );
		HRESULT			Initialize( unsigned int id );
		void			Release();
		BoundingBox*	GetBoundingBox() const;
		BoundingCircle*	GetBoundingCircle() const;
		XMFLOAT3		GetPosition() const;
						RemotePlayer();
						~RemotePlayer();
};

#endif

