#include "Turret.h"

void Turret::Fire()
{
	IEventPtr E1( new Event_Projectile_Fired( -1, mUpperBody->pos, mUpperBody->dir ));
	EventManager::GetInstance()->QueueEvent( E1 );
	mShootTimer = SHOOTCOOLDOWN;
}

void Turret::TrackTarget()
{
	if( mTarget && mTeamID != -1 )
	{
		mTracking = true;

		XMVECTOR turretToTargetVec	= ( XMLoadFloat3( &mTarget->GetBoundingCircle()->center ) - XMLoadFloat3( &mBoundingCircle->center ) );
		turretToTargetVec			= XMVector3Normalize( turretToTargetVec );

		float radians				= atan2f( XMVectorGetZ( turretToTargetVec ), XMVectorGetX( turretToTargetVec ) );
		mUpperBody->dir.y			= -radians;

		//XMStoreFloat3( &mUpperBody->dir, turretToTargetVec );

		if( mShootTimer <= 0.0f )
		{
			Fire();
		}
	}
	else
	{
		mTracking = false;

		mUpperBody->dir.x = 0.0f;
		mUpperBody->dir.z = 0.0f;
	}
	mLowerBody->dir = mUpperBody->dir;
}

void Turret::SetTeamID( int team )
{
	mTeamID = team;
}

void Turret::PickTarget( std::vector<RemotePlayer*> targets )
{
	RemotePlayer* target = nullptr;
	if( !mTarget )
	{
		target = targets.at(0);
		for( UINT i = 1; i < targets.size() - 1; i++ )
		{
			if( mBoundingCircle->Intersect( targets.at(i)->GetBoundingCircle() ) )
			{
				if( targets.at(i)->GetHP() < targets.at(i-1)->GetHP() )
				{
					target = targets.at(i);
				}
			}
		}
		if( target == targets.at(0) && !mBoundingCircle->Intersect( targets.at(0)->GetBoundingCircle() ) )
		{
			target = nullptr;
		}
	}
	mTarget = target;
}

HRESULT Turret::Update( float deltaTime )
{
	mShootTimer -= deltaTime;

	if( !mTracking  )
	{
		mUpperBody->dir.y += sin( deltaTime );
	}
	TrackTarget();

	return S_OK;
}

void Turret::Render()
{
	//float radians = atan2f( mUpperBody->dir.z, mUpperBody->dir.x );
	//RenderManager::GetInstance()->AddObject3dToList( mUpperBody->model, mUpperBody->pos, XMFLOAT3( 0.0f, -radians, 0.0f ) );
	//RenderManager::GetInstance()->AddObject3dToList( mLowerBody->model, mLowerBody->pos, XMFLOAT3( 0.0f, -radians, 0.0f ) );
	
	RenderManager::GetInstance()->AddObject3dToList( mUpperBody->model, mUpperBody->pos, mUpperBody->dir );
	RenderManager::GetInstance()->AddObject3dToList( mLowerBody->model, mLowerBody->pos, mLowerBody->dir );
}

void Turret::Initialize()
{
	mTeamID					= -1;
	mUpperBody				= new Upper();
	Graphics::GetInstance()->LoadStatic3dAsset( "../Content/Assets/Turret/", "turret.pfs", mUpperBody->model );
	mUpperBody->pos			= XMFLOAT3( 0.0f, 1.0f, 0.0f );
	mUpperBody->dir			= XMFLOAT3( 0.0f, 0.0f, 0.0f );

	mLowerBody				= new Lower();
	Graphics::GetInstance()->LoadStatic3dAsset( "../Content/Assets/Turret/", "base2.pfs", mLowerBody->model );
	mLowerBody->pos			= XMFLOAT3( 0.0f, 0.0f, 0.0f );
	mLowerBody->dir			= XMFLOAT3( 0.0f, 0.0f, 0.0f );

	mRange					= 10.0f;
	mLoadOut				= new LoadOut();
	mLoadOut->rangedWeapon	= new RangedInfo( "Machine Gun", 5.0f, 1, 5.0f, 2, 0 );

	mBoundingCircle			= new BoundingCircle( mLowerBody->pos, mRange );
}

void Turret::Release()
{
	mLoadOut->Release();
	SAFE_DELETE( mLoadOut );
	SAFE_DELETE( mUpperBody );
	SAFE_DELETE( mLowerBody );
	SAFE_DELETE( mBoundingCircle );
}

Turret::Turret()
{
	mTeamID			= -1;
	mUpperBody		= nullptr;
	mLowerBody		= nullptr;

	mRange			= 0.0f;
	mLoadOut		= nullptr;
	mShootTimer		= 0.0f;
	mTracking		= false;

	mBoundingCircle	= nullptr;
	mTarget			= nullptr; // Is never initialized
}

Turret::~Turret()
{
}
