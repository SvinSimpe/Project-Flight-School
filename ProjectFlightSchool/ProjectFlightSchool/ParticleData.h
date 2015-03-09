#ifndef _PARTICLEDATA_H_
#define _PARTICLEDATA_H_

#include <windows.h>
#include <utility>
#include <xmmintrin.h>
#include <thread>
#include <DirectXMath.h>
#include <time.h>

#include "Events.h"
#include "..\Graphics\LightStructure.h"

using namespace DirectX;

#define MAX_PARTICLES 10000

#define NR_OF_PARTICLE_TYPES 21


#if !defined(SAFE_DELETE_ARRAY)
#define SAFE_DELETE_ARRAY( x ) if( x ){ delete [] x; x = nullptr; }
#endif

#if !defined(SAFE_DELETE)
#define SAFE_DELETE( x ) if( x ){ delete x; x = nullptr; }
#endif


enum FloatPrecision
{
	LOW_PRECISION,		// One decimal
	MEDIUM_PRECISION,	// Two decimals
	HIGH_PRECISION		// Three decimals
};




enum ParticleType
{
	SniperTrail,
	Shell,
	Debris,
	FIRE,
	Spark,
	Spark_Robot,
	Spark_Electric,
	Blood,
	MuzzleFlash,
	Smoke_MiniGun,
	Spores,
	FireSmoke,
	BlowTorchFire,
	BlowTorchIdle,
	Level_Up,
	Level_Inner,
	Explosion,
	ExplosionSmoke,
	NormalSmoke,
	Fire_Flies,
	Hammer_Effect
};

struct ParticleData
{
	#pragma region Members


	int			capacity				= 0;
	int			nrOfParticlesAlive		= 0;
	size_t		particleType			= std::numeric_limits<unsigned int>::infinity();

	float*		xPosition				= nullptr;
	float*		yPosition				= nullptr;
	float*		zPosition				= nullptr;

	float*		xVelocity				= nullptr;
	float*		yVelocity				= nullptr;
	float*		zVelocity				= nullptr;

	float*		initialXVelocity		= nullptr;
	float*		initialYVelocity		= nullptr;
	float*		initialZVelocity		= nullptr;

	float*		lifeTime				= nullptr;
	float*		deathTime				= nullptr;
	bool*		isAlive					= nullptr;


	float*		randRot					= nullptr;
	float*		damping					= nullptr;
	

	int			nrOfRequestedParticles	= 0;

	XMFLOAT3	randomDirectionVector	= XMFLOAT3( 0.0f, 0.0f, 0.0f );
	

	XMFLOAT3	initialSpawnPos	= XMFLOAT3( 0.0f, 0.0f, 0.0f );
	float*		maxDistanceFromSpawnPos = nullptr;

	PointLight* mPointLightParticleEmitter	= nullptr;
	float		mInitialRadius				= 0;
	bool		isLightActive				= false;

	#pragma endregion

	#pragma region Functions

	void Initialize( int nrOfParticles )
	{
		srand( (unsigned int)time( NULL ) );

		capacity = nrOfParticles;
		nrOfParticlesAlive = 0;

		if( nrOfParticles > MAX_PARTICLES )
			nrOfParticles = MAX_PARTICLES;

		// Allocate alligned memory
		xPosition					= new float[nrOfParticles];
		yPosition					= new float[nrOfParticles];
		zPosition					= new float[nrOfParticles];
			
		xVelocity					= new float[nrOfParticles];
		yVelocity					= new float[nrOfParticles];
		zVelocity					= new float[nrOfParticles];

		initialXVelocity			= new float[nrOfParticles];
		initialYVelocity			= new float[nrOfParticles];
		initialZVelocity			= new float[nrOfParticles];
		
		lifeTime					= new float[nrOfParticles];
		deathTime					= new float[nrOfParticles];
		isAlive						= new bool[nrOfParticles];

		randRot						= new float[nrOfParticles];
		damping						= new float[nrOfParticles];
		maxDistanceFromSpawnPos		= new float[nrOfParticles];

		initialSpawnPos = XMFLOAT3( 0.0f, 0.0f, 0.0f );

		

		for ( int i = 0; i < nrOfParticles; i++ )
		{
			xPosition[i]		= 0.0f;
			yPosition[i]		= 0.0f;
			zPosition[i]		= 0.0f;

			xVelocity[i]		= 0.0f;		 
			yVelocity[i]		= 0.0f;		 
			zVelocity[i]		= 0.0f;

			initialXVelocity[i]	= 0.0f;	
			initialYVelocity[i]	= 0.0f;	
			initialZVelocity[i]	= 0.0f;
			
			lifeTime[i]			= 0.0f;			 
			deathTime[i]		= 0.0f;
			isAlive[i]			= false;

			randRot[i]			= 0.0f;
			damping[i]			= 1.0f;
		}
	}

