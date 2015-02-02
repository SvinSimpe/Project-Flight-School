#ifndef _WEAPONINFO_H_
#define _WEAPONINFO_H_

#include "BoundingGeometry.h"

enum WeaponType
{
	BLOWTORCH,
	CLAYMORE,
	GRENADELAUNCHER,
	HAMMER,
	MINIGUN,
	SAW,
	SHOTGUN,
	SNIPER,
	WEAPON_COUNT,
};

struct RangedInfo
{
	WeaponType	weaponType;
	float		attackRate;
	float		damage;
	float		range;
	int			spread;
	float		areaOfEffect;

	RangedInfo()
	{
		weaponType		= SHOTGUN;
		attackRate		= 0.0f;
		damage			= 0.0f;
		range			= 0.0f;
		spread			= 0;
		areaOfEffect	= 0.0f;
	}

	RangedInfo( WeaponType weaponType, float attackRate, float damage,
				float range, int spread, float areaOfEffect )
	{
		this->weaponType	= weaponType;
		this->attackRate	= attackRate;
		this->damage		= damage;
		this->range			= range;
		this->spread		= spread;
		this->areaOfEffect	= areaOfEffect;
	}
};
struct MeleeInfo
{
	WeaponType		weaponType;
	float			attackRate;
	float			damage;
	float			radius;
	int				spread;
	float			knockBack;
	BoundingCircle*	boundingCircle;

	MeleeInfo()
	{
		weaponType		= CLAYMORE;
		attackRate		= 0.0f;
		damage			= 0.0f;
		radius			= 0.0f;
		spread			= 0;
		knockBack		= 0.0f;
		boundingCircle	= nullptr;
	}

	MeleeInfo( WeaponType weaponType, float attackRate, float damage,
			   float radius, int spread, float knockBack, BoundingCircle* boundingCircle )
	{
		this->weaponType		= weaponType;
		this->attackRate		= attackRate;
		this->damage			= damage;
		this->radius			= radius;
		this->spread			= spread;
		this->knockBack			= knockBack;
		this->boundingCircle	= boundingCircle;
	}
};
#endif