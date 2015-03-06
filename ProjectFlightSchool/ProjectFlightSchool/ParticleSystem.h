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

	#pragma endregion

	#pragma region Functions

	void Initialize( ParticleType particleType, float emitRate, size_t nrOfParticles )
	{
		this->particleType		= particleType;
		this->emitRate			= emitRate;
		ParticleData::Initialize( nrOfParticles );

		switch ( particleType )
		{
			case Shell:
			{
				Graphics::GetInstance()->LoadStatic2dAsset( "../Content/Assets/ParticleSprites/shell.dds", assetID );
				break;
			}
			case Debris:
			{
				Graphics::GetInstance()->LoadStatic2dAsset( "../Content/Assets/ParticleSprites/fireflyParticle.dds", assetID );
				break;
			}
			case Hammer_Effect:
			{
				Graphics::GetInstance()->LoadStatic2dAsset( "../Content/Assets/ParticleSprites/dustParticle1.dds", assetID );
				break;
			}
			case NormalSmoke:
			{
				Graphics::GetInstance()->LoadStatic2dAsset( "../Content/Assets/ParticleSprites/smokeParticle1.dds", assetID );
				break;
			}
			case BlowTorchFire:
			{
				mPointLightParticleEmitter 	= new PointLight;
				Graphics::GetInstance()->LoadStatic2dAsset( "../Content/Assets/ParticleSprites/fireParticle.dds", assetID );
				break;
			}
			case BlowTorchIdle:
			{
				mPointLightParticleEmitter 	= new PointLight;
				Graphics::GetInstance()->LoadStatic2dAsset( "../Content/Assets/ParticleSprites/fireParticle.dds", assetID );
				break;
			}
			case FIRE:
			{
				mPointLightParticleEmitter 	= new PointLight;
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
				mPointLightParticleEmitter 	= new PointLight;
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
				mPointLightParticleEmitter 	= new PointLight;
				Graphics::GetInstance()->LoadStatic2dAsset( "../Content/Assets/ParticleSprites/sparks.dds", assetID );
				break;
			}
			case Spark_Robot:
			{
				mPointLightParticleEmitter 	= new PointLight;
				Graphics::GetInstance()->LoadStatic2dAsset( "../Content/Assets/ParticleSprites/sparks.dds", assetID );
				break;
			}
			case Spark_Electric:
			{
				mPointLightParticleEmitter 	= new PointLight;
				Graphics::GetInstance()->LoadStatic2dAsset( "../Content/Assets/ParticleSprites/electric.dds", assetID );
				break;
			}
			case Smoke_MiniGun:
			{
				Graphics::GetInstance()->LoadStatic2dAsset( "../Content/Assets/ParticleSprites/whiteSmoke.dds", assetID );
				break;
			}
			case Spores:
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
		if( ( particleCount + nrOfParticlesAlive ) >= capacity )
		{
			particleCount = capacity - nrOfParticlesAlive;
			return;
		}
		
		///==================
		// Use emitterDirection as base and randomize a different direction vector with a maximum spread angle deviation
		SetDirection( emitterDirection.x, emitterDirection.y, emitterDirection.z, particleCount, spreadAngle );
		
		if( particleType == Spores )
			GeneratePlanePosition( emitterPosition.x, emitterPosition.y, emitterPosition.z, 30, 30, particleCount );
		else if( particleType == Level_Up )
			GenerateCircleEdgePosition( emitterPosition.x, emitterPosition.y, emitterPosition.z, 1, particleCount );
		else if( particleType == Level_Inner )
			GenerateCirclePosition( emitterPosition.x, emitterPosition.y, emitterPosition.z, 1, particleCount );
		else
			SetPosition( emitterPosition.x, emitterPosition.y, emitterPosition.z, particleCount );	

		//Add Random Rotation
		if(particleType == FIRE )
		{
			SetRandomRotation( particleCount );
			SetRandomDeathTime( 1, 3, particleCount );
			ActivateLight( emitterPosition, XMFLOAT3( 6.0f, 2.0f, 2.0f ), 0.4f );
		}
		else if(particleType == Shell )
		{
			SetRandomRotation( particleCount );
			SetRandomDeathTime( 2, 4, particleCount );
		}
		else if(particleType == Debris )
		{
			SetRandomRotation( particleCount );
			SetRandomDeathTime( 2, 4, particleCount ); 
		}
		else if(particleType == FireSmoke )
		{
			SetRandomRotation( particleCount );
			SetRandomDeathTime( 3, 6, particleCount );
		}
		else if( particleType == Spark )
		{
			SetRandomDeathTime( 1, 2, particleCount );
		}
		else if( particleType == Spark_Robot )
		{
			SetRandomRotation( particleCount ); 
			SetRandomDeathTime( 1, 2, particleCount );
		}
		else if( particleType == Spark_Electric )
		{
			SetRandomRotation( particleCount ); 
			SetRandomDeathTime( 1, 1, particleCount );
			ActivateLight( emitterPosition, XMFLOAT3( 2.0f, 2.0f, 8.0f ), 0.2f );
		}
		else if( particleType == ExplosionSmoke )	
		{
			SetRandomDeathTime( 1, 6, particleCount );
			SetRandomRotation( particleCount ); 
		}
		else if( particleType == BlowTorchFire )	
		{
			SetRandomDeathTime( 1, 2, particleCount );
			SetRandomRotation( particleCount ); 
			ActivateLight( emitterPosition, XMFLOAT3( 6.0f, 2.0f, 2.0f ), 0.2f );
		}
		else if( particleType == BlowTorchIdle )	
		{
			SetRandomDeathTime( 1, 1, particleCount );
			SetRandomRotation( particleCount );
			ActivateLight( emitterPosition, XMFLOAT3( 2.0f, 1.0f, 3.0f ), 0.01f );
		}
		else if( particleType == Explosion )	
		{
			SetRandomDeathTime( 1, 3, particleCount );
			SetRandomRotation( particleCount );
			ActivateLight( XMFLOAT3( emitterPosition.x, emitterPosition.y + 1.3f, emitterPosition.z ), XMFLOAT3( 6.0f, 3.0f, 2.0f ), 0.2f );
		}
		else if( particleType == NormalSmoke )	
		{
			SetRandomDeathTime( 4, 12, particleCount );
			SetRandomRotation( particleCount ); 
		}
		else if( particleType == Hammer_Effect )	
		{
			SetRandomDeathTime( 2, 8, particleCount );
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
		else if( particleType == Spores )
		{
			SetRandomDeathTime( 3, 50, particleCount );
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
		if( particleType == Shell )					Generate( emitterPosition, emitterDirection, 1,  10.0f );
		else if( particleType == Debris )			Generate( emitterPosition, emitterDirection, (int)GetRandomRotation(0, 5),  30.0f );
		else if( particleType == NormalSmoke )		Generate( emitterPosition, emitterDirection, 6,  120.0f );
		else if( particleType == BlowTorchIdle )	Generate( emitterPosition, emitterDirection, 32, 2.0f );
		else if( particleType == BlowTorchFire )	Generate( emitterPosition, emitterDirection, 32, 4.0f );
		else if( particleType == FIRE )				Generate( emitterPosition, emitterDirection, 8, 40.0f );
		else if( particleType == FireSmoke )		Generate( emitterPosition, emitterDirection, 15, 25.0f );					
		else if( particleType == Hammer_Effect )	Generate( emitterPosition, emitterDirection, 64,  180.0f );
		else if( particleType == Explosion )		Generate( emitterPosition, emitterDirection, 50,  360.0f );
		else if( particleType == ExplosionSmoke )	Generate( emitterPosition, emitterDirection, 50,  360.0f );
		else if( particleType == Spark )			Generate( emitterPosition, emitterDirection, 8, 25.0f );
		else if( particleType == Spark_Robot )		Generate( emitterPosition, emitterDirection, 8, 90.0f );
		else if( particleType == Spark_Electric )	Generate( emitterPosition, emitterDirection, 1, 360.0f );
		else if( particleType == Blood )			Generate( emitterPosition, emitterDirection, 8, 25.0f );
		else if( particleType == MuzzleFlash )		Generate( emitterPosition, emitterDirection, 4,  25.0f );
		else if( particleType == Smoke_MiniGun )	Generate( emitterPosition, emitterDirection, 8, 2.0f );
		else if( particleType == Spores )			Generate( emitterPosition, emitterDirection, 8, 20.0f );
		else if( particleType == Level_Up )			Generate( emitterPosition, emitterDirection, 512, 270.0f );
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
			case Shell: 
			{
				// Update Debris logic here
				ShellLogic( deltaTime );
				break;
			}	
			case Debris: 
			{
				// Update Debris logic here
				DebrisLogic( deltaTime );
				break;
			}	
			case NormalSmoke: 
			{
				// Update Normal smoke logic here
				NormalSmokeLogic( deltaTime );
				break;
			}			
			case Hammer_Effect: 
			{
				// Update Normal smoke logic here
				HammerEffectLogic( deltaTime );
				break;
			}
			case FIRE: 
			{
				// Update Fire logic here
				FireLogic( deltaTime );
				HandleLight();
				break;
			}
			case BlowTorchFire: 
			{
				// Update Fire logic here
				BlowTorchFireLogic( deltaTime );
				HandleLight();
				break;
			}
			case BlowTorchIdle: 
			{
				// Update Fire logic here
				BlowTorchIdleLogic( deltaTime );
				HandleLight();
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
				HandleLight();
				break;
			}
			case Spark: 
			{
				// Update Spark logic here
				SparkLogic( deltaTime );
				break;
			}
			case Spark_Robot: 
			{
				// Update Spark logic here
				Spark_RobotLogic( deltaTime );
				break;
			}
			case Spark_Electric: 
			{
				// Update Spark logic here
				Spark_ElectricLogic( deltaTime );
				HandleLight();
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
			case Spores:
			{
				// Update Smoke_MiniGun logic here
				SporesLogic( deltaTime );
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
	
			if( nrOfParticlesAlive + nrOfNewParticles > capacity )
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

	void ActivateLight( XMFLOAT3 position, XMFLOAT3 color, float radius )
	{
		mPointLightParticleEmitter->positionAndIntensity = XMFLOAT4( position.x, position.y, position.z, 1.0f );

		//float radius = nrOfRequestedParticles 
		mPointLightParticleEmitter->colorAndRadius	= XMFLOAT4( color.x, color.y, color.z, radius );
		mInitialRadius								= radius;

		IEventPtr reg( new Event_Add_Point_Light( mPointLightParticleEmitter ) );
		EventManager::GetInstance()->QueueEvent( reg );

		isLightActive = true;
	}

	void DeactivateLight()
	{
		IEventPtr reg( new Event_Remove_Point_Light( mPointLightParticleEmitter ) );
		EventManager::GetInstance()->QueueEvent( reg );
	}

	void UpdateLight()
	{
		if ( particleType == Spark_Electric )
		{
			mPointLightParticleEmitter->positionAndIntensity.w	= 1 + nrOfParticlesAlive * 0.05f;
			mPointLightParticleEmitter->colorAndRadius.w		= mInitialRadius + nrOfParticlesAlive * 0.012f;
		}

		else if ( particleType == BlowTorchFire ) 			
		{
			 mPointLightParticleEmitter->positionAndIntensity.w	= 1 + nrOfParticlesAlive * 0.085f;
			 mPointLightParticleEmitter->colorAndRadius.w		= mInitialRadius + nrOfParticlesAlive * 0.012f;
		}

		else if ( particleType == BlowTorchIdle ) 			
		{
			 mPointLightParticleEmitter->positionAndIntensity.w = 0.2f + nrOfParticlesAlive * 0.085f;
			 mPointLightParticleEmitter->colorAndRadius.w		= mInitialRadius + nrOfParticlesAlive * 0.005f;
		}

		else if ( particleType == FIRE ) 			
		{
			 mPointLightParticleEmitter->positionAndIntensity.w = 1 + nrOfParticlesAlive * 0.02f;
			 mPointLightParticleEmitter->colorAndRadius.w		= mInitialRadius + nrOfParticlesAlive * 0.005f;
		}

		else if ( particleType == Explosion ) 			
		{
			 mPointLightParticleEmitter->positionAndIntensity.w = 1 + nrOfParticlesAlive * 0.9f;
			 mPointLightParticleEmitter->colorAndRadius.w		= mInitialRadius + nrOfParticlesAlive * 0.035f;
		}
	}

	void HandleLight( )
	{
		if ( nrOfParticlesAlive > 0 && isLightActive == true )
			UpdateLight();

		else if( nrOfParticlesAlive == 0 && isLightActive == true )
		{
			DeactivateLight();
			isLightActive = false;
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

	void ShellLogic( float deltaTime )
	{
		for ( int i = 0; i < nrOfParticlesAlive; i++ )
		{
			if ( yPosition[i] < 0 )
			{
				if ( damping[i] > 0 )
					damping[i] -= 0.03f;
				else
					damping[i] = 0.0f;

				yVelocity[i] = ( yVelocity[i] * -1 ) * damping[i];
			}
			else
			{
				yVelocity[i] -= 1.0f;
			}
		}
	}

	void DebrisLogic( float deltaTime )
	{
		for ( int i = 0; i < nrOfParticlesAlive; i++ )
		{
			if ( yPosition[i] < 0 )
			{
				if ( damping[i] > 0 )
					damping[i] -= 0.1f;
				else
					damping[i] = 0.0f;

				yVelocity[i] = ( yVelocity[i] * -1 ) * damping[i];
			}
			else
			{
				yVelocity[i] -= 1.0f;
			}
		}
	}

	void HammerEffectLogic( float deltaTime )
	{

	}

	void FireLogic( float deltaTime )
	{
		for ( int i = 0; i < nrOfParticlesAlive; i++ )
		{
			zPosition[i] += 0.05f; 
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
			if(damping[i] > 0)
				damping[i] -= 0.01f;

			xVelocity[i] = xVelocity[i] * damping[i];
			zVelocity[i] = zVelocity[i] * damping[i];
			yVelocity[i] = zVelocity[i] * damping[i] * 4;
			xPosition[i] += 0.05f * ( 1.0f - damping[i] );
			zPosition[i] += 0.025f * ( 1.0f - damping[i] );
			yPosition[i] += 0.08f * ( 1.0f - damping[i] );
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
			xPosition[i] += 0.05f * ( 1.0f - damping[i] );
			zPosition[i] += 0.025f * ( 1.0f - damping[i] );
			yPosition[i] += 0.08f * ( 1.0f - damping[i] );
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
	
	void Spark_RobotLogic( float deltaTime ) 
	{

	}

	void Spark_ElectricLogic( float deltaTime ) 
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

	void SporesLogic( float deltaTime )
	{
	}

	#pragma endregion
};
#endif