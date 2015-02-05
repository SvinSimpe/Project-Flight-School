#include "Player.h"

void Player::HandleInput( float deltaTime )
{
	mAcceleration = XMFLOAT3( 0.0f, 0.0f, 0.0f );
	if( Input::GetInstance()->IsKeyDown( KEYS::KEYS_W ) && !Input::GetInstance()->IsKeyDown( KEYS::KEYS_S ) )
		mAcceleration.z = mMaxAcceleration;
	
	if( Input::GetInstance()->IsKeyDown( KEYS::KEYS_A ) && !Input::GetInstance()->IsKeyDown( KEYS::KEYS_D ) )
		mAcceleration.x = -mMaxAcceleration;

	if( Input::GetInstance()->IsKeyDown( KEYS::KEYS_S ) && !Input::GetInstance()->IsKeyDown( KEYS::KEYS_W ) )
		mAcceleration.z = -mMaxAcceleration;
	
	if( Input::GetInstance()->IsKeyDown( KEYS::KEYS_D ) && !Input::GetInstance()->IsKeyDown( KEYS::KEYS_A ) )
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


	XMVECTOR playerToCursor = XMVectorSubtract( intersection, XMLoadFloat3( &XMFLOAT3( mLowerBody.position.x, 1.0f, mLowerBody.position.z ) ) );
	XMStoreFloat3( &unPack, playerToCursor );
	playerToCursor = XMVector3Normalize( XMVectorSet( unPack.x, 0.0f, unPack.z, 0.0f ) );
	XMStoreFloat3( &mUpperBody.direction, playerToCursor );

	//== Weapon handling ==
	if( Input::GetInstance()->IsKeyDown( KEYS::KEYS_MOUSE_LEFT ) && mWeaponCoolDown <= 0.0f )
	{
		Fire();
		mWeaponCoolDown = 0.1f;
		
		RenderManager::GetInstance()->AnimationStartNew( mArms.rightArm, mWeaponAnimations[mLoadOut->rangedWeapon->weaponType][ATTACK] );
		mRightArmAnimationCompleted		= false;

		IEventPtr E1( new Event_Player_Attack( RIGHT_ARM_ID, mWeaponAnimations[mLoadOut->rangedWeapon->weaponType][ATTACK] ) );
		EventManager::GetInstance()->QueueEvent( E1 );
	}
	else
		mWeaponCoolDown -= deltaTime;

	if( Input::GetInstance()->IsKeyDown( KEYS::KEYS_MOUSE_RIGHT ) && mMeleeCoolDown <= 0.0f )
	{
		RenderManager::GetInstance()->AnimationStartNew( mArms.leftArm, mWeaponAnimations[mLoadOut->meleeWeapon->weaponType][ATTACK] );
		mLeftArmAnimationCompleted		= false;
		mHasMeleeStarted				= true;
		mMeleeCoolDown					= mLoadOut->meleeWeapon->attackRate;

		//if( mTimeTillattack <= 0.0f )
		//{
		//	mIsMeleeing						= true;
		//	mMeleeCoolDown					= mLoadOut->meleeWeapon->attackRate;
		//	//RenderManager::GetInstance()->AnimationStartNew( mArms.leftArm, mWeaponAnimations[mLoadOut->meleeWeapon->weaponType][ATTACK] );
		//	//mLeftArmAnimationCompleted		= false;

		//	IEventPtr E1( new Event_Player_Attack( LEFT_ARM_ID, mWeaponAnimations[mLoadOut->meleeWeapon->weaponType][ATTACK]) );
		//	EventManager::GetInstance()->QueueEvent( E1 );

		//	mTimeTillattack = mLoadOut->meleeWeapon->timeTillAttack;
		//	mHasMeleeStarted		= false;
		//}
	}
	else
		mMeleeCoolDown -= deltaTime;
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

	// Mele attack
	if( mHasMeleeStarted )
		mTimeTillattack -= deltaTime;

	if( mTimeTillattack <= 0.0f )
	{
		mIsMeleeing						= true;
		mMeleeCoolDown					= mLoadOut->meleeWeapon->attackRate;
		//RenderManager::GetInstance()->AnimationStartNew( mArms.leftArm, mWeaponAnimations[mLoadOut->meleeWeapon->weaponType][ATTACK] );
		//mLeftArmAnimationCompleted		= false;

		IEventPtr E1( new Event_Player_Attack( LEFT_ARM_ID, mWeaponAnimations[mLoadOut->meleeWeapon->weaponType][ATTACK]) );
		EventManager::GetInstance()->QueueEvent( E1 );

		mTimeTillattack = mLoadOut->meleeWeapon->timeTillAttack;
		mHasMeleeStarted		= false;
	}

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
			if( mCurrentVelocity < 0.2f )
			{
				if( mLowerBody.playerModel.mNextAnimation != mAnimations[PLAYER_ANIMATION::LEGS_IDLE] )
				{
					RenderManager::GetInstance()->AnimationStartNew( mLowerBody.playerModel, mAnimations[PLAYER_ANIMATION::LEGS_IDLE] );
				}
			}
			else
			{
				if(	mLowerBody.playerModel.mNextAnimation != mAnimations[PLAYER_ANIMATION::LEGS_WALK] )
				{
					RenderManager::GetInstance()->AnimationStartNew( mLowerBody.playerModel, mAnimations[PLAYER_ANIMATION::LEGS_WALK] );
				}
			}

			RenderManager::GetInstance()->AnimationUpdate( mLowerBody.playerModel, 
				mLowerBody.playerModel.mNextAnimation == mAnimations[PLAYER_ANIMATION::LEGS_WALK] ? deltaTime * mCurrentVelocity / 1.1f : deltaTime );

			if( mLeftArmAnimationCompleted && mArms.leftArm.mNextAnimation != mWeaponAnimations[mLoadOut->meleeWeapon->weaponType][IDLE] )
				RenderManager::GetInstance()->AnimationStartNew( mArms.leftArm, mWeaponAnimations[mLoadOut->meleeWeapon->weaponType][IDLE] );

			if( mRightArmAnimationCompleted && mArms.rightArm.mNextAnimation != mWeaponAnimations[mLoadOut->rangedWeapon->weaponType][IDLE] )
				RenderManager::GetInstance()->AnimationStartNew( mArms.rightArm, mWeaponAnimations[mLoadOut->rangedWeapon->weaponType][IDLE] );

			RenderManager::GetInstance()->AnimationUpdate( mArms.leftArm, deltaTime );
			RenderManager::GetInstance()->AnimationUpdate( mArms.rightArm, deltaTime );

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
	mBoundingBox->position							= mLowerBody.position;
	mBoundingCircle->center							= mLowerBody.position;
	mLoadOut->meleeWeapon->boundingCircle->center	= mLowerBody.position;

	//Update Light
	mPointLight[0]->position = DirectX::XMFLOAT4( mLowerBody.position.x, mLowerBody.position.y + 7.0f, mLowerBody.position.z, 0.0f );
	mPointLight[1]->position = DirectX::XMFLOAT4( mLowerBody.position.x - 10.0f, mLowerBody.position.y + 7.0f, mLowerBody.position.z + 10.0f, 0.0f );
	mPointLight[2]->position = DirectX::XMFLOAT4( mLowerBody.position.x + 10.0f, mLowerBody.position.y + 7.0f, mLowerBody.position.z + 10.0f, 0.0f );
	mPointLight[3]->position = DirectX::XMFLOAT4( mLowerBody.position.x - 10.0f, mLowerBody.position.y + 7.0f, mLowerBody.position.z - 10.0f, 0.0f );
	mPointLight[4]->position = DirectX::XMFLOAT4( mLowerBody.position.x + 10.0f, mLowerBody.position.y + 7.0f, mLowerBody.position.z - 10.0f, 0.0f );

	//== Event to sync player with server ==
	mEventCapTimer += deltaTime;
	if( mEventCapTimer > 0.02f )
	{
		IEventPtr E1( new Event_Player_Update( mLowerBody.position, mVelocity, mUpperBody.direction, mIsAlive ) );
		EventManager::GetInstance()->QueueEvent( E1 );
		mEventCapTimer -= 0.02f;
	}

	return S_OK;
}

