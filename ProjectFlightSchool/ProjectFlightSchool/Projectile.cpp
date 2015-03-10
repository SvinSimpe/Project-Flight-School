#include "Projectile.h"

HRESULT Projectile::Update( float deltaTime )
{
	if( mWeaponType == GRENADELAUNCHER )
	{
		float acceleration	= ( 2.0f * deltaTime );
		mDirection.y -= acceleration;
		RenderManager::GetInstance()->RequestParticleSystem( mPlayerID, GranateTrail, mPosition, DirectX::XMFLOAT3( 0.0f, 1.0f, 0.0f ), DirectX::XMFLOAT3( 0.0f, 0.0f, 0.0f ) );
		////////

	}


	mPosition.x += mDirection.x * mSpeed * deltaTime;
	mPosition.y += mDirection.y * mSpeed * deltaTime;
	mPosition.z += mDirection.z * mSpeed * deltaTime;


	if( mWeaponType == GRENADELAUNCHER )
	{
		if( mPosition.y <= 0.0f )
			Reset();
	}

	else
	{
		if( mLifeTime <= 0.0f || mPosition.y < -0.05f )
		{
			Reset();
		}
		else
			mLifeTime -=deltaTime;
	}
	
	mBoundingCircle->center				= mPosition;
	mPointLight->positionAndIntensity	= XMFLOAT4( mPosition.x, mPosition.y, mPosition.z, 1.0f );

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

void Projectile::SetDirection( unsigned int playerID, unsigned int id, unsigned int teamID, XMFLOAT3 startPosition, XMFLOAT3 direction, float speed, float range, float damage, WeaponType weaponType )
{
	Reset();
	mPlayerID		= playerID;
	mID				= id;
	mTeamID			= teamID;
	mPosition		= startPosition;
	mDirection		= direction;	
	mSpeed			= speed;
	mLifeTime		= range;
	mIsActive		= true;
	mDamage			= damage;
	mWeaponType		= weaponType;
	IEventPtr E1( new Event_Add_Point_Light( mPointLight ) );
	EventManager::GetInstance()->QueueEvent( E1 );
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
	mTeamID		= (UINT)-1;
	mPlayerID	= 0;
	mIsActive	= false;
	mSpeed		= 20.0f;
	mLifeTime	= 4.0f;
	mDamage		= 0.0f;
	mWeaponType	= MINIGUN;
	mHits.clear();
	IEventPtr E1( new Event_Remove_Point_Light( mPointLight ) );
	EventManager::GetInstance()->QueueEvent( E1 );
}

BoundingCircle* Projectile::GetBoundingCircle() const
{
	return mBoundingCircle;
}

unsigned int Projectile::GetID() const
{
	return mID;
}

unsigned int Projectile::GetTeamID() const
{
	return mTeamID;
}

unsigned int Projectile::GetPlayerID() const
{
	return mPlayerID;
}

float Projectile::GetDamage() const
{
	return mDamage;
}

XMFLOAT3 Projectile::GetDirection() const
{
	return mDirection;
}

XMFLOAT3 Projectile::GetPosition() const
{
	return mPosition;
}

WeaponType Projectile::GetWeaponType() const
{
	return mWeaponType;
}

//Returns true if it set the hitID and false if it already had it.
bool Projectile::SetHit( UINT hitID )
{
	bool result = true;
	for( auto h : mHits )
	{
		if( h == hitID )
		{
			result = false;
		}
	}
	if( result )
	{
		mHits.push_back( hitID );
	}
	return result;
}

HRESULT Projectile::Initialize()
{
	mSpeed			= 20.0f;
	mLifeTime		= 4.0f;
	mBoundingCircle = new BoundingCircle( 0.5f );
	Graphics::GetInstance()->LoadStatic3dAsset( "../Content/Assets/PermanentAssets/Bullet/", "bullet2.pfs", mProjectileAsset );
	mWeaponType		= MINIGUN;
	mPointLight		= new PointLight;
	mPointLight->colorAndRadius				= XMFLOAT4( 1.0f, 1.0f, 1.0f, 0.5f );
	mPointLight->positionAndIntensity		= XMFLOAT4( 0.0f, 0.0f, 0.0f, 1.0f );

	return S_OK;
}

void Projectile::Release()
{
	SAFE_DELETE( mBoundingCircle );
	SAFE_DELETE( mPointLight );
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
	mDamage			= 0.0f;
	mWeaponType		= MINIGUN;
	mHits			= std::vector<UINT>( 0 );
}

Projectile::~Projectile()
{

}
