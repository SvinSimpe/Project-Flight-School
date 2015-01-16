#include "RemotePlayer.h"

void RemotePlayer::RemoteUpdate( IEventPtr newEvent )
{
	if ( newEvent->GetEventType() == Event_Remote_Player_Update::GUID )
	{
		std::shared_ptr<Event_Remote_Player_Update>data	= std::static_pointer_cast<Event_Remote_Player_Update>( newEvent );
		if ( mID == data->ID() )
		{
			mLowerBody.position		= data->LowerBodyPos();
			mUpperBody.position		= data->UpperBodyPos();
			mUpperBody.direction	= data->Direction();

			//TEST
			mBoundingBox->position	= mLowerBody.position;
			mBoundingCircle->center	= mLowerBody.position;
		}
	}
}

void RemotePlayer::LookAt( float rotation )
{
}

void RemotePlayer::RemoteInit( unsigned int id, int team, AssetID teamColor, AssetID colorID )
{
	mID				= id;
	mTeam			= team;
	mTeamAsset		= teamColor;
	mColorIDAsset	= colorID;
	EventManager::GetInstance()->AddListener( &RemotePlayer::RemoteUpdate, this, Event_Remote_Player_Update::GUID );
}

void RemotePlayer::BroadcastDeath( unsigned int shooter )
{
	IEventPtr dieEv( new Event_Player_Died( mID, shooter ) );
	EventManager::GetInstance()->QueueEvent( dieEv );
}

// Kill remote player
// Set current hp to 0 to avoid negative values and send event that player has died
void RemotePlayer::Die()
{
	mNrOfDeaths++;
	mIsAlive = false;
	mCurrentHp = 0.0f;
	mTimeTillSpawn	= mSpawnTime;
}

void RemotePlayer::HandleSpawn( float deltaTime )
{
	if( mTimeTillSpawn <= 0.0f )
	{
		Spawn();
	}
	else
	{
		mTimeTillSpawn -= deltaTime;
	}
}

void RemotePlayer::Spawn()
{
	mIsAlive			= true;
	mCurrentHp			= mMaxHp;
	mUpperBody.position = XMFLOAT3( 10.0f, 0.0f, 10.0f ); // Change to ship position + random offset
	mLowerBody.position = XMFLOAT3( 10.0f, 0.0f, 10.0f ); // Change to ship position + random offset
	IEventPtr spawnEv( new Event_Player_Spawned( mID ) );
	EventManager::GetInstance()->QueueEvent( spawnEv );
}

void RemotePlayer::TakeDamage( unsigned int damage, unsigned int shooter )
{
	mCurrentHp -= damage;
	IEventPtr player( new Event_Player_Update_HP( mID, mCurrentHp ) );
	EventManager::GetInstance()->QueueEvent( player );
	if ( mIsAlive && mCurrentHp <= 0.0f )
	{
		Die();
		BroadcastDeath( shooter );
	}
}

void RemotePlayer::SetHP( float hp )
{
	mCurrentHp = hp;
}

void RemotePlayer::CountUpKills()
{
	mNrOfKills++;
}

int RemotePlayer::GetID() const
{
	return mID;
}

int RemotePlayer::GetTeam() const
{
	return mTeam;
}

HRESULT RemotePlayer::Render( float deltaTime )

