#ifndef _PARTICLESYSTEM_H_
#define _PARTICLESYSTEM_H_

#include "ParticleData.h"

#include <Graphics.h>
#include <limits>



struct ParticleSystem : public ParticleData
{
#pragma region Members

	size_t		entityParentID		= std::numeric_limits<unsigned int>::infinity();
	size_t		particleType		= std::numeric_limits<unsigned int>::infinity();
	XMFLOAT3	emitterPosition		= XMFLOAT3( 0.0f, 0.0f, 0.0f );
	XMFLOAT3	emitterDirection	= XMFLOAT3( 0.0f, 0.0f, 0.0f );
	bool		isActive			= false;
	AssetID		assetID;

#pragma endregion

#pragma region Functions

	void Initialize( ParticleType particleType, float emitRate, size_t nrOfParticles )
	{
		this->particleType		= particleType;
		ParticleData::Initialize( emitRate, nrOfParticles );


		Graphics::GetInstance()->LoadStatic2dAsset( "../Content/Assets/PlaceHolderTextures/diffuse.png", assetID );
		// Load asset for particle type
		// - Shader
		// - Texture
		// - etc.
		//Graphics::
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
		entityParentID		= std::numeric_limits<unsigned int>::infinity();
		emitterPosition		= XMFLOAT3( 0.0f, 0.0f, 0.0f );
		emitterDirection	= XMFLOAT3( 0.0f, 0.0f, 0.0f );
		isActive			= false;
	}

	void Generate( ParticleType particleType, XMFLOAT3 emitterPosition, XMFLOAT3 emitterDirection, size_t particleCount, float spreadAngle )
	{
		// Check if there is enough particles to meet request
		if( ( particleCount + nrOfParticlesAlive ) > MAX_PARTICLES )
			particleCount = MAX_PARTICLES - nrOfParticlesAlive;

		// Particlecount must be multiple of 4
		while( particleCount % 4 != 0 )
			particleCount--;
		
		// Initialize particles

		SetLifeTime( 1, 2, particleCount );

		///==================
		// Use emitterDirection as base and randomize a different direction vector
		// with a maximum angle deviation


		///==================

		SetDirection( emitterDirection.x, emitterDirection.y, emitterDirection.z, particleCount, 60.0f );
		SetPosition( emitterPosition.x, emitterPosition.y, emitterPosition.z, particleCount );

		// Wake particles

		size_t endID = nrOfParticlesAlive + particleCount;
		for ( size_t i = nrOfParticlesAlive; i < endID; i++ )
			Wake( i );
	}

	virtual void Emitter( ParticleType particleType, XMFLOAT3 emitterPosition, XMFLOAT3 emitterDirection )
	{
		// Wake particles and assign direction
		if( particleType == MuzzleFlash )
		{
			Generate( MuzzleFlash, emitterPosition, emitterDirection, 100, 5.0f );
		}

		// Add other particle types here!

	}

	virtual void Update( float deltaTime )
	{
		// First instruction
		UpdateLifeTime( deltaTime );

		// Check for dead particles
		CheckDeadParticles();

		// Update logic based on Particle type
		switch( particleType )
		{
			case MuzzleFlash: 
			{
				// Update MuzzleFlash logic here
				break;
			}
			default:
			{
				OutputDebugStringA( "-- ERROR: Particle type unknown --" );
				break;
			}
		}

		// Last instruction
		UpdatePosition( deltaTime );





		//Last instruction
		//nrOfParticlesAlive = 10000;
		//__int64 ctr1 = 0, ctr2 = 0, ctr3 = 0, ctr4 = 0, ctr5 = 0, ctr6 = 0;
		//
		////MESSAURE
		//QueryPerformanceCounter((LARGE_INTEGER *)&ctr1);

		

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