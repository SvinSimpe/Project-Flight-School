#ifndef _WEAPONINFO_H_
#define _WEAPONINFO_H_

#include "BoundingGeometry.h"
#include "WeaponLoadoutInfo.h"

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
	float		spread;
	float		areaOfEffect;
	float		projectileSpeed;

	RangedInfo()
	{
		weaponType		= SHOTGUN;
		attackRate		= 0.0f;
		damage			= 0.0f;
		range			= 0.0f;
		spread			= 0.0f;
		areaOfEffect	= 0.0f;
		projectileSpeed	= 0.0f;
	}

	RangedInfo( WeaponType weaponType )
	{
		this->weaponType	= weaponType;

		switch( weaponType )
		{
			case MINIGUN:
			{
				attackRate		=	MINIGUN_AR;
				damage			=	MINIGUN_DAMAGE;
				range			=	MINIGUN_RANGE;
				spread			=	MINIGUN_SPREAD;
				areaOfEffect	=	MINIGUN_AOE;
				projectileSpeed	=	MINIGUN_SPEED;
			}
				break;

			case SHOTGUN:
			{
				attackRate		=	SHOTGUN_AR;
				damage			=	SHOTGUN_DAMAGE;
				range			=	SHOTGUN_RANGE;
				spread			=	SHOTGUN_SPREAD;
				areaOfEffect	=	SHOTGUN_AOE;
				projectileSpeed	=	SHOTGUN_SPEED;
			}
				break;

			case GRENADELAUNCHER:
			{
				attackRate		=	GL_AR;
				damage			=	GL_DAMAGE;
				range			=	GL_RANGE;
				spread			=	GL_SPREAD;
				areaOfEffect	=	GL_AOE;
				projectileSpeed	=	GL_SPEED;
			}
				break;

			case SNIPER:
			{
				attackRate		=	SNIPER_AR;
				damage			=	SNIPER_DAMAGE;
				range			=	SNIPER_RANGE;
				spread			=	SNIPER_SPREAD;
				areaOfEffect	=	SNIPER_AOE;
				projectileSpeed	=	SNIPER_SPEED;
			}
				break;

			default:
			{
				attackRate		=	MINIGUN_AR;
				damage			=	MINIGUN_DAMAGE;
				range			=	MINIGUN_RANGE;
				spread			=	MINIGUN_SPREAD;
				areaOfEffect	=	MINIGUN_AOE;
				projectileSpeed	=	MINIGUN_SPEED;
			}
		}
	}
};
struct MeleeInfo
{
	WeaponType		weaponType;
	float			attackRate;			// Cooldown from prev attack
	float			damage;
	float			radius;
	float			spread;
	float			knockBack;
	float			timeTillAttack;		// From button press
	float			stun;
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
		timeTillAttack	= 0.0f;
		stun			= 0.0f;
	}

	MeleeInfo( WeaponType weaponType )
	{
		this->weaponType		= weaponType;

		switch( weaponType )
		{
			case CLAYMORE:
			{
				attackRate		= CLAYMORE_AR;
				damage			= CLAYMORE_DAMAGE;
				radius			= CLAYMORE_RADIUS;
				spread			= CLAYMORE_SPREAD;
				knockBack		= CLAYMORE_KB;
				boundingCircle	= new BoundingCircle( CLAYMORE_REACH );
				timeTillAttack	= CLAYMORE_TTA;
				stun			= CLAYMORE_STUN;
			}
				break;

			case HAMMER:
			{
				attackRate		= HAMMER_AR;
				damage			= HAMMER_DAMAGE;
				radius			= HAMMER_RADIUS;
				spread			= HAMMER_SPREAD;
				knockBack		= HAMMER_KB;
				boundingCircle	= new BoundingCircle( HAMMER_REACH );
				timeTillAttack	= HAMMER_TTA;
				stun			= HAMMER_STUN;
			}
				break;

			case BLOWTORCH:
			{
				attackRate		= BLOWTORCH_AR;
				damage			= BLOWTORCH_DAMAGE;
				radius			= BLOWTORCH_RADIUS;
				spread			= BLOWTORCH_SPREAD;
				knockBack		= BLOWTORCH_KB;
				boundingCircle	= new BoundingCircle( BLOWTORCH_REACH );
				timeTillAttack	= BLOWTORCH_TTA;
				stun			= BLOWTORCH_STUN;
			}
				break;

			case SAW:
			{
				attackRate		= SAW_AR;
				damage			= SAW_DAMAGE;
				radius			= SAW_RADIUS;
				spread			= SAW_SPREAD;
				knockBack		= SAW_KB;
				boundingCircle	= new BoundingCircle( SAW_REACH );
				timeTillAttack	= SAW_TTA;
				stun			= SAW_STUN;
			}
				break;

			default:
			{
				attackRate		= HAMMER_AR;
				damage			= HAMMER_DAMAGE;
				radius			= HAMMER_RADIUS;
				spread			= HAMMER_SPREAD;
				knockBack		= HAMMER_KB;
				boundingCircle	= new BoundingCircle( HAMMER_REACH );
				timeTillAttack	= HAMMER_TTA;
				stun			= HAMMER_STUN;
			}
		}
	}
};
#endif