{
	//RenderManager::GetInstance()->AddObject3dToList(mUpperBody.playerModel, mUpperBody.position, mUpperBody.direction);
	RenderManager::GetInstance()->AddObject3dToList(mLowerBody.playerModel, mLowerBody.position);

	DirectX::XMFLOAT3 x;
	DirectX::XMFLOAT3 y;

	if ( mIsAlive )
	{
		RenderManager::GetInstance()->AddObject3dToList(mUpperBody.playerModel, mUpperBody.position, mUpperBody.direction);

		float renderHpSize = ( mCurrentHp * 1.5f / mMaxHp ) + 1; //*1.5 and +1 to make it an appropriate size.

		x = { mLowerBody.position.x - renderHpSize / 2.0f, 0.01f, mLowerBody.position.z + renderHpSize / 2.0f };
		y = { mLowerBody.position.x + renderHpSize / 2.0f, 0.01f, mLowerBody.position.z - renderHpSize / 2.0f };

		if ( mCurrentHp > mMaxHp / 2 )
			RenderManager::GetInstance()->AddPlaneToList( mGreenHPAsset, x, y );
		else if ( mCurrentHp < mMaxHp / 4 )
			RenderManager::GetInstance()->AddPlaneToList( mRedHPAsset, x, y );
		else
			RenderManager::GetInstance()->AddPlaneToList( mOrangeHPAsset, x, y );
	}

	float renderSize;
	if ( mColorIDAsset )
	{
		renderSize = 2.0f + 1;
		x = { mLowerBody.position.x - renderSize / 2.0f, 0.005f, mLowerBody.position.z + renderSize / 2.0f };
		y = { mLowerBody.position.x + renderSize / 2.0f, 0.005f, mLowerBody.position.z - renderSize / 2.0f };
		RenderManager::GetInstance()->AddPlaneToList(mColorIDAsset, x, y);
	}
	
	if ( mTeamAsset )
	{
		renderSize = 2.2f + 1;
		x = { mLowerBody.position.x - renderSize / 2.0f, 0.004f, mLowerBody.position.z + renderSize / 2.0f };
		y = { mLowerBody.position.x + renderSize / 2.0f, 0.004f, mLowerBody.position.z - renderSize / 2.0f };
		RenderManager::GetInstance()->AddPlaneToList( mTeamAsset, x, y );
	}

	return S_OK;
}

HRESULT RemotePlayer::Initialize()
{
	if( FAILED( Graphics::GetInstance()->LoadStatic3dAsset( "../Content/Assets/Robot/", "robotScenebody.pfs", mUpperBody.playerModel ) ) )
		OutputDebugString( L"\nERROR\n" );

	if( FAILED( Graphics::GetInstance()->LoadStatic3dAsset( "../Content/Assets/Robot/", "robotScenelegs.pfs", mLowerBody.playerModel ) ) )
		OutputDebugString( L"\nERROR\n" );

	if( FAILED( Graphics::GetInstance()->LoadStatic2dAsset( "../Content/Assets/Textures/GreenHP.png", mGreenHPAsset ) ) )
		OutputDebugString( L"\nERROR\n" );

	if( FAILED( Graphics::GetInstance()->LoadStatic2dAsset( "../Content/Assets/Textures/RedHP.png", mRedHPAsset ) ) )
		OutputDebugString( L"\nERROR\n" );

	if( FAILED( Graphics::GetInstance()->LoadStatic2dAsset( "../Content/Assets/Textures/OrangeHP.png", mOrangeHPAsset ) ) )
		OutputDebugString( L"\nERROR\n" );

	EventManager::GetInstance()->AddListener( &RemotePlayer::RemoteUpdate, this, Event_Remote_Player_Update::GUID );

	mUpperBody.position	= XMFLOAT3( 3.0f, 0.0f, 0.0f );
	mLowerBody.position	= XMFLOAT3( 3.0f, 0.0f, 0.0f );
	mLowerBody.speed	= 15.0f;
	mNrOfDeaths			= 0;
	mNrOfKills			= 0;

	mBoundingBox			= new BoundingBox( 1.5f, 1.5f );
	mBoundingCircle			= new BoundingCircle( 0.5f );

	return S_OK;
}

void RemotePlayer::Release()
{
	SAFE_DELETE( mBoundingBox );
}

BoundingBox* RemotePlayer::GetBoundingBox() const
{
	return mBoundingBox;
}

BoundingCircle*	RemotePlayer::GetBoundingCircle() const
{
	return mBoundingCircle;
}

XMFLOAT3 RemotePlayer::GetPosition() const
{
	return mLowerBody.position;
}

RemotePlayer::RemotePlayer()
{
	mID						= 0;
	mUpperBody.playerModel	= 0;
	mUpperBody.position		= XMFLOAT3( 0.0f, 0.0f, 0.0f );
	mUpperBody.direction	= XMFLOAT3( 0.0f, 0.0f, 0.0f );

	mLowerBody.playerModel	= 0;
	mLowerBody.position		= XMFLOAT3( 0.0f, 0.0f, 0.0f );
	mLowerBody.direction	= XMFLOAT3( 0.0f, 0.0f, 0.0f );
	mLowerBody.speed		= 0.0f;

	mIsAlive				= true;
	mMaxHp					= 100.0f;
	mCurrentHp				= mMaxHp;
	mSpawnTime				= 4.0f;
	mTimeTillSpawn			= mSpawnTime;

	mBoundingBox			= nullptr;
	mBoundingCircle			= nullptr;
}

RemotePlayer::~RemotePlayer()
{}