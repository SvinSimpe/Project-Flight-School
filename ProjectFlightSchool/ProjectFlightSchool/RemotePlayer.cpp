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
	Graphics::GetInstance()->RenderStatic3dAsset( mUpperBody.playerModel, mUpperBody.position, mUpperBody.direction );
	Graphics::GetInstance()->RenderStatic3dAsset( mLowerBody.playerModel, mLowerBody.position.x, mLowerBody.position.y, mLowerBody.position.z );

	return S_OK;
}

HRESULT RemotePlayer::Initialize()
{
	if( FAILED( Graphics::GetInstance()->LoadStatic3dAsset( "../Content/Assets/Robot/", "robotScenebody.pfs", mUpperBody.playerModel ) ) )
		OutputDebugString( L"\nERROR\n" );

	if( FAILED( Graphics::GetInstance()->LoadStatic3dAsset( "../Content/Assets/Robot/", "robotScenelegs.pfs", mLowerBody.playerModel ) ) )
		OutputDebugString( L"\nERROR\n" );


	mUpperBody.position	= XMFLOAT3( 3.0f, 0.0f, 0.0f );
	mLowerBody.position	= XMFLOAT3( 3.0f, 0.0f, 0.0f );
	mLowerBody.speed	= 15.0f;

	return S_OK;
}

void RemotePlayer::Release()
{
}

RemotePlayer::RemotePlayer()
{
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

	mLowerBody.playerModel	= 0;
	mLowerBody.position		= XMFLOAT3(0.0f, 0.0f, 0.0f);
	mLowerBody.direction	= XMFLOAT3(0.0f, 0.0f, 0.0f);
	mLowerBody.speed		= 0.0f;

	mID						= 0;
}

RemotePlayer::~RemotePlayer()
{}