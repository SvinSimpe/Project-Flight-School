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

	size_t		entityParentID		= -1;
	size_t		particleType		= -1;
	XMFLOAT3	emitterPosition		= XMFLOAT3( 0.0f, 0.0f, 0.0f );
	XMFLOAT3	emitterDirection	= XMFLOAT3( 0.0f, 0.0f, 0.0f );
	bool		isActive			= false;

#pragma endregion

#pragma region Functions

	void Initialize( size_t particleType, float emitRate, size_t nrOfParticles )
	{
		this->particleType		= particleType;
		ParticleData::Initialize( emitRate, nrOfParticles );
	}

	void Activate( size_t entityParentID, XMFLOAT3 emitterPosition, XMFLOAT3 emitterDirection )
	{
		this->entityParentID	= entityParentID;
		this->emitterPosition	= emitterPosition;
		this->emitterDirection	= emitterDirection;
		isActive				= true;
	}

	void Deactive()
	{
		entityParentID		= -1;
		emitterPosition		= XMFLOAT3( 0.0f, 0.0f, 0.0f );
		emitterDirection	= XMFLOAT3( 0.0f, 0.0f, 0.0f );
		isActive			= false;
	}

	virtual void Emitter( size_t nrOfParticles )
	{
	
	}

	virtual void Update( float deltaTime )
	{
		//First instruction
		UpdateLifeTime( deltaTime );







		//Last instruction
		//nrOfParticlesAlive = 10000;
		//__int64 ctr1 = 0, ctr2 = 0, ctr3 = 0, ctr4 = 0, ctr5 = 0, ctr6 = 0;
		//
		////MESSAURE
		//QueryPerformanceCounter((LARGE_INTEGER *)&ctr1);

		UpdatePosition( deltaTime );

		//QueryPerformanceCounter((LARGE_INTEGER *)&ctr2);

		//auto time1 = ctr2 - ctr1;

		////MESSAURE
		//QueryPerformanceCounter((LARGE_INTEGER *)&ctr3);

		//std::thread t1( &ParticleData::UpdateXPositionThreaded, this, deltaTime );
		//std::thread t2( &ParticleData::UpdateYPositionThreaded, this, deltaTime );
		//std::thread t3( &ParticleData::UpdateZPositionThreaded, this, deltaTime );

		//t1.join();
		//t2.join();
		//t3.join();

		//QueryPerformanceCounter((LARGE_INTEGER *)&ctr4);
		//auto time2 = ctr4 - ctr3;
		//
		////MESSAURE
		//QueryPerformanceCounter((LARGE_INTEGER *)&ctr5);

		//UpdatePositionNormal( deltaTime );

		//QueryPerformanceCounter((LARGE_INTEGER *)&ctr6);

		//auto time3 = ctr6 - ctr5;

		//int k = 4;

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