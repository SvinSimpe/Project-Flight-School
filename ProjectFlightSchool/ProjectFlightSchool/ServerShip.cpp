#include "ServerShip.h"

void ServerShip::ChangeTurretLevel( int change )
{
	if( change != 0 )
	{
		mTurretLevel += change;
		if( mTurretLevel < MIN_LEVEL )
		{
			mTurretLevel = MIN_LEVEL;
		}
		else if( mTurretLevel > MAX_LEVEL )
		{
			mTurretLevel = MAX_LEVEL;
		}
	}
}

void ServerShip::ChangeShieldLevel( int change )
{
	if( change != 0 )
	{
		mShieldLevel += change;
		if( mShieldLevel < MIN_LEVEL )
		{
			mShieldLevel = MIN_LEVEL;
		}
		else if( mShieldLevel > MAX_LEVEL )
		{
			mShieldLevel = MAX_LEVEL;
		}
	}
}

void ServerShip::ChangeBuffLevel( int change )
{
	if( change != 0 )
	{
		mTurretLevel += change;
		if( mBuffLevel < MIN_LEVEL )
		{
			mBuffLevel = MIN_LEVEL;
		}
		else if( mBuffLevel > MAX_LEVEL )
		{
			mBuffLevel = MAX_LEVEL;
		}
	}
}

void ServerShip::ClientUpdateShip( IEventPtr eventPtr )
{
	if( eventPtr->GetEventType() == Event_Client_Update_Ship::GUID )
	{
		std::shared_ptr<Event_Client_Update_Ship> data = std::static_pointer_cast<Event_Client_Update_Ship>( eventPtr );

		if( data->ID() == mID && data->Damage() != 0.0f )
		{
			if( !TakeDamage( data->Damage() ) )
			{
				// Handle ship dying here
			}
			mWasUpdated = true;
		}
	}
}

void ServerShip::ClientChangeShipLevels( IEventPtr eventPtr )
{
	if( eventPtr->GetEventType() == Event_Client_Change_Ship_Levels::GUID )
	{
		std::shared_ptr<Event_Client_Change_Ship_Levels> data = std::static_pointer_cast<Event_Client_Change_Ship_Levels>( eventPtr );
		if( data->ID() == mID )
		{
			ChangeTurretLevel( data->TurretLevelChange() );
			ChangeShieldLevel( data->ShieldLevelChange() );
			ChangeBuffLevel( data->BuffLevelChange() );
			mWasChanged = true;
		}
	}
}

bool ServerShip::TakeDamage( float damage )
{
	if( mCurrentShield > 0.0f )
	{
		mCurrentShield -= damage;
		return true;
	}
	else
	{
		if( mCurrentHP > 0.0f )
		{
			mCurrentHP -= damage;
			return true;
		}
	}
	return false;
}

bool ServerShip::Intersect( BoundingCircle* entity )
{
	if( mBuffCircle->Intersect( entity ) )
	{
		return true;
	}
	return false;
}

void ServerShip::Reset( UINT id, UINT teamID, XMFLOAT3 pos, XMFLOAT3 dir )
{
	mBuffCircle->center = pos;
	mBuffMod		= 0.5f;
	mID				= id;
	mTeamID			= teamID;
	mPos			= pos;
	mDir			= dir;
	mTurretLevel	= MIN_LEVEL;
	mShieldLevel	= MIN_LEVEL;
	mBuffLevel		= MIN_LEVEL;
	mMaxShield		= 100.0f;
	mCurrentShield	= mMaxShield;
	mMaxHP			= 100.0f;
	mCurrentHP		= mMaxHP;
}

void ServerShip::Update( float deltaTime )
{
	mWasUpdated = false;
	mWasChanged = false;
}

void ServerShip::Initialize( UINT id, UINT teamID, XMFLOAT3 pos, XMFLOAT3 dir )
{
	mBuffCircle		= new BoundingCircle( pos, 20.0f );
	mBuffMod		= 0.5f;
	mID				= id;
	mTeamID			= teamID;
	mPos			= pos;
	mDir			= dir;
	mTurretLevel	= MIN_LEVEL;
	mShieldLevel	= MIN_LEVEL;
	mBuffLevel		= MIN_LEVEL;
	mMaxShield		= 100.0f;
	mCurrentShield	= mMaxShield;
	mMaxHP			= 100.0f;
	mCurrentHP		= mMaxHP;

	EventManager::GetInstance()->AddListener( &ServerShip::ClientUpdateShip, this, Event_Client_Update_Ship::GUID );
	EventManager::GetInstance()->AddListener( &ServerShip::ClientChangeShipLevels, this, Event_Client_Change_Ship_Levels::GUID );
}

void ServerShip::Release()
{
	if( mBuffCircle )
	{
		delete mBuffCircle;
		mBuffCircle = nullptr;
	}
}

ServerShip::ServerShip()
{
	mBuffCircle	= nullptr;
	mBuffMod	= 0.0f;
	mID			= (UINT)-1;
	mTeamID		= (UINT)-1;
	mPos		= XMFLOAT3( 0.0f, 0.0f, 0.0f );
	mDir		= XMFLOAT3( 0.0f, 0.0f, 0.0f );
	mMaxHP		= 0.0f;
	mCurrentHP	= 0.0f;
}

ServerShip::~ServerShip()
{
}