#include "RemotePlayer.h"

void RemotePlayer::RemoteUpdate( IEventPtr newEvent )
{
	if( newEvent->GetEventType() == Event_Remote_Player_Update::GUID )
	{
		std::shared_ptr<Event_Remote_Player_Update> data = std::static_pointer_cast<Event_Remote_Player_Update>( newEvent );
		if( mID == data->ID() )
		{
			mLowerBody.position								= data->LowerBodyPos();
			XMStoreFloat3( &mLowerBody.direction, XMVector3Normalize( XMLoadFloat3( &data->Velocity() ) ) );
			mVelocity										= data->Velocity();
			mUpperBody.direction							= data->UpperBodyDirection();
			mPlayerName										= data->Name();

			//TEST
			mBoundingBox->position		= mLowerBody.position;
			mBoundingCircle->center		= mLowerBody.position;
			mBoundingCircleAura->center	= mLowerBody.position;
		}
	}
	else if( newEvent->GetEventType() == Event_Remote_Player_Attack::GUID )
	{
		std::shared_ptr<Event_Remote_Player_Attack> data = std::static_pointer_cast<Event_Remote_Player_Attack>( newEvent );
		if( mID == data->ID() )
		{
			if( data->ArmID() == LEFT_ARM_ID )
			{
				RenderManager::GetInstance()->AnimationStartNew( mArms.leftArm, data->Animation() );
				mLeftArmAnimationCompleted = false;
			}
			else if( data->ArmID() == RIGHT_ARM_ID )
			{
				RenderManager::GetInstance()->AnimationStartNew( mArms.rightArm, data->Animation() );
				mRightArmAnimationCompleted = false;
			}
		}
	}
}

