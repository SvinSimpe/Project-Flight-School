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
			case GranateTrail:
			{
				Graphics::GetInstance()->LoadStatic2dAsset( "../Content/Assets/ParticleSprites/smokeFlares.dds", assetID );
				break;
			}
			case SniperTrail:
			{
				Graphics::GetInstance()->LoadStatic2dAsset( "../Content/Assets/ParticleSprites/smokeParticle1.dds", assetID );
				break;
			}
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
				Graphics::GetInstance()->LoadStatic2dAsset( "../Content/Assets/ParticleSprites/fireParticle.dds", assetID );
				break;
			}
			case Spark:
			{
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
			case Fire_Flies:
			{
				mPointLightParticleEmitter 	= new PointLight;
				Graphics::GetInstance()->LoadStatic2dAsset( "../Content/Assets/ParticleSprites/fireflyParticle.dds", assetID );
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

	void Generate( XMFLOAT3 emitterPosition, XMFLOAT3 emitterDirection, int particleCount, float spreadAngle, XMFLOAT3 initialVelocity )
	{
		// Check if there is enough particles to meet request
		if( ( particleCount + nrOfParticlesAlive ) >= capacity )
		{
			particleCount = capacity - nrOfParticlesAlive;
			return;
		}

		switch ( particleType )
		{	
			case GranateTrail:
			{
				SetPosition( emitterPosition.x, emitterPosition.y, emitterPosition.z, particleCount );
				SetRandomRotation( particleCount );
				SetRandomDeathTime( 1, 5, particleCount );
				break;
			}
			case SniperTrail:
			{
				SetPosition( emitterPosition.x, emitterPosition.y, emitterPosition.z, particleCount );
				SetRandomRotation( particleCount );
				SetRandomDeathTime( 1, 5, particleCount );
				break;
			}
			case Shell:
			{
				SetPosition( emitterPosition.x, emitterPosition.y, emitterPosition.z, particleCount );
				SetRandomRotation( particleCount );
				SetRandomDeathTime( 2, 4, particleCount );
				break;
			}

			case Debris:
			{
				SetPosition( emitterPosition.x, emitterPosition.y, emitterPosition.z, particleCount );
				SetRandomRotation( particleCount );
				SetRandomDeathTime( 2, 4, particleCount ); 
				break;
			}

			case FIRE:
			{
				SetPosition( emitterPosition.x, emitterPosition.y, emitterPosition.z, particleCount );
				SetRandomRotation( particleCount );
				SetRandomDeathTime( 1, 3, particleCount );
				ActivateLight( emitterPosition, XMFLOAT3( 6.0f, 2.0f, 2.0f ), 0.4f );
				break;
			}
			case Spark:
			{
				SetPosition( emitterPosition.x, emitterPosition.y, emitterPosition.z, particleCount );
				SetRandomDeathTime( 1, 2, particleCount );

				break;
			}
			case Spark_Robot:
			{
				SetPosition( emitterPosition.x, emitterPosition.y, emitterPosition.z, particleCount );
				SetRandomRotation( particleCount ); 
				SetRandomDeathTime( 1, 2, particleCount );
				break;
			}
			case Spark_Electric:
			{
				SetPosition( emitterPosition.x, emitterPosition.y, emitterPosition.z, particleCount );
				SetRandomRotation( particleCount ); 
				SetRandomDeathTime( 1, 1, particleCount );
				ActivateLight( emitterPosition, XMFLOAT3( 2.0f, 2.0f, 8.0f ), 0.2f );
				break;
			}
			case Blood:
			{
				SetPosition( emitterPosition.x, emitterPosition.y, emitterPosition.z, particleCount );
				SetRandomDeathTime( 1, 2, particleCount );
				break;
			}
			case MuzzleFlash:
			{
				SetPosition( emitterPosition.x, emitterPosition.y, emitterPosition.z, particleCount );
				SetRandomRotation( particleCount ); 
				SetRandomDeathTime( 1, 1, particleCount );
				break;
			}
			case Smoke_MiniGun:
			{
				SetPosition( emitterPosition.x, emitterPosition.y, emitterPosition.z, particleCount );
				SetRandomDeathTime( 1, 6, particleCount );
				break;
			}
			case Spores:
			{
				GeneratePlanePosition( emitterPosition.x, emitterPosition.y, emitterPosition.z, 30, 30, particleCount );
				SetRandomDeathTime( 3, 50, particleCount );
				break;
			}
			case FireSmoke:
			{
				SetPosition( emitterPosition.x, emitterPosition.y, emitterPosition.z, particleCount );
				SetRandomRotation( particleCount );
				SetRandomDeathTime( 3, 6, particleCount );
				break;
			}
			case BlowTorchFire:
			{
				SetPosition( emitterPosition.x, emitterPosition.y, emitterPosition.z, particleCount );
				SetRandomDeathTime( 1, 2, particleCount );
				SetRandomRotation( particleCount );
				ActivateLight( emitterPosition, XMFLOAT3( 6.0f, 2.0f, 2.0f ), 0.2f );
				break;
			}
			case BlowTorchIdle:
			{
				SetPosition( emitterPosition.x, emitterPosition.y, emitterPosition.z, particleCount );
				SetRandomDeathTime( 1, 1, particleCount );
				SetRandomRotation( particleCount );
				ActivateLight( emitterPosition, XMFLOAT3( 2.0f, 1.0f, 3.0f ), 0.01f );
				break;
			}
			case Level_Up:
			{
				GenerateCircleEdgePosition( emitterPosition.x, emitterPosition.y, emitterPosition.z, 1, particleCount );
				SetRandomDeathTime( 2, 6, particleCount );
				break;
			}
			case Level_Inner:
			{
				GenerateCirclePosition( emitterPosition.x, emitterPosition.y, emitterPosition.z, 1, particleCount );
				SetRandomDeathTime( 1, 2, particleCount );
				break;
			}
			case Explosion:
			{
				SetPosition( emitterPosition.x, emitterPosition.y, emitterPosition.z, particleCount );
				SetRandomDeathTime( 1, 2, particleCount );
				SetRandomRotation( particleCount );
				ActivateLight( XMFLOAT3( emitterPosition.x, emitterPosition.y + 1.3f, emitterPosition.z ), XMFLOAT3( 6.0f, 3.0f, 2.0f ), 0.2f );
				break;
			}
			case ExplosionSmoke:
			{
				SetPosition( emitterPosition.x, emitterPosition.y, emitterPosition.z, particleCount );
				SetRandomDeathTime( 5, 10, particleCount );
				SetRandomRotation( particleCount ); 
				break;
			}
			case NormalSmoke:
			{
				SetPosition( emitterPosition.x, emitterPosition.y, emitterPosition.z, particleCount );
				SetRandomDeathTime( 4, 12, particleCount );
				SetRandomRotation( particleCount ); 
				break;
			}

			case Fire_Flies:
			{	
				initialSpawnPos = XMFLOAT3( emitterPosition.x, emitterPosition.y, emitterPosition.z );
				GenerateCirclePosition( emitterPosition.x, emitterPosition.y, emitterPosition.z, 1, particleCount );
				SetRandomDeathTime( 1, 2, particleCount );
				SetRandomDistance( 0, 1, particleCount );
				break;
			}

			case Hammer_Effect:
			{
				SetPosition( emitterPosition.x, emitterPosition.y, emitterPosition.z, particleCount );
				SetRandomDeathTime( 2, 8, particleCount );
				break;
			}

			default:
			{
				//Unvalid ParticleType
				break;
			}
		}


		// NON-SPECIFIC
		//==============
		SetInitialVelocity( initialVelocity.x, initialVelocity.y, initialVelocity.z, particleCount );

		// Use emitterDirection as base and randomize a different direction vector with a maximum spread angle deviation
		SetDirection( emitterDirection.x, emitterDirection.y, emitterDirection.z, particleCount, spreadAngle );

		nrOfRequestedParticles += particleCount;

		//Nya nightwish-låten? :)))
		SpellCasterLifeMaster();
	}
	void Emitter( ParticleType particleType, XMFLOAT3 emitterPosition, XMFLOAT3 emitterDirection, XMFLOAT3 initialVelocity )
	{
		if( particleType == GranateTrail )			Generate( emitterPosition, emitterDirection, (int)GetRandomRotation(3, 10),  1.0f, initialVelocity );
		else if( particleType == SniperTrail )		Generate( emitterPosition, emitterDirection, (int)GetRandomRotation(10, 30),  1.0f, initialVelocity );
		else if( particleType == Shell )			Generate( emitterPosition, emitterDirection, 1,		10.0f,		initialVelocity  );
		else if( particleType == Debris )			Generate( emitterPosition, emitterDirection, (int)GetRandomRotation(0, 5),  30.0f,		initialVelocity  );
		else if( particleType == FIRE )				Generate( emitterPosition, emitterDirection, 8,		40.0f,		initialVelocity );		
		else if( particleType == Spark )			Generate( emitterPosition, emitterDirection, 8,		25.0f,		initialVelocity );
		else if( particleType == Spark_Robot )		Generate( emitterPosition, emitterDirection, 8,		90.0f,		initialVelocity );
		else if( particleType == Spark_Electric )	Generate( emitterPosition, emitterDirection, 1,		360.0f,		initialVelocity );
		else if( particleType == Blood )			Generate( emitterPosition, emitterDirection, 8,		25.0f,		initialVelocity );
		else if( particleType == MuzzleFlash )		Generate( emitterPosition, emitterDirection, 4,		1.0f,		initialVelocity );
		else if( particleType == Smoke_MiniGun )	Generate( emitterPosition, emitterDirection, 8,		2.0f,		initialVelocity );
		else if( particleType == Spores )			Generate( emitterPosition, emitterDirection, 8,		20.0f,		initialVelocity );
		else if( particleType == FireSmoke )		Generate( emitterPosition, emitterDirection, 15,	25.0f,		initialVelocity );	
		else if( particleType == BlowTorchFire )	Generate( emitterPosition, emitterDirection, 32,	4.0f,		initialVelocity );
		else if( particleType == BlowTorchIdle )	Generate( emitterPosition, emitterDirection, 32,	2.0f,		initialVelocity );
		else if( particleType == Level_Up )			Generate( emitterPosition, emitterDirection, 512,	270.0f,		initialVelocity );
		else if( particleType == Level_Inner )		Generate( emitterPosition, emitterDirection, 32,	20.0f,		initialVelocity );
		else if( particleType == Explosion )		Generate( emitterPosition, emitterDirection, 50,	360.0f,		initialVelocity );
		else if( particleType == ExplosionSmoke )	Generate( emitterPosition, emitterDirection, 50,	90.0f,		initialVelocity );		
		else if( particleType == NormalSmoke )		Generate( emitterPosition, emitterDirection, 6,		120.0f,		initialVelocity );
		else if( particleType == Fire_Flies )		Generate( emitterPosition, emitterDirection, 8,		5.0f,		initialVelocity );
		else if( particleType == Hammer_Effect )	Generate( emitterPosition, emitterDirection, 64,	180.0f,		initialVelocity );
	}

	void Update( float deltaTime )
	{
		// First instruction
		if ( particleType != Fire_Flies ) 
			UpdateLifeTime( deltaTime );

		// Check for dead particles
		CheckDeadParticles();

		// Update logic based on Particle type
		switch( particleType )
		{
			case GranateTrail: 
			{
				// Update Debris logic here
				GranateTrailLogic( deltaTime );
				break;
			}
			case SniperTrail: 
			{
				// Update Debris logic here
				SniperTrailLogic( deltaTime );
				break;
			}
			
			case Shell: 
			{
				// Update Shell logic here
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
				// Update Hammer_Effect logic here
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
				// Update BlowTorchFire logic here
				BlowTorchFireLogic( deltaTime );
				HandleLight();
				break;
			}
			case BlowTorchIdle: 
			{
				// Update BlowTorchIdle logic here
				BlowTorchIdleLogic( deltaTime );
				HandleLight();
				break;
			}
			case FireSmoke: 
			{
				// Update FireSmoke logic here
				FireSmokeLogic( deltaTime );
				break;
			}
			case ExplosionSmoke: 
			{
				// Update ExplosionSmoke logic here
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
				// Update Spark_Robot logic here
				Spark_RobotLogic( deltaTime );
				break;
			}
			case Spark_Electric: 
			{
				// Update Spark_Electric logic here
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
				// Update Spores logic here
				SporesLogic( deltaTime );
				break;
			}
			case Level_Up:
			{
				// Update Level_Up logic here
				Level_UpLogic( deltaTime );
				break;
			}
			case Level_Inner:
			{
				// Update Level_Inner logic here
				Level_InnerLogic( deltaTime );
				break;
			}
			case Fire_Flies:
			{
				// Update fireflies logic here
				Fire_FliesLogic( deltaTime );
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
		switch ( particleType )
		{	
			case Spark_Electric:
			{
				mPointLightParticleEmitter->positionAndIntensity.w	= 1 + nrOfParticlesAlive * 0.05f;
				mPointLightParticleEmitter->colorAndRadius.w		= mInitialRadius + nrOfParticlesAlive * 0.012f;
				break;
			}

			case BlowTorchFire:
			{
				mPointLightParticleEmitter->positionAndIntensity.w	= 1 + nrOfParticlesAlive * 0.0005f;
				mPointLightParticleEmitter->colorAndRadius.w		= mInitialRadius + nrOfParticlesAlive * 0.012f;
				break;
			}

			case BlowTorchIdle:
			{
				mPointLightParticleEmitter->positionAndIntensity.w = 0.2f + nrOfParticlesAlive * 0.005f;
				mPointLightParticleEmitter->colorAndRadius.w		= mInitialRadius + nrOfParticlesAlive * 0.005f;
				break;
			}

			case FIRE:
			{
				mPointLightParticleEmitter->positionAndIntensity.w = 1 + nrOfParticlesAlive * 0.001f;
				mPointLightParticleEmitter->colorAndRadius.w		= mInitialRadius + nrOfParticlesAlive * 0.005f;
				break;
			}

			case Explosion:
			{
				mPointLightParticleEmitter->positionAndIntensity.w = 1 + nrOfParticlesAlive * 0.004f;
				mPointLightParticleEmitter->colorAndRadius.w		= mInitialRadius + nrOfParticlesAlive * 0.035f;
				break;
			}
			
			default:
			{
				//Unvalid ParticleType
				break;
			}
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
	
	void GranateTrailLogic( float deltatime )
	{
		for ( int i = 0; i < nrOfParticlesAlive; i++ )
		{
			if( damping[i] > 0.0f )
				damping[i] -= 0.03f;
			
			xPosition[i] += 0.03f * ( 1.0f - damping[i] );
			zPosition[i] += 0.010f * ( 1.0f - damping[i] );
			yPosition[i] += 0.02f * ( 1.0f - damping[i] );
		}
	}

	void SniperTrailLogic( float deltatime )
	{
		for ( int i = 0; i < nrOfParticlesAlive; i++ )
		{
			if( damping[i] > 0.0f )
				damping[i] -= 0.03f;

			xVelocity[i] = xVelocity[i] * damping[i];
			yVelocity[i] = yVelocity[i] * damping[i];
			zVelocity[i] = zVelocity[i] * damping[i];
			
			xPosition[i] += 0.05f * ( 1.0f - damping[i] );
			zPosition[i] += 0.025f * ( 1.0f - damping[i] );
			yPosition[i] += 0.05f * ( 1.0f - damping[i] );
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
			yVelocity[i] = zVelocity[i] * damping[i] * 4;
			xPosition[i] += 0.05f * ( 1.0f - damping[i] );
			zPosition[i] += 0.025f * ( 1.0f - damping[i] );
			yPosition[i] += 0.08f * ( 1.0f - damping[i] );
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
				damping[i] -= 0.017f;

			xVelocity[i] = xVelocity[i] * damping[i];
			zVelocity[i] = zVelocity[i] * damping[i];
			yVelocity[i] = zVelocity[i] * damping[i] * 4;
			xPosition[i] += 0.02f * ( 1.0f - damping[i] );
			zPosition[i] += 0.010f * ( 1.0f - damping[i] );
			yPosition[i] += 0.04f * ( 1.0f - damping[i] );
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
	
	void Fire_FliesLogic( float deltaTime )
	{
		for ( int i = 0; i < nrOfParticlesAlive; i++ )
		{
			float distanceFromSpawnPos = XMVectorGetX( XMVector3Length( XMLoadFloat3( &initialSpawnPos ) - XMLoadFloat3( &XMFLOAT3( xPosition[i], yPosition[i], zPosition[i] ) ) ) );
			if ( distanceFromSpawnPos >= maxDistanceFromSpawnPos[0] )
			{
				float interpolation = max( 0.0f, 1.0f - deltaTime * 0.5f );

				XMVECTOR toCenter	= XMLoadFloat3( &initialSpawnPos ) - XMLoadFloat3( &XMFLOAT3( xPosition[i], yPosition[i], zPosition[i] ) );
				XMVECTOR toRight	= XMVector3Cross( toCenter, XMLoadFloat3( &XMFLOAT3( 0.0f, 1.0f, 0.0f ) ) );
				toCenter = XMVector3Normalize( toCenter );
				xVelocity[i] = xVelocity[i] * interpolation + XMVectorGetX( toCenter * 0.8f + ( i % 2 == 0 ? toRight : -toRight ) * 0.2f ) * ( 1.0f - interpolation );
				yVelocity[i] = yVelocity[i] * interpolation + XMVectorGetY( toCenter * 0.8f + ( i % 2 == 0 ? toRight : -toRight ) * 0.2f ) * ( 1.0f - interpolation );
				zVelocity[i] = zVelocity[i] * interpolation + XMVectorGetZ( toCenter * 0.8f + ( i % 2 == 0 ? toRight : -toRight ) * 0.2f ) * ( 1.0f - interpolation );
			}

		}
	}

	#pragma endregion
};
#endif