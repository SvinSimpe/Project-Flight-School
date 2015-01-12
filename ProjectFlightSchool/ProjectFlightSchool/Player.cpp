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
		mWeaponCoolDown = 0.2f;
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

	mUpperBody.position.x += mLowerBody.direction.x * mLowerBody.speed * deltaTime;
	mUpperBody.position.z += mLowerBody.direction.z * mLowerBody.speed * deltaTime;

	mLowerBody.position.x += mLowerBody.direction.x * mLowerBody.speed * deltaTime;
	mLowerBody.position.z += mLowerBody.direction.z * mLowerBody.speed * deltaTime;

	///Lock camera position to player
	XMFLOAT3 cameraPosition;
	cameraPosition.x = mLowerBody.position.x;
	cameraPosition.y = mLowerBody.position.y + 21.0f;
	cameraPosition.z = mLowerBody.position.z - 21.0f;

	Graphics::GetInstance()->SetEyePosition( cameraPosition );
	Graphics::GetInstance()->SetFocus( mLowerBody.position );

	//Update Projectiles
	if( mNrOfProjectilesFired != 0 )
	{
		for ( size_t i = 0; i < mProjectiles.size(); i++ )
		{
			if( mProjectiles.at(i)->IsActive() )
				mProjectiles.at(i)->Update( deltaTime );
		}
	}

	return S_OK;
}

HRESULT Player::Render( float deltaTime )
{
	RenderManager::GetInstance()->AddObject3dToList( mUpperBody.playerModel, mUpperBody.position, mUpperBody.direction );
	RenderManager::GetInstance()->AddObject3dToList( mLowerBody.playerModel, mLowerBody.position );

	//Update Projectiles
	if( mNrOfProjectilesFired != 0 )
	{
		for ( size_t i = 0; i < mProjectiles.size(); i++ )
		{
			if( mProjectiles.at(i)->IsActive() )
				mProjectiles.at(i)->Render( deltaTime );
		}
	}

	return S_OK;
}

void Player::Fire()
{
	mProjectiles.at( mNrOfProjectilesFired )->SetDirection( mUpperBody.position, mUpperBody.direction );
	mProjectiles.at( mNrOfProjectilesFired )->SetIsActive( true );
	mNrOfProjectilesFired++;
}



HRESULT Player::Initialize()
{
	if( FAILED( Graphics::GetInstance()->LoadStatic3dAsset( "../Content/Assets/Robot/", "robotScenebody.pfs", mUpperBody.playerModel ) ) )
		OutputDebugString( L"\nERROR\n" );

	if( FAILED( Graphics::GetInstance()->LoadStatic3dAsset( "../Content/Assets/Robot/", "robotScenelegs.pfs", mLowerBody.playerModel ) ) )
		OutputDebugString( L"\nERROR\n" );


	mUpperBody.position	= XMFLOAT3( 3.0f, 0.0f, 0.0f );
	mLowerBody.position	= XMFLOAT3( 3.0f, 0.0f, 0.0f );
	mLowerBody.speed	= 15.0f;

	mWeaponCoolDown		= 0.5f;

	//Fill up on Projectiles
	for ( size_t i = 0; i < 2000; i++ )
	{
		Projectile*	projectile = new Projectile();
		projectile->Initialize();
		mProjectiles.push_back( projectile );
	}

	return S_OK;
}

void Player::Release()
{
	for ( size_t i = 0; i < mProjectiles.size(); i++ )
		SAFE_DELETE( mProjectiles.at(i) );
}

Player::Player()
{
	mUpperBody.playerModel	= 0;
	mUpperBody.position		= XMFLOAT3( 0.0f, 0.0f, 0.0f );
	mUpperBody.direction	= XMFLOAT3( 0.0f, 0.0f, 0.0f );

	mLowerBody.playerModel	= 0;
	mLowerBody.position		= XMFLOAT3( 0.0f, 0.0f, 0.0f );
	mLowerBody.direction	= XMFLOAT3( 0.0f, 0.0f, 0.0f );
	mLowerBody.speed		= 0.0f;

	mWeaponCoolDown			= 0.0f;
}

Player::~Player()
{}