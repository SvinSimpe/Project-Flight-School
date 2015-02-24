#include "HelperFunctions.h"
#include <sstream>

bool HelperFunctions::Inside2DTriangle( DirectX::XMFLOAT2 p, DirectX::XMFLOAT2 p0, DirectX::XMFLOAT2 p1, DirectX::XMFLOAT2 p2 )
{
	float A = (float)1 / 2 * ( -p1.y * p2.x + p0.y * ( -p1.x + p2.x ) + p0.x * ( p1.y - p2.y ) + p1.x * p2.y );
	int sign = A < 0 ? -1 : 1;
	float s = ( p0.y * p2.x - p0.x * p2.y + ( p2.y - p0.y ) * p.x + ( p0.x - p2.x ) * p.y ) * sign;
	float t = ( p0.x * p1.y - p0.y * p1.x + ( p0.y - p1.y ) * p.x + ( p1.x - p0.x ) * p.y ) * sign;

	return s > 0 && t > 0 && ( s + t ) < ( 2 * A * sign );
}

float HelperFunctions::DistSquared( DirectX::XMFLOAT3 p1, DirectX::XMFLOAT3 p2 )
{
	return powf( p2.x - p1.x, 2.0f ) + powf( p2.y - p1.y, 2.0f ) + powf( p2.z - p1.z, 2.0f );
}

DirectX::XMFLOAT3 HelperFunctions::GetCenter( DirectX::XMFLOAT3 p1, DirectX::XMFLOAT3 p2, DirectX::XMFLOAT3 p3 )
{
	return DirectX::XMFLOAT3( ( p1.x + p2.x + p3.x ) / 3.0f, ( p1.y + p2.y + p3.y ) / 3.0f, ( p1.z + p2.z + p3.z ) / 3.0f );
}

double HelperFunctions::PCFreq = 0.0;
__int64 HelperFunctions::CounterStart = 0;

void HelperFunctions::StartCounter()
{
	LARGE_INTEGER li;
	if( !QueryPerformanceFrequency( &li ) )
		OutputDebugStringA( "QueryPerformanceFrequency failed!\n" );

	PCFreq = double( li.QuadPart )/1000.0;

	QueryPerformanceCounter( &li );
	CounterStart = li.QuadPart;
}

void HelperFunctions::PrintCounter( std::string text )
{
	LARGE_INTEGER li;
	QueryPerformanceCounter( &li );
	double counter = (double)( li.QuadPart - CounterStart ) / PCFreq;
	std::ostringstream ss;
	ss << text << " " << counter << "ms.\n";
	OutputDebugStringA( ss.str().c_str() );
}