#include "ServerShip.h"

void ServerShip::ChangeTurretLevel( int change )
{
	if( change != 0 )
	{
		if( change > 0 )
		{
			mTurretLevel++;
			if( mTurretLevel > MAX_LEVEL )
			{
				mTurretLevel = MAX_LEVEL;
			}
		}
		else
		{
			mTurretLevel--;
			if( mTurretLevel < MIN_LEVEL )
			{
				mTurretLevel = MIN_LEVEL;
			}
		}
		CalcTurretLevel();
	}
}

void ServerShip::ChangeShieldLevel( int change )
{
	if( change != 0 )
	{
		if( change > 0 )
		{
			mShieldLevel++;
			if( mShieldLevel > MAX_LEVEL )
			{
				mShieldLevel = MAX_LEVEL;
			}
		}
		else
		{
			mShieldLevel--;
			if( mShieldLevel < MIN_LEVEL )
			{
				mShieldLevel = MIN_LEVEL;
			}
		}
		CalcShieldLevel();
	}
}

void ServerShip::ChangeBuffLevel( int change )
{
	if( change != 0 )
	{
		if( change > 0 )
		{
			mBuffLevel++;
			if( mBuffLevel > MAX_LEVEL )
			{
				mBuffLevel = MAX_LEVEL;
			}
		}
		else
		{
			mBuffLevel--;
			if( mBuffLevel < MIN_LEVEL )
			{
				mBuffLevel = MIN_LEVEL;
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
	float mPercent	= PercentShield();
	mMaxShield		= 100.0f * mShieldLevel;
	mCurrentShield	= mMaxShield * mPercent;
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
}

void ServerShip::Initialize( UINT id, UINT teamID, XMFLOAT3 pos, XMFLOAT3 dir )
{
	mBuffCircle		= new BoundingCircle( pos, 20.0f );
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
	mBuffMod		= 0.5f;

	EventManager::GetInstance()->AddListener( &ServerShip::ClientUpdateShip, this, Event_Client_Update_Ship::GUID );
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
	mBuffCircle		= nullptr;
	mID				= (UINT)-1;
	mTeamID			= (UINT)-1;
	mPos			= XMFLOAT3( 0.0f, 0.0f, 0.0f );
	mDir			= XMFLOAT3( 0.0f, 0.0f, 0.0f );
	mTurretLevel	= 0;
	mShieldLevel	= 0;
	mBuffLevel		= 0;
	mMaxHP			= 0.0f;
	mCurrentHP		= 0.0f;
	mBuffMod		= 0.0f;
	mWasUpdated		= false;
}

ServerShip::~ServerShip()
{
}