#ifndef HELPERFUNCTIONS_H
#define HELPERFUNCTIONS_H

#include <DirectXMath.h>
#include <string>

namespace HelperFunctions
{
	bool Inside2DTriangle( DirectX::XMFLOAT2 p, DirectX::XMFLOAT2 p0, DirectX::XMFLOAT2 p1, DirectX::XMFLOAT2 p2 );

	float Dist3Squared( DirectX::XMFLOAT3 p1, DirectX::XMFLOAT3 p2 );
	float Dist2Squared( DirectX::XMFLOAT2 p1, DirectX::XMFLOAT2 p2 );
	DirectX::XMFLOAT3 GetTriCenter( DirectX::XMFLOAT3 p1, DirectX::XMFLOAT3 p2, DirectX::XMFLOAT3 p3 );
	DirectX::XMFLOAT3 GetLineCenter( DirectX::XMFLOAT3 p1, DirectX::XMFLOAT3 p2 );
	float TriArea2( DirectX::XMFLOAT2 p1, DirectX::XMFLOAT2 p2, DirectX::XMFLOAT2 p3 );
	bool Float3Equal( DirectX::XMFLOAT3 p1, DirectX::XMFLOAT3 p2 );
	bool Float2Equal( DirectX::XMFLOAT2 p1, DirectX::XMFLOAT2 p2 );

	extern double PCFreq;
	extern __int64 CounterStart;
	void StartCounter();
	double GetCounter();
	void PrintCounter( std::string text );
}
#endif