#include "Ship.h"

void Ship::BuffPlayer( Player* player )
{
	if( mBuff->Intersect( player->GetBoundingCircle() ) && player->GetTeam() == mTeamID )
	{
		player->SetBuffed( true );
	}
	else
	{
		player->SetBuffed( false );
	}
}


XMFLOAT3 Ship::GetPosition() const
{
	return mModel->pos;
}

BoundingCircle* Ship::GetHitBox() const
{
	return mHitBox;
}

void Ship::TakeDamage( float damage )
{
	mCurrentHull -= damage;
}

void Ship::PickTurretTarget( std::vector<RemotePlayer*> targets )
{
	mTurret->PickTarget( targets );
}

void Ship::Update( float deltaTime )
{
	mTurret->Update( deltaTime );
}

void Ship::Render()
{
	RenderManager::GetInstance()->AddObject3dToList( mModel->model, mModel->pos );
	mTurret->Render();
}

void Ship::Initialize( int team, XMFLOAT3 pos, XMFLOAT3 dir )
{
	mModel = new BodyPart();
	Graphics::GetInstance()->LoadStatic3dAsset( "../Content/Assets/PermanentAssets/Ship/", "ship.pfs", mModel->model );
	mModel->pos = pos;
	mModel->dir = dir;

	mHitBox = new BoundingCircle( pos, 5.0f );
	mBuff	= new BoundingCircle( pos, 20.0f );
	mTeamID = team;
	mTurret = new Turret();

	// Preparing position/rotation for turret
	pos.x += -1.8f;
	pos.y += 2.3f;
	pos.z += 1.0f;
	dir.y += 24.761f;
	mTurret->Initialize( 5, pos, dir ); // Don't forget to change this back!

	mMaxHull = 100.0f;
	mCurrentHull = mMaxHull;
}

void Ship::Release()
{
	SAFE_DELETE( mModel );
	SAFE_DELETE( mHitBox );
	SAFE_DELETE( mBuff );
	mTurret->Release();
	SAFE_DELETE( mTurret );
}

Ship::Ship()
{
	mModel	= nullptr;
	mHitBox = nullptr;
	mTeamID	= 0;
	mTurret	= nullptr;
}

Ship::~Ship()
{
}
