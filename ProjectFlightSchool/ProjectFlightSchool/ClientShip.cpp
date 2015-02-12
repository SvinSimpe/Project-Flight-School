#include "ClientShip.h"

void ClientShip::CalcTurretLevel()
{
	for( UINT i = MIN_LEVEL; i == MAX_LEVEL; i++ )
	{

	}
}

void ClientShip::CalcShieldLevel()
{
	for( UINT i = MIN_LEVEL; i == MAX_LEVEL; i++ )
	{

	}
}

void ClientShip::CalcBuffLevel()
{
	for( UINT i = MIN_LEVEL; i == MAX_LEVEL; i++ )
	{

	}
}

void ClientShip::RemoteUpdateShip( IEventPtr eventPtr )
{
	if( eventPtr->GetEventType() == Event_Remote_Update_Ship::GUID )
	{
		std::shared_ptr<Event_Remote_Update_Ship> data = std::static_pointer_cast<Event_Remote_Update_Ship>( eventPtr );
		if( data->ID() == mID )
		{
			mCurrentShield = data->CurrentShield();
			mCurrentHP = data->CurrentHP();
			mWasUpdated = true;
		}
	}
}

void ClientShip::RemoteChangeShipLevels( IEventPtr eventPtr )
{
	if( eventPtr->GetEventType() == Event_Remote_Change_Ship_Levels::GUID )
	{
		std::shared_ptr<Event_Remote_Change_Ship_Levels> data = std::static_pointer_cast<Event_Remote_Change_Ship_Levels>( eventPtr );
		if( data->ID() == mID )
		{
			if( mTurretLevel != data->TurretLevel() )
			{
				mTurretLevel = data->TurretLevel();
				CalcTurretLevel();
			}
			if( mShieldLevel != data->ShieldLevel() )
			{
				mShieldLevel = data->ShieldLevel();
				CalcShieldLevel();
			}
			if( mBuffLevel != data->BuffLevel() )
			{
				mBuffLevel = data->BuffLevel();
				CalcBuffLevel();
			}
			mWasChanged = true;
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
	mWasUpdated = false;
	mWasChanged = false;
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

	EventManager::GetInstance()->AddListener( &ClientShip::RemoteUpdateShip, this, Event_Remote_Update_Ship::GUID );
	EventManager::GetInstance()->AddListener( &ClientShip::RemoteChangeShipLevels, this, Event_Remote_Change_Ship_Levels::GUID );
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
}

ClientShip::~ClientShip()
{
}