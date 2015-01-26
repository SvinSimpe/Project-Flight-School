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
	unsigned int HP;
	unsigned int projectileID;
};

struct EvInitialize
{
	unsigned int ID;
	unsigned int team;
};

struct EvKilled
{
	unsigned int ID;
	unsigned int killerID;
};

struct EvProjectileFired
{
	unsigned int ID;
	unsigned int projectileID;
	DirectX::XMFLOAT3 position;
	DirectX::XMFLOAT3 direction;
};
struct EvEnemyCreated
{
	unsigned int ID;
	unsigned int maxNrOfEnemies;
	DirectX::XMFLOAT3 position;
};

struct EvSyncEnemy
{
	unsigned int		ID;
	unsigned int		model;
	unsigned int		animation;
	float				hp;
	bool				isAlive;
	DirectX::XMFLOAT3	position;
	DirectX::XMFLOAT3	direction;
};
struct EvMeleeHit
{
	unsigned int			ID;
	float					damage;
	float					knockBack;
	DirectX::XMFLOAT3		direction;
};

#endif