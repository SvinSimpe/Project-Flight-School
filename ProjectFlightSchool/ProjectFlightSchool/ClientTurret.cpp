#include "ClientTurret.h"

void ClientTurret::IdleTurret::Action( ClientTurret* t )
{
	XMStoreFloat4( &t->mTurretHead->rot, XMVector3TransformCoord( XMLoadFloat4( &t->mTurretHead->rot ), XMMatrixRotationY( t->mDT ) ) );
	t->mRot = t->mTurretHead->rot;
}

void ClientTurret::AttackingTurret::Action( ClientTurret* t )
{
	XMVECTOR homingVec = XMVector3Normalize( XMLoadFloat3( &t->mTargetCircle->center ) -  XMLoadFloat3( &t->mScanCircle->center ) );

	XMVECTOR rightDir = XMVector3Cross( XMLoadFloat3( &XMFLOAT3( 0.0f, 0.1f, 0.0f ) ) , XMLoadFloat4( &t->mTurretHead->rot ) );

	// Put the firing logic in here instead of in it's own function
}

void ClientTurret::SwitchMode( int mode )
{
	if( mode >= 0 && mode < MODE_AMOUNT )
	{
		mCurrentMode = mode;
	}
}

void ClientTurret::FindTarget( BoundingCircle* targetCircle )
{
}

void ClientTurret::Render( float deltaTime, DirectX::XMFLOAT4X4 parentWorld )
{
	float yaw = atan2f( mTurretHead->rot.z, mTurretHead->rot.x );
	float roll	= -XMVectorGetX( 
					XMVector3AngleBetweenVectors( 
					XMLoadFloat3( &XMFLOAT3(  mTurretHead->rot.x, 0.0f, mTurretHead->rot.z ) ),
					XMLoadFloat4( &mTurretHead->rot ) ) );

	RenderManager::GetInstance()->AddObject3dToList( mAssetID, mPos, XMFLOAT3( 0.0f, -yaw, 0.0f ) );
	RenderManager::GetInstance()->AddObject3dToList( mTurretHead->model, mTurretHead->pos, XMFLOAT3( 0.0f, -yaw, roll ) );
}

void ClientTurret::Update( float deltaTime )
{
	mDT = deltaTime;
	mModes[mCurrentMode]->Action( this );
}

void ClientTurret::Reset( UINT id, UINT team, XMFLOAT3 pos, XMFLOAT4 rot, XMFLOAT3 scale )
{
	// IMPLEMENT RESET LATER
}

void ClientTurret::Initialize( UINT id, UINT team, XMFLOAT3 pos, XMFLOAT4 rot, XMFLOAT3 scale )
{
	ServerTurret::Initialize( id, team, pos, rot, scale );

	SwitchMode( IDLE_MODE );
	mModes[IDLE_MODE]		= new IdleTurret();
	mModes[ATTACKING_MODE]	= new AttackingTurret();
	mScanCircle				= new BoundingCircle( mPos, 10.0f );

	mLoadOut				= new LoadOut();
	mLoadOut->rangedWeapon	= new RangedInfo( MINIGUN );

	mPos.x -= 1.798f;
	mPos.y += 3.878f;
	mPos.z += 1.609f;

	mRot.x = 1.0f;
	mRot.y = 0.0f;
	mRot.z = 0.0f;

	Graphics::GetInstance()->LoadStatic3dAsset( "../Content/Assets/PermanentAssets/Turret/", "turretBase2.pfs", mAssetID );

	mTurretHead				= new TurretPart();
	Graphics::GetInstance()->LoadStatic3dAsset( "../Content/Assets/PermanentAssets/Turret/", "turret.pfs", mTurretHead->model );
	mTurretHead->pos		= mPos;
	mTurretHead->pos.y		+= 1.2f;
	mTurretHead->rot		= mRot;
	mTurretHead->scale		= mScale;
}

void ClientTurret::Release()
{
	ServerTurret::Release();
	for( int i = 0; i < MODE_AMOUNT; i++ )
	{
		SAFE_DELETE( mModes[i] );
	}
	SAFE_DELETE( mScanCircle );
	SAFE_RELEASE( mLoadOut );
	SAFE_DELETE( mLoadOut );
	SAFE_DELETE( mTurretHead );
}

ClientTurret::ClientTurret() : ServerTurret()
{
	mCurrentMode	= -1;
	for( int i = 0; i < MODE_AMOUNT; i++ )
	{
		mModes[i]	= nullptr;
	}
	mTargetCircle	= nullptr; // Never allocates it's own memory, only points to others
	mScanCircle		= nullptr;
	mLoadOut		= nullptr;
	mTurretHead		= nullptr;
	mDT				= -1.0f;
}

ClientTurret::~ClientTurret()
{
}