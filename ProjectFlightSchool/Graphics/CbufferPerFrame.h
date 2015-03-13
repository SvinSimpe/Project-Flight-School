#ifndef CBUFFERPERFRAME_H
#define CBUFFERPERFRAME_H

#include <DirectXMath.h>

struct CbufferPerFrame
{
	DirectX::XMMATRIX	viewMatrix;
	DirectX::XMMATRIX	projectionMatrix;
	DirectX::XMFLOAT4	cameraPosition;
	float				shipPosAndRad[8];
	int					numPointLights;
	float				timeVariable;

	int					pad[2];
};

struct CbufferPerFrameShadow
{
	DirectX::XMMATRIX	viewMatrix;
	DirectX::XMMATRIX	projectionMatrix;
	DirectX::XMFLOAT4	cameraPosition;
};
#endif