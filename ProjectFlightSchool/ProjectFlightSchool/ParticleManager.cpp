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
	
	mNrOfParticleSystemsPerType[Smoke] = 10;
	mNrOfParticleSystemsPerType[Fire]  = 10;
	mNrOfParticleSystemsPerType[Spark] = 10;
	mNrOfParticleSystemsPerType[Blood] = 10;

	//======= Allocate memory for Particle Systems =======
	mParticleSystems = new ParticleSystem**[NR_OF_PARTICLE_TYPES];
	for (size_t i = 0; i < NR_OF_PARTICLE_TYPES; i++)
	{
		mParticleSystems[i] = new ParticleSystem*[10];
		for (size_t j = 0; j < 10; j++)
			mParticleSystems[i][j] = nullptr;
	}

	//========== Initialize Particle Systems ==========
	//					-EXAMPLE-
	//	1. Allocate memory for a new Particle System
	//		mParticleSystems[Smoke][0] = new ParticleSystem();
	//
	//	2. Initialize it with( ParticleType, EmitRate, ParticleCount )
	//		mParticleSystems[Smoke][0]->Initialize( Smoke, 2.0f, 100 );



}

void ParticleManager::Release()
{
	for ( size_t i = 0; i < NR_OF_PARTICLE_TYPES; i++ )
	{
		for ( size_t j = 0; j < mNrOfParticleSystemsPerType[i]; j++ )
		{
			SAFE_RELEASE_DELETE( mParticleSystems[i][mNrOfParticleSystemsPerType[j]] );
		}
	}

	SAFE_DELETE( mNrOfActiveParticleSystemsPerType );
}

ParticleManager::ParticleManager()
{
	//mNrOfParticleSystems				= 0;
	//mNrOfActiveParticleSystems		= 0;
	mNrOfActiveParticleSystemsPerType	= nullptr;
	mParticleSystems					= nullptr;
}

ParticleManager::~ParticleManager()
{

}

#pragma endregion