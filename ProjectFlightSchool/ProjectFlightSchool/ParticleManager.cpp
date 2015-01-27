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
			mParticleSystems[i][mNrOfActiveParticleSystemsPerType[j]]->Update( deltaTime );
		}
	}
}

void ParticleManager::Render( float deltaTime )
{

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