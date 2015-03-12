#ifndef _WEAPONINFO_H_
#define _WEAPONINFO_H_

#include "BoundingGeometry.h"
#include "WeaponLoadoutInfo.h"
#include <stdlib.h>
#include <time.h>

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
	TURRET,
	WEAPON_COUNT,
};

struct RangedInfo
{
	WeaponType		weaponType;
	unsigned int	level;
	float			attackRate;
	float			slowDown;
	float			damage;
	float			range;
	float			spread;
	float			areaOfEffect;
	float			projectileSpeed;
	float			overheat;

	RangedInfo()
	{
		srand( (UINT)time( NULL ) );
		weaponType		= SHOTGUN;
		level			= 1;
		attackRate		= 0.0f;
		slowDown		= 0.0f;
		damage			= 0.0f;
		range			= 0.0f;
		spread			= 0.0f;
		areaOfEffect	= 0.0f;
		projectileSpeed	= 0.0f;
		overheat		= 0.0f;
	}

	RangedInfo( WeaponType weaponType )
	{
		this->weaponType	= weaponType;
		level				= 1;

		switch( weaponType )
		{
			case MINIGUN:
			{

				attackRate		=	MINIGUN_AR;
				slowDown		=	MINIGUN_SLOWDOWN;
				damage			=	MINIGUN_DAMAGE;
				range			=	MINIGUN_RANGE;
				spread			=	MINIGUN_SPREAD;
				areaOfEffect	=	MINIGUN_AOE;
				projectileSpeed	=	MINIGUN_SPEED;
				overheat		=	0.0f;
			}
				break;

			case SHOTGUN:
			{
				attackRate		=	SHOTGUN_AR;
				slowDown		=	SHOTGUN_SLOWDOWN;
				damage			=	SHOTGUN_DAMAGE;
				range			=	SHOTGUN_RANGE;
				spread			=	SHOTGUN_SPREAD;
				areaOfEffect	=	SHOTGUN_AOE;
				projectileSpeed	=	SHOTGUN_SPEED;
				overheat		=	0.0f;
			}
				break;

			case GRENADELAUNCHER:
			{
				attackRate		=	GL_AR;
				slowDown		=	GL_SLOWDOWN;
				damage			=	GL_DAMAGE;
				range			=	GL_RANGE;
				spread			=	GL_SPREAD;
				areaOfEffect	=	GL_AOE;
				projectileSpeed	=	GL_SPEED;
				overheat		=	0.0f;
			}
				break;

			case SNIPER:
			{
				attackRate		=	SNIPER_AR;
				slowDown		=	SNIPER_SLOWDOWN;
				damage			=	SNIPER_DAMAGE;
				range			=	SNIPER_RANGE;
				spread			=	SNIPER_SPREAD;
				areaOfEffect	=	SNIPER_AOE;
				projectileSpeed	=	SNIPER_SPEED;
				overheat		=	0.0f;
			}
				break;
			case TURRET:
			{
				attackRate		=	TURRET_AR;
				damage			=	TURRET_DAMAGE;
				range			=	TURRET_RANGE;
				spread			=	TURRET_SPREAD;
				areaOfEffect	=	TURRET_AOE;
				projectileSpeed	=	TURRET_SPEED;
				overheat		=	0.0f;
			}
				break;
			default:
			{
				attackRate		=	MINIGUN_AR;
				slowDown		=	MINIGUN_SLOWDOWN;
				damage			=	MINIGUN_DAMAGE;
				range			=	MINIGUN_RANGE;
				spread			=	MINIGUN_SPREAD;
				areaOfEffect	=	MINIGUN_AOE;
				projectileSpeed	=	MINIGUN_SPEED;
				overheat		=	0.0f;
			}
		}
	}

