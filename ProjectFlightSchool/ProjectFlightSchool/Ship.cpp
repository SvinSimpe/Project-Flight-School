#include "Ship.h"

void Ship::AddEnergyCell( UINT energyCellID )
{
	mEnergyCells[mNrOfEnergyCells++] = energyCellID;
}

void Ship::BuffPlayer( Player* player )
{
}

XMFLOAT3 Ship::GetPosition() const
{
	return mModel->pos;
}

BoundingCircle* Ship::GetHitBox() const
{
	return mHitBox;
}

float Ship::GetMaxHull() const
{
	return mMaxHull;
}

float Ship::GetCurrentHull() const
{
	return mCurrentHull;
}

void Ship::TakeDamage( float damage )
{
	mCurrentHull -= damage;
}

void Ship::PickTurretTarget( std::vector<RemotePlayer*> targets )
{
	mTurret->PickTarget( targets );
}

void Ship::Reset( int team, XMFLOAT3 pos, XMFLOAT3 dir )
{
	mModel = new BodyPart();
	Graphics::GetInstance()->LoadStatic3dAsset( "../Content/Assets/PermanentAssets/Ship/", "ship.pfs", mModel->model );
	mModel->pos = pos;
	mModel->dir = dir;

	SAFE_DELETE(mHitBox);
	SAFE_DELETE(mBuff);
	mHitBox = new BoundingCircle( pos, 5.0f );
	mBuff	= new BoundingCircle( pos, 20.0f );
	mTeamID = team;

	// Preparing position/rotation for turret
	pos.x += -1.8f;
	pos.y += 2.3f;
	pos.z += 1.0f;
	dir.y += 24.761f;
	mTurret->Reset( team, pos, dir );

	mMaxHull = 100.0f;
	mCurrentHull = mMaxHull;

	for( UINT i = 0; i < mNrOfEnergyCells; i++ )
	{
		mEnergyCells[i] = (UINT)-1;
	}
	
	mNrOfEnergyCells = 0;
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

	for( UINT i = 0; i < MAX_ENERGY_CELLS; i++ )
	{
		mEnergyCells[i] = (UINT)-1;
	}
	
	mNrOfEnergyCells = 0;
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
	mNrOfEnergyCells = 0;
}

Ship::~Ship()
{
}
