#ifndef _PARTICLESYSTEM_H_
#define _PARTICLESYSTEM_H_

#include "ParticleData.h"
#include <Graphics.h>
#include <limits>

struct ParticleSystem : public ParticleData
{
	#pragma region Members

	size_t		entityParentID				= std::numeric_limits<unsigned int>::infinity();
	XMFLOAT3	emitterPosition				= XMFLOAT3( 0.0f, 0.0f, 0.0f );
	XMFLOAT3	emitterDirection			= XMFLOAT3( 0.0f, 0.0f, 0.0f );
	bool		isEmitting					= false;
	float		emitRate					= 0.0f;
	AssetID		assetID;

	int currCount = 0;
	int prevCount = 0;

	#pragma endregion

	#pragma region Functions

	void Initialize( ParticleType particleType, float emitRate, size_t nrOfParticles )
	{
		this->particleType		= particleType;
		this->emitRate			= emitRate;
		ParticleData::Initialize( nrOfParticles );

		switch ( particleType )
		{
			case Spark:
			{
				Graphics::GetInstance()->LoadStatic2dAsset( "../Content/Assets/ParticleSprites/sparks.dds", assetID );
				break;
			}
			case Blood:
			{
				Graphics::GetInstance()->LoadStatic2dAsset( "../Content/Assets/ParticleSprites/blood.dds", assetID );
				break;
			}
			case MuzzleFlash:
			{
				Graphics::GetInstance()->LoadStatic2dAsset( "../Content/Assets/ParticleSprites/fireSprite.dds", assetID );
				break;
			}
			case Smoke_MiniGun:
			{
				Graphics::GetInstance()->LoadStatic2dAsset( "../Content/Assets/ParticleSprites/whiteSmoke.dds", assetID );
				break;
			}
			case Test_Fountain:
			{
				Graphics::GetInstance()->LoadStatic2dAsset( "../Content/Assets/ParticleSprites/smokeParticle1.dds", assetID );
				break;
			}
			default:
				break;
		}
	}

	void Activate( size_t entityParentID, XMFLOAT3 emitterPosition, XMFLOAT3 emitterDirection )
	{
		this->entityParentID	= entityParentID;
		this->emitterPosition	= emitterPosition;
		this->emitterDirection	= emitterDirection;
		isEmitting				= true;
	}

	void Deactive()
	{
		entityParentID		= std::numeric_limits<unsigned int>::infinity();
		emitterPosition		= XMFLOAT3( 0.0f, 0.0f, 0.0f );
		emitterDirection	= XMFLOAT3( 0.0f, 0.0f, 0.0f );
		isEmitting			= false;
	}

	void Generate( XMFLOAT3 emitterPosition, XMFLOAT3 emitterDirection, int particleCount, float spreadAngle )
	{
		// Check if there is enough particles to meet request
		if( ( particleCount + nrOfParticlesAlive ) > capacity )
		{
			particleCount = capacity - nrOfParticlesAlive;
			return;
		}
		
		///==================
		// Use emitterDirection as base and randomize a different direction vector with a maximum spread angle deviation
		SetDirection( emitterDirection.x, emitterDirection.y, emitterDirection.z, particleCount, spreadAngle );
		
		if( particleType == Test_Fountain )
			GenerateCirclePosition( emitterPosition.x, emitterPosition.y, emitterPosition.z, 3.0f, particleCount );
		else
			SetPosition( emitterPosition.x, emitterPosition.y, emitterPosition.z, particleCount );
		
		if( particleType == Spark )	SetRandomDeathTime( 1, 2, particleCount );
		else if( particleType == Blood )	SetRandomDeathTime( 1, 2, particleCount );
		else if( particleType == MuzzleFlash )	SetRandomDeathTime( 1, 2, particleCount );
		else if( particleType == Smoke_MiniGun )	SetRandomDeathTime( 1, 6, particleCount );
		else if( particleType == Test_Fountain )	SetRandomDeathTime( 1, 8, particleCount );

		nrOfRequestedParticles += particleCount;

		SpellCasterLifeMaster();
	}

	virtual void Emitter( ParticleType particleType, XMFLOAT3 emitterPosition, XMFLOAT3 emitterDirection )
	{	
		if( particleType == Spark )	Generate( emitterPosition, emitterDirection, 8, 25.0f );	
		else if( particleType == Blood )	Generate( emitterPosition, emitterDirection, 8, 25.0f );
		else if( particleType == MuzzleFlash )	Generate( emitterPosition, emitterDirection, 4,  25.0f );
		else if( particleType == Smoke_MiniGun )	Generate( emitterPosition, emitterDirection, 8, 2.0f );
		else if( particleType == Test_Fountain )	Generate( emitterPosition, emitterDirection, 32, 20.0f );
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
			case Spark: 
			{
				// Update Spark logic here
				SparkLogic( deltaTime );
				break;
			}
			case Blood: 
			{
				// Update Blood logic here
				BloodLogic( deltaTime );
				break;
			}
			case MuzzleFlash: 
			{
				// Update MuzzleFlash logic here
				MuzzleFlashLogic( deltaTime );
				break;
			}
			case Smoke_MiniGun:
			{
				// Update Smoke_MiniGun logic here
				Smoke_MiniGunLogic( deltaTime );
				break;
			}
			case Test_Fountain:
			{
				// Update Smoke_MiniGun logic here
				Test_FountainLogic( deltaTime );
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
	}

	virtual void Render( float deltaTime )
	{
		
	}

	void Release()
	{
		ParticleData::Release();
	}

	void SpellCasterLifeMaster()
	{
		if( nrOfParticlesAlive < capacity )
		{
			// Calculate Particle count for this frame
			int nrOfNewParticles = (int)emitRate;
	
			if( nrOfNewParticles > capacity )
				return;

			// Wake Particles
			size_t endID = nrOfParticlesAlive + nrOfNewParticles;
			for ( size_t i = nrOfParticlesAlive; i < endID; i++ )
				Wake( i );

			nrOfRequestedParticles -= nrOfNewParticles;

			if( nrOfRequestedParticles < 0 )
				nrOfRequestedParticles = 0;

			else
				nrOfRequestedParticles = 0;	
		}
	}
	
	void SparkLogic( float deltaTime ) 
	{

	}

	void BloodLogic( float deltaTime )
	{
		for ( int i = 0; i < nrOfParticlesAlive; i++ )
		{
			yVelocity[i] -= 0.1f;
		}
	}

	void MuzzleFlashLogic( float deltaTime )
	{
		
	}

	void Smoke_MiniGunLogic( float deltaTime )
	{
		IncrementValueY( 0.02f );
	}

	void Test_FountainLogic( float deltaTime )
	{
		currCount = nrOfParticlesAlive;
		if( currCount > prevCount )
		{
			prevCount = currCount;
			currCount = 0;
		}
		
		if( currCount == prevCount )
			int k = 4;
	}

	#pragma endregion
};
#endif