	void SwapData( size_t a, size_t b )
	{
		std::swap( xPosition[a], xPosition[b] );
		std::swap( yPosition[a], yPosition[b] );
		std::swap( zPosition[a], zPosition[b] );

		std::swap( xVelocity[a], xVelocity[b] );
		std::swap( yVelocity[a], yVelocity[b] );
		std::swap( zVelocity[a], zVelocity[b] );

		std::swap( initialXVelocity[a], initialXVelocity[b] );
		std::swap( initialYVelocity[a], initialYVelocity[b] );
		std::swap( initialZVelocity[a], initialZVelocity[b] );

		std::swap( lifeTime[a], lifeTime[b] );
		std::swap( deathTime[a], deathTime[b] );
		std::swap( isAlive[a], isAlive[b] );

		std::swap( randRot[a], randRot[b] );
		std::swap( damping[a], damping[b] );
	}

	void Wake( size_t id )
	{
		if( nrOfParticlesAlive < capacity )
		{
			isAlive[id] = true;
			SwapData( id, nrOfParticlesAlive );
			nrOfParticlesAlive++;
		}
	}

	void Kill( size_t id )
	{
		if( nrOfParticlesAlive > 0 )
		{
			ResetParticle( id );
			SwapData( id, nrOfParticlesAlive - 1 );
			nrOfParticlesAlive--;
		}
	}

	void ResetParticle( size_t id )
	{
		xPosition[id]			= 0.0f;
		zPosition[id]			= 0.0f;
		yPosition[id]			= 0.0f;
				 
		xVelocity[id]			= 0.0f;
		yVelocity[id]			= 0.0f;
		zVelocity[id]			= 0.0f;

		initialXVelocity[id]	= 0.0f;
		initialYVelocity[id]	= 0.0f;
		initialZVelocity[id]	= 0.0f;

		lifeTime[id]			= 0.0f;
		deathTime[id]			= 0.0f;
		isAlive[id]				= false;

		randRot[id]				= 0.0f;
		damping[id]				= 1.0f;
	}

	void UpdatePosition( float deltaTime )
	{
		for ( int i = 0; i < nrOfParticlesAlive; i++ )
		{
			xPosition[i] += ( xVelocity[i] * deltaTime );
			yPosition[i] += ( yVelocity[i] * deltaTime );
			zPosition[i] += ( zVelocity[i] * deltaTime );
		}
	}

	void UpdateLifeTime( float deltaTime )
	{
		for ( int i = 0; i < nrOfParticlesAlive; i++ )
		{
			lifeTime[i] += 0.01f;//deltaTime;
		}
	}

	float GetRandomFloat( float lowerBound, float upperBound, FloatPrecision precision )
	{
		float result = 0.0f;

		switch ( precision )
		{
			case LOW_PRECISION:
			{
				lowerBound *= 10.0f;
				upperBound *= 10.0f;
				result = ( lowerBound + ( rand() % (int)( upperBound - lowerBound + 1 ) ) );
				result *= 0.1f;
				break;
			}

			case MEDIUM_PRECISION:
			{
				lowerBound *= 100.0f;
				upperBound *= 100.0f;
				result = ( lowerBound + ( rand() % (int)( upperBound - lowerBound + 1 ) ) );
				result *= 0.01f;
				break;
			}

			case HIGH_PRECISION:
			{
				lowerBound *= 1000.0f;
				upperBound *= 1000.0f;
				result = ( lowerBound + ( rand() % (int)( upperBound - lowerBound + 1 ) ) );
				result *= 0.001f;
				break;
			}
		default:
			break;
		}
		
		return result;
	}

