#ifndef _WEAPONINFO_H_
#define _WEAPONINFO_H_

struct RangedInfo
{
	char*	weaponName;
	float	attackRate;
	int		damage;
	float	range;
	int		spread;
	float	areaOfEffect;

	RangedInfo()
	{
		weaponName		= "";
		attackRate		= 0.0f;
		damage			= 0;
		range			= 0.0f;
		spread			= 0;
		areaOfEffect	= 0.0f;
	}

	RangedInfo( char* weaponName, float attackRate, int damage,
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
	char*	weaponName;
	float	attackRate;
	int		damage;
	int		spread;
	float	knockBack;

	MeleeInfo()
	{
		weaponName		= "";
		attackRate		= 0.0f;
		damage			= 0;
		spread			= 0;
		knockBack		= 0.0f;
	}

	MeleeInfo( char* weaponName, float attackRate, int damage,
			   int spread, float knockBack )
	{
		this->weaponName	= weaponName;
		this->attackRate	= attackRate;
		this->damage		= damage;
		this->spread		= spread;
		this->knockBack		= knockBack;
	}
};
#endif