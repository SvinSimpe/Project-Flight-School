#ifndef STRUCTS_H
#define STRUCTS_H

#include <DirectXMath.h>

struct Message
{
	char* msg = "";
};

struct EvPlayerMoved
{
	DirectX::XMFLOAT3 lowerBody;
	DirectX::XMFLOAT3 upperBody;
	DirectX::XMFLOAT3 direction;
};

struct EvPlayerConnection
{
	unsigned int ID;
};
#endif