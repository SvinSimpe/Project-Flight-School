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
#define MINIGUN_DAMAGE		0.7f								// Damage
#define MINIGUN_RANGE		0.6f								// Range
#define MINIGUN_SPREAD		5.0f								// Spread
#define MINIGUN_AOE			0.0f								// Area of effect
#define MINIGUN_SPEED		45.0f								// Projectile Speed 30
#define MINIGUN_OFFSETS		XMFLOAT3( 0.75f, 0.5f, 1.3f )		// Offsets forward, right, up
#define MINIGUN_OVERHEAT_CD 2.0f								// Cooldown that occurs when the minigun overheats	
#define MINIGUN_OVERHEAT	1.0f								// Amount of heat the minigun get's for firing once

// Shotgun
#define SHOTGUN_AR			2.0f								// Attack Rate
#define SHOTGUN_DAMAGE		3.0f								// Damage
#define SHOTGUN_RANGE		0.3f								// Range
#define SHOTGUN_SPREAD		7.0f								// Spread
#define SHOTGUN_AOE			0.0f								// Area of effect
#define SHOTGUN_SPEED		30.0f								// Projectile Speed 35
#define SHOTGUN_OFFSETS		XMFLOAT3( 0.93f, 0.5f, 1.45f )		// Offsets forward, right, up

// Grenade Launcher
#define GL_AR				3.0f								// Attack Rate
#define GL_DAMAGE			3.0f								// Damage
#define GL_RANGE			25.0f								// Range
#define GL_SPREAD			0.0f								// Spread
#define GL_AOE				2.0f								// Area of effect
#define GL_SPEED			20.0f								// Projectile Speed 10
#define GL_OFFSETS			XMFLOAT3( 0.92f, 0.5f, 1.45f )		// Offsets forward, right, up

// Sniper
#define SNIPER_AR			3.2f								// Attack Rate
#define SNIPER_DAMAGE		4.0f								// Damage
#define SNIPER_RANGE		5.0f								// Range
#define SNIPER_SPREAD		0.0f								// Spread
#define SNIPER_AOE			0.0f								// Area of effect
#define SNIPER_SPEED		70.0f								// Projectile Speed 90
#define SNIPER_OFFSETS		XMFLOAT3( 1.15f, 0.45f, 1.9f )		// Offsets forward, right, up

//////////////////////////////////
//		 MELEE WEAPONS			
//////////////////////////////////

// Claymore
#define CLAYMORE_AR			1.0f								// Attack Rate
#define CLAYMORE_DAMAGE		2.0f								// Damage
#define CLAYMORE_RADIUS		5.0f								// Radius
#define CLAYMORE_SPREAD		7									// Spread
#define CLAYMORE_KB			1.0f								// Knockback
#define CLAYMORE_TTA		0.65f								// Time till Attack
#define CLAYMORE_STUN		0.3f								// Stun
#define CLAYMORE_REACH		3.0f								// Reach
#define CLAYMORE_OFFSETS	XMFLOAT3( 0.8f, -0.65f, 1.3f )		// Offsets forward, right, up

// Hammer
#define HAMMER_AR			3.0f								// Attack Rate Can't be lower than 3.5f
#define HAMMER_DAMAGE		10.0f								// Damage
#define HAMMER_RADIUS		2.0f								// Radius
#define HAMMER_SPREAD		20									// Spread
#define HAMMER_KB			3.0f								// Knockback
#define HAMMER_TTA			0.82f								// Time till Attack Can't be changed
#define HAMMER_STUN			3.0f								// Stun
#define HAMMER_REACH		2.0f								// Reach
#define HAMMER_OFFSETS		XMFLOAT3( 1.5f, -0.70f, 1.3f )		// Offsets forward, right, up

// Blowtorch
#define BLOWTORCH_AR		0.1f								// Attack Rate
#define BLOWTORCH_DAMAGE	5.0f								// Damage
#define BLOWTORCH_RADIUS	4.0f								// Radius
#define BLOWTORCH_SPREAD	3									// Spread
#define BLOWTORCH_KB		0.0f								// Knockback
#define BLOWTORCH_TTA		0.2f								// Time till Attack
#define BLOWTORCH_STUN		0.0f								// Stun
#define BLOWTORCH_REACH		1.0f								// Reach
#define BLOWTORCH_OFFSETS	XMFLOAT3( -0.35f, -0.65f, 1.3f )	// Offsets forward, right, up

// Saw
#define SAW_AR				3.0f								// Attack Rate
#define SAW_DAMAGE			5.0f								// Damage
#define SAW_RADIUS			5.0f								// Radius
#define SAW_SPREAD			7									// Spread
#define SAW_KB				2.0f								// Knockback
#define SAW_TTA				0.95f								// Time till Attack
#define SAW_STUN			0.0f								// Stun
#define SAW_REACH			1.3f								// Reach

//////////////////////////////////
//		 TURRET WEAPON
//////////////////////////////////

#define TURRET_AR			0.05f								// Attack Rate
#define TURRET_DAMAGE		0.6f								// Damage
#define TURRET_RANGE		0.8f								// Range
#define TURRET_SPREAD		0.1f								// Spread
#define TURRET_AOE			0.0f								// Area of effect
#define TURRET_SPEED		40.0f								// Projectile Speed 30
#define TURRET_OFFSETS		XMFLOAT3( 0.75f, 0.5f, 1.3f )		// Offsets forward, right, up