void RemotePlayer::RemoteInit( unsigned int id, int team, AssetID teamColor, AssetID colorID )
{
	mID				= id;
	mTeam			= team;
	mTeamAsset		= teamColor;
	mColorIDAsset	= colorID;
	EventManager::GetInstance()->AddListener( &RemotePlayer::RemoteUpdate, this, Event_Remote_Player_Update::GUID );
	EventManager::GetInstance()->AddListener( &RemotePlayer::RemoteUpdate, this, Event_Remote_Player_Attack::GUID );
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
	IEventPtr reg( new Event_Remove_Point_Light( mPointLightIfDown ) );
	EventManager::GetInstance()->QueueEvent( reg );
	SAFE_DELETE( mPointLightIfDown );
	mNrOfDeaths++;
	mIsAlive		= false;
	mIsDown			= false;
	mCurrentHp		= 0.0f;
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

void RemotePlayer::HandleDeath( float deltaTime )
{
	if( mTimeTillDeath <= 0.0f )
	{
		Die();
		BroadcastDeath( mLastKiller );
	}
	else
	{
		mTimeTillDeath -= deltaTime;
	}
}

void RemotePlayer::Spawn()
{
	mIsAlive			= true;
	mCurrentHp			= mMaxHp;
	mLowerBody.position = XMFLOAT3( 10.0f, 0.0f, 10.0f ); // Change to ship position + random offset
	IEventPtr spawnEv( new Event_Player_Spawned( mID ) );
	EventManager::GetInstance()->QueueEvent( spawnEv );
}

void RemotePlayer::TakeDamage( float damage, unsigned int shooter )
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

void RemotePlayer::SetName( std::string name )
{
	mPlayerName = name;
}

void RemotePlayer::SetHP( float hp )
{
	mCurrentHp = hp;
}

void RemotePlayer::CountUpKills()
{
	mNrOfKills++;
}

void RemotePlayer::GoDown()
{
	mTimeTillDeath				= mDeathTime;
	mTimeTillRevive				= mReviveTime;
	mIsAlive					= false;
	mIsDown						= true;
	mPointLightIfDown			= new PointLight;
	mPointLightIfDown->position	= DirectX::XMFLOAT4( mLowerBody.position.x, mLowerBody.position.y, mLowerBody.position.z, 0.0f );
	IEventPtr reg( new Event_Add_Point_Light( mPointLightIfDown ) );
	EventManager::GetInstance()->QueueEvent( reg );
	mPointLightIfDown->colorAndRadius = DirectX::XMFLOAT4( 0.6f, 0.2f, 0.6f, 8.0f );
}

void RemotePlayer::GoUp()
{
	mIsAlive	= true;
	mIsDown		= false;
	mCurrentHp	= mMaxHp / 5.0f;
	IEventPtr reg( new Event_Remove_Point_Light( mPointLightIfDown ) );
	EventManager::GetInstance()->QueueEvent( reg );
	SAFE_DELETE( mPointLightIfDown );
}

bool RemotePlayer::IsAlive() const
{
	return mIsAlive;
}

std::string RemotePlayer::GetName() const
{
	return mPlayerName;
}

bool RemotePlayer::IsDown() const
{
	return mIsDown;
}

LoadOut* RemotePlayer::GetLoadOut() const
{
	return mLoadOut;
}

float RemotePlayer::GetHP() const
{
	return mCurrentHp;
}

float RemotePlayer::GetMaxHP() const
{
	return mMaxHp;
}

int RemotePlayer::GetID() const
{
	return mID;
}

int RemotePlayer::GetTeam() const
{
	return mTeam;
}

HRESULT RemotePlayer::Update( float deltaTime )
{
	// Update Animation
	float currentVelocity = XMVectorGetX( XMVector3Length( XMLoadFloat3( &mVelocity ) ) );

	if( currentVelocity < 0.2f )
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
		mLowerBody.playerModel.mNextAnimation == mAnimations[PLAYER_ANIMATION::LEGS_WALK] ? deltaTime * currentVelocity / 1.1f : deltaTime );

	if( mLeftArmAnimationCompleted && mArms.leftArm.mNextAnimation != mWeaponAnimations[mLoadOut->meleeWeapon->weaponType][IDLE] )
		RenderManager::GetInstance()->AnimationStartNew( mArms.leftArm, mWeaponAnimations[mLoadOut->meleeWeapon->weaponType][IDLE] );

	if( mRightArmAnimationCompleted && mArms.rightArm.mNextAnimation != mWeaponAnimations[mLoadOut->rangedWeapon->weaponType][IDLE] )
		RenderManager::GetInstance()->AnimationStartNew( mArms.rightArm, mWeaponAnimations[mLoadOut->rangedWeapon->weaponType][IDLE] );

	RenderManager::GetInstance()->AnimationUpdate( mArms.leftArm, deltaTime );
	RenderManager::GetInstance()->AnimationUpdate( mArms.rightArm, deltaTime );

	/*if( mIsDown )
	{
		mPointLightIfDown->position = DirectX::XMFLOAT4( mLowerBody.position.x, mLowerBody.position.y + 7.0f, mLowerBody.position.z, 0.0f );
	}*/

	return S_OK;
}

