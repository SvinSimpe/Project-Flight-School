#include "Ship.h"

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
	mTeamID = team;
	mModel = new BodyPart();
	Graphics::GetInstance()->LoadStatic3dAsset( "../Content/Assets/Ship/", "ship.pfs", mModel->model );
	mModel->pos = pos;
	mModel->dir = dir;

	mTurret = new Turret();

	// Preparing position/rotation for turret
	pos.x += -1.8f;
	pos.y += 2.282f;
	pos.z += 1.0f;
	dir.y += 24.761f;
	mTurret->Initialize( mTeamID, pos, dir );
}

void Ship::Release()
{
	SAFE_DELETE( mModel );
	mTurret->Release();
	SAFE_DELETE( mTurret );
}

Ship::Ship()
{
	mModel = nullptr;
	mTeamID = 0;
	mTurret = nullptr;
}

Ship::~Ship()
{
}
