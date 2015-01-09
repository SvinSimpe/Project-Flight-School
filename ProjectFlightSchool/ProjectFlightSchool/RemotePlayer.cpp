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

HRESULT RemotePlayer::Initialize( unsigned int id )
{
	if ( FAILED( Graphics::GetInstance()->LoadStatic3dAsset( "../Content/Assets/Robot/", "robotScenebody.pfs", mUpperBody.playerModel) ) )
		OutputDebugString( L"\nERROR\n" );

	if ( FAILED( Graphics::GetInstance()->LoadStatic3dAsset( "../Content/Assets/Robot/", "robotScenelegs.pfs", mLowerBody.playerModel ) ) )
		OutputDebugString( L"\nERROR\n" );

	mID					= id;
	mUpperBody.position = XMFLOAT3( 10.0f, 0.0f, 10.0f );
	mLowerBody.position = XMFLOAT3( 10.0f, 0.0f, 10.0f );
	mLowerBody.speed	= 0.005f;

	mBoundingBox			= new BoundingBox();
	mBoundingBox->height	= 1.5f;
	mBoundingBox->width		= 1.5f;

	mBoundingCircle			= new BoundingCircle();
	mBoundingCircle->radius	= 0.5f;



	EventManager::GetInstance()->AddListener( &RemotePlayer::RemoteUpdate, this, Event_Remote_Player_Update::GUID );

	return S_OK;
}

void RemotePlayer::Release()
{
	EventManager::GetInstance()->RemoveListener( &RemotePlayer::RemoteUpdate, this, Event_Remote_Player_Update::GUID );
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
	mUpperBody.position		= XMFLOAT3(0.0f, 0.0f, 0.0f);
	mUpperBody.direction	= XMFLOAT3(0.0f, 0.0f, 0.0f);

	mLowerBody.playerModel	= 0;
	mLowerBody.position		= XMFLOAT3(0.0f, 0.0f, 0.0f);
	mLowerBody.direction	= XMFLOAT3(0.0f, 0.0f, 0.0f);
	mLowerBody.speed		= 0.0f;

	mBoundingBox			= nullptr;
	mBoundingCircle			= nullptr;
	mIsMovable				= true;
}

RemotePlayer::~RemotePlayer()
{}