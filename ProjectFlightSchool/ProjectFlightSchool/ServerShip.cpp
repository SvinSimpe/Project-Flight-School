#include "ServerShip.h"

void ServerShip::ChangeTurretLevel( int change )
{
	if( change != 0 )
	{
		if( change > 0 && mNrOfAvailableEnergyCells > 0 )
		{
			if( mTurretLevel != MAX_LEVEL )
			{
				mTurretLevel++;
				mNrOfAvailableEnergyCells--;
			}
		}
		else
		{
			if( mTurretLevel != MIN_LEVEL )
			{
				mTurretLevel--;
				mNrOfAvailableEnergyCells++;
			}
		}
		CalcTurretLevel();
	}
}

void ServerShip::ChangeShieldLevel( int change )
{
	if( change != 0 )
	{
		if( change > 0 && mNrOfAvailableEnergyCells > 0 )
		{
			if( mShieldLevel != MAX_LEVEL )
			{
				mShieldLevel++;
				mNrOfAvailableEnergyCells--;
			}
		}
		else
		{
			if( mShieldLevel != MIN_LEVEL )
			{
				mShieldLevel--;
				mNrOfAvailableEnergyCells++;
			}
		}
		CalcShieldLevel();
	}
}

void ServerShip::ChangeBuffLevel( int change )
{
	if( change != 0 )
	{
		if( change > 0 && mNrOfAvailableEnergyCells > 0 )
		{
			if( mBuffLevel != MAX_LEVEL )
			{
				mBuffLevel++;
				mNrOfAvailableEnergyCells--;
			}
		}
		else
		{
			if( mBuffLevel != MIN_LEVEL )
			{
				mBuffLevel--;
				mNrOfAvailableEnergyCells++;
			}
		}
		CalcBuffMod();
	}
}

void ServerShip::CalcTurretLevel()
{
	// Stuff
}

void ServerShip::CalcShieldLevel()
{
	float percent	= PercentShield();
	mMaxShield		= 100.0f * mShieldLevel;
	mCurrentShield	= mMaxShield * percent;
}

void ServerShip::CalcBuffMod()
{
	mBuffMod = 0.5f + ( 0.2f * (mBuffLevel - 1) );
}

void ServerShip::ClientUpdateShip( IEventPtr eventPtr )
{
	if( eventPtr->GetEventType() == Event_Client_Update_Ship::GUID )
	{
		std::shared_ptr<Event_Client_Update_Ship> data = std::static_pointer_cast<Event_Client_Update_Ship>( eventPtr );

		if( data->ID() == mID && data->Damage() != 0.0f && !mWasUpdated )
		{
			if( !TakeDamage( data->Damage() ) )
			{
				// Handle ship dying here
			}
			mWasUpdated = true;
		}
	}
}

void ServerShip::AddEnergyCell( UINT energyCellOwnerID )
{
	if( energyCellOwnerID == mID )
	{
		mNrOfEnergyCells++;
		mNrOfAvailableEnergyCells++;
	}
}

float ServerShip::PercentShield() const
{
	return mCurrentShield/mMaxShield;
}

float ServerShip::PercentHP() const
{
	return mCurrentHP/mMaxHP;
}