HRESULT RemotePlayer::Render( int position )
{
	XMFLOAT4X4 upperMatrix	= Graphics::GetInstance()->GetRootMatrix( mLowerBody.playerModel );
	XMFLOAT3 offsetPos		= XMFLOAT3( upperMatrix._41 + mLowerBody.position.x, upperMatrix._42, upperMatrix._43 + mLowerBody.position.z );

	//Render upper body
	float radians = atan2f( mUpperBody.direction.z, mUpperBody.direction.x );
	RenderManager::GetInstance()->AddObject3dToList( mUpperBody.playerModel, offsetPos, XMFLOAT3( 0.0f, -radians, 0.0f ) );

	//Render Arms
	if( RenderManager::GetInstance()->AddAnim3dToList(	mArms.leftArm,
														ANIMATION_PLAY_ONCE,
														offsetPos,
														XMFLOAT3( 0.0f, -radians, 0.0f ) ) )
		mLeftArmAnimationCompleted = true;

	if( RenderManager::GetInstance()->AddAnim3dToList(	mArms.rightArm,
														ANIMATION_PLAY_ONCE,
														offsetPos, 
														XMFLOAT3( 0.0f, -radians, 0.0f ) ) )
		mRightArmAnimationCompleted = true;

	//Render lower body
	radians = atan2f( mLowerBody.direction.z, mLowerBody.direction.x );
	RenderManager::GetInstance()->AddAnim3dToList(	mLowerBody.playerModel,
													ANIMATION_PLAY_LOOPED,
													mLowerBody.position,
													XMFLOAT3( 0.0f, -radians, 0.0f ) );

	DirectX::XMFLOAT3 x;
	DirectX::XMFLOAT3 y;

	if ( mIsAlive )
	{
		float renderHpSize = ( mCurrentHp * 1.5f / mMaxHp ) + 1; //*1.5 and +1 to make it an appropriate size.

		x = { mLowerBody.position.x - renderHpSize * 0.5f, 0.01f, mLowerBody.position.z + renderHpSize * 0.5f };
		y = { mLowerBody.position.x + renderHpSize * 0.5f, 0.01f, mLowerBody.position.z - renderHpSize * 0.5f };

		if ( mCurrentHp > ( mMaxHp * 0.5f ) )
			RenderManager::GetInstance()->AddPlaneToList( mGreenHPAsset, x, y );
		else if ( mCurrentHp < ( mMaxHp * 0.25f ) )
			RenderManager::GetInstance()->AddPlaneToList( mRedHPAsset, x, y );
		else
			RenderManager::GetInstance()->AddPlaneToList( mOrangeHPAsset, x, y );
	}

	float renderSize;
	if ( mColorIDAsset )
	{
		renderSize = 2.0f + 1;
		x = { mLowerBody.position.x - renderSize * 0.5f, 0.005f, mLowerBody.position.z + renderSize * 0.5f };
		y = { mLowerBody.position.x + renderSize * 0.5f, 0.005f, mLowerBody.position.z - renderSize * 0.5f };
		RenderManager::GetInstance()->AddPlaneToList(mColorIDAsset, x, y);
	}
	
	if ( mTeamAsset )
	{
		renderSize = 2.2f + 1;
		x = { mLowerBody.position.x - renderSize * 0.5f, 0.004f, mLowerBody.position.z + renderSize * 0.5f };
		y = { mLowerBody.position.x + renderSize * 0.5f, 0.004f, mLowerBody.position.z - renderSize * 0.5f };
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

		mFont.WriteText( textToWrite, 25.0f, ((20.0f*(float)position)-7), 1.95f );
	}

	if( mIsDown )
	{
		std::string textToWrite = std::to_string( mID );
		textToWrite				+= " is down";
		mFont.WriteText( textToWrite, 500, 500, 3.0f );
	}

	return S_OK;
}

