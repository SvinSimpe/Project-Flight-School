#ifndef _PARTICLEMANAGER_H_
#define _PARTICLEMANAGER_H_

#include "ParticleSystem.h"

#define SAFE_RELEASE_DELETE( x ) if( x ) { ( x )->Release(); delete x; ( x ) = nullptr; }
#define SAFE_DELETE( x ) if( x ) { delete x; ( x ) = nullptr; }

#define MAX_PARTICLE_SYSTEMS 50
#define NR_OF_PARTICLE_TYPES 4

class ParticleManager
{
	private:
		ParticleSystem***	mParticleSystems;
		size_t				mNrOfParticleSystems;
		size_t				mNrOfActiveParticleSystems;
		size_t*				mNrOfParticleSystemsPerType;
		size_t*				mNrOfActiveParticleSystemsPerType;

	public:
		void Update( float deltaTime );
		void Render( float deltaTime );

		bool RequestParticleSystem( size_t entityID, size_t particleType, XMFLOAT3 position, XMFLOAT3 direction );
		bool DeactivateParticleSystem( size_t entityID, size_t particleType );

		void Initialize();
		void Release();

		ParticleManager();
		~ParticleManager();
};
#endif