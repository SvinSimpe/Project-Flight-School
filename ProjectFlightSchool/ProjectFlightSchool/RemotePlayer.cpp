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

void RemotePlayer::RemoteInit( unsigned int id )
{
	mID = id;
	EventManager::GetInstance()->AddListener( &RemotePlayer::RemoteUpdate, this, Event_Remote_Player_Update::GUID );
}

// Kill remote player
void RemotePlayer::Die()
{
	mCurrentHp = 0.0f;
	mIsAlive = false;
	mTimeTillSpawn	= mSpawnTime;
}

int RemotePlayer::GetID() const
{
	return mID;
}

HRESULT RemotePlayer::Render( float deltaTime )

{
	RenderManager::GetInstance()->AddObject3dToList(mUpperBody.playerModel, mUpperBody.position, mUpperBody.direction);
	RenderManager::GetInstance()->AddObject3dToList(mLowerBody.playerModel, mLowerBody.position);

	if ( mIsAlive )
	{
		mCurrentHp -= 0.07;
		float renderHpSize = ( mCurrentHp * 1.5f / mMaxHp ) + 1; //*1.5 and +1 to make it an appropriate size.

		DirectX::XMFLOAT3 x = { mLowerBody.position.x - renderHpSize / 2.0f, 0.01f, mLowerBody.position.z + renderHpSize / 2.0f };
		DirectX::XMFLOAT3 y = { mLowerBody.position.x + renderHpSize / 2.0f, 0.01f, mLowerBody.position.z - renderHpSize / 2.0f };

		if ( mCurrentHp > mMaxHp / 2 )
			RenderManager::GetInstance()->AddPlaneToList( mGreenHPAsset, x, y );
		else if ( mCurrentHp < mMaxHp / 4 )
			RenderManager::GetInstance()->AddPlaneToList( mRedHPAsset, x, y );
		else
			RenderManager::GetInstance()->AddPlaneToList( mOrangeHPAsset, x, y );
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
	mSpawnTime				= 10.0f;
	mTimeTillSpawn			= mSpawnTime;

	mBoundingBox			= nullptr;
	mBoundingCircle			= nullptr;
}

RemotePlayer::~RemotePlayer()
{}