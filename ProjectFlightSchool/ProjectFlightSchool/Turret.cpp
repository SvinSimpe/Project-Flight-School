#include "Turret.h"

void Turret::IdleTurret::Action( Turret* t )
{
	XMStoreFloat3( &t->mUpperBody->dir, XMVector3TransformCoord( XMLoadFloat3( &t->mUpperBody->dir ), XMMatrixRotationY( t->mDT ) ) );
	t->mMiddleBody->dir = t->mUpperBody->dir;
}

void Turret::AttackingTurret::Action( Turret* t )
{
	XMVECTOR homingVec	= XMVector3Normalize( XMLoadFloat3( &t->mTarget->GetBoundingCircle()->center ) - XMLoadFloat3( &t->mBoundingCircle->center ) );

	// Rotate y direction
	XMVECTOR rightDir = XMVector3Cross( XMLoadFloat3( &XMFLOAT3( 0.0f, 0.1f, 0.0f ) ), XMLoadFloat3( &t->mUpperBody->dir ) );

	float rightDot	= XMVectorGetX( XMVector3Dot( rightDir, homingVec ) );
	float leftDot	= XMVectorGetX( XMVector3Dot( -rightDir, homingVec ) );

	float angleY = t->mDT * ( rightDot < leftDot ? -leftDot : rightDot ) * TURRET_ROTATION_SPEED;

	XMStoreFloat3( &t->mUpperBody->dir, XMVector3TransformNormal( XMLoadFloat3( &t->mUpperBody->dir ), XMMatrixRotationY( angleY ) ) );

	// Rotate Z
	t->mUpperBody->dir.y = XMVectorGetY( homingVec );
	XMStoreFloat3( &t->mUpperBody->dir, XMVector3Normalize( XMLoadFloat3( &t->mUpperBody->dir ) ) );

	t->mMiddleBody->dir = t->mUpperBody->dir;

	float dot = XMVectorGetY( XMVector3Dot( XMLoadFloat3( &t->mUpperBody->dir ), homingVec ) );

	if( t->mShootTimer <= 0.0f && dot > 0.90f )
	{
		t->Fire();
	}
}

void Turret::Fire()
{
	XMFLOAT3 firePos;
	XMStoreFloat3( &firePos, XMLoadFloat3( &mUpperBody->pos ) + XMLoadFloat3( &mUpperBody->dir ) * 3.0f );

	IEventPtr E1( new Event_Client_Fired_Projectile( mTeamID, firePos, mUpperBody->dir, TURRET_FIRE_SPEED, TURRET_RANGE ) );

	EventManager::GetInstance()->QueueEvent( E1 );
	mShootTimer = SHOOTCOOLDOWN;
}

void Turret::SwitchMode( ITurretMode* mode )
{
	mCurrentMode = mode;
}

void Turret::CheckTarget( std::vector<RemotePlayer*> targets )
{
	// Update it here
	bool targetInGame = false;
	for( auto& it : targets )
	{
		if( it == mTarget )
		{
			targetInGame = true;
			mTarget = it;
			break;
		}
	}
	if( !targetInGame || !mBoundingCircle->Intersect( mTarget->GetBoundingCircle() ) || !mTarget->IsAlive() || mTarget->GetTeam() == mTeamID )
	{
		mTarget = nullptr;
		SwitchMode( mIdle );
	}
}

void Turret::PickTarget( std::vector<RemotePlayer*> targets )
{
	if( !mTarget )
	{
		SwitchMode( mIdle );
		std::vector<RemotePlayer*> temps;
		for( auto& it : targets )
		{
			if( it->GetTeam() == mTeamID )
			{
				continue;
			}
			else if( it->IsAlive() && mBoundingCircle->Intersect( it->GetBoundingCircle() ) )
			{
				temps.push_back( it );
			}
		}
		if( temps.size() > 0 )
		{
			mTarget = temps.at(0);
			for( auto& it : temps )
			{
				if( mTarget->GetHP() > it->GetHP() )
				{
					mTarget = it;
				}
			}
			SwitchMode( mAttacking );
		}
	}
	else // If it has a target
	{
		CheckTarget( targets );
	}
}