HRESULT Player::Render( float deltaTime, int position )
{
	if( !mIsAlive )
	{
		
        std::string textToWrite = "TEN";
		
		if( mTimeTillSpawn < 1.0f )
		{
			textToWrite = "ONE";
		}
		else if( mTimeTillSpawn < 2.0f )
		{
			textToWrite = "TWO";
		}
		else if( mTimeTillSpawn < 3.0f )
		{
			textToWrite = "THREE";
		}
		else if( mTimeTillSpawn < 4.0f )
		{
			textToWrite = "FOUR";
		}
		else if( mTimeTillSpawn < 5.0f )
		{
			textToWrite = "FIVE";
		}
		else if( mTimeTillSpawn < 6.0f )
		{
			textToWrite = "SIX";
		}
		else if( mTimeTillSpawn < 7.0f )
		{
			textToWrite = "SEVEN";
		}
		else if( mTimeTillSpawn < 8.0f )
		{
			textToWrite = "EIGHT";
		}
		else if( mTimeTillSpawn < 9.0f )
		{
			textToWrite = "NINE";
		}
		


		mFont.WriteText( textToWrite, 500.0f, 500.0f, 7.8f );
	}

	RemotePlayer::Render( position );


	return S_OK;
}

void Player::TakeDamage( float damage, unsigned int shooter )
{
	if( mIsBuffed )
	{
		float moddedDmg = damage * mBuffMod;
		damage -= moddedDmg;

	}
	RemotePlayer::TakeDamage( damage, shooter );
}

