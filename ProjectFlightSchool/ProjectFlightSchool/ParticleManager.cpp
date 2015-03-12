#include "ParticleManager.h"
#include "RenderManager.h"

#pragma region Private functions

#pragma endregion

#pragma region Public functions

void ParticleManager::Update( float deltaTime )
{
	for ( int i = 0; i < NR_OF_PARTICLE_TYPES; i++ )
	{
		for ( int j = 0; j < mNrOfActiveParticleSystemsPerType[i]; j++ )
		{
			mParticleSystems[i][j]->Update( deltaTime );
		}
	}
}

void ParticleManager::Render()
{
	RenderManager::GetInstance()->AddParticleSystemToList( mParticleSystems, mNrOfActiveParticleSystemsPerType );
}

bool ParticleManager::RequestParticleSystem( size_t entityID, ParticleType particleType, XMFLOAT3 position, XMFLOAT3 direction, XMFLOAT3 initialVelocity )
{
	// Check if there is any available Particle System
	if( mNrOfActiveParticleSystems == mNrOfParticleSystems )
		OutputDebugStringA( "-- Maximum number of allocated Particle system reached --\n" );


	// Check if there is any available Particle System of requested type
	if( mNrOfActiveParticleSystemsPerType[particleType] == mMaxNrOfParticleSystemsPerType[particleType] )
	{
		//OutputDebugStringA( "-- Maximum number of Particle type reached --\n" );
	}

	// Check if entity already has a particle system of request type connected to it
	for ( int i = 0; i < mNrOfActiveParticleSystemsPerType[particleType]; i++ )
	{
		if( mParticleSystems[particleType][i]->entityParentID == entityID )
		{
			mParticleSystems[particleType][i]->Emitter( particleType, position, direction, initialVelocity );
			return true;
		}
	}

	if( mNrOfActiveParticleSystemsPerType[particleType] == mMaxNrOfParticleSystemsPerType[particleType])
		return false;

	// Activate requested Particle System type and connect entityID to it
	mParticleSystems[particleType][mNrOfActiveParticleSystemsPerType[particleType]++]->Activate( entityID, position, direction );
	mNrOfActiveParticleSystems++;
	mParticleSystems[particleType][mNrOfActiveParticleSystemsPerType[particleType] - 1 ]->Emitter( particleType, position, direction, initialVelocity );

	return true;
}

bool ParticleManager::DeactivateParticleSystem( size_t entityID, ParticleType particleType )
{
	for ( int i = 0; i < mNrOfActiveParticleSystemsPerType[particleType]; i++ )
	{
		if( mParticleSystems[particleType][i]->isEmitting && mParticleSystems[particleType][i]->entityParentID == entityID )
		{
			mParticleSystems[particleType][i]->Deactive();
			mNrOfActiveParticleSystemsPerType[particleType]--;
			mNrOfActiveParticleSystems--;
		}
	}
	return true;
}

