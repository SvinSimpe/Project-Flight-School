#ifndef STRUCTS_H
#define STRUCTS_H

#include <DirectXMath.h>

struct Message
{
	char* msg = "";
};

struct EvPlayerUpdate
{
	unsigned int		id;
	DirectX::XMFLOAT3	lowerBodyPosition;
	DirectX::XMFLOAT3	lowerBodyDirection;
	unsigned int		lowerBodyAnimation;
	float				lowerBodyAnimationTime;
	DirectX::XMFLOAT3	upperBodyPosition;
	DirectX::XMFLOAT3	upperBodyDirection;
};

struct EvPlayerID
{
	unsigned int ID;
};

struct EvProjectileFired
{
	unsigned int ID;
	DirectX::XMFLOAT3 position;
	DirectX::XMFLOAT3 direction;
};
#endif