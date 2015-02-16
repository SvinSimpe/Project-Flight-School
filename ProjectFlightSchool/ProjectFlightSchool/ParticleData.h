#ifndef _PARTICLEDATA_H_
#define _PARTICLEDATA_H_

#include <windows.h>
#include <utility>
#include <xmmintrin.h>
#include <thread>
#include <DirectXMath.h>
#include <time.h>
using namespace DirectX;

#define MAX_PARTICLES 10000
#define NR_OF_PARTICLE_TYPES 7

enum ParticleType
{
	Smoke,
	Fire,
	Spark,
	Blood,
	MuzzleFlash,
	Smoke_MiniGun,
	Test_Fountain
};

struct ParticleData
{
	#pragma region Members

	int	nrOfParticlesAlive = 0;
	size_t		particleType				= std::numeric_limits<unsigned int>::infinity();

	float*	xPosition = nullptr;
	float*	yPosition = nullptr;
	float*	zPosition = nullptr;

	float*	xVelocity = nullptr;
	float*	yVelocity = nullptr;
	float*	zVelocity = nullptr;

	float*	lifeTime	= nullptr;
	float*	deathTime	= nullptr;
	bool*	isAlive		= nullptr;

	XMFLOAT3 randomDirectionVector;
	int		 nrOfRequestedParticles		= 0;

	#pragma endregion

	#pragma region Functions

	void Initialize( int nrOfParticles ) //NrOfParticles must be a multiple of 4
	{
		srand( (unsigned int)time( NULL ) );

		nrOfParticlesAlive = 0;

		if( nrOfParticles > MAX_PARTICLES )
			nrOfParticles = MAX_PARTICLES;

		// Allocate alligned memory
		xPosition = new float[nrOfParticles];
		yPosition = new float[nrOfParticles];
		zPosition = new float[nrOfParticles];
			
		xVelocity = new float[nrOfParticles];
		yVelocity = new float[nrOfParticles];
		zVelocity = new float[nrOfParticles];
		
		lifeTime	= new float[nrOfParticles];
		deathTime	= new float[nrOfParticles];
		isAlive		= new bool[nrOfParticles];

		for ( int i = 0; i < nrOfParticles; i += 4 )
		{
			//Position
			xPosition[i]	= 0.0f;
			xPosition[i+1]	= 0.0f;
			xPosition[i+2]	= 0.0f;
			xPosition[i+3]	= 0.0f;

			yPosition[i]	= 0.0f;
			yPosition[i+1]	= 0.0f;
			yPosition[i+2]	= 0.0f;
			yPosition[i+3]	= 0.0f;

			zPosition[i]	= 0.0f;
			zPosition[i+1]	= 0.0f;
			zPosition[i+2]	= 0.0f;
			zPosition[i+3]	= 0.0f;

			//Velocity
			xVelocity[i]	= 0.0f;
			xVelocity[i+1]	= 0.0f;
			xVelocity[i+2]	= 0.0f;
			xVelocity[i+3]	= 0.0f;
			 
			yVelocity[i]	= 0.0f;
			yVelocity[i+1]	= 0.0f;
			yVelocity[i+2]	= 0.0f;
			yVelocity[i+3]	= 0.0f;
			 
			zVelocity[i]	= 0.0f;
			zVelocity[i+1]	= 0.0f;
			zVelocity[i+2]	= 0.0f;
			zVelocity[i+3]	= 0.0f;
			
			lifeTime[i]		= 0.0f;
			lifeTime[i+1]	= 0.0f;
			lifeTime[i+2]	= 0.0f;
			lifeTime[i+3]	= 0.0f;
			 
			deathTime[i]	= 0.0f;
			deathTime[i+1]	= 0.0f;
			deathTime[i+2]	= 0.0f;
			deathTime[i+3]	= 0.0f;

			isAlive[i]		= false;
			isAlive[i+1]	= false;
			isAlive[i+2]	= false;
			isAlive[i+3]	= false;
		}

			randomDirectionVector = XMFLOAT3( 0.0f, 0.0f, 0.0f );
	}

	void SwapData( size_t a, size_t b )
	{
		std::swap( xPosition[a], xPosition[b] );
		std::swap( yPosition[a], yPosition[b] );
		std::swap( zPosition[a], zPosition[b] );

		std::swap( xVelocity[a], xVelocity[b] );
		std::swap( yVelocity[a], yVelocity[b] );
		std::swap( zVelocity[a], zVelocity[b] );

		std::swap( lifeTime[a], lifeTime[b] );
		std::swap( isAlive[a], isAlive[b] );
	}

