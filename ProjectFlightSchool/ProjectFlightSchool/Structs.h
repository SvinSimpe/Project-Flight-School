#ifndef STRUCTS_H
#define STRUCTS_H

#include <DirectXMath.h>

struct Message
{
	char* msg = "";
};

struct EvPlayerMoved
{
	unsigned int id;
	DirectX::XMFLOAT3 lowerBody;
	DirectX::XMFLOAT3 upperBody;
	DirectX::XMFLOAT3 direction;
};

struct EvPlayerID
{
	unsigned int ID;
	unsigned int HP;
	unsigned int projectileID;
};

struct EvInitialize
{
	unsigned int ID;
	unsigned int team;
};

struct EvProjectileFired
{
	unsigned int ID;
	unsigned int projectileID;
	DirectX::XMFLOAT3 position;
	DirectX::XMFLOAT3 direction;
};
#endif