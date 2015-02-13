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

void ClientShip::Reset( UINT id, UINT teamID, XMFLOAT3 pos, XMFLOAT3 dir )
{
	ServerShip::Reset( id, teamID, pos, dir );
	mHitCircle->center = pos;
}

UINT ClientShip::GetID() const
{
	return mID;
}

UINT ClientShip::GetTeamID() const
{
	return mTeamID;
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

void ClientShip::Render()
{
	RenderManager::GetInstance()->AddObject3dToList( mAssetID, mPos );
}

void ClientShip::Initialize( UINT id, UINT teamID, XMFLOAT3 pos, XMFLOAT3 dir )
{
	ServerShip::Initialize( id, teamID, pos, dir );
	Graphics::GetInstance()->LoadStatic3dAsset( "../Content/Assets/PermanentAssets/Ship/", "ship.pfs", mAssetID );
	mHitCircle = new BoundingCircle( mPos, 5.0f );

	EventManager::GetInstance()->AddListener( &ClientShip::RemoteUpdateShip, this, Event_Server_Update_Ship::GUID );
}

void ClientShip::Release()
{
	ServerShip::Release();
	SAFE_DELETE( mHitCircle );
}

ClientShip::ClientShip() : ServerShip()
{
	mAssetID	= CUBE_PLACEHOLDER;
	mHitCircle	= nullptr;
	mWasUpdated = false;
}

ClientShip::~ClientShip()
{
}