HRESULT RemotePlayer::Initialize()
{
	////////////////
	//Animations
	// BODY
	if( FAILED( Graphics::GetInstance()->LoadAnimationAsset( "../Content/Assets/PermanentAssets/Robot/Animations/", "robotLegsIdle.PaMan", mAnimations[PLAYER_ANIMATION::LEGS_IDLE] ) ) )
		OutputDebugString( L"\nERROR loading player model\n" );
	if( FAILED( Graphics::GetInstance()->LoadAnimationAsset( "../Content/Assets/PermanentAssets/Robot/Animations/", "robotLegsWalk.PaMan", mAnimations[PLAYER_ANIMATION::LEGS_WALK] ) ) )
		OutputDebugString( L"\nERROR loading player model\n" );

	//BLOWTORCH
	if( FAILED( Graphics::GetInstance()->LoadAnimationAsset( "../Content/Assets/PermanentAssets/Robot/Weapons/Blowtorch/Animations/", "blowtorchIdle.PaMan",			mWeaponAnimations[BLOWTORCH][IDLE] ) ) )
		OutputDebugString( L"\nERROR loading player model\n" );
	if( FAILED( Graphics::GetInstance()->LoadAnimationAsset( "../Content/Assets/PermanentAssets/Robot/Weapons/Blowtorch/Animations/", "blowtorchAttackStart.PaMan",		mWeaponAnimations[BLOWTORCH][ATTACK_START] ) ) )
		OutputDebugString( L"\nERROR loading player model\n" );																							
	if( FAILED( Graphics::GetInstance()->LoadAnimationAsset( "../Content/Assets/PermanentAssets/Robot/Weapons/Blowtorch/Animations/", "blowtorchAttackIdle.PaMan",		mWeaponAnimations[BLOWTORCH][ATTACK_IDLE] ) ) )
		OutputDebugString( L"\nERROR loading player model\n" );																										
	if( FAILED( Graphics::GetInstance()->LoadAnimationAsset( "../Content/Assets/PermanentAssets/Robot/Weapons/Blowtorch/Animations/", "blowtorchAttackEnd.PaMan",		mWeaponAnimations[BLOWTORCH][ATTACK_END] ) ) )
		OutputDebugString( L"\nERROR loading player model\n" );																										
	if( FAILED( Graphics::GetInstance()->LoadAnimationAsset( "../Content/Assets/PermanentAssets/Robot/Weapons/Blowtorch/Animations/", "blowtorchOverheatStart.PaMan",	mWeaponAnimations[BLOWTORCH][OVERHEAT_START] ) ) )
		OutputDebugString( L"\nERROR loading player model\n" );																										
	if( FAILED( Graphics::GetInstance()->LoadAnimationAsset( "../Content/Assets/PermanentAssets/Robot/Weapons/Blowtorch/Animations/", "blowtorchOverheatEnd.PaMan",		mWeaponAnimations[BLOWTORCH][OVERHEAT_END] ) ) )
		OutputDebugString( L"\nERROR loading player model\n" );

	//CLAYMORE
	if( FAILED( Graphics::GetInstance()->LoadAnimationAsset( "../Content/Assets/PermanentAssets/Robot/Weapons/Claymore/Animations/", "claymoreIdle.PaMan"	, mWeaponAnimations[CLAYMORE][IDLE] ) ) )
		OutputDebugString( L"\nERROR loading player model\n" );																						
	if( FAILED( Graphics::GetInstance()->LoadAnimationAsset( "../Content/Assets/PermanentAssets/Robot/Weapons/Claymore/Animations/", "claymoreWalk.PaMan"	, mWeaponAnimations[CLAYMORE][WALK] ) ) )
		OutputDebugString( L"\nERROR loading player model\n" );																								
	if( FAILED( Graphics::GetInstance()->LoadAnimationAsset( "../Content/Assets/PermanentAssets/Robot/Weapons/Claymore/Animations/", "claymoreAttack.PaMan"	, mWeaponAnimations[CLAYMORE][ATTACK] ) ) )
		OutputDebugString( L"\nERROR loading player model\n" );
	
	//HAMMER
	if( FAILED( Graphics::GetInstance()->LoadAnimationAsset( "../Content/Assets/PermanentAssets/Robot/Weapons/Hammer/Animations/", "hammerIdle.PaMan",	mWeaponAnimations[HAMMER][IDLE] ) ) )
		OutputDebugString( L"\nERROR loading player model\n" );																							
	if( FAILED( Graphics::GetInstance()->LoadAnimationAsset( "../Content/Assets/PermanentAssets/Robot/Weapons/Hammer/Animations/", "hammerWalk.PaMan",	mWeaponAnimations[HAMMER][WALK] ) ) )
		OutputDebugString( L"\nERROR loading player model\n" );																							
	if( FAILED( Graphics::GetInstance()->LoadAnimationAsset( "../Content/Assets/PermanentAssets/Robot/Weapons/Hammer/Animations/", "hammerAttack1.PaMan", mWeaponAnimations[HAMMER][ATTACK] ) ) )
		OutputDebugString( L"\nERROR loading player model\n" );																							
	if( FAILED( Graphics::GetInstance()->LoadAnimationAsset( "../Content/Assets/PermanentAssets/Robot/Weapons/Hammer/Animations/", "hammerAttack2.PaMan",	mWeaponAnimations[HAMMER][ATTACK2] ) ) )
		OutputDebugString( L"\nERROR loading player model\n" );

	//MINIGUN
	if( FAILED( Graphics::GetInstance()->LoadAnimationAsset( "../Content/Assets/PermanentAssets/Robot/Weapons/Minigun/Animations/", "minigunWalknIdle.PaMan",	mWeaponAnimations[MINIGUN][IDLE] ) ) )
		OutputDebugString( L"\nERROR loading player model\n" );
	else
		mWeaponAnimations[MINIGUN][WALK] = mWeaponAnimations[MINIGUN][IDLE];

	if( FAILED( Graphics::GetInstance()->LoadAnimationAsset( "../Content/Assets/PermanentAssets/Robot/Weapons/Minigun/Animations/", "minigunAttack.PaMan",	mWeaponAnimations[MINIGUN][ATTACK] ) ) )
		OutputDebugString( L"\nERROR loading player model\n" );																							

	//GRENADELAUNCHER
	if( FAILED( Graphics::GetInstance()->LoadAnimationAsset( "../Content/Assets/PermanentAssets/Robot/Weapons/Grenadelauncher/Animations/", "grenadeLauncherIdleWalk.PaMan", mWeaponAnimations[GRENADELAUNCHER][IDLE] ) ) )
		OutputDebugString( L"\nERROR loading player model\n" );																							
	else
		mWeaponAnimations[GRENADELAUNCHER][WALK] = mWeaponAnimations[GRENADELAUNCHER][IDLE];

	if( FAILED( Graphics::GetInstance()->LoadAnimationAsset( "../Content/Assets/PermanentAssets/Robot/Weapons/Grenadelauncher/Animations/", "grenadeLauncherAttack.PaMan", mWeaponAnimations[GRENADELAUNCHER][ATTACK] ) ) )
		OutputDebugString( L"\nERROR loading player model\n" );																							

	//SHOTGUN
	if( FAILED( Graphics::GetInstance()->LoadAnimationAsset( "../Content/Assets/PermanentAssets/Robot/Weapons/Shotgun/Animations/", "shotgunIdle.PaMan",	mWeaponAnimations[SHOTGUN][IDLE] ) ) )
		OutputDebugString( L"\nERROR loading player model\n" );
	if( FAILED( Graphics::GetInstance()->LoadAnimationAsset( "../Content/Assets/PermanentAssets/Robot/Weapons/Shotgun/Animations/", "shotgunWalk.PaMan",	mWeaponAnimations[SHOTGUN][WALK] ) ) )
		OutputDebugString( L"\nERROR loading player model\n" );
	if( FAILED( Graphics::GetInstance()->LoadAnimationAsset( "../Content/Assets/PermanentAssets/Robot/Weapons/Shotgun/Animations/", "shotgunAttack.PaMan",	mWeaponAnimations[SHOTGUN][ATTACK] ) ) )
		OutputDebugString( L"\nERROR loading player model\n" );

	//SNIPER
	if( FAILED( Graphics::GetInstance()->LoadAnimationAsset( "../Content/Assets/PermanentAssets/Robot/Weapons/Sniper/Animations/", "sniperWalknIdle.PaMan",	mWeaponAnimations[SNIPER][IDLE] ) ) )
		OutputDebugString( L"\nERROR loading player model\n" );
	else
		mWeaponAnimations[SNIPER][WALK] = mWeaponAnimations[SNIPER][IDLE];

	if( FAILED( Graphics::GetInstance()->LoadAnimationAsset( "../Content/Assets/PermanentAssets/Robot/Weapons/Sniper/Animations/", "sniperAnim.PaMan",	mWeaponAnimations[SNIPER][ATTACK] ) ) )
		OutputDebugString( L"\nERROR loading player model\n" );

	//////////////////////////////////////
	//			ROBOT PARTS
	/////////////////////////////////////
	AssetID model		= 0;
	AssetID skeleton	= 0;
	if( FAILED( Graphics::GetInstance()->LoadStatic3dAsset( "../Content/Assets/PermanentAssets/Robot/", "robotUpperbody.pfs", mUpperBody.playerModel ) ) )
		OutputDebugString( L"\nERROR loading player model\n" );

	if( Graphics::GetInstance()->LoadSkeletonAsset( "../Content/Assets/PermanentAssets/Robot/Animations/", "robotLegs.Skel", skeleton ) ) //Skeleton for legs
		OutputDebugString( L"\nERROR loading player model\n" );
	if( FAILED( Graphics::GetInstance()->LoadAnimated3dAsset( "../Content/Assets/PermanentAssets/Robot/", "robotLegs.apfs", skeleton, model ) ) )
		OutputDebugString( L"\nERROR loading player model\n" );

	RenderManager::GetInstance()->AnimationInitialize( mLowerBody.playerModel, model, mAnimations[PLAYER_ANIMATION::LEGS_IDLE] );

	//WEAPON MODELS
	//Blowtorch
	if( Graphics::GetInstance()->LoadSkeletonAsset( "../Content/Assets/PermanentAssets/Robot/Weapons/Blowtorch/Animations/", "blowtorchSkel.Skel", skeleton ) )
		OutputDebugString( L"\nERROR loading player model\n" );
	if( FAILED( Graphics::GetInstance()->LoadAnimated3dAsset( "../Content/Assets/PermanentAssets/Robot/Weapons/Blowtorch/", "blowtorch.apfs", skeleton, mWeaponModels[BLOWTORCH] ) ) )
		OutputDebugString( L"\nERROR loading player model\n" );

	//Claymore
	if( Graphics::GetInstance()->LoadSkeletonAsset( "../Content/Assets/PermanentAssets/Robot/Weapons/Claymore/Animations/", "claymoreSkel.Skel", skeleton ) )
		OutputDebugString( L"\nERROR loading player model\n" );
	if( FAILED( Graphics::GetInstance()->LoadAnimated3dAsset( "../Content/Assets/PermanentAssets/Robot/Weapons/Claymore/", "claymore.apfs", skeleton, mWeaponModels[CLAYMORE] ) ) )
		OutputDebugString( L"\nERROR loading player model\n" );

	//Grenadelauncher
	if( Graphics::GetInstance()->LoadSkeletonAsset( "../Content/Assets/PermanentAssets/Robot/Weapons/Grenadelauncher/Animations/", "grenadeLauncherSkel.Skel", skeleton ) )
		OutputDebugString( L"\nERROR loading player model\n" );
	if( FAILED( Graphics::GetInstance()->LoadAnimated3dAsset( "../Content/Assets/PermanentAssets/Robot/Weapons/Grenadelauncher/", "grenadeLauncher.apfs", skeleton, mWeaponModels[GRENADELAUNCHER] ) ) )
		OutputDebugString( L"\nERROR loading player model\n" );

	//Hammer
	if( Graphics::GetInstance()->LoadSkeletonAsset( "../Content/Assets/PermanentAssets/Robot/Weapons/Hammer/Animations/", "hammerSkel.Skel", skeleton ) )
		OutputDebugString( L"\nERROR loading player model\n" );
	if( FAILED( Graphics::GetInstance()->LoadAnimated3dAsset( "../Content/Assets/PermanentAssets/Robot/Weapons/Hammer/", "hammer.apfs", skeleton, mWeaponModels[HAMMER] ) ) )
		OutputDebugString( L"\nERROR loading player model\n" );

	//Minigun
	if( Graphics::GetInstance()->LoadSkeletonAsset( "../Content/Assets/PermanentAssets/Robot/Weapons/Minigun/Animations/", "minigunSkel.Skel", skeleton ) )
		OutputDebugString( L"\nERROR loading player model\n" );
	if( FAILED( Graphics::GetInstance()->LoadAnimated3dAsset( "../Content/Assets/PermanentAssets/Robot/Weapons/Minigun/", "minigun.apfs", skeleton, mWeaponModels[MINIGUN] ) ) )
		OutputDebugString( L"\nERROR loading player model\n" );

	//Saw
	if( Graphics::GetInstance()->LoadSkeletonAsset( "../Content/Assets/PermanentAssets/Robot/Weapons/Saw/Animations/", "sawSkel.Skel", skeleton ) )
		OutputDebugString( L"\nERROR loading player model\n" );
	if( FAILED( Graphics::GetInstance()->LoadAnimated3dAsset( "../Content/Assets/PermanentAssets/Robot/Weapons/Saw/", "saw.apfs", skeleton, mWeaponModels[SAW] ) ) )
		OutputDebugString( L"\nERROR loading player model\n" );

	//Shotgun
	if( Graphics::GetInstance()->LoadSkeletonAsset( "../Content/Assets/PermanentAssets/Robot/Weapons/Shotgun/Animations/", "shotgunSkel.Skel", skeleton ) )
		OutputDebugString( L"\nERROR loading player model\n" );
	if( FAILED( Graphics::GetInstance()->LoadAnimated3dAsset( "../Content/Assets/PermanentAssets/Robot/Weapons/Shotgun/", "shotgun.apfs", skeleton, mWeaponModels[SHOTGUN] ) ) )
		OutputDebugString( L"\nERROR loading player model\n" );

	//Sniper
	if( Graphics::GetInstance()->LoadSkeletonAsset( "../Content/Assets/PermanentAssets/Robot/Weapons/Sniper/Animations/", "sniperSkel.Skel", skeleton ) )
		OutputDebugString( L"\nERROR loading player model\n" );
	if( FAILED( Graphics::GetInstance()->LoadAnimated3dAsset( "../Content/Assets/PermanentAssets/Robot/Weapons/Sniper/", "sniper.apfs", skeleton, mWeaponModels[SNIPER] ) ) )
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

	mLowerBody.position						= XMFLOAT3( 3.0f, 0.0f, 0.0f );
	mNrOfDeaths								= 0;
	mNrOfKills								= 0;

	mPlayerName = "";

	mLeftArmAnimationCompleted				= false;
	mRightArmAnimationCompleted				= false;

	mBoundingBox			= new BoundingBox( 1.5f, 1.5f );
	mBoundingCircle			= new BoundingCircle( 0.5f );
	mBoundingCircleAura		= new BoundingCircle( 1.0f );
	
	mFont.Initialize( "../Content/Assets/Fonts/mv_boli_26_red/" );

	mIsAlive				= true;
	mIsDown					= false;
	mMaxHp					= 100.0f;
	mCurrentHp				= mMaxHp;
	mSpawnTime				= 10.0f;
	mDeathTime				= 10.0f;
	mReviveTime				= 3.0f;
	mTimeTillSpawn			= mSpawnTime;
	mTimeTillDeath			= mDeathTime;
	mTimeTillRevive			= mReviveTime;

	//Weapon Initialization
	mLoadOut				= new LoadOut();
	mLoadOut->rangedWeapon	= new RangedInfo( MINIGUN, 5.0f, 1, 5.0f, 2, 0 );
	mLoadOut->meleeWeapon	= new MeleeInfo( HAMMER, 4.0f, 3, 2.0f, 7, 2.0f, new BoundingCircle( 2.0f ) );

	RenderManager::GetInstance()->AnimationInitialize( mArms.leftArm, mWeaponModels[mLoadOut->meleeWeapon->weaponType], mWeaponAnimations[mLoadOut->meleeWeapon->weaponType][IDLE] );
	RenderManager::GetInstance()->AnimationInitialize( mArms.rightArm, mWeaponModels[mLoadOut->rangedWeapon->weaponType], mWeaponAnimations[mLoadOut->rangedWeapon->weaponType][IDLE] );

	return S_OK;
}

