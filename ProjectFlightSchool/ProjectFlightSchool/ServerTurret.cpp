#include "ServerTurret.h"

void ServerTurret::IdleTurret::Action( ServerTurret* t, float dt )
{
	XMStoreFloat4( &t->mTurretHead->rot, XMVector3TransformCoord( XMLoadFloat4( &t->mTurretHead->rot ), XMMatrixRotationY( dt ) ) );
	t->mRot = t->mTurretHead->rot;
}

void ServerTurret::AttackingTurret::Action( ServerTurret* t, float dt )
{
	XMVECTOR homingVec	= XMVector3Normalize( XMLoadFloat3( &t->mTarget->center ) -  XMLoadFloat3( &t->mScanCircle->center ) );
	XMVECTOR rightDir	= XMVector3Cross( XMLoadFloat3( &XMFLOAT3( 0.0f, 0.1f, 0.0f ) ) , XMLoadFloat4( &t->mTurretHead->rot ) );

	float rightDot		= XMVectorGetX( XMVector3Dot( rightDir, homingVec ) );
	float leftDot		= XMVectorGetX( XMVector3Dot( -rightDir, homingVec ) );
	float angleY		= dt * ( rightDot < leftDot ? -leftDot : rightDot ) * 50.0f; // The last number is the turret rotation speed

	XMStoreFloat4( &t->mTurretHead->rot, XMVector3TransformNormal( XMLoadFloat4( &t->mTurretHead->rot ), XMMatrixRotationY( angleY ) ) );

	t->mTurretHead->rot.y = XMVectorGetY( homingVec );
	XMStoreFloat4( &t->mTurretHead->rot, XMVector3Normalize( XMLoadFloat4( &t->mTurretHead->rot ) ) );
	t->mRot = t->mTurretHead->rot;

	float dot = XMVectorGetY( XMVector3Dot( XMLoadFloat4( &t->mTurretHead->rot ), homingVec ) );

	if( dot > 0.9f )
	{
		t->Fire();
	}
}

void ServerTurret::SwitchMode( UINT mode )
{
	if( mode >= 0 && mode < MODE_AMOUNT )
	{
		mCurrentMode = mode;
	}
}

void ServerTurret::Fire()
{
	XMFLOAT3 weaponOffsets =	mLoadOut->rangedWeapon->weaponType == MINIGUN			? MINIGUN_OFFSETS : 
								mLoadOut->rangedWeapon->weaponType == SHOTGUN			? SHOTGUN_OFFSETS :
								mLoadOut->rangedWeapon->weaponType == GRENADELAUNCHER	? GL_OFFSETS :
								mLoadOut->rangedWeapon->weaponType == SNIPER			? SNIPER_OFFSETS : XMFLOAT3( 0.0f, 0.0f, 1.0f );

	XMVECTOR pos = XMLoadFloat3( &mTurretHead->pos );
	XMVECTOR dir = XMLoadFloat3( &XMFLOAT3( mTurretHead->pos.x, mTurretHead->pos.y, mTurretHead->pos.z ) );
	XMVECTOR off = XMLoadFloat3( &XMFLOAT3( mTurretHead->pos.x, weaponOffsets.z, mTurretHead->pos.z ) );

	off += XMVector3Normalize( XMVector3Cross( XMLoadFloat3( &XMFLOAT3( 0.0f, 1.0f, 0.0f ) ), dir ) ) * weaponOffsets.y;
	off += dir * weaponOffsets.x;

	XMFLOAT3 loadDir;
	XMStoreFloat3( &loadDir, off );

	XMFLOAT3 rot3 = XMFLOAT3( mRot.x, mRot.y, mRot.z );

	if( mLoadOut->rangedWeapon->spread != 0.0f )
	{
		float dirOff = (float)( rand() % 100 ) * 0.001f - mLoadOut->rangedWeapon->spread;
		XMFLOAT3 fireDir = XMFLOAT3( rot3.x + dirOff, rot3.y, rot3.z + dirOff );
		IEventPtr E1( new Event_Turret_Fired_Projectile( 
			mID, 
			loadDir, 
			fireDir, 
			mLoadOut->rangedWeapon->GetRandomProjectileSpeed(), 
			mLoadOut->rangedWeapon->range ) );
		EventManager::GetInstance()->QueueEvent( E1 );
	}
	else
	{
		IEventPtr E1( new Event_Turret_Fired_Projectile( mID, 
			loadDir, 
			rot3, 
			mLoadOut->rangedWeapon->GetRandomProjectileSpeed(), 
			mLoadOut->rangedWeapon->range ) );
		EventManager::GetInstance()->QueueEvent( E1 );
	}
}

UINT ServerTurret::CheckMode() const
{
	return mCurrentMode;
}

