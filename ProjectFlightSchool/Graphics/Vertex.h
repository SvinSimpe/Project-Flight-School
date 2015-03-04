#ifndef VERTEX_H
#define VERTEX_H

#include <DirectXMath.h>

struct Vertex12
{
	float position[3];
};

struct BillboardInstanced
{
	float position[3];
	float width;
	float height;
};

struct ParticleVertex16
{
	float position[3];
	float age;
	float timeTillDeath;
	float randomRotation;
};

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

struct CircleVertex
{
	float position[3];
	float radius;
	float color[3];
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