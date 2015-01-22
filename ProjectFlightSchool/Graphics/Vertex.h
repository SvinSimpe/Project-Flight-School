#ifndef VERTEX_H
#define VERTEX_H

#include <DirectXMath.h>

struct Vertex24
{
	float position[3];
	float normal[3];
};
struct StaticVertex
{
	float position[3];
	float normal[3];
	float tangent[3];
	float uv[2];
};

struct StaticInstance
{
	DirectX::XMFLOAT4X4 world;
};

struct AnimatedVertex
{
	float position[3];
	float normal[3];
	float tangent[3];
	float uv[2];

	float weights[4];
	UINT jointIndex[4];
};

struct AnimatedInstance
{
	DirectX::XMFLOAT4X4 world;
	UINT instanceId;
};
#endif