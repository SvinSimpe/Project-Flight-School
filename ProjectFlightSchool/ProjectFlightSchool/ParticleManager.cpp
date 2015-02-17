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
		OutputDebugStringA( "-- Maximum number of Particle type reached --\n" );
	}

	// Check if entity already has a particle system of request type connected to it
	for ( int i = 0; i < mNrOfActiveParticleSystemsPerType[particleType]; i++ )
	{
		if( mParticleSystems[particleType][i]->entityParentID == entityID )
		{
			//OutputDebugStringA( "-- Entity already has a Particle system of request type connected to it --\n" );
			//OutputDebugStringA( "-- Sending new burst from emitter --\n" );
			mParticleSystems[particleType][i]->Emitter( particleType, position, direction );
			return true;
		}
	}

	// Activate requested Particle System type and connect entityID to it
	mParticleSystems[particleType][mNrOfActiveParticleSystemsPerType[particleType]++]->Activate( entityID, position, direction );
	mNrOfActiveParticleSystems++;
	mParticleSystems[particleType][mNrOfActiveParticleSystemsPerType[particleType] - 1 ]->Emitter( particleType, position, direction );

	// Request granted!
	OutputDebugStringA( "-- ACTIVATED: Particle System connected to entity --\n" );

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
	mNrOfParticleSystemsPerType[Test_Fountain]	= 0; // Below this
	

	mMaxNrOfParticleSystemsPerType[Smoke]			= 10;
	mMaxNrOfParticleSystemsPerType[Fire]			= 10;
	mMaxNrOfParticleSystemsPerType[Spark]			= 10;
	mMaxNrOfParticleSystemsPerType[Blood]			= 10;
	mMaxNrOfParticleSystemsPerType[MuzzleFlash]		= 10;
	mMaxNrOfParticleSystemsPerType[Smoke_MiniGun]	= 10;
	mMaxNrOfParticleSystemsPerType[Test_Fountain]	= 10; // Aswell as this

	mNrOfActiveParticleSystemsPerType[Smoke]			= 0;
	mNrOfActiveParticleSystemsPerType[Fire]				= 0;
	mNrOfActiveParticleSystemsPerType[Spark]			= 0;
	mNrOfActiveParticleSystemsPerType[Blood]			= 0;
	mNrOfActiveParticleSystemsPerType[MuzzleFlash]		= 0;
	mNrOfActiveParticleSystemsPerType[Smoke_MiniGun]	= 0;
	mNrOfActiveParticleSystemsPerType[Test_Fountain]	= 0; // And this

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

	
	for ( int i = 0; i < mMaxNrOfParticleSystemsPerType[Blood]; i++ )
	{
		mParticleSystems[Blood][i]->Initialize( Blood, 64.0f, 288 );
		mNrOfParticleSystemsPerType[Blood]++;
		mNrOfParticleSystems++;
	}

	for ( int i = 0; i < mMaxNrOfParticleSystemsPerType[MuzzleFlash]; i++ )
	{
		mParticleSystems[MuzzleFlash][i]->Initialize( MuzzleFlash, 4.0f, 512 );
		mNrOfParticleSystemsPerType[MuzzleFlash]++;
		mNrOfParticleSystems++;
	}

	for ( int i = 0; i < mMaxNrOfParticleSystemsPerType[Smoke_MiniGun]; i++ )
	{
		mParticleSystems[Smoke_MiniGun][i]->Initialize( Smoke_MiniGun, 32.0f, 2000 );
		mNrOfParticleSystemsPerType[Smoke_MiniGun]++;
		mNrOfParticleSystems++;
	}

	for ( int i = 0; i < mMaxNrOfParticleSystemsPerType[Test_Fountain]; i++ )
	{
		mParticleSystems[Test_Fountain][i]->Initialize( Test_Fountain, 4.0f, 128 );
		mNrOfParticleSystemsPerType[Test_Fountain]++;
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