#ifndef VERTEX_H
#define VERTEX_H

#include <DirectXMath.h>

struct Vertex
{
	DirectX::XMVECTOR position;
	DirectX::XMVECTOR normal;
	DirectX::XMVECTOR uv;
};
#endif