	void GetRandomSpread( float spreadAngle )
	{
		XMVECTOR aimingDirection = XMLoadFloat3( &randomDirectionVector );

		float randomSpreadAngle = (float)( rand() % (int)spreadAngle * 2 ) - spreadAngle;
		XMVECTOR randomAimingDirection = XMVectorSet( 0.0f, 0.0f, 0.0f, 0.0f );

		if( particleType == Spores || particleType == Fire_Flies ) 
		{
			randomAimingDirection = XMVector3TransformCoord( aimingDirection, XMMatrixRotationX( XMConvertToRadians( randomSpreadAngle ) ) );
			randomAimingDirection = XMVector3TransformCoord( randomAimingDirection, XMMatrixRotationZ( XMConvertToRadians( randomSpreadAngle ) ) );	
		}
	
		else
			randomAimingDirection = XMVector3TransformCoord( aimingDirection, XMMatrixRotationY( XMConvertToRadians( randomSpreadAngle ) ) );

		XMStoreFloat3( &randomDirectionVector, randomAimingDirection );

		if ( particleType == Spark )
		{
			//Elevation for Spark effect
			SparkElevationY( 1.0f, 2.0f );
		}

		if( particleType != SniperTrail		&&
			particleType != Spores			&&
			particleType != Spark			&& 
			particleType != Level_Up		&& 
			particleType != Level_Inner		&& 
			particleType != Explosion		&& 
			particleType != NormalSmoke		&& 
			particleType != BlowTorchIdle	&& 
			particleType != Hammer_Effect	&& 
			particleType != FIRE			)
		{
			//Get random elevation
			float randomElevation = ( (float)( rand() % 20 ) - 10 ) * 0.1f;
			randomDirectionVector.y = randomElevation;
		}
	}   

	float GetRandomSpeed( size_t lowerBound, size_t upperBound )
	{
		return (float)( rand() % upperBound + (float)lowerBound ) * 0.1f;
	}
	
	float GetRandomRotation( int lowerBound, int upperBound )
	{
		return (float)( rand() % upperBound + (float)lowerBound );
	}

	float GetRandomDistance( int lowerBound, int upperBound )
	{
		lowerBound *= 10;
		upperBound *= 10;

		return ( (float)( rand() % upperBound + (float)lowerBound ) ) * 0.1f;
	}

	void SetRandomDistance( size_t lowerBound, size_t upperBound, size_t particleCount )
	{
		for ( size_t i = nrOfParticlesAlive + nrOfRequestedParticles; i < nrOfParticlesAlive + nrOfRequestedParticles + particleCount; i++ )
		{
			maxDistanceFromSpawnPos[i] = GetRandomDistance( lowerBound, upperBound );
		}	
	}

	void SetRandomDeathTime( size_t lowerBound, size_t upperBound, size_t particleCount ) // If 2.0f is upperBound, send 20
	{
		for ( size_t i = nrOfParticlesAlive + nrOfRequestedParticles; i < nrOfParticlesAlive + nrOfRequestedParticles + particleCount; i++ )
		{
			float randomDeathTime = (float)( rand() % upperBound + (float)lowerBound ) * 0.1f;
			deathTime[i] = randomDeathTime;
		}	
	}