void ServerTurret::FindTarget( BoundingCircle* enemy )
{
	if( !mTarget )
	{
		SwitchMode( IDLE_MODE );
		if( enemy->Intersect( mScanCircle ) )
		{
			mTarget = enemy;
			SwitchMode( ATTACK_MODE );
		}
	}
	else
	{
		SwitchMode( ATTACK_MODE );

		float dx = mPos.x - mTarget->center.x;
		float dy = mPos.y - mTarget->center.y;
		float dz = mPos.z - mTarget->center.z;

		float distTarget = sqrt( dx * dx + dy * dy + dz * dz );

		dx = mPos.x - enemy->center.x;
		dy = mPos.y - enemy->center.y;
		dz = mPos.z - enemy->center.z;

		float distEnemy = sqrt( dx * dx + dy * dy + dz * dz );

		if( distEnemy < distTarget && enemy->Intersect( mScanCircle ) )
		{
			mTarget = enemy;
		}
		else if( !mTarget->Intersect( mScanCircle ) )
		{
			mTarget = nullptr;
			SwitchMode( IDLE_MODE );
		}
	}
}

// Send an update event to the clients after every update here
void ServerTurret::Update( float deltaTime )
{
	mModes[mCurrentMode]->Action( this, deltaTime );
}

void ServerTurret::Reset( UINT id, UINT team, XMFLOAT3 pos, XMFLOAT4 rot, XMFLOAT3 scale )
{
	GameObject::Initialize( pos, rot, scale, mAssetID );

	mID		= id;
	mTeamID	= team;

	SwitchMode( IDLE_MODE );

	mPos.x -= 1.798f;
	mPos.y += 3.878f;
	mPos.z += 1.609f;

	mRot.x = 1.0f;
	mRot.y = 0.0f;
	mRot.z = 0.0f;

	mTurretHead->pos		= mPos;
	mTurretHead->pos.y		+= 1.2f;
	mTurretHead->rot		= mRot;
	mTurretHead->scale		= mScale;
}

void ServerTurret::Initialize( UINT id, UINT team, XMFLOAT3 pos, XMFLOAT4 rot, XMFLOAT3 scale, AssetID assetID )
{
	GameObject::Initialize( pos, rot, scale, assetID );

	mID		= id;
	mTeamID = team;

	SwitchMode( IDLE_MODE );
	mModes[IDLE_MODE]		= new IdleTurret();
	mModes[ATTACK_MODE]		= new AttackingTurret();

	mLoadOut				= new LoadOut();
	mLoadOut->rangedWeapon	= new RangedInfo( MINIGUN );

	mPos.x -= 1.798f;
	mPos.y += 3.878f;
	mPos.z += 1.609f;

	mRot.x = 1.0f;
	mRot.y = 0.0f;
	mRot.z = 0.0f;

	mTurretHead				= new TurretHead();
	mTurretHead->pos		= mPos;
	mTurretHead->pos.y		+= 1.2f;
	mTurretHead->rot		= mRot;
	mTurretHead->scale		= mScale;

	mScanCircle				= new BoundingCircle( mTurretHead->pos, 20.0f );
}

void ServerTurret::Initialize( UINT id, UINT team, GameObjectInfo gameObjectInfo, AssetID assetID )
{
	GameObject::Initialize( gameObjectInfo, assetID );

	mID		= id;
	mTeamID	= team;

	SwitchMode( IDLE_MODE );
	mModes[IDLE_MODE]		= new IdleTurret();
	mModes[ATTACK_MODE]		= new AttackingTurret();

	mLoadOut				= new LoadOut();
	mLoadOut->rangedWeapon	= new RangedInfo( MINIGUN );

	mPos.x -= 1.798f;
	mPos.y += 3.878f;
	mPos.z += 1.609f;

	mRot.x = 1.0f;
	mRot.y = 0.0f;
	mRot.z = 0.0f;

	mTurretHead				= new TurretHead();
	mTurretHead->pos		= mPos;
	mTurretHead->pos.y		+= 1.2f;
	mTurretHead->rot		= mRot;
	mTurretHead->scale		= mScale;

	mScanCircle				= new BoundingCircle( mTurretHead->pos, 20.0f );
}

void ServerTurret::Release()
{
	for( UINT i = 0; i < MODE_AMOUNT; i++ )
	{
		SAFE_DELETE( mModes[i] );
	}
	SAFE_DELETE( mScanCircle );
	SAFE_DELETE( mTurretHead );
	SAFE_RELEASE_DELETE( mLoadOut );
}

ServerTurret::ServerTurret() : GameObject()
{
	mCurrentMode	= (UINT)-1;
	for( UINT i = 0; i < MODE_AMOUNT; i++ )
	{
		mModes[i] = nullptr;
	}
	mTarget			= nullptr;
	mScanCircle		= nullptr;
	mLoadOut		= nullptr;
	mID				= (UINT)-1;
	mTeamID			= (UINT)-1;
}

ServerTurret::~ServerTurret()
{
}