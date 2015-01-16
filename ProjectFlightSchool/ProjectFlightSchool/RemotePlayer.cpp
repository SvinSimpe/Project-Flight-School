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

void RemotePlayer::RemoteInit( unsigned int id )
{
	mID										= id;
	EventManager::GetInstance()->AddListener( &RemotePlayer::RemoteUpdate, this, Event_Remote_Player_Update::GUID );
}

// Kill remote player
void RemotePlayer::Die()
{
	mCurrentHp		= 0.0f;
	mIsAlive		= false;
	mTimeTillSpawn	= mSpawnTime;
}

int RemotePlayer::GetID() const
{
	return mID;
}

HRESULT RemotePlayer::Render()
{
	RenderManager::GetInstance()->AddObject3dToList( mUpperBody.playerModel, mUpperBody.position, mUpperBody.direction );

	float radians				= atan2f( mLowerBody.direction.z, mLowerBody.direction.x );
	DirectX::XMFLOAT3 rotation	= DirectX::XMFLOAT3( 0.0f, -radians, 0.0f );
	Graphics::GetInstance()->RenderAnimated3dAsset( mLowerBody.playerModel, mAnimations[mLowerBody.currentLowerAnimation], mLowerBody.currentLowerAnimationTime, mLowerBody.position, rotation ); // NOT OKAY, NEED RENDERMAN

	return S_OK;
}

HRESULT RemotePlayer::Initialize()
{
	AssetID skeleton = 0;

	if( FAILED( Graphics::GetInstance()->LoadStatic3dAsset( "../Content/Assets/Robot/", "robotUpperbody.pfs", mUpperBody.playerModel ) ) )
		OutputDebugString( L"\nERROR\n" );

	Graphics::GetInstance()->LoadSkeletonAsset( "../Content/Assets/Robot/Animations/", "robotLegs.Skel", skeleton ); //Skeleton for legs

	if( FAILED( Graphics::GetInstance()->LoadAnimated3dAsset( "../Content/Assets/Robot/", "robotLegs.apfs", skeleton, mLowerBody.playerModel ) ) )
		OutputDebugString( L"\nERROR\n" );
	
	Graphics::GetInstance()->LoadAnimationAsset( "../Content/Assets/Robot/Animations/", "robotLegsWalk.PaMan", mAnimations[PLAYER_ANIMATION_LEGS_WALK] );
	Graphics::GetInstance()->LoadAnimationAsset( "../Content/Assets/Robot/Animations/", "robotLegsIdle.PaMan", mAnimations[PLAYER_ANIMATION_LEGS_IDLE] );

	EventManager::GetInstance()->AddListener( &RemotePlayer::RemoteUpdate, this, Event_Remote_Player_Update::GUID );

	mUpperBody.position	= XMFLOAT3( 3.0f, 0.0f, 0.0f );
	mLowerBody.position	= XMFLOAT3( 3.0f, 0.0f, 0.0f );

	mLowerBody.currentLowerAnimation		= PLAYER_ANIMATION_LEGS_IDLE;
	mLowerBody.currentLowerAnimationTime	= 1.0f;

	mBoundingBox			= new BoundingBox( 1.5f, 1.5f );
	mBoundingCircle			= new BoundingCircle( 0.5f );

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