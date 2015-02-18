#include "ClientShip.h"

void ClientShip::RemoteUpdateShip( IEventPtr eventPtr )
{
	if( eventPtr->GetEventType() == Event_Server_Update_Ship::GUID )
	{
		std::shared_ptr<Event_Server_Update_Ship> data = std::static_pointer_cast<Event_Server_Update_Ship>( eventPtr );
		if( data->ID() == mID )
		{
			mMaxShield = data->MaxShield();
			mCurrentShield = data->CurrentShield();
			mCurrentHP = data->CurrentHP();
			mWasUpdated = true;
		}
	}
}

UINT ClientShip::GetID() const
{
	return mID;
}

UINT ClientShip::GetTeamID() const
{
	return mTeamID;
}

void ClientShip::CalculatePlayerRespawnPosition( IEventPtr eventPtr )
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
				float xMin = mBuffCircle->center.x - 10.0f;
				float xMax = mBuffCircle->center.x + 10.0f;

				float zMin = mBuffCircle->center.z - 10.0f;
				float zMax = mBuffCircle->center.z + 10.0f;

				int randX = (int)( xMax*2 - xMin );
				int randZ = (int)( zMax*2 - zMin );

				spawnX = (float)( rand() % randX );
				spawnZ = (float)( rand() % randZ );
			}
			while( spawnX > mBuffCircle->center.x - 5.0f && spawnX < mBuffCircle->center.x + 5.0f &&
				   spawnZ > mBuffCircle->center.z - 5.0f && spawnZ < mBuffCircle->center.z + 5.0f );


			IEventPtr E1( new Event_New_Player_Spawn_Position( data->PlayerID(), XMFLOAT2( spawnX, spawnZ ) ) );
			EventManager::GetInstance()->QueueEvent( E1 );
		}
	}
}

void ClientShip::Reset( UINT id, UINT teamID, XMFLOAT3 pos, XMFLOAT4 rot, XMFLOAT3 scale )
{
	ServerShip::Reset( id, teamID, pos, rot, scale );
	mHitCircle->center = pos;
	mClientTurret->Reset( id, teamID, pos, rot, scale );
}

bool ClientShip::Intersect( BoundingCircle* entity )
{
	if( mHitCircle->Intersect( entity ) )
	{
		return true;
	}
	return false;
}

void ClientShip::Update( float deltaTime )
{
	ServerShip::Update( deltaTime );
}

void ClientShip::Render( float deltaTime, DirectX::XMFLOAT4X4 parentWorld )
{
	GameObject::Render( deltaTime, parentWorld );
	mClientTurret->Render( deltaTime, parentWorld);
}

void ClientShip::Initialize( UINT id, UINT teamID, XMFLOAT3 pos, XMFLOAT4 rot, XMFLOAT3 scale )
{
	ServerShip::Initialize( id, teamID, pos, rot, scale );

	Graphics::GetInstance()->LoadStatic3dAsset( "../Content/Assets/PermanentAssets/Ship/", "ShipWithTripod.pfs", mAssetID );
	mHitCircle = new BoundingCircle( mPos, 5.0f );

	mClientTurret = new ClientTurret();
	mClientTurret->Initialize( id + 10, teamID, pos, rot, scale );

	EventManager::GetInstance()->AddListener( &ClientShip::RemoteUpdateShip, this, Event_Server_Update_Ship::GUID );
	EventManager::GetInstance()->AddListener( &ClientShip::CalculatePlayerRespawnPosition, this, Event_Request_Player_Spawn_Position::GUID );
}

void ClientShip::Release()
{
	ServerShip::Release();
	SAFE_DELETE( mHitCircle );
	SAFE_RELEASE_DELETE( mClientTurret );
}

ClientShip::ClientShip() : ServerShip()
{
	mAssetID		= CUBE_PLACEHOLDER;
	mHitCircle		= nullptr;
	mWasUpdated		= false;
	mClientTurret	= nullptr;
}

ClientShip::~ClientShip()
{
}