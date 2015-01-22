#ifndef _WEAPONINFO_H_
#define _WEAPONINFO_H_

#include "BoundingGeometry.h"

struct RangedInfo
{
	char*	weaponName;
	float	attackRate;
	float	damage;
	float	range;
	int		spread;
	float	areaOfEffect;

	RangedInfo()
	{
		weaponName		= "";
		attackRate		= 0.0f;
		damage			= 0.0f;
		range			= 0.0f;
		spread			= 0;
		areaOfEffect	= 0.0f;
	}

	RangedInfo( char* weaponName, float attackRate, float damage,
				float range, int spread, float areaOfEffect )
	{
		this->weaponName	= weaponName;
		this->attackRate	= attackRate;
		this->damage		= damage;
		this->range			= range;
		this->spread		= spread;
		this->areaOfEffect	= areaOfEffect;
	}
};
struct MeleeInfo
{
	char*			weaponName;
	float			attackRate;
	float			damage;
	float			radius;
	int				spread;
	float			knockBack;
	BoundingCircle*	boundingCircle;

	MeleeInfo()
	{
		weaponName		= "";
		attackRate		= 0.0f;
		damage			= 0.0f;
		radius			= 0.0f;
		spread			= 0;
		knockBack		= 0.0f;
		boundingCircle	= nullptr;
	}

	MeleeInfo( char* weaponName, float attackRate, float damage,
			   float radius, int spread, float knockBack, BoundingCircle* boundingCircle )
	{
		this->weaponName		= weaponName;
		this->attackRate		= attackRate;
		this->damage			= damage;
		this->radius			= radius;
		this->spread			= spread;
		this->knockBack			= knockBack;
		this->boundingCircle	= boundingCircle;
	}
};
#endif