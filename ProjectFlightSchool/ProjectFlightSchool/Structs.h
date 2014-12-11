#ifndef STRUCTS_H
#define STRUCTS_H

#include <DirectXMath.h>

struct Message
{
	char* msg = "";
};

struct Position
{
	int x = 0;
	int y = 0;
	int z = 0;
};

struct EvPlayerMoved
{
	DirectX::XMFLOAT3 lowerBody;
	DirectX::XMFLOAT3 upperBody;
};

struct EvPlayerConnection
{
	unsigned int ID;
};
#endif