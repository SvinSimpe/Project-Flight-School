#include "Player.h"

void Player::HandleInput( float deltaTime )
{
	mLowerBody.direction = XMFLOAT3( 0.0f, 0.0f, 0.0f );

	if( Input::GetInstance()->mCurrentFrame.at(KEYS::KEYS_W) && !Input::GetInstance()->mCurrentFrame.at(KEYS::KEYS_S) )
		Move( XMFLOAT3( 0.0f, 0.0f, 1.0f ) );
	
	if( Input::GetInstance()->mCurrentFrame.at(KEYS::KEYS_A) && !Input::GetInstance()->mCurrentFrame.at(KEYS::KEYS_D) )
		Move( XMFLOAT3( -1.0f, 0.0f, 0.0f ) );

	if( Input::GetInstance()->mCurrentFrame.at(KEYS::KEYS_S) && !Input::GetInstance()->mCurrentFrame.at(KEYS::KEYS_W) )
		Move( XMFLOAT3( 0.0f, 0.0f, -1.0f ) );
	
	if( Input::GetInstance()->mCurrentFrame.at(KEYS::KEYS_D) && !Input::GetInstance()->mCurrentFrame.at(KEYS::KEYS_A) )
		Move( XMFLOAT3( 1.0f, 0.0f, 0.0f ) );



	


	//== Calculate upper body rotation ==
	XMVECTOR rayOrigin	= XMVECTOR( Input::GetInstance()->mCurrentNDCMousePos );
	XMVECTOR rayDir		= rayOrigin;

	XMFLOAT3 unPack;
	XMStoreFloat3( &unPack, rayOrigin );
	rayDir = XMVectorSet( unPack.x, unPack.y, 1.0f, 1.0f );

	XMMATRIX viewInverse;
	Graphics::GetInstance()->SetInverseViewMatrix( viewInverse );

	XMMATRIX projectionInverse;
	Graphics::GetInstance()->SetInverseProjectionMatrix( projectionInverse );

	XMMATRIX combinedInverse = XMMatrixMultiply( projectionInverse, viewInverse );

	XMVECTOR rayPosInWorld	= XMVector3TransformCoord( rayOrigin, combinedInverse );
	XMVECTOR rayDirInWorld	= XMVector3TransformCoord( rayDir, combinedInverse );
	rayDirInWorld			= XMVector3Normalize( rayDirInWorld - rayPosInWorld );



	float t					= 0.0f;
	XMVECTOR planeNormal	= XMVectorSet( 0.0f, 1.0f, 0.0f, 0.0f );
	XMVECTOR result			= -( XMVector3Dot( rayPosInWorld, planeNormal ) ) / ( XMVector3Dot( rayDirInWorld, planeNormal ) );
	XMStoreFloat( &t, result );
	XMVECTOR intersection	= XMVectorAdd( rayPosInWorld, rayDirInWorld * t );


	XMVECTOR playerToCursor = XMVectorSubtract( intersection, XMLoadFloat3( &mUpperBody.position ) );
	XMStoreFloat3( &unPack, playerToCursor );
	playerToCursor = XMVectorSet( unPack.x, 0.0f, unPack.z, 0.0f );


	playerToCursor	= XMVector3Normalize( playerToCursor );
	float radians	= atan2f( XMVectorGetZ( playerToCursor ), XMVectorGetX( playerToCursor ) );


 	mUpperBody.direction.x = 0.0f;
	mUpperBody.direction.y = -radians;
	mUpperBody.direction.z = 0.0f;

	if( Input::GetInstance()->mCurrentFrame.at(KEYS::KEYS_SPACE) && mWeaponCoolDown <= 0.0f )
	{
		Fire();
		mWeaponCoolDown = 0.1f;
	}
	else
		mWeaponCoolDown -= deltaTime;

	IEventPtr E1( new Event_Player_Moved( mLowerBody.position, mUpperBody.position, mUpperBody.direction ) );
	EventManager::GetInstance()->QueueEvent( E1 );
}

void Player::Move( XMFLOAT3 direction )
{
	mLowerBody.direction.x += direction.x;
	mLowerBody.direction.z += direction.z;
	
	//Normalize direction vector
	if ( direction.x < 0.00001f && direction.z < 0.00001f )
	{
		mLowerBody.direction.x /= pow((pow(mLowerBody.direction.x, 2) + pow(mLowerBody.direction.y, 2) + pow(mLowerBody.direction.z, 2)), 0.5f);
		mLowerBody.direction.z /= pow((pow(mLowerBody.direction.x, 2) + pow(mLowerBody.direction.y, 2) + pow(mLowerBody.direction.z, 2)), 0.5f);
	}
}

