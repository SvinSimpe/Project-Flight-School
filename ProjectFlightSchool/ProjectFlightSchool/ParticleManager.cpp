#include "ParticleManager.h"

#pragma region Private functions

#pragma endregion

#pragma region Public functions

void ParticleManager::Update( float deltaTime )
{
	for ( size_t i = 0; i < NR_OF_PARTICLE_TYPES; i++ )
	{
		for (size_t j = 0; j < mNrOfActiveParticleSystemsPerType[i]; j++)
		{
			mParticleSystems[i][j]->Update( deltaTime );
		}
	}
}

void ParticleManager::Render( float deltaTime )
{
	for ( size_t i = 0; i < NR_OF_PARTICLE_TYPES; i++ )
	{
		for (size_t j = 0; j < mNrOfActiveParticleSystemsPerType[i]; j++)
		{
			mParticleSystems[i][j]->Render( deltaTime );
		}
	}
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
		return false;
	}

	// Check if entity already has a particle system of request type connected to it
	for ( size_t i = 0; i < mNrOfActiveParticleSystemsPerType[particleType]; i++ )
	{
		if( mParticleSystems[particleType][i]->entityParentID == entityID )
		{
			OutputDebugStringA( "-- Entity already has a Particle system of request type connected to it --\n" );
			OutputDebugStringA( "-- Sending new burst from emitter --\n" );
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
	for (size_t i = 0; i < mNrOfActiveParticleSystemsPerType[particleType]; i++)
	{
		if( mParticleSystems[particleType][i]->isActive && mParticleSystems[particleType][i]->entityParentID == entityID )
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

	mNrOfParticleSystemsPerType			= new size_t[NR_OF_PARTICLE_TYPES];
	mMaxNrOfParticleSystemsPerType		= new size_t[NR_OF_PARTICLE_TYPES];
	mNrOfActiveParticleSystemsPerType	= new size_t[NR_OF_PARTICLE_TYPES];
	
	mNrOfParticleSystemsPerType[Smoke]			= 0;
	mNrOfParticleSystemsPerType[Fire]			= 0;
	mNrOfParticleSystemsPerType[Spark]			= 0;
	mNrOfParticleSystemsPerType[Blood]			= 0;
	mNrOfParticleSystemsPerType[MuzzleFlash]	= 0;

	mMaxNrOfParticleSystemsPerType[Smoke]		= 10;
	mMaxNrOfParticleSystemsPerType[Fire]		= 10;
	mMaxNrOfParticleSystemsPerType[Spark]		= 10;
	mMaxNrOfParticleSystemsPerType[Blood]		= 10;
	mMaxNrOfParticleSystemsPerType[MuzzleFlash]	= 10;

	mNrOfActiveParticleSystemsPerType[Smoke]		= 0;
	mNrOfActiveParticleSystemsPerType[Fire]			= 0;
	mNrOfActiveParticleSystemsPerType[Spark]		= 0;
	mNrOfActiveParticleSystemsPerType[Blood]		= 0;
	mNrOfActiveParticleSystemsPerType[MuzzleFlash]	= 0;

	//======= Allocate memory for Particle Systems =======
	mParticleSystems = new ParticleSystem**[NR_OF_PARTICLE_TYPES];
	for (size_t i = 0; i < NR_OF_PARTICLE_TYPES; i++)
	{
		mParticleSystems[i] = new ParticleSystem*[mMaxNrOfParticleSystemsPerType[i]];
		for (size_t j = 0; j < mMaxNrOfParticleSystemsPerType[i]; j++)
			mParticleSystems[i][j] = new ParticleSystem();
	}

	//========== Initialize Particle Systems ==========
	//					-EXAMPLE-
	//	1. Allocate memory for a new Particle System
	//		mParticleSystems[Smoke][0] = new ParticleSystem();
	//
	//	2. Initialize it with( ParticleType, EmitRate, ParticleCount )
	//		mParticleSystems[Smoke][0]->Initialize( Smoke, 2.0f, 100 );
	//		mNrOfParticleSystems++;



	//mParticleSystems[Smoke][0]->Initialize( Smoke, 2.0f, 10000 );
	//mNrOfParticleSystemsPerType[Smoke]++;
	//mNrOfParticleSystems++;


	//mParticleSystems[Fire][0]->Initialize( Fire, 2.0f, 10000 );
	//mNrOfParticleSystemsPerType[Fire]++;
	//mNrOfParticleSystems++;


	//mParticleSystems[Blood][0]->Initialize( Blood, 2.0f, 10000 );
	//mNrOfParticleSystemsPerType[Blood]++;
	//mNrOfParticleSystems++;

	mParticleSystems[MuzzleFlash][0]->Initialize( MuzzleFlash, 2.0f, 10000 );
	mNrOfParticleSystemsPerType[MuzzleFlash]++;
	mNrOfParticleSystems++;
}

void ParticleManager::Release()
{
	for ( size_t i = 0; i < NR_OF_PARTICLE_TYPES; i++ )
	{
		for ( size_t j = 0; j < mMaxNrOfParticleSystemsPerType[i]; j++ )
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