	void SetDirection( float xDirection, float yDirection, float zDirection, size_t particleCount, float spreadAngle )
	{
		for ( size_t i = nrOfParticlesAlive + nrOfRequestedParticles; i < nrOfParticlesAlive + nrOfRequestedParticles + particleCount; i++ )
		{
			switch ( particleType )
			{
				case SniperTrail:
				{
					float magnitude = GetRandomSpeed( 40, 400 );
					randomDirectionVector.x = xDirection * magnitude;
					randomDirectionVector.z = zDirection * magnitude;
					break;
				}
				case Shell:
				{
					randomDirectionVector.x = xDirection * GetRandomSpeed( 30, 50 );
 					randomDirectionVector.y = yDirection * GetRandomSpeed( 30, 50 );
					randomDirectionVector.z = zDirection * GetRandomSpeed( 30, 50 );	
					break;
				}

				case Debris:
				{
					randomDirectionVector.x = xDirection * GetRandomSpeed( 10, 80 );
					randomDirectionVector.y = yDirection * GetRandomSpeed( 10, 80 );
					randomDirectionVector.z = zDirection * GetRandomSpeed( 10, 80 );
					break;
				}

				case FIRE:
				{
					randomDirectionVector.x = xDirection * GetRandomSpeed( 20, 50 );
 					randomDirectionVector.y = yDirection * GetRandomSpeed( 20, 30 );
					randomDirectionVector.z = zDirection * GetRandomSpeed( 20, 50 );	
					break;
				}

				case Spark:
				{
					randomDirectionVector.x = xDirection * GetRandomSpeed( 20, 80 );
 					randomDirectionVector.y = yDirection * GetRandomSpeed( 1, 5 );
					randomDirectionVector.z = zDirection * GetRandomSpeed( 20, 80 );
					break;
				}

				case Spark_Robot:
				{
					randomDirectionVector.x = xDirection * GetRandomSpeed( 20, 50 );
		 			randomDirectionVector.y = yDirection * GetRandomSpeed( 20, 50 );
					randomDirectionVector.z = zDirection * GetRandomSpeed( 20, 50 );
					break;
				}

				case Spark_Electric:
				{
					randomDirectionVector.x = xDirection * GetRandomSpeed( 1, 3 );
					randomDirectionVector.z = zDirection * GetRandomSpeed( 1, 3 );
					randomDirectionVector.y = yDirection * GetRandomSpeed( 1, 3 );
					break;
				}

				case Blood:
				{
					randomDirectionVector.x = xDirection * GetRandomSpeed( 20, 80 );
					randomDirectionVector.y = yDirection * GetRandomSpeed( 1, 10 );
					randomDirectionVector.z = zDirection * GetRandomSpeed( 20, 80 );
					break;
				}

				case MuzzleFlash:
				{
					randomDirectionVector.x = xDirection * GetRandomSpeed( 10, 80 );
					randomDirectionVector.y = yDirection * GetRandomSpeed( 10, 80 );
					randomDirectionVector.z = zDirection * GetRandomSpeed( 10, 80 );
					break;
				}

				case Smoke_MiniGun:
				{
					randomDirectionVector.x = xDirection * GetRandomSpeed( 10, 20 );
					randomDirectionVector.y = yDirection * GetRandomSpeed( 10, 20 );
					randomDirectionVector.z = zDirection * GetRandomSpeed( 10, 20 );
					break;
				}

				case Spores:
				{
					randomDirectionVector.x = xDirection * GetRandomSpeed( 2, 3 );
					randomDirectionVector.y = yDirection * GetRandomSpeed( 3, 10 );
					randomDirectionVector.z = zDirection * GetRandomSpeed( 2, 3 );	
					break;
				}

				case FireSmoke:
				{
					randomDirectionVector.x = xDirection * GetRandomSpeed( 20, 40 );
					randomDirectionVector.y = yDirection * GetRandomSpeed( 20, 30 );
					randomDirectionVector.z = zDirection * GetRandomSpeed( 20, 40 );
					break;
				}

				case BlowTorchFire:
				{
					randomDirectionVector.x = xDirection * GetRandomSpeed( 35, 55 );
					randomDirectionVector.y = yDirection * GetRandomSpeed( 55, 85 );
					randomDirectionVector.z = zDirection * GetRandomSpeed( 35, 55 );
					break;
				}

				case BlowTorchIdle:
				{
					randomDirectionVector.x = xDirection * GetRandomSpeed( 80, 80 );
					randomDirectionVector.y = yDirection * GetRandomSpeed( 80, 80 );
					randomDirectionVector.z = zDirection * GetRandomSpeed( 80, 80 );
					break;
				}

				case Level_Up:
				{
					randomDirectionVector.y = yDirection * GetRandomSpeed( 20, 40 );
					break;
				}

				case Level_Inner:
				{
					randomDirectionVector.x = xDirection * GetRandomSpeed( 2, 10 );
					randomDirectionVector.y = yDirection * GetRandomSpeed( 1, 5 );
					randomDirectionVector.z = zDirection * GetRandomSpeed( 2, 10 );	
					break;
				}

				case Explosion:
				{
					randomDirectionVector.x = xDirection * GetRandomSpeed( 1, 60 );
					randomDirectionVector.y = yDirection * GetRandomSpeed( 1, 100 );
					randomDirectionVector.z = zDirection * GetRandomSpeed( 1, 60 );	
					break;
				}

				case ExplosionSmoke:
				{
					randomDirectionVector.x = xDirection * GetRandomSpeed( 1, 70 );
					randomDirectionVector.y = yDirection * GetRandomSpeed( 1, 120 );
					randomDirectionVector.z = zDirection * GetRandomSpeed( 1, 70 );	
					break;
				}

				case NormalSmoke:
				{
					randomDirectionVector.x = xDirection * GetRandomSpeed( 1, 1 );
					randomDirectionVector.y = yDirection * GetRandomSpeed( 1, 2 );
					randomDirectionVector.z = zDirection * GetRandomSpeed( 1, 1 );
					break;
				}

				case Fire_Flies:
				{
					randomDirectionVector.x = xDirection * GetRandomSpeed( 1, 5 );
					randomDirectionVector.y = yDirection * GetRandomSpeed( 1, 3 );
					randomDirectionVector.z = zDirection * GetRandomSpeed( 1, 4 );
					break;
				}

				case Hammer_Effect:
				{
					randomDirectionVector.x = xDirection * GetRandomSpeed( 1, 20 );
					randomDirectionVector.z = zDirection * GetRandomSpeed( 1, 20 );	
					break;
				}

				default:
					// Invalid ParticleType
					break;
			}

			GetRandomSpread( spreadAngle );

			xVelocity[i] = randomDirectionVector.x + initialXVelocity[i];
			yVelocity[i] = randomDirectionVector.y + initialYVelocity[i];
			zVelocity[i] = randomDirectionVector.z + initialZVelocity[i];
			
			ResetRandomDirectionVector();
		}
	}

