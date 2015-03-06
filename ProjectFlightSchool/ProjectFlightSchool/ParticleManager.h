#ifndef _PARTICLEMANAGER_H_
#define _PARTICLEMANAGER_H_

#include "ParticleSystem.h"


#define MAX_NR_OF_PARTICLES  500000


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

		bool RequestParticleSystem( size_t entityID, ParticleType particleType, XMFLOAT3 position, XMFLOAT3 direction, XMFLOAT3 initialVelocity = XMFLOAT3( 0.0f, 0.0f, 0.0f ) );
		bool DeactivateParticleSystem( size_t entityID, ParticleType particleType );

		void Initialize();
		void Release();
		void Reset();

		ParticleManager();
		~ParticleManager();
};
#endif