	void Reset()
	{
		level	= 1;

		switch( weaponType )
		{
			case MINIGUN:
			{
				attackRate		=	MINIGUN_AR;
				slowDown		=	MINIGUN_SLOWDOWN;
				damage			=	MINIGUN_DAMAGE;
				range			=	MINIGUN_RANGE;
				spread			=	MINIGUN_SPREAD;
				areaOfEffect	=	MINIGUN_AOE;
				projectileSpeed	=	MINIGUN_SPEED;
				overheat		=	0.0f;
			}
				break;

			case SHOTGUN:
			{
				attackRate		=	SHOTGUN_AR;
				slowDown		=	SHOTGUN_SLOWDOWN;
				damage			=	SHOTGUN_DAMAGE;
				range			=	SHOTGUN_RANGE;
				spread			=	SHOTGUN_SPREAD;
				areaOfEffect	=	SHOTGUN_AOE;
				projectileSpeed	=	SHOTGUN_SPEED;
				overheat		=	0.0f;
			}
				break;

			case GRENADELAUNCHER:
			{
				attackRate		=	GL_AR;
				slowDown		=	GL_SLOWDOWN;
				damage			=	GL_DAMAGE;
				range			=	GL_RANGE;
				spread			=	GL_SPREAD;
				areaOfEffect	=	GL_AOE;
				projectileSpeed	=	GL_SPEED;
				overheat		=	0.0f;
			}
				break;

			case SNIPER:
			{
				attackRate		=	SNIPER_AR;
				slowDown		=	SNIPER_SLOWDOWN;
				damage			=	SNIPER_DAMAGE;
				range			=	SNIPER_RANGE;
				spread			=	SNIPER_SPREAD;
				areaOfEffect	=	SNIPER_AOE;
				projectileSpeed	=	SNIPER_SPEED;
				overheat		=	0.0f;
			}
				break;

			default:
			{
				attackRate		=	MINIGUN_AR;
				slowDown		=	MINIGUN_SLOWDOWN;
				damage			=	MINIGUN_DAMAGE;
				range			=	MINIGUN_RANGE;
				spread			=	MINIGUN_SPREAD;
				areaOfEffect	=	MINIGUN_AOE;
				projectileSpeed	=	MINIGUN_SPEED;
				overheat		=	0.0f;
			}
		}
	}

	void LevelUp()
	{
		level++;
		switch( weaponType )
		{
			case MINIGUN:
			{
				switch( level )
				{
					case 2:
					{
						attackRate		= 0.12f;
						damage			= 5.3f;
						spread			= 4.7f;
						overheat		= 1.8f;
					}
						break;

					case 3:
					{
						attackRate		= 0.1f;
						damage			= 5.8f;
						spread			= 4.3f;
						overheat		= 1.6f;
					}
						break;

					case 4:
					{
						attackRate		= 0.09f;
						damage			= 6.5f;
						spread			= 4.0f;
						overheat		= 1.4f;
					}
						break;

					case 5:
					{
						attackRate		= 0.08f;
						damage			= 7.2f;
						spread			= 3.6f;
						overheat		= 1.2f;
					}
						break;

				}
			}
				break;

			case SHOTGUN:
			{
				switch( level )
				{
					case 2:
					{
						attackRate		= 1.6f;
						//damage			= 18.0f;
						spread			= 5.7f;
					}
						break;

					case 3:
					{
						attackRate		= 1.5f;
						//damage			= 20.0f;
						spread			= 5.4f;
					}
						break;

					case 4:
					{
						attackRate		= 1.4f;
						//damage			= 22.0f;
						spread			= 5.1f;
					}
						break;

					case 5:
					{
						attackRate		= 1.1f;
						//damage			= 25.0f;
						spread			= 4.8f;
					}
						break;

				}
			}
				break;

			case GRENADELAUNCHER:
			{
				switch( level )
				{
					case 2:
					{
						attackRate		= 2.8f;
						//damage			= 27.4f;
						areaOfEffect	= 2.5f;
					}
						break;

					case 3:
					{
						attackRate		= 2.5f;
						//damage			= 30.9f;
						areaOfEffect	= 3.0f;
					}
						break;

					case 4:
					{
						attackRate		= 2.2f;
						//damage			= 34.0f;
						areaOfEffect	= 3.5f;
					}
						break;

					case 5:
					{
						attackRate		= 2.0f;
						damage			= 35.0f;
						areaOfEffect	= 3.7f;
					}
						break;

				}
			}
				break;

			case SNIPER:
			{
				switch( level )
				{
					case 2:
					{
						attackRate		= 3.6f;
						damage			= 55.0f;
					}
						break;

					case 3:
					{
						attackRate		= 3.3f;
						damage			= 65.0f;
					}
						break;

					case 4:
					{
						attackRate		= 3.0f;
						damage			= 75.0f;
					}
						break;

					case 5:
					{
						attackRate		= 2.7f;
						damage			= 85.0f;
					}
						break;

				}
			}
				break;

			default:
			{
			}
		}
	}

	float GetRandomProjectileSpeed()
	{
		return projectileSpeed * (1.0f - (float)( rand() % 100 ) * 0.005f);
	}
};

struct MeleeInfo
{
	WeaponType		weaponType;
	unsigned int	level;
	float			attackRate;			// Cooldown from prev attack
	float			slowDown;
	float			damage;
	float			radius;
	float			spread;
	float			knockBack;
	float			timeTillAttack;		// From button press
	float			stun;
	float			reach;
	XMFLOAT3		offSet;
	BoundingCircle*	boundingCircle;

	MeleeInfo()
	{
		weaponType		= CLAYMORE;
		level			= 1;
		attackRate		= 0.0f;
		slowDown		= 0.0f;
		damage			= 0.0f;
		radius			= 0.0f;
		reach			= 0.0f;
		spread			= 0;
		knockBack		= 0.0f;
		boundingCircle	= nullptr;
		timeTillAttack	= 0.0f;
		stun			= 0.0f;
	}

