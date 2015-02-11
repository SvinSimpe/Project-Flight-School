#ifndef _PARTICLESYSTEM_H_
#define _PARTICLESYSTEM_H_

#include "ParticleData.h"

#include <Graphics.h>
#include <limits>



struct ParticleSystem : public ParticleData
{
#pragma region Members

	size_t		entityParentID				= std::numeric_limits<unsigned int>::infinity();
	size_t		particleType				= std::numeric_limits<unsigned int>::infinity();
	int			nrOfRequestedParticles		= 0;
	XMFLOAT3	emitterPosition				= XMFLOAT3( 0.0f, 0.0f, 0.0f );
	XMFLOAT3	emitterDirection			= XMFLOAT3( 0.0f, 0.0f, 0.0f );
	bool		isEmitting					= false;
	float		emitRate					= 0.0f;
	AssetID		assetID;


#pragma endregion

#pragma region Functions

	void Initialize( ParticleType particleType, float emitRate, size_t nrOfParticles )
	{
		this->particleType		= particleType;
		this->emitRate			= emitRate;
		ParticleData::Initialize( nrOfParticles );


		Graphics::GetInstance()->LoadStatic2dAsset( "../Content/Assets/ParticleSprites/testSparkParticle.dds", assetID );
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
		if( ( particleCount + nrOfParticlesAlive ) > MAX_PARTICLES )
			particleCount = MAX_PARTICLES - nrOfParticlesAlive;

		// Particlecount must be multiple of 4
		while( particleCount % 4 != 0 )
			particleCount--;
		
		///==================
		// Use emitterDirection as base and randomize a different direction vector with a maximum spread angle deviation
		SetDirection( emitterDirection.x, emitterDirection.y, emitterDirection.z, particleCount, spreadAngle );
		SetPosition( emitterPosition.x, emitterPosition.y, emitterPosition.z, particleCount );
		SetLifeTime( 1, 2, particleCount );

		nrOfRequestedParticles += particleCount;
	}

	virtual void Emitter( ParticleType particleType, XMFLOAT3 emitterPosition, XMFLOAT3 emitterDirection )
	{	
			if( particleType == MuzzleFlash )	Generate( emitterPosition, emitterDirection, 128, 20.0f );
	}

	virtual void Update( float deltaTime )
	{
		if( deltaTime  > 0.0116370535f )
			deltaTime = 0.0116370535f;

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

		// Check if new Particles is requested
 		if( nrOfRequestedParticles >= 4 )
		{
			// Calculate Particle count for this frame
			int nrOfNewParticles = (int)( emitRate );// * deltaTime);
			
			if( nrOfNewParticles > MAX_PARTICLES)
				return;

			// Check if Particle count is a multiple of 4 and is available
			while( nrOfNewParticles % 4 != 0 &&  nrOfNewParticles <= nrOfRequestedParticles )
			{
				nrOfNewParticles--;
				if( nrOfNewParticles <= 0 )
				{
					nrOfNewParticles		= 0;
					nrOfRequestedParticles	= 0;
					return;
				}
			}

			
			//// Wake Particles
			size_t endID = nrOfParticlesAlive + nrOfNewParticles;
			for ( size_t i = nrOfParticlesAlive; i < endID; i++ )
				Wake( i );

			nrOfRequestedParticles -= nrOfNewParticles;
			if( nrOfRequestedParticles < 0 )
				nrOfRequestedParticles = 0;
		}
		else
			nrOfRequestedParticles = 0;


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

#pragma endregion
};

#endif