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
			case NormalSmoke:
			{
				Graphics::GetInstance()->LoadStatic2dAsset( "../Content/Assets/ParticleSprites/smokeParticle1.dds", assetID );
				break;
			}
			case BlowTorchFire:
			{
				Graphics::GetInstance()->LoadStatic2dAsset( "../Content/Assets/ParticleSprites/fireParticle.dds", assetID );
				break;
			}
			case BlowTorchIdle:
			{
				Graphics::GetInstance()->LoadStatic2dAsset( "../Content/Assets/ParticleSprites/fireParticle.dds", assetID );
				break;
			}
			case Fire:
			{
				Graphics::GetInstance()->LoadStatic2dAsset( "../Content/Assets/ParticleSprites/fireParticle.dds", assetID );
				break;
			}
			case ExplosionSmoke:
			{
				Graphics::GetInstance()->LoadStatic2dAsset( "../Content/Assets/ParticleSprites/smokeFlares.dds", assetID );
				break;
			}
			case Explosion:
			{
				Graphics::GetInstance()->LoadStatic2dAsset( "../Content/Assets/ParticleSprites/fireParticle.dds", assetID );
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
			case Spark:
			{
				Graphics::GetInstance()->LoadStatic2dAsset( "../Content/Assets/ParticleSprites/sparks.dds", assetID );
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
			case FireSmoke:
			{
				Graphics::GetInstance()->LoadStatic2dAsset( "../Content/Assets/ParticleSprites/smokeFlares.dds", assetID );
				break;
			}
			case Level_Up:
			{
				Graphics::GetInstance()->LoadStatic2dAsset( "../Content/Assets/ParticleSprites/levelUpParticle2.dds", assetID );
				break;
			}
			case Level_Inner:
			{
				Graphics::GetInstance()->LoadStatic2dAsset( "../Content/Assets/ParticleSprites/levelUpParticle3.dds", assetID );
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
			GeneratePlanePosition( emitterPosition.x, emitterPosition.y, emitterPosition.z, 60, 60, particleCount );
		else if( particleType == Fire )
			GenerateCirclePosition( emitterPosition.x, emitterPosition.y, emitterPosition.z, 1.5f, particleCount );	//-----------------circle spawn instead of point
		else if( particleType == FireSmoke )
			GenerateCirclePosition( emitterPosition.x, emitterPosition.y, emitterPosition.z, 4.0f, particleCount );	//-----------------circle spawn instead of point
		else if( particleType == Level_Up )
			GenerateCircleEdgePosition( emitterPosition.x, emitterPosition.y, emitterPosition.z, 1, particleCount );
		else if( particleType == Level_Inner )
			GenerateCirclePosition( emitterPosition.x, emitterPosition.y, emitterPosition.z, 1, particleCount );
		else
			SetPosition( emitterPosition.x, emitterPosition.y, emitterPosition.z, particleCount );	

		if( particleType == Explosion )	SetRandomDeathTime( 1, 3, particleCount );
		else if( particleType == Fire )	SetRandomDeathTime( 1, 12, particleCount );	//---------------------------------------------------------random lifetime
		else if( particleType == FireSmoke )	SetRandomDeathTime( 15, 25, particleCount );	//---------------------------------------------------------random lifetime
		else if( particleType == Blood )	SetRandomDeathTime( 1, 2, particleCount );
		else if( particleType == MuzzleFlash )	SetRandomDeathTime( 1, 2, particleCount );
		else if( particleType == Smoke_MiniGun )	SetRandomDeathTime( 1, 6, particleCount );
		else if( particleType == Test_Fountain )	SetRandomDeathTime( 1, 50, particleCount );

		//Add Random Rotation
		if(particleType == Fire || FireSmoke)
			SetRandomRotation( particleCount ); 
		
		if( particleType == Spark )
		{
			SetRandomDeathTime( 1, 2, particleCount );
		}
		if( particleType == ExplosionSmoke )	
		{
			SetRandomDeathTime( 1, 6, particleCount );
			SetRandomRotation( particleCount ); 
		}
		else if( particleType == BlowTorchFire )	
		{
			SetRandomDeathTime( 1, 2, particleCount );
			SetRandomRotation( particleCount ); 
		}
		else if( particleType == BlowTorchIdle )	
		{
			SetRandomDeathTime( 1, 1, particleCount );
			SetRandomRotation( particleCount ); 
		}
		else if( particleType == Explosion )	
		{
			SetRandomDeathTime( 1, 2, particleCount );
			SetRandomRotation( particleCount ); 
		}

		else if( particleType == NormalSmoke )	
		{
			SetRandomDeathTime( 4, 12, particleCount );
			SetRandomRotation( particleCount ); 
		}

		else if( particleType == Spark )
		{
			SetRandomDeathTime( 1, 2, particleCount );
		}
		else if( particleType == Blood )
		{
			SetRandomDeathTime( 1, 2, particleCount );
		}
		else if( particleType == MuzzleFlash )
		{
			SetRandomDeathTime( 1, 2, particleCount );
		}
		else if( particleType == Smoke_MiniGun )
		{
			SetRandomDeathTime( 1, 6, particleCount );
		}
		else if( particleType == Test_Fountain )
		{
			SetRandomDeathTime( 1, 50, particleCount );
		}
		else if( particleType == Level_Up )	
		{
			SetRandomDeathTime( 2, 6, particleCount );
		}
		else if( particleType == Level_Inner )		
		{
			SetRandomDeathTime( 1, 2, particleCount );
		}

		nrOfRequestedParticles += particleCount;

		SpellCasterLifeMaster();
	}

	virtual void Emitter( ParticleType particleType, XMFLOAT3 emitterPosition, XMFLOAT3 emitterDirection )
	{
		if( particleType == NormalSmoke )			Generate( emitterPosition, emitterDirection, 6,  120.0f );
		else if( particleType == Spark )			Generate( emitterPosition, emitterDirection, 16, 25.0f );	
		else if( particleType == BlowTorchFire )	Generate( emitterPosition, emitterDirection, 32, 4.0f );
		else if( particleType == BlowTorchIdle )	Generate( emitterPosition, emitterDirection, 32, 2.0f );
		else if( particleType == Fire )				Generate( emitterPosition, emitterDirection, 8, 35.0f );					//------------particle count and spreadangle
		else if( particleType == FireSmoke )		Generate( emitterPosition, emitterDirection, 15, 25.0f );					//------------particle count and spreadangle
		else if( particleType == Explosion )		Generate( emitterPosition, emitterDirection, 50,  360.0f );
		else if( particleType == ExplosionSmoke )	Generate( emitterPosition, emitterDirection, 50,  360.0f );
		else if( particleType == Spark )			Generate( emitterPosition, emitterDirection, 8, 25.0f );
		else if( particleType == Blood )			Generate( emitterPosition, emitterDirection, 8, 25.0f );
		else if( particleType == MuzzleFlash )		Generate( emitterPosition, emitterDirection, 4,  25.0f );
		else if( particleType == Smoke_MiniGun )	Generate( emitterPosition, emitterDirection, 8, 2.0f );
		else if( particleType == Test_Fountain )	Generate( emitterPosition, emitterDirection, 32, 20.0f );
		else if( particleType == Level_Up )			Generate( emitterPosition, emitterDirection, 1024, 270.0f );
		else if( particleType == Level_Inner )		Generate( emitterPosition, emitterDirection, 32, 20.0f );
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
			case NormalSmoke: 
			{
				// Update Normal smoke logic here
				NormalSmokeLogic( deltaTime );
			}
			case Fire: 
			{
				// Update Fire logic here
				FireLogic( deltaTime );
				break;
			}
			case BlowTorchFire: 
			{
				// Update Fire logic here
				BlowTorchFireLogic( deltaTime );
				break;
			}
			case BlowTorchIdle: 
			{
				// Update Fire logic here
				BlowTorchIdleLogic( deltaTime );
				break;
			}
			case FireSmoke: 
			{
				// Update Fire logic here
				FireSmokeLogic( deltaTime );
				break;
			}
			case ExplosionSmoke: 
			{
				// Update Explosion smoke logic here
				ExplosionSmokeLogic( deltaTime );
				break;
			}
			case Explosion: 
			{
				// Update Explosion logic here
				ExplosionLogic( deltaTime );
				break;
			}
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
			case Level_Up:
			{
				// Update Smoke_MiniGun logic here
				Level_UpLogic( deltaTime );
				break;
			}
			case Level_Inner:
			{
				// Update Smoke_MiniGun logic here
				Level_InnerLogic( deltaTime );
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
	
	void NormalSmokeLogic( float deltatime )
	{
		for ( int i = 0; i < nrOfParticlesAlive; i++ )
		{
			if(damping[i] > 0)
				damping[i] -= 0.01f;

			xVelocity[i] = xVelocity[i] * damping[i];
			zVelocity[i] = zVelocity[i] * damping[i];
			yVelocity[i] = zVelocity[i] * damping[i];
			xPosition[i] += 0.05f * ( 1.0f - damping[i] );
			zPosition[i] += 0.025f * ( 1.0f - damping[i] );
			yPosition[i] += 0.05f * ( 1.0f - damping[i] );
		}
	}

	void FireLogic( float deltaTime )
	{
		for ( int i = 0; i < nrOfParticlesAlive; i++ )
		{
			
		}
	}
	
	void BlowTorchFireLogic( float deltaTime )
	{
		for ( int i = 0; i < nrOfParticlesAlive; i++ )
		{
			if( damping[i] > 0 )
				damping[i] += 0.004f;

			xVelocity[i] = xVelocity[i] * damping[i];
			zVelocity[i] = zVelocity[i] * damping[i];
			
			if( damping[i] > 0.08f )
				yPosition[i] = yPosition[i] * 1.03f;
		}
	}

	void BlowTorchIdleLogic( float deltaTime )
	{
		for ( int i = 0; i < nrOfParticlesAlive; i++ )
		{
			deathTime[i] = 0.07f;
		}
	}

	void FireSmokeLogic( float deltaTime )
	{
		for ( int i = 0; i < nrOfParticlesAlive; i++ )
		{

		}
	}

	void ExplosionSmokeLogic( float deltatime )
	{
		for ( int i = 0; i < nrOfParticlesAlive; i++ )
		{
			if(damping[i] > 0)
				damping[i] -= 0.01f;

			xVelocity[i] = xVelocity[i] * damping[i];
			zVelocity[i] = zVelocity[i] * damping[i];
			yVelocity[i] = zVelocity[i] * damping[i] * 4;
			xPosition[i] += 0.5f * ( 1.0f - damping[i] );
			zPosition[i] += 0.25f * ( 1.0f - damping[i] );
			yPosition[i] += 0.8f * ( 1.0f - damping[i] );
		}
	}

	void ExplosionLogic( float deltaTime )
	{
		for ( int i = 0; i < nrOfParticlesAlive; i++ )
		{
			if(damping[i] > 0)
				damping[i] -= 0.005f;

			xVelocity[i] = xVelocity[i] * damping[i];
			zVelocity[i] = zVelocity[i] * damping[i];
		}

	}

	void SparkLogic( float deltaTime ) 
	{

	}

	void Level_UpLogic( float deltaTime )
	{

	}
	
	void Level_InnerLogic( float deltaTime )
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