#ifndef LIGHTSTRUCTURE_H
#define LIGHTSTRUCTURE_H

#include <DirectXMath.h>

#define MAX_NUM_POINTLIGHTS 200

struct PointLight
{
	DirectX::XMFLOAT4 positionAndIntensity;
	DirectX::XMFLOAT4 colorAndRadius;
};

struct LightStructure
{
	PointLight pointLights[MAX_NUM_POINTLIGHTS];
};
#endif