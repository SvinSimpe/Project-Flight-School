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
	//Graphics::GetInstance()->RenderStatic3dAsset( 1, mPosition.x, mPosition.y, mPosition.z );
	RenderManager::GetInstance()->AddObject3dToList( mProjectileAsset, mPosition, XMFLOAT3( 0.0f, mRotation, 0.0f ) );

	return S_OK;
}

void Projectile::SetDirection( unsigned int playerID, unsigned int id, XMFLOAT3 startPosition, XMFLOAT3 direction )
{
	Reset();
	mID				= id;
	mPlayerID		= playerID;
	mPosition		= startPosition;

	float radians = atan2f( direction.z, direction.x );

	mDirection.x	= cosf( -radians );
	mDirection.y	= 0.0f;
	mDirection.z	= -sinf( -radians );
	mRotation		= -atan2f( mDirection.z, mDirection.x );
	mIsActive		= true;
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
	mID			= 0;
	mPlayerID	= 0;
	mIsActive	= false;
	mSpeed		= 20.0f;
	mLifeTime	= 4.0f;
}

BoundingCircle* Projectile::GetBoundingCircle() const
{
	return mBoundingCircle;
}

unsigned int Projectile::GetPlayerID() const
{
	return mPlayerID;
}

unsigned int Projectile::GetID() const
{
	return mID;
}

HRESULT Projectile::Initialize()
{
	mSpeed		= 20.0f;
	mLifeTime	= 4.0f;
	mBoundingCircle = new BoundingCircle( 0.5f );
	Graphics::GetInstance()->LoadStatic3dAsset( "../Content/Assets/PermanentAssets/Bullet/", "bullet.pfs", mProjectileAsset );

	return S_OK;
}

void Projectile::Release()
{
	SAFE_DELETE( mBoundingCircle );
}

Projectile::Projectile()
{
	mID				= 0;
	mPlayerID		= 0;
	mPosition		= XMFLOAT3( 0.0f, 0.0f, 0.0f );
	mDirection		= XMFLOAT3( 0.0f, 0.0f, 0.0f );
	mSpeed			= 0.0f;
	mIsActive		= false;
	mRotation		= 0.0f;
	mLifeTime		= 0.0f;
	mBoundingCircle	= nullptr;	
}

Projectile::~Projectile()
{

}