	void SetPosition( float xPosition, float yPosition, float zPosition, size_t particleCount )
	{
		for ( size_t i = nrOfParticlesAlive + nrOfRequestedParticles; i < nrOfParticlesAlive + nrOfRequestedParticles + particleCount; i++ )
		{
			this->xPosition[i] = xPosition;
			this->yPosition[i] = yPosition;
			this->zPosition[i] = zPosition;
		}
	}

	void SetInitialVelocity( float initialXVelocity, float initialYVelocity, float initialZVelocity, size_t particleCount )
	{
		for ( size_t i = nrOfParticlesAlive + nrOfRequestedParticles; i < nrOfParticlesAlive + nrOfRequestedParticles + particleCount; i++ )
		{
			this->initialXVelocity[i] = initialXVelocity;
			this->initialYVelocity[i] = initialYVelocity;
			this->initialZVelocity[i] = initialZVelocity;
		}
	}

	void SetRandomRotation( size_t particleCount )
	{
		for ( size_t i = nrOfParticlesAlive + nrOfRequestedParticles; i < nrOfParticlesAlive + nrOfRequestedParticles + particleCount; i++ )
		{
			randRot[i] = GetRandomRotation( -100, 100 );
			int k = 4;
		}
	}

	void IncrementValueY( float value )
	{
		for ( int i = 0; i < nrOfParticlesAlive; i++ )
		{
			yVelocity[i] += value;
		}
	}

	void SparkElevationY( float lowerBound, float upperBound )
	{
		float randomElevation = (float)( rand() % (int)upperBound ) - lowerBound;
		randomDirectionVector.y = randomElevation;
	}

	void ResetRandomDirectionVector()
	{
			randomDirectionVector = XMFLOAT3( 0.0f, 0.0f, 0.0f );
	}

	void CheckDeadParticles()
	{
		for ( int i = 0; i < nrOfParticlesAlive; i++ )
		{
  			if( lifeTime[i] >= deathTime[i] )
				Kill( i );	
		}
	}

