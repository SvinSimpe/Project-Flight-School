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

	//Use for moving objects, update position frame by frame
	BoundingBox( float width, float height )
	{
		this->width		= width;
		this->height	= height;
	}

	//Use for static objects
	BoundingBox( XMFLOAT3 position, float width, float height )
	{
		this->position	= position;
		this->width		= width;
		this->height	= height;	
	}

	bool Intersect( BoundingBox* inBox ) 
	{
		return ( ( position.x < inBox->position.x + inBox->width  ) && ( position.x + width  > inBox->position.x ) &&
				 ( position.z < inBox->position.z + inBox->height ) && ( position.z + height > inBox->position.z ) );		
	}

	bool Intersect( XMFLOAT3 point )
	{
		/* Triangle A */ XMVECTOR triAreaA = XMVector4Length( XMVector3Cross( XMLoadFloat3( &point ) - XMLoadFloat3( &position ), 
											 XMLoadFloat3( &point ) - ( XMLoadFloat3( &XMFLOAT3( position.x, position.y, position.z - height ) ) ) ) ) / 2;

		/* Triangle B */ XMVECTOR triAreaB = XMVector4Length( XMVector3Cross( XMLoadFloat3( &point ) - XMLoadFloat3( &position ), 
											 XMLoadFloat3( &point ) - ( XMLoadFloat3( &XMFLOAT3( position.x + width, position.y, position.z ) ) ) ) ) / 2;

		/* Triangle	C */ XMVECTOR triAreaC = XMVector4Length( XMVector3Cross( XMLoadFloat3( &point ) - XMLoadFloat3 ( &XMFLOAT3( position.x + width, position.y, position.z ) ), 
											 XMLoadFloat3( &point ) - ( XMLoadFloat3( &XMFLOAT3( position.x + width, position.y, position.z - height ) ) ) ) ) / 2;

		/* Triangle D */ XMVECTOR triAreaD = XMVector4Length( XMVector3Cross( XMLoadFloat3( &point ) - XMLoadFloat3 ( &XMFLOAT3( position.x, position.y, position.z - height ) ), 
											 XMLoadFloat3( &point ) - ( XMLoadFloat3( &XMFLOAT3( position.x + width, position.y, position.z - height ) ) ) ) ) / 2;

		float areaSum = 0.0f;
		XMStoreFloat( &areaSum, ( triAreaA + triAreaB + triAreaC + triAreaD ) );

		//If area sum is greater than box area, point is outside box
		if( areaSum > ( width * height ) )
			return false;
		else
			return true;		
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

	//Use for moving objects, update position frame by frame
	BoundingCircle( float radius )
	{
		this->radius	= radius;
	}

	//Use for static objects
	BoundingCircle( XMFLOAT3 center, float radius )
	{
		this->center = center;
		this->radius = radius;
	}

	bool Intersect( BoundingCircle* inCircle ) const
	{
		return ( pow( center.x - inCircle->center.x, 2 ) + pow( center.z - inCircle->center.z, 2 ) ) < pow( radius + inCircle->radius, 2 );
	}
};
#endif