HRESULT Player::Update( float deltaTime )
{
	HandleInput( deltaTime );

	// If player is alive, update position. If hp <= 0 kill player
	if( mIsAlive )
	{
			mUpperBody.position.x += mLowerBody.direction.x * mLowerBody.speed * deltaTime;
			mUpperBody.position.z += mLowerBody.direction.z * mLowerBody.speed * deltaTime;

			mLowerBody.position.x += mLowerBody.direction.x * mLowerBody.speed * deltaTime;
			mLowerBody.position.z += mLowerBody.direction.z * mLowerBody.speed * deltaTime;
	}
	else
	{
		RemotePlayer::HandleSpawn( deltaTime );
	}


	///Lock camera position to player
	XMFLOAT3 cameraPosition;
	cameraPosition.x = mLowerBody.position.x;
	cameraPosition.y = mLowerBody.position.y + 25.0f;
	cameraPosition.z = mLowerBody.position.z - 15.0f;

	Graphics::GetInstance()->SetEyePosition( cameraPosition );
	Graphics::GetInstance()->SetFocus( mLowerBody.position );

	//Update Bounding Primitives
	mBoundingBox->position	= mLowerBody.position;
	mBoundingCircle->center	= mLowerBody.position;

	return S_OK;
}

HRESULT Player::Render( float deltaTime )
{

	//RenderManager::GetInstance()->AddObject3dToList( mUpperBody.playerModel, mUpperBody.position, mUpperBody.direction );
	//RenderManager::GetInstance()->AddObject3dToList( mLowerBody.playerModel, mLowerBody.position );

	RemotePlayer::Render( deltaTime );

	return S_OK;
}

void Player::SetID( unsigned int id )
{
	mID = id;
}

void Player::SetTeam( int team )
{
	mTeam = team;
}

XMFLOAT3 Player::GetPlayerPosition() const
{
	return mLowerBody.position;
}

void Player::SetPosition( XMVECTOR position )
{
	XMStoreFloat3( &mLowerBody.position, position );
	XMStoreFloat3( &mUpperBody.position, position );
}

XMFLOAT3 Player::GetUpperBodyDirection() const
{
	return mUpperBody.direction;
}

void Player::Fire()
{
	IEventPtr E1( new Event_Projectile_Fired( mID, mUpperBody.position, mUpperBody.direction ) );
	EventManager::GetInstance()->QueueEvent( E1 );
}

HRESULT Player::Initialize()
{
	RemotePlayer::Initialize();
	if( FAILED( Graphics::GetInstance()->LoadStatic3dAsset( "../Content/Assets/Robot/", "robotScenebody.pfs", mUpperBody.playerModel ) ) )
		OutputDebugString( L"\nERROR\n" );

	if( FAILED( Graphics::GetInstance()->LoadStatic3dAsset( "../Content/Assets/Robot/", "robotScenelegs.pfs", mLowerBody.playerModel ) ) )
		OutputDebugString( L"\nERROR\n" );


	mUpperBody.position	= XMFLOAT3( 3.0f, 0.0f, 0.0f );
	mLowerBody.position	= XMFLOAT3( 3.0f, 0.0f, 0.0f );
	mLowerBody.speed	= 15.0f;

	mWeaponCoolDown		= 0.1f;

	mBoundingBox			= new BoundingBox( 1.5f, 1.5f );
	mBoundingCircle			= new BoundingCircle( 0.5f );

	mWeaponCoolDown		= 0.1f;


	//Weapon Initialization
	mLoadOut				= new LoadOut();
	mLoadOut->rangedWeapon	= new RangedInfo( "Machine Gun", 5.0f, 1, 5.0f, 2, 0 );
	mLoadOut->meleeWeapon	= new MeleeInfo( "Sword", 4.0f, 3, 7, 2.0f );

	return S_OK;
}

void Player::Release()
{
	mLoadOut->Release();
	SAFE_DELETE( mLoadOut );
	
	RemotePlayer::Release();
}

Player::Player()
	:RemotePlayer()
{
	mWeaponCoolDown	= 0.0f;
	mLoadOut		= nullptr;
}

Player::~Player()
{}