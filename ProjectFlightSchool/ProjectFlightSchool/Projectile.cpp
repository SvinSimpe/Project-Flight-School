#include "Projectile.h"

HRESULT Projectile::Update( float deltaTime )
{
	mPosition.x += mDirection.x * mSpeed * deltaTime;
	mPosition.z += mDirection.z * mSpeed * deltaTime;

	if( mLifeTime <= 0.0f )
	{
		Reset();
	}
	else
		mLifeTime -=deltaTime;

	mBoundingCircle->center = mPosition;

	return S_OK;
}

HRESULT Projectile::Render( )
{

	mRotation += 0.10f;
	//Graphics::GetInstance()->RenderStatic3dAsset( 1, mPosition.x, mPosition.y, mPosition.z );
	Graphics::GetInstance()->RenderStatic3dAsset( 1, mPosition, XMFLOAT3( 0.0f, mRotation, 0.0f ) );

	return S_OK;
}

void Projectile::SetDirection( XMFLOAT3 startPosition, XMFLOAT3 direction )
{
	Reset();
	mPosition		= startPosition;
	mDirection.x	= cosf( direction.y );
	mDirection.y	= 0.0f;
	mDirection.z	= -sinf( direction.y );
}

void Projectile::SetIsActive( bool isActive )
{
	Reset();
	mIsActive = isActive;
}

bool Projectile::IsActive() const
{
	return mIsActive;
}

void Projectile::Reset()
{
	mIsActive = false;
	mSpeed		= 20.0f;
	mLifeTime	= 5.0f;
}

BoundingCircle* Projectile::GetBoundingCircle() const
{
	return mBoundingCircle;
}

HRESULT Projectile::Initialize()
{
	mSpeed		= 20.0f;
	mLifeTime	= 5.0f;
	mBoundingCircle = new BoundingCircle( 0.5f );
	return S_OK;
}

void Projectile::Release()
{

}

Projectile::Projectile()
{
	mPosition		= XMFLOAT3();
	mDirection		= XMFLOAT3();
	mSpeed			= 0.0f;
	mIsActive		= false;
	mRotation		= 0.0f;
	mLifeTime		= 0.0f;
	mBoundingCircle	= nullptr;	
}

Projectile::~Projectile()
{

}