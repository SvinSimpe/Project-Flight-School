#include "Player.h"

void Player::HandleInput( float deltaTime )
{
	mAcceleration = XMFLOAT3( 0.0f, 0.0f, 0.0f );
	if( Input::GetInstance()->mCurrentFrame.at(KEYS::KEYS_W) && !Input::GetInstance()->mCurrentFrame.at(KEYS::KEYS_S) )
		mAcceleration.z = mMaxAcceleration;
	
	if( Input::GetInstance()->mCurrentFrame.at(KEYS::KEYS_A) && !Input::GetInstance()->mCurrentFrame.at(KEYS::KEYS_D) )
		mAcceleration.x = -mMaxAcceleration;

	if( Input::GetInstance()->mCurrentFrame.at(KEYS::KEYS_S) && !Input::GetInstance()->mCurrentFrame.at(KEYS::KEYS_W) )
		mAcceleration.z = -mMaxAcceleration;
	
	if( Input::GetInstance()->mCurrentFrame.at(KEYS::KEYS_D) && !Input::GetInstance()->mCurrentFrame.at(KEYS::KEYS_A) )
		mAcceleration.x = mMaxAcceleration;

	//Normalize acceleration 
	XMVECTOR normalizer = XMVector3Length( XMLoadFloat3( &mAcceleration ) );
	if( XMVectorGetX( normalizer ) > mMaxAcceleration )
	{
		normalizer	 = XMVector3Normalize( XMLoadFloat3( &mAcceleration ) );
		normalizer	*= mMaxAcceleration;
		XMStoreFloat3( &mAcceleration, normalizer );
	}

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

	//== Weapon handling ==
	if( Input::GetInstance()->mCurrentFrame.at(KEYS::KEYS_SPACE) && mWeaponCoolDown <= 0.0f )
	{
		Fire();
		mWeaponCoolDown = 0.1f;
	}
	else
		mWeaponCoolDown -= deltaTime;

	//== Event to sync player with server ==
	IEventPtr E1( new Event_Player_Update( mLowerBody.position, mLowerBody.direction, mLowerBody.currentLowerAnimation, mLowerBody.currentLowerAnimationTime, mUpperBody.position, mUpperBody.direction ) );
	EventManager::GetInstance()->QueueEvent( E1 );
}

void Player::Move( float deltaTime )
{
	mVelocity.x += mAcceleration.x * deltaTime - mVelocity.x * VELOCITY_FALLOFF * deltaTime;
	mVelocity.y = 0.0f;
	mVelocity.z += mAcceleration.z * deltaTime - mVelocity.z * VELOCITY_FALLOFF * deltaTime;

	XMVECTOR normalizer = XMVector3Length( XMLoadFloat3( &mVelocity ) );
	mCurrentVelocity = XMVectorGetX( normalizer );
	if(  mCurrentVelocity > mMaxVelocity )
	{
		normalizer	 = XMVector3Normalize( XMLoadFloat3( &mVelocity ) );
		normalizer	*= mMaxVelocity;
		XMStoreFloat3( &mVelocity, normalizer );
	}

	mLowerBody.direction.x	= mVelocity.x;
	mLowerBody.direction.y	= 0.0f;
	mLowerBody.direction.z	= mVelocity.z;
	normalizer				= XMVector3Normalize( XMLoadFloat3( &mLowerBody.direction ) );
	XMStoreFloat3( &mLowerBody.direction, normalizer );

	mLowerBody.position.x += mVelocity.x * deltaTime;
	mLowerBody.position.z += mVelocity.z * deltaTime;
}

HRESULT Player::Update( float deltaTime )
{
	HandleInput( deltaTime );

	// If player is alive, update position. If hp <= 0 kill player
	if( mIsAlive )
	{
		if ( mCurrentHp <= 0.0f )
		{
			Die();
		}
		else
		{
			Move( deltaTime );

			// Update Animation
			XMFLOAT4X4 upperMatrix	= Graphics::GetInstance()->GetRootMatrix( mLowerBody.playerModel, mAnimations[mLowerBody.currentLowerAnimation], mLowerBody.currentLowerAnimationTime );
			mUpperBody.position		= XMFLOAT3( upperMatrix._41 + mLowerBody.position.x, upperMatrix._42, upperMatrix._43 + mLowerBody.position.z );

			//if( mLowerBody.currentLowerAnimationTime > 0.9f )
			if( mCurrentVelocity < 0.1f )
			{
				mLowerBody.currentLowerAnimation = PLAYER_ANIMATION_LEGS_IDLE;

				if( mLowerBody.currentLowerAnimation != PLAYER_ANIMATION_LEGS_IDLE )
					mLowerBody.currentLowerAnimationTime	= 1.0f;
			}
			else if( mLowerBody.currentLowerAnimation == PLAYER_ANIMATION_LEGS_IDLE )
			{
				mLowerBody.currentLowerAnimation		= PLAYER_ANIMATION_LEGS_WALK;
				mLowerBody.currentLowerAnimationTime	= 1.0f;
			}


			if( mLowerBody.currentLowerAnimation == PLAYER_ANIMATION_LEGS_WALK )
				mLowerBody.currentLowerAnimationTime += deltaTime * mCurrentVelocity / 1.1f;
			else
				mLowerBody.currentLowerAnimationTime += deltaTime;
		}
	}
	else
	{
		RemotePlayer::HandleSpawn( deltaTime );
	}

	///Lock camera position to player
	XMFLOAT3 cameraPosition;
	cameraPosition.x = mLowerBody.position.x;
	cameraPosition.y = mLowerBody.position.y + 20.0f;
	cameraPosition.z = mLowerBody.position.z - 12.0f;

	Graphics::GetInstance()->SetEyePosition( cameraPosition );
	Graphics::GetInstance()->SetFocus( mLowerBody.position );

	//Update Bounding Primitives
	mBoundingBox->position	= mLowerBody.position;
	mBoundingCircle->center	= mLowerBody.position;

	return S_OK;
}


//HRESULT Player::Render( float deltaTime )
//{
//
//	//RenderManager::GetInstance()->AddObject3dToList( mUpperBody.playerModel, mUpperBody.position, mUpperBody.direction );
//	//RenderManager::GetInstance()->AddObject3dToList( mLowerBody.playerModel, mLowerBody.position );
//
//	RemotePlayer::Render( deltaTime );
//
//	return S_OK;
//}

void Player::SetID( unsigned int id )
{
	mID = id;
}

void Player::SetTeam( int team, AssetID teamColor )
{
	mTeam		= team;
	mTeamAsset	= teamColor;
}

void Player::SetColor( AssetID color )
{
	mColorIDAsset = color;
}

XMFLOAT3 Player::GetPlayerPosition() const
{
	return mLowerBody.position;
}

void Player::SetPosition( XMVECTOR position )
{
	XMStoreFloat3( &mLowerBody.position, position );
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

	mUpperBody.position	= XMFLOAT3( 3.0f, 0.0f, 0.0f );
	mLowerBody.position	= XMFLOAT3( 3.0f, 0.0f, 0.0f );

	mMaxVelocity		= 7.7f;
	mCurrentVelocity	= 0.0f;
	mMaxAcceleration	= 20.0f;
	mAcceleration		= XMFLOAT3( 0.0f, 0.0f, 0.0f );
	mVelocity			= XMFLOAT3( 0.0f, 0.0f, 0.0f );


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