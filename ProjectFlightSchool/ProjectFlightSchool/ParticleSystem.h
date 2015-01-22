#ifndef _PARTICLESYSTEM_H_
#define _PARTICLESYSTEM_H_

#include "ParticleData.h"

struct ParticleSystem : public ParticleData
{
	virtual void Emitter( size_t nrOfParticles )
	{
	
	}

	virtual void Update( float deltaTime )
	{
	
	}

	virtual void Render( float deltaTime )
	{
	
	}

	void Release()
	{
		ParticleData::Release();
	}
};

#endif