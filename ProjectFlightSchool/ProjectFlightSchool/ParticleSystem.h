#ifndef _PARTICLESYSTEM_H_
#define _PARTICLESYSTEM_H_

#include "ParticleData.h"

enum ParticleType
{
	Smoke,
	Fire,
	Spark,
	Blood
};

struct ParticleSystem : public ParticleData
{
#pragma region Members

	size_t particleType = -1;
	bool	isActive	= false;

#pragma endregion

#pragma region Functions

	void Initialize( size_t particleType, float emitRate, size_t nrOfParticles )
	{
		this->particleType = particleType;
		ParticleData::Initialize( emitRate, nrOfParticles );
	}

	virtual void Emitter( size_t nrOfParticles )
	{
	
	}

	virtual void Update( float deltaTime )
	{
		//First instruction
		UpdateLifeTime( deltaTime );







		//Last instruction
		UpdatePosition( deltaTime );
	}

	virtual void Render( float deltaTime )
	{
	
	}

	void Release()
	{
		ParticleData::Release();
	}

#pragma endregion
};

#endif