#ifndef LIGHTSTRUCTURE_H
#define LIGHTSTRUCTURE_H

#include <DirectXMath.h>

#define NUM_POINTLIGHTS 8

struct PointLight
{
	DirectX::XMFLOAT4 position;
	DirectX::XMFLOAT4 colorAndRadius;
};

struct LightStructure
{
	PointLight pointLights[NUM_POINTLIGHTS];
};
#endif