void RemotePlayer::Release()
{
	mLoadOut->Release();
	SAFE_DELETE( mLoadOut );

	SAFE_DELETE( mBoundingBox );
	SAFE_DELETE( mBoundingCircle );
	SAFE_DELETE( mBoundingCircleAura );
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

BoundingCircle*	RemotePlayer::GetBoundingCircleAura() const
{
	return mBoundingCircleAura;
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
	//mUpperBody.position		= XMFLOAT3( 0.0f, 0.0f, 0.0f );
	mUpperBody.direction	= XMFLOAT3( 0.0f, 0.0f, 0.0f );

	mLowerBody.position		= XMFLOAT3( 0.0f, 0.0f, 0.0f );
	mLowerBody.direction	= XMFLOAT3( 0.0f, 0.0f, 0.0f );

	mIsAlive				= true;
	mMaxHp					= 0.0f;
	mCurrentHp				= 0.0f;
	mSpawnTime				= 0.0f;
	mTimeTillSpawn			= 0.0f;
	mDeathTime				= 0.0f;
	mTimeTillDeath			= 0.0f;
	mReviveTime				= 0.0f;
	mTimeTillRevive			= 0.0f;
	mLastKiller				= 0;

	mBoundingBox			= nullptr;
	mBoundingCircle			= nullptr;
	mBoundingCircleAura		= nullptr;
	mLoadOut				= nullptr;
	mPointLightIfDown		= nullptr;
}

RemotePlayer::~RemotePlayer()
{}

void RemotePlayer::TakeDamage( float damage )
{
	mCurrentHp -= damage;
}