	void Reset()
	{
		level	= 1;

		switch( weaponType )
		{
			case CLAYMORE:
			{
				attackRate		= CLAYMORE_AR;
				slowDown		= CLAYMORE_SLOWDOWN;
				damage			= CLAYMORE_DAMAGE;
				radius			= CLAYMORE_RADIUS;
				reach			= CLAYMORE_REACH;
				spread			= CLAYMORE_SPREAD;
				knockBack		= CLAYMORE_KB;
				boundingCircle	= new BoundingCircle( CLAYMORE_RADIUS );
				timeTillAttack	= CLAYMORE_TTA;
				stun			= CLAYMORE_STUN;
				offSet			= CLAYMORE_OFFSETS;
			}
				break;

			case HAMMER:
			{
				attackRate		= HAMMER_AR;
				slowDown		= HAMMER_SLOWDOWN;
				damage			= HAMMER_DAMAGE;
				radius			= HAMMER_RADIUS;
				spread			= HAMMER_SPREAD;
				reach			= HAMMER_REACH;
				knockBack		= HAMMER_KB;
				boundingCircle	= new BoundingCircle( HAMMER_RADIUS );
				timeTillAttack	= HAMMER_TTA;
				stun			= HAMMER_STUN;
				offSet			= HAMMER_OFFSETS;
			}
				break;

			case BLOWTORCH:
			{
				attackRate		= BLOWTORCH_AR;
				slowDown		= BLOWTORCH_SLOWDOWN;
				damage			= BLOWTORCH_DAMAGE;
				radius			= BLOWTORCH_RADIUS;
				reach			= BLOWTORCH_REACH;
				spread			= BLOWTORCH_SPREAD;
				knockBack		= BLOWTORCH_KB;
				boundingCircle	= new BoundingCircle( BLOWTORCH_RADIUS );
				timeTillAttack	= BLOWTORCH_TTA;
				stun			= BLOWTORCH_STUN;
				offSet			= BLOWTORCH_OFFSETS;
			}
				break;

			case SAW:
			{
				attackRate		= SAW_AR;
				slowDown		= SAW_SLOWDOWN;
				damage			= SAW_DAMAGE;
				radius			= SAW_RADIUS;
				reach			= SAW_REACH;
				spread			= SAW_SPREAD;
				knockBack		= SAW_KB;
				boundingCircle	= new BoundingCircle( SAW_RADIUS );
				timeTillAttack	= SAW_TTA;
				stun			= SAW_STUN;
			}
				break;

			default:
			{
				attackRate		= HAMMER_AR;
				slowDown		= HAMMER_SLOWDOWN;
				damage			= HAMMER_DAMAGE;
				radius			= HAMMER_RADIUS;
				reach			= HAMMER_REACH;
				spread			= HAMMER_SPREAD;
				knockBack		= HAMMER_KB;
				boundingCircle	= new BoundingCircle( HAMMER_RADIUS );
				timeTillAttack	= HAMMER_TTA;
				stun			= HAMMER_STUN;
				offSet			= HAMMER_OFFSETS;
			}
		}
	}

