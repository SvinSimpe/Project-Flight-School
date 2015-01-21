#include "RemotePlayer.h"

void RemotePlayer::RemoteUpdate( IEventPtr newEvent )
{
	if ( newEvent->GetEventType() == Event_Remote_Player_Update::GUID )
	{
		std::shared_ptr<Event_Remote_Player_Update>data	= std::static_pointer_cast<Event_Remote_Player_Update>( newEvent );
		if ( mID == data->ID() )
		{
			mLowerBody.position						= data->LowerBodyPos();
			mLowerBody.direction					= data->LowerBodyDirection();
			mLowerBody.currentLowerAnimation		= data->LowerBodyAnimation();
			mLowerBody.currentLowerAnimationTime	= data->LowerBodyAnimationTime();

			mUpperBody.position		= data->UpperBodyPos();
			mUpperBody.direction	= data->UpperBodyDirection();

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

HRESULT RemotePlayer::Render( float deltaTime, int position )
{
	//Render upper body
	float radians = atan2f( mUpperBody.direction.z, mUpperBody.direction.x );
	RenderManager::GetInstance()->AddObject3dToList( mUpperBody.playerModel, mUpperBody.position, XMFLOAT3( 0.0f, -radians, 0.0f ) );

	//Render Arms
	XMFLOAT4X4 upperMatrix	= Graphics::GetInstance()->GetRootMatrix( mLowerBody.playerModel, mAnimations[mLowerBody.currentLowerAnimation], mLowerBody.currentLowerAnimationTime );
	XMFLOAT3 offsetPos		= XMFLOAT3( upperMatrix._41 + mLowerBody.position.x, upperMatrix._42, upperMatrix._43 + mLowerBody.position.z );

	RenderManager::GetInstance()->AddObject3dToList( mLeftArm, offsetPos, XMFLOAT3( 0.0f, -radians, 0.0f ) );
	RenderManager::GetInstance()->AddObject3dToList( mRightArm, offsetPos, XMFLOAT3( 0.0f, -radians, 0.0f ) );

	//Render lower body
	radians = atan2f( mLowerBody.direction.z, mLowerBody.direction.x );
	RenderManager::GetInstance()->AddAnim3dToList( mLowerBody.playerModel, mAnimations[mLowerBody.currentLowerAnimation], &mLowerBody.currentLowerAnimationTime, mLowerBody.position, XMFLOAT3( 0.0f, -radians, 0.0f ) );



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

	if( mColorIDAsset )
	{
		DirectX::XMFLOAT2 topLeft	= { 10.0f, 20.0f*(float)position };
		DirectX::XMFLOAT2 size		= { 10, 10 };
		RenderManager::GetInstance()->AddObject2dToList( mColorIDAsset, topLeft, size );

		int currentKills		= mNrOfKills;
		std::string textToWrite = "K ";
		while( currentKills > 100 )
		{
			textToWrite		+= "C";
			currentKills	-= 100;
		}
		while( currentKills > 50 )
		{
			textToWrite		+= "L";
			currentKills	-= 50;
		}
		while( currentKills > 10 )
		{
			textToWrite		+= "X";
			currentKills	-= 10;
		}
		while( currentKills > 5 )
		{
			textToWrite		+= "V";
			currentKills	-= 5;
		}
		while( currentKills > 0 )
		{
			textToWrite	+= "I";
			currentKills--;
		}

		int currentDeaths = mNrOfDeaths;
		textToWrite += " D ";
		while( currentDeaths > 100 )
		{
			textToWrite		+= "C";
			currentDeaths	-= 100;
		}
		while( currentDeaths > 50 )
		{
			textToWrite		+= "L";
			currentDeaths	-= 50;
		}
		while( currentDeaths > 10 )
		{
			textToWrite		+= "X";
			currentDeaths	-= 10;
		}
		while( currentDeaths > 5 )
		{
			textToWrite		+= "V";
			currentDeaths	-= 5;
		}
		while( currentDeaths > 0 )
		{
			textToWrite	+= "I";
			currentDeaths--;
		}
		mFont.WriteText( textToWrite, 25.0f, ((20.0f*(float)position)-7), 0.25f );
		
	}

	return S_OK;
}

HRESULT RemotePlayer::Initialize()
{
	//////////////////////////////////////
	//			ROBOT PARTS
	/////////////////////////////////////
	AssetID skeleton = 0;
	if( FAILED( Graphics::GetInstance()->LoadStatic3dAsset( "../Content/Assets/Robot/", "robotUpperbody.pfs", mUpperBody.playerModel ) ) )
		OutputDebugString( L"\nERROR loading player model\n" );

	if( Graphics::GetInstance()->LoadSkeletonAsset( "../Content/Assets/Robot/Animations/", "robotLegs.Skel", skeleton ) ) //Skeleton for legs
		OutputDebugString( L"\nERROR loading player model\n" );

	if( FAILED( Graphics::GetInstance()->LoadAnimated3dAsset( "../Content/Assets/Robot/", "robotLegs.apfs", skeleton, mLowerBody.playerModel ) ) )
		OutputDebugString( L"\nERROR loading player model\n" );

	if( FAILED( Graphics::GetInstance()->LoadStatic3dAsset( "../Content/Assets/Robot/Weapons/", "Claymore.pfs", mLeftArm ) ) )
		OutputDebugString( L"\nERROR loading player model\n" );

	if( FAILED( Graphics::GetInstance()->LoadStatic3dAsset( "../Content/Assets/Robot/Weapons/", "minigunStatic.pfs", mRightArm ) ) )
		OutputDebugString( L"\nERROR loading player model\n" );

	////////////////
	//Animations
	if( FAILED( Graphics::GetInstance()->LoadAnimationAsset( "../Content/Assets/Robot/Animations/", "robotLegsWalk.PaMan", mAnimations[PLAYER_ANIMATION_LEGS_WALK] ) ) )
		OutputDebugString( L"\nERROR loading player model\n" );

	if( FAILED( Graphics::GetInstance()->LoadAnimationAsset( "../Content/Assets/Robot/Animations/", "robotLegsIdle.PaMan", mAnimations[PLAYER_ANIMATION_LEGS_IDLE] ) ) )
		OutputDebugString( L"\nERROR loading player model\n" );

	//////////////////////////////////////
	//			HUD ELEMENTS
	/////////////////////////////////////

	if( FAILED( Graphics::GetInstance()->LoadStatic2dAsset( "../Content/Assets/Textures/GreenHP.png", mGreenHPAsset ) ) )
		OutputDebugString( L"\nERROR\n" );

	if( FAILED( Graphics::GetInstance()->LoadStatic2dAsset( "../Content/Assets/Textures/RedHP.png", mRedHPAsset ) ) )
		OutputDebugString( L"\nERROR\n" );

	if( FAILED( Graphics::GetInstance()->LoadStatic2dAsset( "../Content/Assets/Textures/OrangeHP.png", mOrangeHPAsset ) ) )
		OutputDebugString( L"\nERROR\n" );

	mUpperBody.position						= XMFLOAT3( 3.0f, 0.0f, 0.0f );
	mLowerBody.position						= XMFLOAT3( 3.0f, 0.0f, 0.0f );
	mNrOfDeaths								= 0;
	mNrOfKills								= 0;
	mLowerBody.currentLowerAnimation		= PLAYER_ANIMATION_LEGS_IDLE;
	mLowerBody.currentLowerAnimationTime	= 1.0f;

	mBoundingBox			= new BoundingBox( 1.5f, 1.5f );
	mBoundingCircle			= new BoundingCircle( 0.5f );
	
	mFont.Initialize( "../Content/Assets/Fonts/mv_boli_26_red/" );

	mIsAlive				= true;
	mMaxHp					= 100.0f;
	mCurrentHp				= mMaxHp;
	mSpawnTime				= 10.0f;
	mTimeTillSpawn			= mSpawnTime;

	return S_OK;
}

void RemotePlayer::Release()
{
	SAFE_DELETE( mBoundingBox );
	SAFE_DELETE( mBoundingCircle );
	mFont.Release();
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

XMFLOAT3 RemotePlayer::GetDirection() const
{
	return mLowerBody.direction;
}

void RemotePlayer::SetDirection( XMFLOAT3 direction )
{
	XMStoreFloat3( &mLowerBody.direction, ( XMLoadFloat3( &mLowerBody.direction ) += XMLoadFloat3( &direction ) ) );
	XMStoreFloat3( &mLowerBody.position, XMLoadFloat3( &mLowerBody.direction ) );
}

void RemotePlayer::AddImpuls( XMFLOAT3 impuls )
{
	mVelocity.x += impuls.x;
	mVelocity.z += impuls.z;
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

	mIsAlive				= true;
	mMaxHp					= 0.0f;
	mCurrentHp				= 0.0f;
	mSpawnTime				= 0.0f;
	mTimeTillSpawn			= 0.0f;

	mBoundingBox			= nullptr;
	mBoundingCircle			= nullptr;
}

RemotePlayer::~RemotePlayer()
{}

void RemotePlayer::TakeDamage( float damage )
{
	mCurrentHp -= damage;
}
