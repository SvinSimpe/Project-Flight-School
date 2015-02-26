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

bool ParticleManager::RequestParticleSystem( size_t entityID, ParticleType particleType, XMFLOAT3 position, XMFLOAT3 direction )
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
			mParticleSystems[particleType][i]->Emitter( particleType, position, direction );
			return true;
		}
	}

	if( mNrOfActiveParticleSystemsPerType[particleType] == mMaxNrOfParticleSystemsPerType[particleType])
		return false;

	// Activate requested Particle System type and connect entityID to it
	mParticleSystems[particleType][mNrOfActiveParticleSystemsPerType[particleType]++]->Activate( entityID, position, direction );
	mNrOfActiveParticleSystems++;
	mParticleSystems[particleType][mNrOfActiveParticleSystemsPerType[particleType] - 1 ]->Emitter( particleType, position, direction );

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
	// Function need to have nrOfPlayers to allocate Particle Systems for
	// each muzzle flash and melee attack swing

	mNrOfParticleSystemsPerType			= new int[NR_OF_PARTICLE_TYPES];
	mMaxNrOfParticleSystemsPerType		= new int[NR_OF_PARTICLE_TYPES];
	mNrOfActiveParticleSystemsPerType	= new int[NR_OF_PARTICLE_TYPES];
	
	// For every new ParticleType initiate values below

	mNrOfParticleSystemsPerType[Smoke]			= 0;
	mNrOfParticleSystemsPerType[Fire]			= 0;
	mNrOfParticleSystemsPerType[Spark]			= 0;
	mNrOfParticleSystemsPerType[Blood]			= 0;
	mNrOfParticleSystemsPerType[MuzzleFlash]	= 0;
	mNrOfParticleSystemsPerType[Smoke_MiniGun]	= 0;
	mNrOfParticleSystemsPerType[Level_Up]		= 0;
	mNrOfParticleSystemsPerType[Level_Inner]	= 0;
	mNrOfParticleSystemsPerType[Test_Fountain]	= 0;
	mNrOfParticleSystemsPerType[Explosion]		= 0;
	mNrOfParticleSystemsPerType[ExplosionSmoke]	= 0;
	mNrOfParticleSystemsPerType[FireSmoke]		= 0;
	mNrOfParticleSystemsPerType[ExplosionSmoke]	= 0;
	mNrOfParticleSystemsPerType[NormalSmoke]	= 0;// Below this


	mMaxNrOfParticleSystemsPerType[Smoke]			= 1;
	mMaxNrOfParticleSystemsPerType[Fire]			= 1;
	mMaxNrOfParticleSystemsPerType[Spark]			= 10;
	mMaxNrOfParticleSystemsPerType[Blood]			= 10;
	mMaxNrOfParticleSystemsPerType[MuzzleFlash]		= 8;
	mMaxNrOfParticleSystemsPerType[Smoke_MiniGun]	= 8;
	mMaxNrOfParticleSystemsPerType[Level_Up]		= 5;
	mMaxNrOfParticleSystemsPerType[Level_Inner]		= 5;
	mMaxNrOfParticleSystemsPerType[Test_Fountain]	= 5;
	mMaxNrOfParticleSystemsPerType[Explosion]		= 1;
	mMaxNrOfParticleSystemsPerType[ExplosionSmoke]	= 3;
	mMaxNrOfParticleSystemsPerType[FireSmoke]		= 4;
	mMaxNrOfParticleSystemsPerType[NormalSmoke]		= 3;// Aswell as this

	mNrOfActiveParticleSystemsPerType[Smoke]			= 0;
	mNrOfActiveParticleSystemsPerType[Fire]				= 0;
	mNrOfActiveParticleSystemsPerType[Spark]			= 0;
	mNrOfActiveParticleSystemsPerType[Blood]			= 0;
	mNrOfActiveParticleSystemsPerType[MuzzleFlash]		= 0;
	mNrOfActiveParticleSystemsPerType[Smoke_MiniGun]	= 0;
	mNrOfActiveParticleSystemsPerType[Level_Up]			= 0;
	mNrOfActiveParticleSystemsPerType[Level_Inner]		= 0;
	mNrOfActiveParticleSystemsPerType[Test_Fountain]	= 0;
	mNrOfActiveParticleSystemsPerType[Explosion]		= 0;
	mNrOfActiveParticleSystemsPerType[ExplosionSmoke]	= 0;
	mNrOfActiveParticleSystemsPerType[FireSmoke]		= 0;
	mNrOfActiveParticleSystemsPerType[NormalSmoke]		= 0;// And this

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

	for ( int i = 0; i < mMaxNrOfParticleSystemsPerType[NormalSmoke]; i++ )
	{
		mParticleSystems[NormalSmoke][i]->Initialize( NormalSmoke, 4.0f, 48 );
		mNrOfParticleSystemsPerType[NormalSmoke]++;
		mNrOfParticleSystems++;
	}

	for ( int i = 0; i < mMaxNrOfParticleSystemsPerType[Fire]; i++ )
	{
		mParticleSystems[Fire][i]->Initialize( Fire, 8.0f, 256 );	//-------------------------------------------------emitter rate and particle count
		mNrOfParticleSystemsPerType[Fire]++;
		mNrOfParticleSystems++;
	}

	for ( int i = 0; i < mMaxNrOfParticleSystemsPerType[ExplosionSmoke]; i++ )
	{
		mParticleSystems[ExplosionSmoke][i]->Initialize( ExplosionSmoke, 50.0f, 1000 );
		mNrOfParticleSystemsPerType[ExplosionSmoke]++;
		mNrOfParticleSystems++;
	}

	for ( int i = 0; i < mMaxNrOfParticleSystemsPerType[Explosion]; i++ )
	{
		mParticleSystems[Explosion][i]->Initialize( Explosion, 20.0f, 1000 );
		mNrOfParticleSystemsPerType[Explosion]++;
		mNrOfParticleSystems++;
	}

	for ( int i = 0; i < mMaxNrOfParticleSystemsPerType[Level_Inner]; i++ )
	{
		mParticleSystems[Level_Inner][i]->Initialize( Level_Inner, 32.0f, 256 );
		mNrOfParticleSystemsPerType[Level_Inner]++;
		mNrOfParticleSystems++;
	}

	for ( int i = 0; i < mMaxNrOfParticleSystemsPerType[Level_Up]; i++ )
	{
		mParticleSystems[Level_Up][i]->Initialize( Level_Up, 1024.0f, 1024 );
		mNrOfParticleSystemsPerType[Level_Up]++;
		mNrOfParticleSystems++;
	}

	for ( int i = 0; i < mMaxNrOfParticleSystemsPerType[Spark]; i++ )
	{
		mParticleSystems[Spark][i]->Initialize( Spark, 8.0f, 144 );
		mNrOfParticleSystemsPerType[Spark]++;
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
		mParticleSystems[MuzzleFlash][i]->Initialize( MuzzleFlash, 1.0f, 64 );
		mNrOfParticleSystemsPerType[MuzzleFlash]++;
		mNrOfParticleSystems++;
	}

	for ( int i = 0; i < mMaxNrOfParticleSystemsPerType[Smoke_MiniGun]; i++ )
	{
		mParticleSystems[Smoke_MiniGun][i]->Initialize( Smoke_MiniGun, 4.0f, 64 );
		mNrOfParticleSystemsPerType[Smoke_MiniGun]++;
		mNrOfParticleSystems++;
	}

	for ( int i = 0; i < mMaxNrOfParticleSystemsPerType[Test_Fountain]; i++ )
	{
		mParticleSystems[Test_Fountain][i]->Initialize( Test_Fountain, 320.0f, 10000 );
		mNrOfParticleSystemsPerType[Test_Fountain]++;
		mNrOfParticleSystems++;
	}
	
	for ( int i = 0; i < mMaxNrOfParticleSystemsPerType[FireSmoke]; i++ )
	{
		mParticleSystems[FireSmoke][i]->Initialize( FireSmoke, 15.0f, 512 );	//-------------------------------------------------emitter rate and particle count
		mNrOfParticleSystemsPerType[FireSmoke]++;
		mNrOfParticleSystems++;
	}
	// Place initialize here!
	//-----------------------
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