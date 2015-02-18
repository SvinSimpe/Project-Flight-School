#include "Projectile.h"

HRESULT Projectile::Update( float deltaTime )
{
	mPosition.x += mDirection.x * mSpeed * deltaTime;
	mPosition.y += mDirection.y * mSpeed * deltaTime;
	mPosition.z += mDirection.z * mSpeed * deltaTime;

	if( mLifeTime <= 0.0f || mPosition.y < -0.05f )
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
	float yaw	= -atan2f( mDirection.z, mDirection.x );
	float roll	= -XMVectorGetX( 
					XMVector3AngleBetweenVectors( 
					XMLoadFloat3( &XMFLOAT3( mDirection.x, 0.0f, mDirection.z ) ),
					XMLoadFloat3( &mDirection ) ) );
	
	RenderManager::GetInstance()->AddObject3dToList( mProjectileAsset, mPosition, XMFLOAT3( 0.0f, yaw, roll ) );

	return S_OK;
}

void Projectile::SetDirection( unsigned int playerID, unsigned int id, XMFLOAT3 startPosition, XMFLOAT3 direction, float speed, float range )
{
	Reset();
	mPlayerID		= playerID;
	mID				= id;
	mPosition		= startPosition;
	mDirection		= direction;	
	mSpeed			= speed;
	mLifeTime		= range;
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
	mSpeed			= 20.0f;
	mLifeTime		= 4.0f;
	mBoundingCircle = new BoundingCircle( 0.5f );
	Graphics::GetInstance()->LoadStatic3dAsset( "../Content/Assets/PermanentAssets/Bullet/", "bullet2.pfs", mProjectileAsset );

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
	mLifeTime		= 0.0f;
	mBoundingCircle	= nullptr;	
}

Projectile::~Projectile()
{

}
