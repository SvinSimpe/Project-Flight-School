#ifndef VERTEX_H
#define VERTEX_H

#include <DirectXMath.h>

struct StaticVertex
{
	float position[3];
	float normal[3];
	float tangent[3];
	float uv[2];
};

struct AnimateVertex
{
	float position[3];
	float normal[3];
	float tangent[3];
	float uv[2];

	float weights[4];
	float jointIndex[4];
};
#endif