	void Release()
	{
		// Free the alligned memory
		SAFE_DELETE_ARRAY( xVelocity );
		SAFE_DELETE_ARRAY( yVelocity );
		SAFE_DELETE_ARRAY( zVelocity );

		SAFE_DELETE_ARRAY( xPosition );
		SAFE_DELETE_ARRAY( yPosition );
		SAFE_DELETE_ARRAY( zPosition );
		
		SAFE_DELETE_ARRAY( initialXVelocity );
		SAFE_DELETE_ARRAY( initialYVelocity );
		SAFE_DELETE_ARRAY( initialZVelocity );
		
		if ( mPointLightParticleEmitter )
			SAFE_DELETE( mPointLightParticleEmitter );
		
		SAFE_DELETE_ARRAY( lifeTime );
		SAFE_DELETE_ARRAY( deathTime );
		SAFE_DELETE_ARRAY( isAlive );
		
		SAFE_DELETE_ARRAY( randRot );
		SAFE_DELETE_ARRAY( damping );
		SAFE_DELETE_ARRAY( maxDistanceFromSpawnPos );
	}

	void GenerateCirclePosition( float xPosition, float yPosition, float zPosition, float radius, size_t particleCount )
	{
		for ( size_t i = nrOfParticlesAlive + nrOfRequestedParticles; i < nrOfParticlesAlive + nrOfRequestedParticles + particleCount; i++ )
		{
			
			float randomAngle = (float)( rand() % 360 + 1 );

			float randomRadius = (float)( rand() % (int)( radius * 100.0f ) + 1 );
			randomRadius *= 0.01f;

			XMVECTOR randomDirection = XMVector3TransformCoord( XMVectorSet( 1.0f, 0.0f, 0.0f, 0.0f ), XMMatrixRotationY( XMConvertToRadians( randomAngle ) ) );

			randomDirection *= randomRadius;

			XMFLOAT3 temp = XMFLOAT3( 0.0f, 0.0f, 0.0f );
			XMStoreFloat3( &temp, XMLoadFloat3( &XMFLOAT3( xPosition, yPosition, zPosition ) ) + randomDirection );

			this->xPosition[i] = temp.x;
			this->yPosition[i] = temp.y;
			this->zPosition[i] = temp.z;
		}
	}

	void GenerateCircleEdgePosition( float xPosition, float yPosition, float zPosition, float radius, size_t particleCount )
	{
		for ( size_t i = nrOfParticlesAlive + nrOfRequestedParticles; i < nrOfParticlesAlive + nrOfRequestedParticles + particleCount; i++ )
		{
			float randomAngle = (float)( rand() % 360 + 1 );

			XMVECTOR randomDirection = XMVector3TransformCoord( XMVectorSet( 1.0f, 0.0f, 0.0f, 0.0f ), XMMatrixRotationY( XMConvertToRadians( randomAngle ) ) );

			randomDirection *= radius;

			XMFLOAT3 temp = XMFLOAT3( 0.0f, 0.0f, 0.0f );
			XMStoreFloat3( &temp, XMLoadFloat3( &XMFLOAT3( xPosition, yPosition, zPosition ) ) + randomDirection );

			this->xPosition[i] = temp.x;
			this->yPosition[i] = temp.y;
			this->zPosition[i] = temp.z;
		}
	}

	void GeneratePlanePosition( float xPosition, float yPosition, float zPosition, int width, int height, size_t particleCount )
	{
		float randX, randZ = 0;
		float xMin = ( xPosition - width  ) * 100.0f;
		float xMax = ( xPosition + width  ) * 100.0f;
		float zMin = ( zPosition - height ) * 100.0f;
		float zMax = ( zPosition + height ) * 100.0f;


		for ( size_t i = nrOfParticlesAlive + nrOfRequestedParticles; i < nrOfParticlesAlive + nrOfRequestedParticles + particleCount; i++ )
		{
			randX = ( xMin + ( rand() % (int)( xMax - xMin + 1 ) ) );
			randZ = ( zMin + ( rand() % (int)( zMax - zMin + 1 ) ) );

			randX *= 0.01f;
			randZ *= 0.01f;

			this->xPosition[i] = (float)randX;
			this->yPosition[i] = yPosition;
			this->zPosition[i] = (float)randZ;
		}
	}

	#pragma endregion
};
#endif