void Player::TakeEnemyDamage( float damage )
{
	mCurrentHp -= damage;
	IEventPtr player( new Event_Player_Update_HP( mID, mCurrentHp ) );
	EventManager::GetInstance()->QueueEvent( player );
	if ( mIsAlive && mCurrentHp <= 0.0f )
	{
		Die();
	}
}

void Player::SetBuffed( bool buffed )
{
	mIsBuffed = buffed;
}

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

bool Player::GetIsMeleeing() const
{
	return mIsMeleeing;
}

void Player::SetIsMeleeing( bool isMeleeing )
{
	mIsMeleeing = isMeleeing;
}

void Player::Fire()
{
	//Hardcoded to match shotgun
	XMVECTOR position	= XMLoadFloat3( &mLowerBody.position );
	XMVECTOR direction	= XMLoadFloat3( &mUpperBody.direction );
	XMVECTOR offset		= XMLoadFloat3( &XMFLOAT3( mLowerBody.position.x, 1.0f, mLowerBody.position.z ) );
	
	offset += XMVector3Normalize( XMVector3Cross( XMLoadFloat3( &XMFLOAT3( 0.0f, 1.0f, 0.0f ) ), direction ) ) * 0.5f;
	offset += direction * 1.8f;

	XMFLOAT3 loadDir;
	XMStoreFloat3( &loadDir, offset );

	IEventPtr E1( new Event_Projectile_Fired( mID, XMFLOAT3( loadDir ), mUpperBody.direction ) );
	EventManager::GetInstance()->QueueEvent( E1 );
}

HRESULT Player::Initialize()
{
	RemotePlayer::Initialize();

	mLowerBody.position	= XMFLOAT3( 3.0f, 0.0f, 0.0f );

	////////////
	// Light
	for( int i = 0; i < 5; i++ )
	{
		mPointLight[i]						= new PointLight;
		mPointLight[i]->position			= DirectX::XMFLOAT4( mLowerBody.position.x, mLowerBody.position.y, mLowerBody.position.z, 0.0f );
		IEventPtr reg( new Event_Add_Point_Light( mPointLight[i] ) );
		EventManager::GetInstance()->QueueEvent( reg );
	}

	mPointLight[0]->colorAndRadius		= DirectX::XMFLOAT4( 0.6f, 0.6f, 0.6f, 20.0f );
	mPointLight[1]->colorAndRadius		= DirectX::XMFLOAT4( 0.6f, 0.2f, 0.2f, 20.0f );
	mPointLight[2]->colorAndRadius		= DirectX::XMFLOAT4( 0.2f, 0.6f, 0.2f, 20.0f );
	mPointLight[3]->colorAndRadius		= DirectX::XMFLOAT4( 0.2f, 0.2f, 0.6f, 20.0f );
	mPointLight[4]->colorAndRadius		= DirectX::XMFLOAT4( 0.6f, 0.6f, 0.2f, 20.0f );

	mMaxVelocity		= 7.7f;
	mCurrentVelocity	= 0.0f;
	mMaxAcceleration	= 20.0f;
	mAcceleration		= XMFLOAT3( 0.0f, 0.0f, 0.0f );
	mVelocity			= XMFLOAT3( 0.0f, 0.0f, 0.0f );

	mBuffMod			= 0.5f;

	mTimeTillattack	= mLoadOut->meleeWeapon->timeTillAttack;

	return S_OK;
}

void Player::Release()
{
	RemotePlayer::Release();
	for( int i = 0; i < 5; i++ )
	{
		IEventPtr reg( new Event_Remove_Point_Light( mPointLight[0] ) );
		EventManager::GetInstance()->QueueEvent( reg );
		SAFE_DELETE( mPointLight[i] );
	}
}

Player::Player()
	:RemotePlayer()
{
	mEventCapTimer		= 0.0f;

	for( int i = 0; i < 5; i++ )
		mPointLight[i]	= nullptr;

	mWeaponCoolDown		= 0.0f;
	mMeleeCoolDown		= 0.0f;
	mTimeTillattack		= 0.0f;
	mIsMeleeing			= false;

	mMaxVelocity		= 0.0f;
	mCurrentVelocity	= 0.0f;
	mMaxAcceleration	= 0.0f;
	mAcceleration		= XMFLOAT3( 0.0f, 0.0f, 0.0f );
	mIsBuffed			= false;
	mBuffMod			= 0.0f;
	mHasMeleeStarted	= false;
}

Player::~Player()
{}