void ServerShip::ClientChangeShipLevels( int changeTurretLevel, int changeShieldLevel, int changeBuffLevel )
{
	if ( !mWasUpdated )
	{
		ChangeTurretLevel( changeTurretLevel );
		ChangeShieldLevel( changeShieldLevel );
		ChangeBuffLevel( changeBuffLevel );
		mWasUpdated = true;
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

void ServerShip::Update( float deltaTime )
{
	mWasUpdated = false;
	mServerTurret->Update( deltaTime );
}

void ServerShip::FindTurretTarget( std::vector<BoundingCircle*> enemies )
{
	for( auto& enemy : enemies )
	{
		mServerTurret->FindTarget( enemy );
	}
}

void ServerShip::Reset( UINT id, UINT teamID, XMFLOAT3 pos, XMFLOAT4 rot, XMFLOAT3 scale, AssetID assetID )
{
	GameObject::Initialize( pos, rot, scale, assetID );

	mBuffMod		= 0.5f;
	mBuffCircle->center = pos;
	mID				= id;
	mTeamID			= teamID;

	mTurretLevel				= MIN_LEVEL;
	mShieldLevel				= MIN_LEVEL;
	mBuffLevel					= MIN_LEVEL;
	mMaxShield					= 100.0f;
	mCurrentShield				= mMaxShield;
	mMaxHP						= 100.0f;
	mCurrentHP					= mMaxHP;
	mNrOfEnergyCells			= 0;
	mNrOfAvailableEnergyCells	= 0;

	mServerTurret->Reset( id + 10, teamID, pos, rot, scale );
	for( UINT i = 0; i < MAX_LEVEL; i++ )
	{
		ClientChangeShipLevels( -1, -1, -1 );
	}
}

void ServerShip::Initialize( UINT id, UINT teamID, XMFLOAT3 pos, XMFLOAT4 rot, XMFLOAT3 scale, AssetID assetID )
{
	GameObject::Initialize( pos, rot, scale, assetID );
	mServerTurret	= new ServerTurret();
	mBuffCircle		= new BoundingCircle( mPos, 20.0f );

	mID				= id;
	mTeamID			= teamID;
	mTurretLevel	= MIN_LEVEL;
	mShieldLevel	= MIN_LEVEL;
	mBuffLevel		= MIN_LEVEL;
	mMaxShield		= 100.0f;
	mCurrentShield	= mMaxShield;
	mMaxHP			= 100.0f;
	mCurrentHP		= mMaxHP;

	for( UINT i = 0; i < MAX_LEVEL; i++ )
	{
		ClientChangeShipLevels( -1, -1, -1 );
	}

	mServerTurret->Initialize( id + 10, teamID, pos, rot, scale, assetID );

	EventManager::GetInstance()->AddListener( &ServerShip::ClientUpdateShip, this, Event_Client_Update_Ship::GUID );
}

void ServerShip::Initialize( UINT id, UINT teamID, GameObjectInfo gameObjectInfo, AssetID assetID )
{
	GameObject::Initialize( gameObjectInfo, assetID );
	mServerTurret	= new ServerTurret();
	mBuffCircle		= new BoundingCircle( 20.0f );

	mID				= id;
	mTeamID			= teamID;
	mTurretLevel	= MIN_LEVEL;
	mShieldLevel	= MIN_LEVEL;
	mBuffLevel		= MIN_LEVEL;
	mMaxShield		= 100.0f;
	mCurrentShield	= mMaxShield;
	mMaxHP			= 100.0f;
	mCurrentHP		= mMaxHP;

	for( UINT i = 0; i < MAX_LEVEL; i++ )
	{
		ClientChangeShipLevels( -1, -1, -1 );
	}

	mServerTurret->Initialize( id, teamID, gameObjectInfo );

	EventManager::GetInstance()->AddListener( &ServerShip::ClientUpdateShip, this, Event_Client_Update_Ship::GUID );
}

void ServerShip::Release()
{
	if( mServerTurret )
	{
		mServerTurret->Release();
		delete mServerTurret;
		mServerTurret = nullptr;
	}
	if( mBuffCircle )
	{
		delete mBuffCircle;
		mBuffCircle = nullptr;
	}
}

ServerShip::ServerShip() : GameObject()
{
	mBuffMod					= 0.0f;
	mServerTurret				= nullptr;
	mBuffCircle					= nullptr;
	mID							= (UINT)-1;
	mTeamID						= (UINT)-1;
	mTurretLevel				= 0;
	mShieldLevel				= 0;
	mBuffLevel					= 0;
	mMaxHP						= 0.0f;
	mCurrentHP					= 0.0f;
	mWasUpdated					= false;
	mNrOfEnergyCells			= 0;
	mNrOfAvailableEnergyCells	= 0;
}

ServerShip::~ServerShip()
{
}