	void Wake( size_t id )
	{
		if( nrOfParticlesAlive < MAX_PARTICLES )
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
			isAlive[id] = false;
			lifeTime[id] = 0.0f;
			SwapData( id, nrOfParticlesAlive - 1 );
			nrOfParticlesAlive--;
		}
	}

	void UpdatePosition( float deltaTime )
	{
		for ( int i = 0; i < nrOfParticlesAlive; i++ )
		{
			xPosition[i] += xVelocity[i] * deltaTime;
			yPosition[i] += yVelocity[i] * deltaTime;
			zPosition[i] += zVelocity[i] * deltaTime;
		}
	}

	void UpdateLifeTime( float deltaTime )
	{
		for ( int i = 0; i < nrOfParticlesAlive; i++ )
		{
			lifeTime[i] += deltaTime;
		}
	}

	void GetRandomSpread( float spreadAngle )
	{
		XMVECTOR aimingDirection = XMLoadFloat3( &randomDirectionVector );

		float randomSpreadAngle = (float)( rand() % (int)spreadAngle * 2 ) - spreadAngle;
	

		XMVECTOR randomAimingDirection = XMVectorSet( 0.0f, 0.0f, 0.0f, 0.0f );
		if( particleType != Test_Fountain )
			randomAimingDirection = XMVector3TransformCoord( aimingDirection, XMMatrixRotationY( XMConvertToRadians( randomSpreadAngle ) ) );		

		else
		{
			randomAimingDirection = XMVector3TransformCoord( aimingDirection, XMMatrixRotationX( XMConvertToRadians( randomSpreadAngle ) ) );
			randomAimingDirection = XMVector3TransformCoord( randomAimingDirection, XMMatrixRotationZ( XMConvertToRadians( randomSpreadAngle ) ) );
		}


		XMStoreFloat3( &randomDirectionVector, randomAimingDirection );
		
		if( particleType != Test_Fountain )
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

	void SetRandomDeathTime( size_t lowerBound, size_t upperBound, size_t particleCount ) // If 2.0f is upperBound, send 20
	{
		for ( size_t i = nrOfParticlesAlive + nrOfRequestedParticles; i < nrOfParticlesAlive + nrOfRequestedParticles + particleCount; i++)
		{
			float randomDeathTime = (float)( rand() % upperBound + (float)lowerBound ) * 0.1f;
			deathTime[i] = randomDeathTime;
		}	
	}

	void SetDirection( float xDirection, float yDirection, float zDirection, size_t particleCount, float spreadAngle )
	{
		for ( size_t i = nrOfParticlesAlive + nrOfRequestedParticles; i < nrOfParticlesAlive + nrOfRequestedParticles + particleCount; i++ )
		{
			if( particleType == MuzzleFlash )
			{
				randomDirectionVector.x = xDirection * GetRandomSpeed( 10, 80 );
 				randomDirectionVector.y = yDirection * GetRandomSpeed( 10, 80 );
				randomDirectionVector.z = zDirection * GetRandomSpeed( 10, 80 );		
			}
			else if( particleType == Smoke_MiniGun )
			{
				randomDirectionVector.x = xDirection * GetRandomSpeed( 10, 20 );
 				randomDirectionVector.y = yDirection * GetRandomSpeed( 10, 20 );
				randomDirectionVector.z = zDirection * GetRandomSpeed( 10, 20 );		
			}
			else if( particleType == Test_Fountain )
			{
				randomDirectionVector.x = xDirection * GetRandomSpeed( 10, 80 );
 				randomDirectionVector.y = yDirection * GetRandomSpeed( 10, 80 );
				randomDirectionVector.z = zDirection * GetRandomSpeed( 10, 80 );		
			}

			GetRandomSpread( spreadAngle );

			xVelocity[i] = randomDirectionVector.x;
			yVelocity[i] = randomDirectionVector.y;
			zVelocity[i] = randomDirectionVector.z;
			
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

	void IncrementValueY( float value )
	{
		for ( int i = 0; i < nrOfParticlesAlive; i++ )
		{
			yVelocity[i] += value;
		}
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
		delete [] xPosition;
		delete [] yPosition;
		delete [] zPosition;

		delete [] xVelocity;
		delete [] yVelocity;
		delete [] zVelocity;

		delete [] lifeTime;
		delete [] isAlive;
	}

	virtual void Emitter( ParticleType particleType, XMFLOAT3 emitterPosition, XMFLOAT3 emiterDirection) = 0;

	virtual void Update( float deltaTime ) = 0;

	virtual void Render( float deltaTime ) = 0;

	#pragma endregion
};

#endif