void ParticleManager::Initialize()
{
	mNrOfParticleSystemsPerType			= new int[NR_OF_PARTICLE_TYPES];
	mMaxNrOfParticleSystemsPerType		= new int[NR_OF_PARTICLE_TYPES];
	mNrOfActiveParticleSystemsPerType	= new int[NR_OF_PARTICLE_TYPES];
	
	// For every new ParticleType initiate values below

	mNrOfParticleSystemsPerType[SpitterTrail]	= 0;
	mNrOfParticleSystemsPerType[GranateTrail]	= 0;
	mNrOfParticleSystemsPerType[SniperTrail]	= 0;
	mNrOfParticleSystemsPerType[Shell]			= 0;
	mNrOfParticleSystemsPerType[Debris]			= 0;
	mNrOfParticleSystemsPerType[FIRE]			= 0;
	mNrOfParticleSystemsPerType[Spark]			= 0;
	mNrOfParticleSystemsPerType[Spark_Robot]	= 0;
	mNrOfParticleSystemsPerType[Spark_Electric]	= 0;
	mNrOfParticleSystemsPerType[Blood]			= 0;
	mNrOfParticleSystemsPerType[MuzzleFlash]	= 0;
	mNrOfParticleSystemsPerType[Smoke_MiniGun]	= 0;
	mNrOfParticleSystemsPerType[Level_Up]		= 0;
	mNrOfParticleSystemsPerType[Level_Inner]	= 0;
	mNrOfParticleSystemsPerType[Spores]			= 0;
	mNrOfParticleSystemsPerType[Explosion]		= 0;
	mNrOfParticleSystemsPerType[ExplosionSmoke]	= 0;
	mNrOfParticleSystemsPerType[FireSmoke]		= 0;
	mNrOfParticleSystemsPerType[BlowTorchFire]	= 0;
	mNrOfParticleSystemsPerType[BlowTorchIdle]	= 0;
	mNrOfParticleSystemsPerType[Fire_Flies]		= 0;
	mNrOfParticleSystemsPerType[NormalSmoke]	= 0;
	mNrOfParticleSystemsPerType[Hammer_Effect]	= 0;

	mMaxNrOfParticleSystemsPerType[SpitterTrail]	= 6;
	mMaxNrOfParticleSystemsPerType[GranateTrail]	= 6;
	mMaxNrOfParticleSystemsPerType[SniperTrail]		= 15;
	mMaxNrOfParticleSystemsPerType[Shell]			= 6;
	mMaxNrOfParticleSystemsPerType[Debris]			= 10;
	mMaxNrOfParticleSystemsPerType[FIRE]			= 10;
	mMaxNrOfParticleSystemsPerType[Spark]			= 10;
	mMaxNrOfParticleSystemsPerType[Spark_Robot]		= 10;
	mMaxNrOfParticleSystemsPerType[Spark_Electric]	= 8;
	mMaxNrOfParticleSystemsPerType[Blood]			= 10;
	mMaxNrOfParticleSystemsPerType[MuzzleFlash]		= 8;
	mMaxNrOfParticleSystemsPerType[Smoke_MiniGun]	= 8;
	mMaxNrOfParticleSystemsPerType[Level_Up]		= 8;
	mMaxNrOfParticleSystemsPerType[Level_Inner]		= 8;
	mMaxNrOfParticleSystemsPerType[Spores]			= 1;
	mMaxNrOfParticleSystemsPerType[Explosion]		= 8;
	mMaxNrOfParticleSystemsPerType[ExplosionSmoke]	= 8;
	mMaxNrOfParticleSystemsPerType[FireSmoke]		= 8;
	mMaxNrOfParticleSystemsPerType[BlowTorchFire]	= 8;
	mMaxNrOfParticleSystemsPerType[BlowTorchIdle]	= 8;
	mMaxNrOfParticleSystemsPerType[Fire_Flies]		= 100;
	mMaxNrOfParticleSystemsPerType[NormalSmoke]		= 8;
	mMaxNrOfParticleSystemsPerType[Hammer_Effect]	= 8;

	mNrOfActiveParticleSystemsPerType[SpitterTrail]		= 0;
	mNrOfActiveParticleSystemsPerType[GranateTrail]		= 0;
	mNrOfActiveParticleSystemsPerType[SniperTrail]		= 0;
	mNrOfActiveParticleSystemsPerType[Shell]			= 0;
	mNrOfActiveParticleSystemsPerType[Debris]			= 0;
	mNrOfActiveParticleSystemsPerType[FIRE]				= 0;
	mNrOfActiveParticleSystemsPerType[Spark]			= 0;
	mNrOfActiveParticleSystemsPerType[Spark_Robot]		= 0;
	mNrOfActiveParticleSystemsPerType[Spark_Electric]	= 0;
	mNrOfActiveParticleSystemsPerType[Blood]			= 0;
	mNrOfActiveParticleSystemsPerType[MuzzleFlash]		= 0;
	mNrOfActiveParticleSystemsPerType[Smoke_MiniGun]	= 0;
	mNrOfActiveParticleSystemsPerType[Level_Up]			= 0;
	mNrOfActiveParticleSystemsPerType[Level_Inner]		= 0;
	mNrOfActiveParticleSystemsPerType[Spores]			= 0;
	mNrOfActiveParticleSystemsPerType[Explosion]		= 0;
	mNrOfActiveParticleSystemsPerType[ExplosionSmoke]	= 0;
	mNrOfActiveParticleSystemsPerType[BlowTorchFire]	= 0;
	mNrOfActiveParticleSystemsPerType[BlowTorchIdle]	= 0;
	mNrOfActiveParticleSystemsPerType[FireSmoke]		= 0;
	mNrOfActiveParticleSystemsPerType[Fire_Flies]		= 0;
	mNrOfActiveParticleSystemsPerType[NormalSmoke]		= 0;
	mNrOfActiveParticleSystemsPerType[Hammer_Effect]	= 0;

	//======= Allocate memory for Particle Systems =======
	mParticleSystems = new ParticleSystem**[NR_OF_PARTICLE_TYPES];
	for ( int i = 0; i < NR_OF_PARTICLE_TYPES; i++ )
	{
		mParticleSystems[i] = new ParticleSystem*[mMaxNrOfParticleSystemsPerType[i]];
		for ( int j = 0; j < mMaxNrOfParticleSystemsPerType[i]; j++ )
			mParticleSystems[i][j] = new ParticleSystem();
	}
	//========== Initialize Particle Systems ==========
	//					-EXAMPLE-
	//
	//	Initialize with ParticleType, Emitrate and ParticleCount
	//
	//	for ( int i = 0; i < mMaxNrOfParticleSystemsPerType[MuzzleFlash]; i++ )
	//	{
	//		mParticleSystems[MuzzleFlash][i]->Initialize( MuzzleFlash, 16.0f, 2000 );
	//		mNrOfParticleSystemsPerType[MuzzleFlash]++;
	//		mNrOfParticleSystems++;
	//	}

	for ( int i = 0; i < mMaxNrOfParticleSystemsPerType[SpitterTrail]; i++ )
	{
		mParticleSystems[SpitterTrail][i]->Initialize( SpitterTrail, 6.0f, 1000 );
		mNrOfParticleSystemsPerType[SpitterTrail]++;
		mNrOfParticleSystems++;
	}

	for ( int i = 0; i < mMaxNrOfParticleSystemsPerType[GranateTrail]; i++ )
	{
		mParticleSystems[GranateTrail][i]->Initialize( GranateTrail, 6.0f, 1000 );
		mNrOfParticleSystemsPerType[GranateTrail]++;
		mNrOfParticleSystems++;
	}

	for ( int i = 0; i < mMaxNrOfParticleSystemsPerType[SniperTrail]; i++ )
	{
		mParticleSystems[SniperTrail][i]->Initialize( SniperTrail, 100.0f, 200 );
		mNrOfParticleSystemsPerType[SniperTrail]++;
		mNrOfParticleSystems++;
	}

	for ( int i = 0; i < mMaxNrOfParticleSystemsPerType[Shell]; i++ )
	{
		mParticleSystems[Shell][i]->Initialize( Shell, 8.0f, 200 );
		mNrOfParticleSystemsPerType[Shell]++;
		mNrOfParticleSystems++;
	}

	for ( int i = 0; i < mMaxNrOfParticleSystemsPerType[Debris]; i++ )
	{
		mParticleSystems[Debris][i]->Initialize( Debris, 8.0f, 200 );
		mNrOfParticleSystemsPerType[Debris]++;
		mNrOfParticleSystems++;
	}

	for ( int i = 0; i < mMaxNrOfParticleSystemsPerType[FIRE]; i++ )
	{
		mParticleSystems[FIRE][i]->Initialize( FIRE, 8.0f, 256 );
		mNrOfParticleSystemsPerType[FIRE]++;
		mNrOfParticleSystems++;
	}

	for ( int i = 0; i < mMaxNrOfParticleSystemsPerType[Spark]; i++ )
	{
		mParticleSystems[Spark][i]->Initialize( Spark, 8.0f, 144 );
		mNrOfParticleSystemsPerType[Spark]++;
		mNrOfParticleSystems++;
	}

	for ( int i = 0; i < mMaxNrOfParticleSystemsPerType[Spark_Robot]; i++ )
	{
		mParticleSystems[Spark_Robot][i]->Initialize( Spark_Robot, 8.0f, 64 );
		mNrOfParticleSystemsPerType[Spark_Robot]++;
		mNrOfParticleSystems++;
	}

	for ( int i = 0; i < mMaxNrOfParticleSystemsPerType[Spark_Electric]; i++ )
	{
		mParticleSystems[Spark_Electric][i]->Initialize( Spark_Electric, 1.0f, 2 );
		mNrOfParticleSystemsPerType[Spark_Electric]++;
		mNrOfParticleSystems++;
	}

	for ( int i = 0; i < mMaxNrOfParticleSystemsPerType[Blood]; i++ )
	{
		mParticleSystems[Blood][i]->Initialize( Blood, 8.0f, 144 );
		mNrOfParticleSystemsPerType[Blood]++;
		mNrOfParticleSystems++;
	}

	for ( int i = 0; i < mMaxNrOfParticleSystemsPerType[MuzzleFlash]; i++ )
	{
		mParticleSystems[MuzzleFlash][i]->Initialize( MuzzleFlash, 6.0f, 64 );
		mNrOfParticleSystemsPerType[MuzzleFlash]++;
		mNrOfParticleSystems++;
	}

	for ( int i = 0; i < mMaxNrOfParticleSystemsPerType[Smoke_MiniGun]; i++ )
	{
		mParticleSystems[Smoke_MiniGun][i]->Initialize( Smoke_MiniGun, 4.0f, 64 );
		mNrOfParticleSystemsPerType[Smoke_MiniGun]++;
		mNrOfParticleSystems++;
	}

	for ( int i = 0; i < mMaxNrOfParticleSystemsPerType[Spores]; i++ )
	{
		mParticleSystems[Spores][i]->Initialize( Spores, 8.0f, 600 );
		mNrOfParticleSystemsPerType[Spores]++;
		mNrOfParticleSystems++;
	}

	for ( int i = 0; i < mMaxNrOfParticleSystemsPerType[FireSmoke]; i++ )
	{
		mParticleSystems[FireSmoke][i]->Initialize( FireSmoke, 15.0f, 512 );
		mNrOfParticleSystemsPerType[FireSmoke]++;
		mNrOfParticleSystems++;
	}

	for ( int i = 0; i < mMaxNrOfParticleSystemsPerType[BlowTorchFire]; i++ )
	{
		mParticleSystems[BlowTorchFire][i]->Initialize( BlowTorchFire, 16.0f, 128 );
		mNrOfParticleSystemsPerType[BlowTorchFire]++;
		mNrOfParticleSystems++;
	}
	
	for ( int i = 0; i < mMaxNrOfParticleSystemsPerType[BlowTorchIdle]; i++ )
	{
		mParticleSystems[BlowTorchIdle][i]->Initialize( BlowTorchIdle, 12.0f, 102 );
		mNrOfParticleSystemsPerType[BlowTorchIdle]++;
		mNrOfParticleSystems++;
	}

	for ( int i = 0; i < mMaxNrOfParticleSystemsPerType[Level_Up]; i++ )
	{
		mParticleSystems[Level_Up][i]->Initialize( Level_Up, 512.0f, 1024 );
		mNrOfParticleSystemsPerType[Level_Up]++;
		mNrOfParticleSystems++;
	}

	for ( int i = 0; i < mMaxNrOfParticleSystemsPerType[Level_Inner]; i++ )
	{
		mParticleSystems[Level_Inner][i]->Initialize( Level_Inner, 32.0f, 256 );
		mNrOfParticleSystemsPerType[Level_Inner]++;
		mNrOfParticleSystems++;
	}

	for ( int i = 0; i < mMaxNrOfParticleSystemsPerType[Explosion]; i++ )
	{
		mParticleSystems[Explosion][i]->Initialize( Explosion, 20.0f, 1000 );
		mNrOfParticleSystemsPerType[Explosion]++;
		mNrOfParticleSystems++;
	}

	for ( int i = 0; i < mMaxNrOfParticleSystemsPerType[ExplosionSmoke]; i++ )
	{
		mParticleSystems[ExplosionSmoke][i]->Initialize( ExplosionSmoke, 20.0f, 100 );
		mNrOfParticleSystemsPerType[ExplosionSmoke]++;
		mNrOfParticleSystems++;
	}

	for ( int i = 0; i < mMaxNrOfParticleSystemsPerType[NormalSmoke]; i++ )
	{
		mParticleSystems[NormalSmoke][i]->Initialize( NormalSmoke, 4.0f, 48 );
		mNrOfParticleSystemsPerType[NormalSmoke]++;
		mNrOfParticleSystems++;
	}

	for ( int i = 0; i < mMaxNrOfParticleSystemsPerType[Fire_Flies]; i++ )
	{
		mParticleSystems[Fire_Flies][i]->Initialize( Fire_Flies, 8.0f, 10 );
		mNrOfParticleSystemsPerType[Fire_Flies]++;
		mNrOfParticleSystems++;
	}

	for ( int i = 0; i < mMaxNrOfParticleSystemsPerType[Hammer_Effect]; i++ )
	{
		mParticleSystems[Hammer_Effect][i]->Initialize( Hammer_Effect, 64.0f, 256 );
		mNrOfParticleSystemsPerType[Hammer_Effect]++;
		mNrOfParticleSystems++;
	}
}

void ParticleManager::Release()
{
	for ( int i = 0; i < NR_OF_PARTICLE_TYPES; i++ )
	{
		for ( int j = 0; j < mMaxNrOfParticleSystemsPerType[i]; j++ )
		{
			SAFE_RELEASE_DELETE( mParticleSystems[i][j] );
		}
		delete [] mParticleSystems[i];
	}
	delete [] mParticleSystems;

	SAFE_DELETE( mNrOfParticleSystemsPerType );
	SAFE_DELETE( mMaxNrOfParticleSystemsPerType );
	SAFE_DELETE( mNrOfActiveParticleSystemsPerType );
}

void ParticleManager::Reset()
{
	Release();
	Initialize();
}

ParticleManager::ParticleManager()
{
	mParticleSystems					= nullptr;
	mNrOfParticleSystems				= 0;
	mNrOfActiveParticleSystems			= 0;
	mNrOfParticleSystemsPerType			= nullptr;
	mMaxNrOfParticleSystemsPerType		= nullptr;
	mNrOfActiveParticleSystemsPerType	= nullptr;
}

ParticleManager::~ParticleManager()
{

}

#pragma endregion