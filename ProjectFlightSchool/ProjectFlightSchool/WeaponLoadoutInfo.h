// Melee weapon animation alterations
#define HAMMER_SPEED_INCREASE 3.85f
#define CLAYMORE_SPEED_INCREASE 3.14f


//		LEVEL 1 WEAPON DEFINES

//////////////////////////////////
//		 RANGED WEAPONS
//////////////////////////////////
// Attack Rate		Low value = high fire speed
// Damage			Damage / projectile
// Range			Lifetime of projectile.
// Spread			A small random offset to projectile direction. Low value gives a straighter direction
// Area of effect   Area for grenade blas
// Projectile Speed	Travel speed of projectile. High value gives a fast projectile

// Minigun
#define MINIGUN_AR			0.13f								// Attack Rate
#define MINIGUN_DAMAGE		4.9f								// Damage
#define MINIGUN_RANGE		0.35f								// Range
#define MINIGUN_SPREAD		5.0f								// Spread
#define MINIGUN_AOE			0.0f								// Area of effect
#define MINIGUN_SPEED		40.0f								// Projectile Speed 30
#define MINIGUN_OFFSETS		XMFLOAT3( 0.75f, 0.5f, 1.4f )		// Offsets forward, right, up
#define MINIGUN_OVERHEAT_CD 3.0f								// Cooldown that occurs when the minigun overheats	
#define MINIGUN_OVERHEAT	2.0f								// Amount of heat the minigun get's for firing once
#define MINIGUN_SLOWDOWN	0.02f								// How much is the player slowed down
// Shotgun
#define SHOTGUN_AR			1.8f								// Attack Rate
#define SHOTGUN_DAMAGE		14.0f								// Damage
#define SHOTGUN_RANGE		0.23f								// Range
#define SHOTGUN_SPREAD		6.1f								// Spread
#define SHOTGUN_AOE			0.0f								// Area of effect
#define SHOTGUN_SPEED		40.0f								// Projectile Speed 35
#define SHOTGUN_OFFSETS		XMFLOAT3( 0.93f, 0.5f, 1.45f )		// Offsets forward, right, up
#define SHOTGUN_SLOWDOWN	0.4f								// How much is the player slowed down
// Grenade Launcher
#define GL_AR				2.2f								// Attack Rate
#define GL_DAMAGE			27.0f								// Damage
#define GL_RANGE			25.0f								// Range
#define GL_SPREAD			0.0f								// Spread
#define GL_AOE				2.0f								// Area of effect
#define GL_SPEED			20.0f								// Projectile Speed 10
#define GL_OFFSETS			XMFLOAT3( 0.92f, 0.5f, 1.45f )		// Offsets forward, right, up
#define GL_SLOWDOWN			0.15f								// How much is the player slowed down
// Sniper
#define SNIPER_AR			3.0f								// Attack Rate
#define SNIPER_DAMAGE		45.0f								// Damage
#define SNIPER_RANGE		1.0f								// Range
#define SNIPER_SPREAD		0.0f								// Spread
#define SNIPER_AOE			0.0f								// Area of effect
#define SNIPER_SPEED		40.0f								// Projectile Speed 90
#define SNIPER_OFFSETS		XMFLOAT3( 1.15f, 0.45f, 1.9f )		// Offsets forward, right, up
#define SNIPER_SLOWDOWN		0.55f								// How much is the player slowed down

//////////////////////////////////
//		 MELEE WEAPONS			
//////////////////////////////////

// Claymore
#define CLAYMORE_AR			3.2f								// Attack Rate
#define CLAYMORE_DAMAGE		20.0f								// Damage
#define CLAYMORE_RADIUS		2.45f								// Radius
#define CLAYMORE_SPREAD		8									// Spread
#define CLAYMORE_KB			1.0f								// Knockback
#define CLAYMORE_TTA		0.75f / CLAYMORE_SPEED_INCREASE		// Time till Attack
#define CLAYMORE_STUN		0.3f								// Stun
#define CLAYMORE_REACH		3.0f								// Reach
#define CLAYMORE_OFFSETS	XMFLOAT3( 0.8f, -0.65f, 1.3f )		// Offsets forward, right, up
#define CLAYMORE_SLOWDOWN	0.25f								// How much is the player slowed down
// Hammer

#define HAMMER_AR			4.0f								// Attack Rate Can't be lower than 3.5f
#define HAMMER_DAMAGE		25.0f								// Damage
#define HAMMER_RADIUS		2.0f								// Radius
#define HAMMER_SPREAD		20									// Spread
#define HAMMER_KB			3.0f								// Knockback
#define HAMMER_TTA			0.82f / HAMMER_SPEED_INCREASE		// Time till Attack Can't be changed
#define HAMMER_STUN			3.0f								// Stun
#define HAMMER_REACH		2.0f								// Reach
#define HAMMER_OFFSETS		XMFLOAT3( 1.5f, -0.70f, 1.3f )		// Offsets forward, right, up
#define HAMMER_SLOWDOWN		0.5f								// How much is the player slowed down
// Blowtorch

#define BLOWTORCH_AR		0.05f								// Attack Rate
#define BLOWTORCH_DAMAGE	6.0f								// Damage
#define BLOWTORCH_RADIUS	4.0f								// Radius
#define BLOWTORCH_SPREAD	3									// Spread
#define BLOWTORCH_KB		0.0f								// Knockback
#define BLOWTORCH_TTA		0.2f								// Time till Attack
#define BLOWTORCH_STUN		0.0f								// Stun
#define BLOWTORCH_REACH		1.0f								// Reach
#define BLOWTORCH_OFFSETS	XMFLOAT3( -0.35f, -0.65f, 1.3f )	// Offsets forward, right, up
#define BLOWTORCH_SLOWDOWN	0.01f								// How much is the player slowed down
// Saw

#define SAW_AR				1.5f								// Attack Rate
#define SAW_DAMAGE			17.0f								// Damage
#define SAW_RADIUS			2.8f								// Radius
#define SAW_SPREAD			4									// Spread
#define SAW_KB				0.0f								// Knockback
#define SAW_TTA				0.65f								// Time till Attack
#define SAW_STUN			0.0f								// Stun
#define SAW_REACH			1.3f								// Reach
#define SAW_OFFSETS			XMFLOAT3( -0.15f, -0.45f, 1.3f )	// Offsets forward, right, up
#define SAW_SLOWDOWN		0.15f								// How much is the player slowed down

//////////////////////////////////
//		 TURRET WEAPON
//////////////////////////////////

#define TURRET_AR			0.05f								// Attack Rate
#define TURRET_DAMAGE		4.0f								// Damage
#define TURRET_RANGE		0.8f								// Range
#define TURRET_SPREAD		0.1f								// Spread
#define TURRET_AOE			0.0f								// Area of effect
#define TURRET_SPEED		40.0f								// Projectile Speed 30
#define TURRET_OFFSETS		XMFLOAT3( 0.75f, 0.5f, 1.3f )		// Offsets forward, right, up
#define TURRET_TURNSPEED	60.0f								// Turn speed
