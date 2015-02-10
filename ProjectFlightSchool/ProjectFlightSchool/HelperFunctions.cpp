#include "HelperFunctions.h"


bool HelperFunctions::Inside2DTriangle( DirectX::XMFLOAT2 p, DirectX::XMFLOAT2 p0, DirectX::XMFLOAT2 p1, DirectX::XMFLOAT2 p2 )
{
	float A = (float)1 / 2 * ( -p1.y * p2.x + p0.y * ( -p1.x + p2.x ) + p0.x * ( p1.y - p2.y ) + p1.x * p2.y );
	int sign = A < 0 ? -1 : 1;
	float s = ( p0.y * p2.x - p0.x * p2.y + ( p2.y - p0.y ) * p.x + ( p0.x - p2.x ) * p.y ) * sign;
	float t = ( p0.x * p1.y - p0.y * p1.x + ( p0.y - p1.y ) * p.x + ( p1.x - p0.x ) * p.y ) * sign;

	return s > 0 && t > 0 && ( s + t ) < ( 2 * A * sign );
}