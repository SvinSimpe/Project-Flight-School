#include "ClientShip.h"
#include "Pathfinder.h"

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

UINT ClientShip::GetNrOfEnergyCells() const
{
	return mNrOfEnergyCells;
}

bool ClientShip::CheckLose()
{
	return mCurrentHP <= 0.0f;
}

void ClientShip::CalculatePlayerRespawnPosition( IEventPtr eventPtr )
{
	if ( eventPtr->GetEventType() == Event_Request_Player_Spawn_Position::GUID )
	{
		std::shared_ptr<Event_Request_Player_Spawn_Position> data = std::static_pointer_cast<Event_Request_Player_Spawn_Position>( eventPtr );

		if ( data->TeamID() == mTeamID )
		{
			int width	= 15;
			int height	= 15;
			int xMin	= 0;
			int xMax	= 0;
			int zMin	= 0;
			int zMax	= 0;

			int spawnX, spawnZ = 0;


			xMin = (int)mBuffCircle->center.x - width;
			xMax = (int)mBuffCircle->center.x + width;
			zMin = (int)mBuffCircle->center.z - height;
			zMax = (int)mBuffCircle->center.z + height;

			//Check if min OR max is origo
			if( xMin || xMax == 0 || zMin == 0 || zMax == 0 )
			{
				width++;
				height++;

				xMin = (int)mBuffCircle->center.x - width;
				xMax = (int)mBuffCircle->center.x + width;
				zMin = (int)mBuffCircle->center.z - height;
				zMax = (int)mBuffCircle->center.z + height;
			}
			BoundingCircle pos;
			do
			{
				do
				{
				spawnX = ( xMin + ( rand() % (int)( xMax - xMin + 1 ) ) );
				spawnZ = ( zMin + ( rand() % (int)( zMax - zMin + 1 ) ) );
				pos = BoundingCircle( XMFLOAT3( (float)spawnX, 0.0f, (float)spawnZ ), 0.5f );
				}
				while( PositionVsShip( &pos, XMFLOAT3( 0.0f, 1.0f, 0.0f ) ) );
			}
			while( ( (float)spawnX > mBuffCircle->center.x - 15.0f && (float)spawnX < mBuffCircle->center.x + 15.0f &&
				   (float)spawnZ > mBuffCircle->center.z - 15.0f && (float)spawnZ < mBuffCircle->center.z + 15.0f ) && Pathfinder::GetInstance()->IsOnNavMesh( XMFLOAT3( (float)spawnX, 0.0f, (float)spawnZ ) ) );

			IEventPtr E1( new Event_New_Player_Spawn_Position( data->PlayerID(), XMFLOAT2( (float)spawnX, (float)spawnZ ) ) );
			EventManager::GetInstance()->QueueEvent( E1 );
		}
	}
}

void ClientShip::Reset( UINT id, UINT teamID, XMFLOAT3 pos, XMFLOAT4 rot, XMFLOAT3 scale )
{
	ServerShip::Reset( id, teamID, pos, rot, scale );
	mHitCircle->center = pos;
	mClientTurret->Reset( id, teamID, pos, rot, scale );

	mNrOfEnergyCells = 0;
}

bool ClientShip::Intersect( BoundingCircle* entity )
{
	if( mHitCircle != nullptr )
	{
		if( mHitCircle->Intersect( entity ) )
		{
			return true;
		}
		else 
		{
			return false;
		}
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
	mPos = pos;

	Graphics::GetInstance()->LoadStatic3dAsset( "../Content/Assets/PermanentAssets/Ship/", "ShipWithTripod.pfs", mAssetID );
	
	mNrOfEnergyCells = 0;

	mClientTurret = new ClientTurret();
	mClientTurret->Initialize( id + 10, teamID, pos, rot, scale );

	EventManager::GetInstance()->AddListener( &ClientShip::RemoteUpdateShip, this, Event_Server_Update_Ship::GUID );
	EventManager::GetInstance()->AddListener( &ClientShip::CalculatePlayerRespawnPosition, this, Event_Request_Player_Spawn_Position::GUID );
}

void ClientShip::Release()
{
	ServerShip::Release();
	SAFE_RELEASE_DELETE( mClientTurret );
}

ClientShip::ClientShip() : ServerShip()
{

	mAssetID			= CUBE_PLACEHOLDER;
	mWasUpdated			= false;
	mClientTurret		= nullptr;
	mNrOfEnergyCells	= 0;	

}

ClientShip::~ClientShip()
{
}
