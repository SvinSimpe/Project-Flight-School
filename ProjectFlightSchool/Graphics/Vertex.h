#ifndef VERTEX_H
#define VERTEX_H

#include <DirectXMath.h>

struct Vertex
{
	float position[3];
	float normal[3];
	float tangent[3];
	float uv[2];
};
#endif