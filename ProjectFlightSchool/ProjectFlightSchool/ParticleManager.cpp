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

bool ParticleManager::RequestParticleSystem( size_t entityID, size_t particleType, XMFLOAT3 position, XMFLOAT3 direction )
{
	// Check is there is any available Particle System
	if( mNrOfActiveParticleSystems == mNrOfParticleSystems )
	{
		OutputDebugStringA( "-- Maximum number of allocated Particle system reached --" );
		return false;
	}

	// Check is there is any available Particle System of requested type
	if( mNrOfActiveParticleSystemsPerType[particleType] == mNrOfParticleSystemsPerType[particleType] )
	{
		OutputDebugStringA( "-- Maximum number of Particle type " );
		OutputDebugStringA( (LPCSTR)particleType );
		OutputDebugStringA( " reached --" );
		return false;
	}

	// Activate requested Particle System type and connect entityID to it
	mParticleSystems[particleType][mNrOfActiveParticleSystemsPerType[particleType]++]->Activate( entityID, position, direction );
	mNrOfActiveParticleSystems++;

	// Print activation info
	OutputDebugStringA( "-- ACTIVATED: Particle System of type '" );
	OutputDebugStringA( LPCSTR(particleType) );
	OutputDebugStringA( "' to entity '" );
	OutputDebugStringA( LPCSTR(entityID) );
	OutputDebugStringA( "' --" );
}

bool ParticleManager::DeactivateParticleSystem( size_t entityID, size_t particleType )
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
}

void ParticleManager::Initialize()
{
	// Function need to have nrOfPlayers to allocate Particle Systems for
	// each muzzle flash and melee attack swing

	mNrOfParticleSystemsPerType			= new size_t[NR_OF_PARTICLE_TYPES];
	mNrOfActiveParticleSystemsPerType	= new size_t[NR_OF_PARTICLE_TYPES];
	
	mNrOfParticleSystemsPerType[Smoke] = 0;
	mNrOfParticleSystemsPerType[Fire]  = 0;
	mNrOfParticleSystemsPerType[Spark] = 0;
	mNrOfParticleSystemsPerType[Blood] = 0;

	mNrOfActiveParticleSystemsPerType[Smoke] = 0;
	mNrOfActiveParticleSystemsPerType[Fire]  = 0;
	mNrOfActiveParticleSystemsPerType[Spark] = 0;
	mNrOfActiveParticleSystemsPerType[Blood] = 0;

	//======= Allocate memory for Particle Systems =======
	mParticleSystems = new ParticleSystem**[NR_OF_PARTICLE_TYPES];
	for (size_t i = 0; i < NR_OF_PARTICLE_TYPES; i++)
	{
		mParticleSystems[i] = new ParticleSystem*[mNrOfParticleSystemsPerType[i]];
		for (size_t j = 0; j < mNrOfParticleSystemsPerType[i]; j++)
			mParticleSystems[i][j] = nullptr;
	}

	//========== Initialize Particle Systems ==========
	//					-EXAMPLE-
	//	1. Allocate memory for a new Particle System
	//		mParticleSystems[Smoke][0] = new ParticleSystem();
	//
	//	2. Initialize it with( ParticleType, EmitRate, ParticleCount )
	//		mParticleSystems[Smoke][0]->Initialize( Smoke, 2.0f, 100 );
	//		mNrOfParticleSystems++;


	mParticleSystems[Smoke][0] = new ParticleSystem();
	mParticleSystems[Smoke][0]->Initialize( Smoke, 2.0f, 10000 );
	mNrOfActiveParticleSystemsPerType[Smoke]++;
	mNrOfParticleSystems++;

	mParticleSystems[Fire][0] = new ParticleSystem();
	mParticleSystems[Fire][0]->Initialize( Fire, 2.0f, 10000 );
	mNrOfActiveParticleSystemsPerType[Fire]++;
	mNrOfParticleSystems++;

	mParticleSystems[Blood][0] = new ParticleSystem();
	mParticleSystems[Blood][0]->Initialize( Blood, 2.0f, 10000 );
	mNrOfActiveParticleSystemsPerType[Blood]++;
	mNrOfParticleSystems++;
}

void ParticleManager::Release()
{
	for ( size_t i = 0; i < NR_OF_PARTICLE_TYPES; i++ )
	{
		for ( size_t j = 0; j < mNrOfParticleSystemsPerType[i]; j++ )
		{
			SAFE_RELEASE_DELETE( mParticleSystems[i][mNrOfParticleSystemsPerType[j]] );
		}
		delete [] mParticleSystems[i];
	}
	SAFE_DELETE( mNrOfParticleSystemsPerType );
	SAFE_DELETE( mNrOfActiveParticleSystemsPerType );
}

ParticleManager::ParticleManager()
{
	mParticleSystems					= nullptr;
	mNrOfParticleSystems				= 0;
	mNrOfActiveParticleSystems			= 0;
	mNrOfParticleSystemsPerType			= nullptr;
	mNrOfActiveParticleSystemsPerType	= nullptr;
}

ParticleManager::~ParticleManager()
{

}

#pragma endregion