#ifndef CBUFFERPERFRAME_H
#define CBUFFERPERFRAME_H

#include <DirectXMath.h>

struct CbufferPerFrame
{
	DirectX::XMMATRIX	viewMatrix;
	DirectX::XMMATRIX	projectionMatrix;
	DirectX::XMFLOAT4	cameraPosition;
	int					numPointLights;
	int					pad[3];
};

struct CbufferPerFrameShadow
{
	DirectX::XMMATRIX	viewMatrix;
	DirectX::XMMATRIX	projectionMatrix;
	DirectX::XMFLOAT4	cameraPosition;
};
#endif