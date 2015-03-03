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
#define MINIGUN_AR			0.05f	// Attack Rate
#define MINIGUN_DAMAGE		2.0f	// Damage
#define MINIGUN_RANGE		0.8f	// Range
#define MINIGUN_SPREAD		0.1f	// Spread
#define MINIGUN_AOE			5.0f	// Area of effect
#define MINIGUN_SPEED		40.0f	// Projectile Speed 30
#define MINIGUN_OFFSETS		XMFLOAT3( 0.75f, 0.5f, 1.3f ) // Offsets forward, right, up
#define MINIGUN_OVERHEAT_CD 2.0f	// Cooldown that occurs when the minigun overheats	
#define MINIGUN_OVERHEAT	1.0f	// Amount of heat the minigun get's for firing once
// Shotgun
#define SHOTGUN_AR			1.0f	// Attack Rate
#define SHOTGUN_DAMAGE		3.0f	// Damage
#define SHOTGUN_RANGE		0.3f	// Range
#define SHOTGUN_SPREAD		0.0f	// Spread
#define SHOTGUN_AOE			0.0f	// Area of effect
#define SHOTGUN_SPEED		40.0f	// Projectile Speed 35
#define SHOTGUN_OFFSETS		XMFLOAT3( 0.93f, 0.5f, 1.45f ) // Offsets forward, right, up

// Grenade Launcher
#define GL_AR				1.0f	// Attack Rate
#define GL_DAMAGE			15.0f	// Damage
#define GL_RANGE			25.0f	// Range
#define GL_SPREAD			0.0f	// Spread
#define GL_AOE				2.0f	// Area of effect
#define GL_SPEED			20.0f	// Projectile Speed 10
#define GL_OFFSETS			XMFLOAT3( 0.92f, 0.5f, 1.45f ) // Offsets forward, right, up

// Sniper
#define SNIPER_AR			2.5f	// Attack Rate
#define SNIPER_DAMAGE		20.0f	// Damage
#define SNIPER_RANGE		5.0f	// Range
#define SNIPER_SPREAD		0.0f	// Spread
#define SNIPER_AOE			0.0f	// Area of effect
#define SNIPER_SPEED		90.0f	// Projectile Speed 90
#define SNIPER_OFFSETS		XMFLOAT3( 1.15f, 0.45f, 1.9f ) // Offsets forward, right, up

//////////////////////////////////
//		 MELEE WEAPONS
//////////////////////////////////d

// Claymore
#define CLAYMORE_AR			1.0f	// Attack Rate
#define CLAYMORE_DAMAGE		2.0f	// Damage
#define CLAYMORE_RADIUS		5.0f	// Radius
#define CLAYMORE_SPREAD		7		// Spread
#define CLAYMORE_KB			1.0f	// Knockback
#define CLAYMORE_TTA		0.65f	// Time till Attack
#define CLAYMORE_STUN		0.3f	// Stun
#define CLAYMORE_REACH		3.0f	// Reach

// Hammer
#define HAMMER_AR			3.5f	// Attack Rate Can't be lower than 3.5f
#define HAMMER_DAMAGE		5.0f	// Damage
#define HAMMER_RADIUS		1.0f	// Radius
#define HAMMER_SPREAD		7		// Spread
#define HAMMER_KB			3.0f	// Knockback
#define HAMMER_TTA			0.92f	// Time till Attack Can't be changed
#define HAMMER_STUN			3.0f	// Stun
#define HAMMER_REACH		2.0f	// Reach

// Blowtorch
#define BLOWTORCH_AR		3.0f	// Attack Rate
#define BLOWTORCH_DAMAGE	5.0f	// Damage
#define BLOWTORCH_RADIUS	5.0f	// Radius
#define BLOWTORCH_SPREAD	7		// Spread
#define BLOWTORCH_KB		2.0f	// Knockback
#define BLOWTORCH_TTA		0.95f	// Time till Attack
#define BLOWTORCH_STUN		0.0f	// Stun
#define BLOWTORCH_REACH		2.3f	// Reach

// Saw
#define SAW_AR				3.0f	// Attack Rate
#define SAW_DAMAGE			5.0f	// Damage
#define SAW_RADIUS			5.0f	// Radius
#define SAW_SPREAD			7		// Spread
#define SAW_KB				2.0f	// Knockback
#define SAW_TTA				0.95f	// Time till Attack
#define SAW_STUN			0.0f	// Stun
#define SAW_REACH			1.3f	// Reach

//////////////////////////////////
//		 TURRET WEAPON
//////////////////////////////////

#define TURRET_AR			0.05f	// Attack Rate
#define TURRET_DAMAGE		0.6f	// Damage
#define TURRET_RANGE		0.8f	// Range
#define TURRET_SPREAD		0.1f	// Spread
#define TURRET_AOE			0.0f	// Area of effect
#define TURRET_SPEED		40.0f	// Projectile Speed 30
#define TURRET_OFFSETS		XMFLOAT3( 0.75f, 0.5f, 1.3f ) // Offsets forward, right, up