void Turret::Reset( int team, XMFLOAT3 pos, XMFLOAT3 dir )
{
	mTeamID					= team;

	pos.y					+= 1.6f;
	mUpperBody->pos			= pos;
	mUpperBody->dir			= dir;

	pos.y					-= 1.0f;
	mMiddleBody->pos		= pos;
	mMiddleBody->dir		= dir;

	pos.y					-= 0.6f;
	mLowerBody->pos			= pos;
	mLowerBody->dir			= dir;

	SAFE_DELETE(mBoundingCircle);
	mBoundingCircle			= new BoundingCircle( mMiddleBody->pos, 10.0f ); // Range of the turret is set here
	mCurrentMode			= mIdle;
}

HRESULT Turret::Update( float deltaTime )
{
	mDT = deltaTime;
	mShootTimer -= mDT;

	mCurrentMode->Action( this );

	return S_OK;
}

void Turret::Render()
{
	float yaw = atan2f( mUpperBody->dir.z, mUpperBody->dir.x );
	float roll	= -XMVectorGetX( 
					XMVector3AngleBetweenVectors( 
					XMLoadFloat3( &XMFLOAT3(  mUpperBody->dir.x, 0.0f, mUpperBody->dir.z ) ),
					XMLoadFloat3( &mUpperBody->dir ) ) );

	RenderManager::GetInstance()->AddObject3dToList( mUpperBody->model, mUpperBody->pos, XMFLOAT3( 0.0f, -yaw, roll ) );
	RenderManager::GetInstance()->AddObject3dToList( mMiddleBody->model, mMiddleBody->pos, XMFLOAT3( 0.0f, -yaw, 0.0f ) );
	RenderManager::GetInstance()->AddObject3dToList( mLowerBody->model, mLowerBody->pos);
}

void Turret::Initialize( int team, XMFLOAT3 pos, XMFLOAT3 dir )
{
	mTeamID					= team;

	pos.y					+= 1.6f;
	mUpperBody				= new BodyPart();
	Graphics::GetInstance()->LoadStatic3dAsset( "../Content/Assets/PermanentAssets/Turret/", "turret.pfs", mUpperBody->model );
	mUpperBody->pos			= pos;
	mUpperBody->dir			= dir;

	pos.y					-= 1.0f;
	mMiddleBody				= new BodyPart();
	Graphics::GetInstance()->LoadStatic3dAsset( "../Content/Assets/PermanentAssets/Turret/", "base2.pfs", mMiddleBody->model );
	mMiddleBody->pos		= pos;
	mMiddleBody->dir		= dir;

	pos.y					-= 0.6f;
	mLowerBody				= new BodyPart();
	Graphics::GetInstance()->LoadStatic3dAsset( "../Content/Assets/PermanentAssets/Ship/", "tripod.pfs", mLowerBody->model );
	mLowerBody->pos			= pos;
	mLowerBody->dir			= dir;

	mLoadOut				= new LoadOut();
	mLoadOut->rangedWeapon	= new RangedInfo( MINIGUN );

	mBoundingCircle			= new BoundingCircle( mMiddleBody->pos, 10.0f ); // Range of the turret is set here
	mIdle					= new IdleTurret();
	mAttacking				= new AttackingTurret();
	mCurrentMode			= mIdle;
}

void Turret::Release()
{
	mLoadOut->Release();
	SAFE_DELETE( mLoadOut );
	SAFE_DELETE( mUpperBody );
	SAFE_DELETE( mMiddleBody );
	SAFE_DELETE( mLowerBody );
	SAFE_DELETE( mBoundingCircle );
	SAFE_DELETE( mIdle );
	SAFE_DELETE( mAttacking );
}

Turret::Turret()
{
	mTeamID			= -1;
	mUpperBody		= nullptr;
	mMiddleBody		= nullptr;
	mLowerBody		= nullptr;

	mRange			= 0.0f;
	mLoadOut		= nullptr;
	mShootTimer		= 0.0f;

	mBoundingCircle	= nullptr;
	mTarget			= nullptr; // Is never initialized
	mCurrentMode	= nullptr;
	mIdle			= nullptr;
	mAttacking		= nullptr;
	mDT				= 0.0f;
}

Turret::~Turret()
{
}
