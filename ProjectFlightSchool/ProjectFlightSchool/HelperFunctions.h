#ifndef HELPERFUNCTIONS_H
#define HELPERFUNCTIONS_H

#include <DirectXMath.h>


namespace HelperFunctions
{
	bool Inside2DTriangle( DirectX::XMFLOAT2 p, DirectX::XMFLOAT2 p0, DirectX::XMFLOAT2 p1, DirectX::XMFLOAT2 p2 );
}
#endif