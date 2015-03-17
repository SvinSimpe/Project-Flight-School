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
		
		//Graphics::GetInstance()->SetShipPosAndRad( mBuffCircle->center, mBuffCircle->radius, mID );
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
	mBuffMod = 1.0f + (mBuffLevel - 1);
}

void ServerShip::ClientUpdateShip( IEventPtr eventPtr )
{
	if( eventPtr->GetEventType() == Event_Client_Update_Ship::GUID )
	{
		std::shared_ptr<Event_Client_Update_Ship> data = std::static_pointer_cast<Event_Client_Update_Ship>( eventPtr );

		if( data->ID() == mID && data->Damage() != 0.0f && !mWasUpdated )
		{
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

bool ServerShip::PositionVsShip( BoundingCircle* entity, XMFLOAT3 &normal )
{
	for( int i = 0; i < 3; i++ )
	{
		if( mCollisionCircles[i]->Intersect( entity, normal ) )
			return true;
	}
	return false;
}

UINT ServerShip::GetID() const
{
	return mID;
}

BoundingCircle* ServerShip::GetHitCircle() const
{
	return mHitCircle;
}

BoundingCircle* ServerShip::GetBuffCircle() const
{
	return mBuffCircle;
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
	mMaxHP						= 50000.0f;
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
	XMFLOAT3 posOffset;
	XMStoreFloat3( &posOffset, XMLoadFloat3 ( &pos ) + XMLoadFloat3 ( &XMFLOAT3( -1.60f, 0.0f, 1.2f ) ) );
	
	mBuffCircle		= new BoundingCircle( posOffset, 20.0f );
	mHitCircle		= new BoundingCircle( posOffset, 10.0f );
	for( int i = 0; i < 3; i++ )
		mCollisionCircles[i] = new BoundingCircle( XMFLOAT3( mPos.x - 2.0f, 0.0f, mPos.z - 5.0f + 5.0f * float(i) ), 5.0f + 1.0f * (float)i );

	mBuffMod		= 1.0f;
	mID				= id;
	mTeamID			= teamID;
	mTurretLevel	= MIN_LEVEL;
	mShieldLevel	= MIN_LEVEL;
	mBuffLevel		= MIN_LEVEL;
	mMaxShield		= 6000.0f;
	mCurrentShield	= mMaxShield;
	mMaxHP			= 50000.0f;

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
	for( int i = 0; i < 3; i++ )
		if( mCollisionCircles[i] )
			delete mCollisionCircles[i], mCollisionCircles[i] = nullptr;

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