	MeleeInfo( WeaponType weaponType )
	{
		this->weaponType		= weaponType;
		level					= 1;

		switch( weaponType )
		{
			case CLAYMORE:
			{
				attackRate		= CLAYMORE_AR;
				slowDown		= CLAYMORE_SLOWDOWN;
				damage			= CLAYMORE_DAMAGE;
				radius			= CLAYMORE_RADIUS;
				reach			= CLAYMORE_REACH;
				spread			= CLAYMORE_SPREAD;
				knockBack		= CLAYMORE_KB;
				boundingCircle	= new BoundingCircle( CLAYMORE_RADIUS );
				timeTillAttack	= CLAYMORE_TTA;
				stun			= CLAYMORE_STUN;
				offSet			= CLAYMORE_OFFSETS;
			}
				break;

			case HAMMER:
			{
				attackRate		= HAMMER_AR;
				slowDown		= HAMMER_SLOWDOWN;
				damage			= HAMMER_DAMAGE;
				radius			= HAMMER_RADIUS;
				spread			= HAMMER_SPREAD;
				reach			= HAMMER_REACH;
				knockBack		= HAMMER_KB;
				boundingCircle	= new BoundingCircle( HAMMER_RADIUS );
				timeTillAttack	= HAMMER_TTA;
				stun			= HAMMER_STUN;
				offSet			= HAMMER_OFFSETS;
			}
				break;

			case BLOWTORCH:
			{
				attackRate		= BLOWTORCH_AR;
				slowDown		= BLOWTORCH_SLOWDOWN;
				damage			= BLOWTORCH_DAMAGE;
				radius			= BLOWTORCH_RADIUS;
				reach			= BLOWTORCH_REACH;
				spread			= BLOWTORCH_SPREAD;
				knockBack		= BLOWTORCH_KB;
				boundingCircle	= new BoundingCircle( BLOWTORCH_RADIUS );
				timeTillAttack	= BLOWTORCH_TTA;
				stun			= BLOWTORCH_STUN;
				offSet			= BLOWTORCH_OFFSETS;
			}
				break;

			case SAW:
			{
				attackRate		= SAW_AR;
				slowDown		= SAW_SLOWDOWN;
				damage			= SAW_DAMAGE;
				radius			= SAW_RADIUS;
				reach			= SAW_REACH;
				spread			= SAW_SPREAD;
				knockBack		= SAW_KB;
				boundingCircle	= new BoundingCircle( SAW_RADIUS );
				timeTillAttack	= SAW_TTA;
				stun			= SAW_STUN;
				offSet			= SAW_OFFSETS;
			}
				break;

			default:
			{
				attackRate		= HAMMER_AR;
				slowDown		= HAMMER_SLOWDOWN;
				damage			= HAMMER_DAMAGE;
				radius			= HAMMER_RADIUS;
				reach			= HAMMER_REACH;
				spread			= HAMMER_SPREAD;
				knockBack		= HAMMER_KB;
				boundingCircle	= new BoundingCircle( HAMMER_RADIUS );
				timeTillAttack	= HAMMER_TTA;
				stun			= HAMMER_STUN;
				offSet			= HAMMER_OFFSETS;
			}
		}
	}

	void LevelUp()
	{
		level++;
		switch( weaponType )
		{
			case CLAYMORE:
			{
				switch( level )
				{
					case 2:
					{
						damage					= 40.0f;
						knockBack				= 1.2f;
						stun					= 0.4f;

					}
						break;

					case 3:
					{
						damage					= 55.0f;
						knockBack				= 1.6f;
					}
						break;

					case 4:
					{
						damage					= 68.0f;
						knockBack				= 1.8f;
						stun					= 0.5f;
					}
						break;

					case 5:
					{
						damage					= 80.0f;
						knockBack				= 2.0f;
						stun					= 0.6f;
					}
						break;

				}
			}
				break;

			case HAMMER:
			{
				switch( level )
				{
					case 2:
					{
						attackRate				= 3.8f;
						damage					= 20.0f;
						radius					= 3.0f;
						spread					= 7;
						knockBack				= 3.2f;
						boundingCircle->radius	= radius;
						stun					= 3.3f;
					}
						break;

					case 3:
					{
						attackRate				= 3.7f;
						damage					= 30.0f;
						radius					= 3.5f;
						spread					= 7;
						knockBack				= 3.5f;
						boundingCircle->radius	= radius;
						stun					= 3.5f;
					}
						break;

					case 4:
					{
						attackRate				= 3.6f;
						damage					= 40.0;
						radius					= 4.0;
						spread					= 7;
						knockBack				= 3.8f;
						boundingCircle->radius	= radius;
						stun					= 3.5f;
					}
						break;

					case 5:
					{
						attackRate				= 3.5f;
						damage					= 50.0f;
						radius					= 4.5f;
						spread					= 7;
						knockBack				= 4.0f;
						boundingCircle->radius	= radius;
						stun					= 3.5f;
					}
						break;

				}
			}
				break;

			case BLOWTORCH:
			{
				switch( level )
				{
					case 2:
					{
						damage	= 10.0f;
					}
						break;	

					case 3:
					{
						damage	= 15.0f;
					}
						break;	

					case 4:
					{
						damage	= 23.0f;
					}
						break;	

					case 5:
					{
						damage	= 30.0f;
					}
						break;

				}
			}
				break;

			case SAW:
			{
				switch( level )
				{
					case 2:
					{
						damage	= 7.0f;
					}
						break;

					case 3:
					{
						damage	= 12.0f;
					}
						break;

					case 4:
					{
						damage	= 15.0f;
					}
						break;

					case 5:
					{
						damage	= 18.0f;
					}
						break;

				}
			}
				break;

			default:
			{
			}
		}
	}
};

struct LoadOut
{
	RangedInfo*	rangedWeapon;
	MeleeInfo*	meleeWeapon;

	LoadOut()
	{
		rangedWeapon	= nullptr;
		meleeWeapon		= nullptr;
	}

	void Release()
	{
		if( rangedWeapon )
		{
			delete rangedWeapon;
		}
		if( meleeWeapon )
		{
			if( meleeWeapon->boundingCircle )
				delete meleeWeapon->boundingCircle;
			delete meleeWeapon;
		}
	}
};
#endif