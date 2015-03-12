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

void ServerShip::ChangeEngineLevel( int change )
{
	if( change != 0 )
	{
		if( change > 0 && mEngineLevel != MAX_LEVEL * 2 && mNrOfAvailableEnergyCells > 0 )
		{
			mEngineLevel++;
			mNrOfAvailableEnergyCells--;
		}
		else
		{
			mEngineLevel--;
			mNrOfAvailableEnergyCells++;
		}
	}
}

void ServerShip::CalcTurretLevel()
{
	mServerTurret->ChangeLevel( mTurretLevel );
}

void ServerShip::CalcShieldLevel()
{
	float percent	= PercentShield();
	mMaxShield		= 100.0f * mShieldLevel;
	mCurrentShield	= mMaxShield * percent;
}

void ServerShip::CalcBuffMod()
{
	//mBuffMod = 0.5f + ( 0.2f * (mBuffLevel - 1) );
	//mBuffMod = 2.0f + ( ( mBuffLevel - 1 ) * 2 );
	mBuffMod += 1.0f;
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

void ServerShip::AddEnergyCell()
{
	mNrOfEnergyCells++;
	mNrOfAvailableEnergyCells++;
}

float ServerShip::PercentShield() const
{
	return mCurrentShield/mMaxShield;
}

float ServerShip::PercentHP() const
{
	return mCurrentHP/mMaxHP;
}

void ServerShip::ClientChangeShipLevels( int changeTurretLevel, int changeShieldLevel, int changeBuffLevel, int changeEngineLevel )
{
	if ( !mWasUpdated )
	{
		ChangeTurretLevel( changeTurretLevel );
		ChangeShieldLevel( changeShieldLevel );
		ChangeBuffLevel( changeBuffLevel );
		ChangeEngineLevel( changeEngineLevel );
		mWasUpdated = true;
	}
}

UINT ServerShip::GetID() const
{
	return mID;
}

BoundingCircle* ServerShip::GetHitCircle() const
{
	return mHitCircle;
}

bool ServerShip::IsAlive() const
{
	return mIsAlive;
}

UINT ServerShip::GetTeamID() const
{
	return mTeamID;
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
			if( mCurrentHP <= 0.0f )
				mIsAlive = false;

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

	mBuffMod		= 1.0f;
	mBuffCircle->center = pos;
	mHitCircle->center	= pos;
	mID				= id;
	mTeamID			= teamID;

	mTurretLevel				= MIN_LEVEL;
	mShieldLevel				= MIN_LEVEL;
	mBuffLevel					= MIN_LEVEL;
	mEngineLevel				= 0;
	mMaxShield					= 100.0f;
	mCurrentShield				= mMaxShield;
	mMaxHP						= 20000.0f;
	//mMaxHP						*= 5.0f; // remove this later
	mCurrentHP					= mMaxHP;
	mNrOfEnergyCells			= 0;
	mNrOfAvailableEnergyCells	= 0;
	mWasUpdated					= false;
	mIsAlive					= true;

	mServerTurret->Reset( id + 10, teamID, pos, rot, scale );
	float percent	= PercentShield();
	mMaxShield		= 100.0f * mShieldLevel;
	mCurrentShield	= mMaxShield * percent;
}

void ServerShip::Initialize( UINT id, UINT teamID, XMFLOAT3 pos, XMFLOAT4 rot, XMFLOAT3 scale, AssetID assetID )
{
	GameObject::Initialize( pos, rot, scale, assetID );
	mBuffCircle		= new BoundingCircle( mPos, 20.0f );
	mHitCircle		= new BoundingCircle( mPos, 10.0f );

	mBuffMod		= 1.0f;
	mID				= id;
	mTeamID			= teamID;
	mTurretLevel	= MIN_LEVEL;
	mShieldLevel	= MIN_LEVEL;
	mBuffLevel		= MIN_LEVEL;
	mMaxShield		= 5000.0f;
	mCurrentShield	= mMaxShield;
	mMaxHP			= 20000.0f;
	mMaxHP *= 10.0f;
	mCurrentHP		= mMaxHP;
	mIsAlive		= true;

	float percent	= PercentShield();
	mMaxShield		= 100.0f * mShieldLevel;
	mCurrentShield	= mMaxShield * percent;

	mServerTurret	= new ServerTurret();
	mServerTurret->Initialize( id + 10, teamID, pos, rot, scale, assetID );	
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
	if( mHitCircle )
	{
		delete mHitCircle;
		mHitCircle = nullptr;
	}
}

ServerShip::ServerShip() : GameObject()
{
	mBuffMod					= 0.0f;
	mServerTurret				= nullptr;
	mBuffCircle					= nullptr;
	mHitCircle					= nullptr;
	mID							= (UINT)-1;
	mTeamID						= (UINT)-1;
	mTurretLevel				= 0;
	mShieldLevel				= 0;
	mBuffLevel					= 0;
	mEngineLevel				= 0;
	mMaxHP						= 0.0f;
	mCurrentHP					= 0.0f;
	mWasUpdated					= false;
	mNrOfEnergyCells			= 0;
	mNrOfAvailableEnergyCells	= 0;
	mIsAlive					= true;
	EnemiesTargetMe				= 0;
	TanksTargetMe				= 0;
}

ServerShip::~ServerShip()
{
}
