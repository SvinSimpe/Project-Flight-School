#include "Ship.h"

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

void Ship::CalculatePlayerRespawnPosition( IEventPtr eventPtr )
{
	if ( eventPtr->GetEventType() == Event_Request_Player_Spawn_Position::GUID )
	{
		std::shared_ptr<Event_Request_Player_Spawn_Position> data = std::static_pointer_cast<Event_Request_Player_Spawn_Position>( eventPtr );
		if ( data->TeamID() == mTeamID )
		{
			float spawnX = 0.0f;
			float spawnZ = 0.0f;
			do
			{
				float xMin = mBuff->center.x - 10.0f;
				float xMax = mBuff->center.x + 10.0f;

				float zMin = mBuff->center.z - 10.0f;
				float zMax = mBuff->center.z + 10.0f;

				spawnX = (float)( rand() % (int)xMax*2 - xMin );
				spawnZ = (float)( rand() % (int)zMax*2 - zMin );
			}
			while( spawnX > mBuff->center.x - 5.0f && spawnX < mBuff->center.x + 5.0f &&
				   spawnZ > mBuff->center.z - 5.0f && spawnZ < mBuff->center.z + 5.0f );


			IEventPtr E1( new Event_New_Player_Spawn_Position( data->PlayerID(), XMFLOAT2( spawnX, spawnZ ) ) );
			EventManager::GetInstance()->QueueEvent( E1 );
		}
	}
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

	EventManager::GetInstance()->AddListener( &Ship::CalculatePlayerRespawnPosition, this, Event_Request_Player_Spawn_Position::GUID );
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
