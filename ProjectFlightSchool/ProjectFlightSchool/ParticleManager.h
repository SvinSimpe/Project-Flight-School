#ifndef _PARTICLEMANAGER_H_
#define _PARTICLEMANAGER_H_

#include "ParticleSystem.h"


#define MAX_NR_OF_PARTICLES  50000


class ParticleManager
{
	private:
		ParticleSystem***	mParticleSystems;
		int					mNrOfParticleSystems;
		int					mNrOfActiveParticleSystems;
		int*				mNrOfParticleSystemsPerType;
		int*				mMaxNrOfParticleSystemsPerType;
		int*				mNrOfActiveParticleSystemsPerType;
		
	public:
		void Update( float deltaTime );
		void Render();

		bool RequestParticleSystem( size_t entityID, ParticleType particleType, XMFLOAT3 position, XMFLOAT3 direction );
		bool DeactivateParticleSystem( size_t entityID, ParticleType particleType );

		void Initialize();
		void Release();

		ParticleManager();
		~ParticleManager();
};
#endif