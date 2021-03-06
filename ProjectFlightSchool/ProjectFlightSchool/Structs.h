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
	DirectX::XMFLOAT3	velocity;
	DirectX::XMFLOAT3	upperBodyDirection;
	bool				isAlive;
	std::string			playerName;
};

struct EvPlayerID
{
	unsigned int ID;
	unsigned int HP;
	unsigned int projectileID;
};

struct EvInitialize
{
	unsigned int	ID;
	unsigned int	team;
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
	float			  speed;
	float			  range;
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
	unsigned int		state;
	unsigned int		type;
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

struct EvPlayerAttack
{
	unsigned int ID;
	unsigned int armID;
	unsigned int animation;
};

struct EvSyncSapwn
{
	unsigned int		ID;
	DirectX::XMFLOAT3	position;
};

struct EvUpdateEnemyPosition
{
	unsigned int		ID;
	DirectX::XMFLOAT3	position;
	DirectX::XMFLOAT3	direction;
	bool				isAlive;
};

struct EvSetEnemyState
{
	unsigned int	ID;
	unsigned int	state;
};

struct EvEnemyProjectileDamage
{
	unsigned int	shooterID;
	unsigned int	projectileID;
	unsigned int	enemyID;
	float			damage;
};

struct EvEnemyAttackPlayer
{
	unsigned int playerID;
	float		 damage;
};

struct EvEnemyMeleeHit
{
	unsigned int			ID;
	float					damage;
	float					knockBack;
	DirectX::XMFLOAT3		direction;
	float					stun;
};

struct EvIDAndTime
{
	unsigned int playerID;
	float		 deltaTime;
};

#endif