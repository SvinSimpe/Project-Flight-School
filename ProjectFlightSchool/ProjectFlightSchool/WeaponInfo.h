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
	float			damage;
	float			range;
	float			spread;
	float			areaOfEffect;
	float			projectileSpeed;

	RangedInfo()
	{
		srand( (UINT)time( NULL ) );
		weaponType		= SHOTGUN;
		level			= 1;
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
		level				= 1;

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
				/*attackRate		= 0.005f;
				damage			= 2.0f;
				range			= 0.1f;
				spread			= 0.2f;
				areaOfEffect	= 0.0f;
				projectileSpeed	= 60.0f;*/
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
			case TURRET:
			{
				attackRate		=	TURRET_AR;
				damage			=	TURRET_DAMAGE;
				range			=	TURRET_RANGE;
				spread			=	TURRET_SPREAD;
				areaOfEffect	=	TURRET_AOE;
				projectileSpeed	=	TURRET_SPEED;
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

	void Reset()
	{
		level	= 1;

		switch( weaponType )
		{
			case MINIGUN:
			{
				/*attackRate		=	MINIGUN_AR;
				damage			=	MINIGUN_DAMAGE;
				range			=	MINIGUN_RANGE;
				spread			=	MINIGUN_SPREAD;
				areaOfEffect	=	MINIGUN_AOE;
				projectileSpeed	=	MINIGUN_SPEED;*/
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
						attackRate		= 0.04f;
						damage			= 1.5f;
						range			= 1.0f;
						spread			= 0.15f;
						areaOfEffect	= 0.0f;
						projectileSpeed	= 60.0f;
					}
						break;

					case 3:
					{
						attackRate		= 0.03f;
						damage			= 2.0f;
						range			= 1.5f;
						spread			= 0.15f;
						areaOfEffect	= 0.0f;
						projectileSpeed	= 60.0f;
					}
						break;

					case 4:
					{
						attackRate		= 0.02f;
						damage			= 2.8f;
						range			= 2.0f;
						spread			= 0.15f;
						areaOfEffect	= 0.0f;
						projectileSpeed	= 60.0f;
					}
						break;

					case 5:
					{
						//attackRate		= 0.01f;
						//damage			= 3.0f;
						//range			= 0.5f;
						//spread			= 0.2f;
						//areaOfEffect	= 0.0f;
						//projectileSpeed	= 60.0f;

						attackRate		= 0.02f;
						damage			= 4.0f;
						range			= 2.0f;
						spread			= 0.15f;
						areaOfEffect	= 0.0f;
						projectileSpeed	= 60.0f;
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
					}
						break;

					case 3:
					{
					}
						break;

					case 4:
					{
					}
						break;

					case 5:
					{
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
					}
						break;

					case 3:
					{
					}
						break;

					case 4:
					{
					}
						break;

					case 5:
					{
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
					}
						break;

					case 3:
					{
					}
						break;

					case 4:
					{
					}
						break;

					case 5:
					{
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
	float			damage;
	float			radius;
	float			spread;
	float			knockBack;
	float			timeTillAttack;		// From button press
	float			stun;
	float			reach;
	BoundingCircle*	boundingCircle;

	MeleeInfo()
	{
		weaponType		= CLAYMORE;
		level			= 1;
		attackRate		= 0.0f;
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
				damage			= CLAYMORE_DAMAGE;
				radius			= CLAYMORE_RADIUS;
				reach			= CLAYMORE_REACH;
				spread			= CLAYMORE_SPREAD;
				knockBack		= CLAYMORE_KB;
				boundingCircle	= new BoundingCircle( CLAYMORE_RADIUS );
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
				reach			= HAMMER_REACH;
				knockBack		= HAMMER_KB;
				boundingCircle	= new BoundingCircle( HAMMER_RADIUS );
				timeTillAttack	= HAMMER_TTA;
				stun			= HAMMER_STUN;
			}
				break;

			case BLOWTORCH:
			{
				attackRate		= BLOWTORCH_AR;
				damage			= BLOWTORCH_DAMAGE;
				radius			= BLOWTORCH_RADIUS;
				reach			= BLOWTORCH_REACH;
				spread			= BLOWTORCH_SPREAD;
				knockBack		= BLOWTORCH_KB;
				boundingCircle	= new BoundingCircle( BLOWTORCH_RADIUS );
				timeTillAttack	= BLOWTORCH_TTA;
				stun			= BLOWTORCH_STUN;
			}
				break;

			case SAW:
			{
				attackRate		= SAW_AR;
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
				damage			= HAMMER_DAMAGE;
				radius			= HAMMER_RADIUS;
				reach			= HAMMER_REACH;
				spread			= HAMMER_SPREAD;
				knockBack		= HAMMER_KB;
				boundingCircle	= new BoundingCircle( HAMMER_RADIUS );
				timeTillAttack	= HAMMER_TTA;
				stun			= HAMMER_STUN;
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
					}
						break;

					case 3:
					{
					}
						break;

					case 4:
					{
					}
						break;

					case 5:
					{
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
						damage			= 15.0f;
						radius			= 5.0f;
						spread			= 7;
						knockBack		= 3.2f;
						boundingCircle	= new BoundingCircle( HAMMER_REACH );
						stun			= 3.3f;
					}
						break;

					case 3:
					{
						damage			= 18.0f;
						radius			= 5.0f;
						spread			= 7;
						knockBack		= 3.5f;
						boundingCircle	= new BoundingCircle( HAMMER_REACH );
						stun			= 3.5f;
					}
						break;

					case 4:
					{
						damage			= 24.0f;
						radius			= 5.0f;
						spread			= 7;
						knockBack		= 3.8f;
						boundingCircle	= new BoundingCircle( HAMMER_REACH );
						stun			= 3.5f;
					}
						break;

					case 5:
					{
						damage			= 40.0f;
						radius			= 5.0f;
						spread			= 7;
						knockBack		= 4.0f;
						boundingCircle	= new BoundingCircle( HAMMER_REACH );
						stun			= 3.5f;
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
					}
						break;

					case 3:
					{
					}
						break;

					case 4:
					{
					}
						break;

					case 5:
					{
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
					}
						break;

					case 3:
					{
					}
						break;

					case 4:
					{
					}
						break;

					case 5:
					{
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