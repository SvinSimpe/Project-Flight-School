#ifndef _BOUNDINGGEOMETRY_H_
#define _BOUNDINGGEOMETRY_H_

#include <DirectXMath.h>
using namespace DirectX;

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

#endif