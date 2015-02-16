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
	bool*	isAlive		= nullptr;

	XMFLOAT3 randomDirectionVector[4];
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
		xPosition = (float*)_mm_malloc( nrOfParticles * sizeof(float), 16 );
		yPosition = (float*)_mm_malloc( nrOfParticles * sizeof(float), 16 );
		zPosition = (float*)_mm_malloc( nrOfParticles * sizeof(float), 16 );

		xVelocity = (float*)_mm_malloc( nrOfParticles * sizeof(float), 16 );
		yVelocity = (float*)_mm_malloc( nrOfParticles * sizeof(float), 16 );
		zVelocity = (float*)_mm_malloc( nrOfParticles * sizeof(float), 16 );

		lifeTime = (float*)_mm_malloc( nrOfParticles * sizeof(float), 16 );
		isAlive	 = (bool*)_mm_malloc(  nrOfParticles * sizeof(bool),  16 );

		for ( int i = 0; i < nrOfParticles; i += 4 )
		{
			//========= Initialize position ===========
			__m128 xmm0 = _mm_load_ps( &xPosition[i] );
			xmm0 = _mm_set_ps( 0.0f, 0.0f, 0.0f, 0.0f );
			_mm_store_ps( &xPosition[i], xmm0 );

			xmm0 = _mm_load_ps( &yPosition[i] );
			xmm0 = _mm_set_ps( 0.0f, 0.0f, 0.0f, 0.0f );
			_mm_store_ps( &yPosition[i], xmm0 );

			xmm0 = _mm_load_ps( &zPosition[i] );
			xmm0 = _mm_set_ps( 0.0f, 0.0f, 0.0f, 0.0f );
			_mm_store_ps( &zPosition[i], xmm0 );

			//========= Initialize velocity ===========
			xmm0 = _mm_load_ps( &xVelocity[i] );
			xmm0 = _mm_set_ps( 0.0f, 0.0f, 0.0f, 0.0f );
			_mm_store_ps( &xVelocity[i], xmm0 );

			xmm0 = _mm_load_ps( &yVelocity[i] );
			xmm0 = _mm_set_ps( 0.0f, 0.0f, 0.0f, 0.0f );
			_mm_store_ps( &yVelocity[i], xmm0 );

			xmm0 = _mm_load_ps( &zVelocity[i] );
			xmm0 = _mm_set_ps( 0.0f, 0.0f, 0.0f, 0.0f );
			_mm_store_ps( &zVelocity[i], xmm0 );

			//========= Initialize life time ===========
			xmm0 = _mm_load_ps( &lifeTime[i] );
			xmm0 = _mm_set_ps( 0.0f, 0.0f, 0.0f, 0.0f );
			_mm_store_ps( &lifeTime[i], xmm0 );

			//========= Initialize isAlive ===========
			isAlive[i]		= false;
			isAlive[i+1]	= false;
			isAlive[i+2]	= false;
			isAlive[i+3]	= false;
		}

		for (size_t i = 0; i < 4; i++)
			randomDirectionVector[i] = XMFLOAT3( 0.0f, 0.0f, 0.0f );
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
		const __m128 scalar = _mm_set1_ps( deltaTime );

		for ( int i = 0; i < nrOfParticlesAlive; i += 4 )
		{
			//==== xPosition += xVelocity * deltaTime ====
			__m128 xmm0				= _mm_load_ps( &xPosition[i] );
			__m128 xmm1				= _mm_load_ps( &xVelocity[i] );
			__m128 velocityDelta	= _mm_mul_ps( xmm1, scalar );
			xmm0 = _mm_add_ps( xmm0, velocityDelta );
			_mm_store_ps( &xPosition[i], xmm0 );

			//==== yPosition += yVelocity * deltaTime ====
			xmm0				= _mm_load_ps( &yPosition[i] );
			xmm1				= _mm_load_ps( &yVelocity[i] );
			velocityDelta	= _mm_mul_ps( xmm1, scalar );
			xmm0 = _mm_add_ps( xmm0, velocityDelta ); 
			_mm_store_ps( &yPosition[i], xmm0 );

			//==== zPosition += zVelocity * deltaTime ====
			xmm0				= _mm_load_ps( &zPosition[i] );
			xmm1				= _mm_load_ps( &zVelocity[i] );
			velocityDelta	= _mm_mul_ps( xmm1, scalar );
			xmm0 = _mm_add_ps( xmm0, velocityDelta );
			_mm_store_ps( &zPosition[i], xmm0 );
		}
	}

	void UpdateLifeTime( float deltaTime )
	{
		const __m128 delta = _mm_set1_ps( deltaTime );

		for ( int i = 0; i < nrOfParticlesAlive; i += 4 )
		{
			//======== lifeTime -= deltaTime  ========
			__m128 xmm0 = _mm_load_ps( &lifeTime[i] );
			xmm0 = _mm_sub_ps( xmm0, delta );
			_mm_store_ps( &lifeTime[i], xmm0 );
		}
	}

	void GetRandomSpread( float spreadAngle )
	{
		XMVECTOR aimingDirection = XMLoadFloat3( &randomDirectionVector[0] );

		for (size_t i = 0; i < 4; i++)
		{
			float randomSpreadAngle = (float)( rand() % (int)spreadAngle * 2 ) - spreadAngle;
		

			XMVECTOR randomAimingDirection = XMVectorSet( 0.0f, 0.0f, 0.0f, 0.0f );
			if( particleType != Test_Fountain )
				randomAimingDirection = XMVector3TransformCoord( aimingDirection, XMMatrixRotationY( XMConvertToRadians( randomSpreadAngle ) ) );		

			else
			{
				randomAimingDirection = XMVector3TransformCoord( aimingDirection, XMMatrixRotationX( XMConvertToRadians( randomSpreadAngle ) ) );
				randomAimingDirection = XMVector3TransformCoord( randomAimingDirection, XMMatrixRotationZ( XMConvertToRadians( randomSpreadAngle ) ) );
			}


			XMStoreFloat3( &randomDirectionVector[i], randomAimingDirection );
			
			if( particleType != Test_Fountain )
			{
				//Get random elevation
				float randomElevation = ( (float)( rand() % 20 ) - 10 ) * 0.1f;
				randomDirectionVector[i].y = randomElevation;
			}
		}
	}

	float GetRandomSpeed( size_t lowerBound, size_t upperBound )
	{
		return (float)( rand() % upperBound + (float)lowerBound ) * 0.1f;
	}

	void SetLifeTime( size_t lowerBound, size_t upperBound, size_t particleCount ) // If 2.0f is uppeBound, send 20
	{
		//======== this->lifeTime = randLifeTime  ========
		__declspec( align( 16 ) ) float randomLife[4] = {0.0f};
		__m128 xmm0;

		for ( size_t i = nrOfParticlesAlive + nrOfRequestedParticles; i < nrOfParticlesAlive + nrOfRequestedParticles + particleCount; i += 4 )
		{
			while( i % 4 != 0 )
				i--;
			
			// Generate and set random life time
			for (size_t j = 0; j < 4; j++)
				randomLife[j] = (float)( rand() % upperBound + (float)lowerBound ) * 0.1f;

			// Store random lifeTime
			xmm0 = _mm_load_ps( &lifeTime[i] );
			xmm0 = _mm_set_ps( randomLife[0], randomLife[1], randomLife[2], randomLife[3] );
			_mm_store_ps( &lifeTime[i], xmm0 );
		}	
	}

	void SetDirection( float xDirection, float yDirection, float zDirection, size_t particleCount, float spreadAngle )
	{
		for ( size_t i = nrOfParticlesAlive + nrOfRequestedParticles; i < nrOfParticlesAlive + nrOfRequestedParticles + particleCount; i += 4 )
		{
			while( i % 4 != 0 )
				i--;

			if( particleType == Blood )
			{
				randomDirectionVector[0].x = xDirection * GetRandomSpeed( 10, 20 );
 				randomDirectionVector[0].y = yDirection * GetRandomSpeed( 10, 20 );
				randomDirectionVector[0].z = zDirection * GetRandomSpeed( 10, 20 );		
			}
			else if( particleType == MuzzleFlash )
			{
				randomDirectionVector[0].x = xDirection * GetRandomSpeed( 10, 80 );
 				randomDirectionVector[0].y = yDirection * GetRandomSpeed( 10, 80 );
				randomDirectionVector[0].z = zDirection * GetRandomSpeed( 10, 80 );		
			}
			else if( particleType == Smoke_MiniGun )
			{
				randomDirectionVector[0].x = xDirection * GetRandomSpeed( 10, 20 );
 				randomDirectionVector[0].y = yDirection * GetRandomSpeed( 10, 20 );
				randomDirectionVector[0].z = zDirection * GetRandomSpeed( 10, 20 );		
			}
			else if( particleType == Test_Fountain )
			{
				randomDirectionVector[0].x = xDirection * GetRandomSpeed( 10, 20 );
 				randomDirectionVector[0].y = yDirection * GetRandomSpeed( 10, 20 );
				randomDirectionVector[0].z = zDirection * GetRandomSpeed( 10, 20 );		
			}

			GetRandomSpread( spreadAngle );

			//==== xVelocity = xDirection ====
			__m128 xmm0	= _mm_load_ps( &xVelocity[i] );
			xmm0 = _mm_set_ps( randomDirectionVector[0].x, randomDirectionVector[1].x, randomDirectionVector[2].x, randomDirectionVector[3].x );
			_mm_store_ps( &xVelocity[i], xmm0 );

			//==== yVelocity = yDirection ====
			xmm0	= _mm_load_ps( &yVelocity[i] );
			xmm0 = _mm_set_ps( randomDirectionVector[0].y, randomDirectionVector[1].y, randomDirectionVector[2].y, randomDirectionVector[3].y );
			_mm_store_ps( &yVelocity[i], xmm0 );

			//==== yVelocity = yDirection ====
			xmm0	= _mm_load_ps( &zVelocity[i] );
			xmm0 = _mm_set_ps( randomDirectionVector[0].z, randomDirectionVector[1].z, randomDirectionVector[2].z, randomDirectionVector[3].z );
			_mm_store_ps( &zVelocity[i], xmm0 );

			ResetRandomDirectionVector();
		}
	}

	void SetPosition( float xPosition, float yPosition, float zPosition, size_t particleCount )
	{
		for ( size_t i = nrOfParticlesAlive + nrOfRequestedParticles; i < nrOfParticlesAlive + nrOfRequestedParticles + particleCount; i += 4 )
		{
			while( i % 4 != 0 )
				i--;

			//==== this->xPosition = xPosition ====
			__m128 xmm0	= _mm_load_ps( &this->xPosition[i] );
			xmm0 = _mm_set_ps( xPosition, xPosition, xPosition, xPosition );
			_mm_store_ps( &this->xPosition[i], xmm0 );

			//==== this->yPosition = yPosition ====
			xmm0		= _mm_load_ps( &this->yPosition[i] );
			xmm0 = _mm_set_ps( yPosition, yPosition, yPosition, yPosition );
			_mm_store_ps( &this->yPosition[i], xmm0 );

			//==== this->yPosition = zPosition ====
			xmm0		= _mm_load_ps( &this->zPosition[i] );
			xmm0 = _mm_set_ps( zPosition, zPosition, zPosition, zPosition );
			_mm_store_ps( &this->zPosition[i], xmm0 );
		}
	}

	void IncrementValueY()
	{
		const __m128 increment = _mm_set1_ps( 0.01f );

		for ( int i = 0; i < nrOfParticlesAlive; i += 4 )
		{
			//======== lifeTime -= deltaTime  ========
			__m128 xmm0 = _mm_load_ps( &yVelocity[i] );
			xmm0 = _mm_add_ps( xmm0, increment );
			_mm_store_ps( &yVelocity[i], xmm0 );
		}
	}

	void ResetRandomDirectionVector()
	{
		for (size_t i = 0; i < 4; i++)
			randomDirectionVector[i] = XMFLOAT3( 0.0f, 0.0f, 0.0f );
	}

	void CheckDeadParticles()
	{
		for ( int i = 0; i < nrOfParticlesAlive; i++ )
		{
  			if( lifeTime[i] <= 0.0f )
				Kill( i );	
		}
	}

	void Release()
	{
		// Free the alligned memory
		_mm_free( xPosition );
		_mm_free( yPosition );
		_mm_free( zPosition );

		_mm_free( xVelocity );
		_mm_free( yVelocity );
		_mm_free( zVelocity );

		_mm_free( lifeTime );
		_mm_free( isAlive );
	}

	virtual void Emitter( ParticleType particleType, XMFLOAT3 emitterPosition, XMFLOAT3 emiterDirection) = 0;

	virtual void Update( float deltaTime ) = 0;

	virtual void Render( float deltaTime ) = 0;

	#pragma endregion
};

#endif