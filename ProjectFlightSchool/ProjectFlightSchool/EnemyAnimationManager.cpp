#include "EnemyAnimationManager.h"

AssetID EnemyAnimationManager::GetAnimation( EnemyType enemy, EnemyState state ) const
{
	switch( enemy )
	{
		case Standard:
		{
			if( state == MoveToShip || 
				state == HuntPlayer )		return mAnimations[ENEMY_ANIMATION_STANDARD_RUN];
			else if( state == TakeDamage )	return mAnimations[ENEMY_ANIMATION_STANDARD_RUN_DAMAGED];
			else if( state == Idle ||
				state == Stunned )			return mAnimations[ENEMY_ANIMATION_STANDARD_IDLE];			
			else if( state == Attack )		return mAnimations[ENEMY_ANIMATION_STANDARD_ATTACK];
			else if( state == Death  )		return mAnimations[ENEMY_ANIMATION_STANDARD_DEATH];
			else OutputDebugStringA( "--Error getting Standard animation" );

		}
		break;
		case Ranged:
		{
			if( state == MoveToShip || 
				state == HuntPlayer )		return mAnimations[ENEMY_ANIMATION_RANGED_RUN];
			else if( state == TakeDamage )	return mAnimations[ENEMY_ANIMATION_RANGED_RUN_DAMAGED];
			else if( state == Idle ||
				state == Stunned )			return mAnimations[ENEMY_ANIMATION_RANGED_IDLE];	
			else if( state == Attack )		return mAnimations[ENEMY_ANIMATION_RANGED_ATTACK];
			else if( state == Death  )		return mAnimations[ENEMY_ANIMATION_RANGED_DEATH];
			else OutputDebugStringA( "--Error getting Ranged animation" );
		}
		break;
		case Boomer:
		{
			if( state == MoveToShip || 
				state == HuntPlayer )		return mAnimations[ENEMY_ANIMATION_BOOMER_RUN];
			else if( state == TakeDamage )	return mAnimations[ENEMY_ANIMATION_BOOMER_RUN_DAMAGED];
			else if( state == Idle ||
				state == Stunned )			return mAnimations[ENEMY_ANIMATION_BOOMER_IDLE];	
			else if( state == Attack )		return mAnimations[ENEMY_ANIMATION_BOOMER_ATTACK];
			else if( state == Death  )		return mAnimations[ENEMY_ANIMATION_BOOMER_DEATH];
			else OutputDebugStringA( "--Error getting Boomer animation" );
		}
		break;
		case Tank:
		{
			if( state == MoveToShip || 
				state == HuntPlayer )		return mAnimations[ENEMY_ANIMATION_TANK_RUN];
			else if( state == TakeDamage )	return mAnimations[ENEMY_ANIMATION_TANK_RUN];
			else if( state == Idle ||
				state == Stunned )			return mAnimations[ENEMY_ANIMATION_TANK_IDLE];	
			else if( state == Attack )		return mAnimations[ENEMY_ANIMATION_TANK_ATTACK];
			else if( state == Death  )		return mAnimations[ENEMY_ANIMATION_TANK_DEATH];
			else OutputDebugStringA( "--Error getting Tank animation" );
		}
		break;
		default:
			OutputDebugStringA( "--Error getting animation " );
	}

	return 0;
}

AssetID EnemyAnimationManager::GetDefaultAnimation( EnemyType enemy ) const
{
	if( enemy == Standard )		return mAnimations[ENEMY_ANIMATION_STANDARD_IDLE];
	else if( enemy == Ranged )	return mAnimations[ENEMY_ANIMATION_RANGED_IDLE];
	else if( enemy == Boomer )	return mAnimations[ENEMY_ANIMATION_BOOMER_IDLE];
	else if( enemy == Tank )	return mAnimations[ENEMY_ANIMATION_TANK_IDLE];

	return 0;
}

AssetID EnemyAnimationManager::GetModel( EnemyType enemy ) const
{
	if( enemy == Standard )		return mModelStandard;
	else if( enemy == Ranged )	return mModelRanged;
	else if( enemy == Boomer )	return mModelBoomer;
	else if( enemy == Tank )	return mModelTank;

	return 0;
}

HRESULT EnemyAnimationManager::Initialize()
{
	// ----------------
	//	 LOAD MODELS
	// ----------------

	AssetID skeleton	= 0;

	// -------------------
	// Load Standard Model
	// Load skeleton
	Graphics::GetInstance()->LoadSkeletonAsset( "../Content/Assets/ForestAssets/Enemies/Raptor/Animations/", "raptor.Skel", skeleton );
	// Load animated 3d asset
	Graphics::GetInstance()->LoadAnimated3dAsset( "../Content/Assets/ForestAssets/Enemies/Raptor/", "scaledScene.apfs", skeleton, mModelStandard );

	// -------------------
	// Load Ranged Model
	// Load skeleton
	Graphics::GetInstance()->LoadSkeletonAsset( "../Content/Assets/ForestAssets/Enemies/Goblin/Animations/", "goblinSkel.Skel", skeleton );
	// Load animated 3d asset
	Graphics::GetInstance()->LoadAnimated3dAsset( "../Content/Assets/ForestAssets/Enemies/Goblin/", "goblin.apfs", skeleton, mModelRanged );

	// -------------------
	// Load Boomer Model
	// Load skeleton
	Graphics::GetInstance()->LoadSkeletonAsset( "../Content/Assets/ForestAssets/Enemies/Blowuposaur/Animations/", "blowuposaurSkel.Skel", skeleton );
	// Load animated 3d asset
	Graphics::GetInstance()->LoadAnimated3dAsset( "../Content/Assets/ForestAssets/Enemies/Blowuposaur/", "blowuposaur.apfs", skeleton, mModelBoomer );

	// ------------------- ( Pate Waiting )
	// Load Tank Model
	// Load skeleton
	Graphics::GetInstance()->LoadSkeletonAsset( "../Content/Assets/ForestAssets/Enemies/Tank/Animations/", "tankSkel.Skel", skeleton );
	// Load animated 3d asset
	Graphics::GetInstance()->LoadAnimated3dAsset( "../Content/Assets/ForestAssets/Enemies/Tank/", "EnemyTank.apfs", skeleton, mModelTank );

	// ----------------
	// LOAD ANMIMATIONS
	// ----------------

	// -------------------
	// Load Standard Animations
	Graphics::GetInstance()->LoadAnimationAsset( "../Content/Assets/ForestAssets/Enemies/Raptor/Animations/", "raptorIdle.PaMan",				mAnimations[ENEMY_ANIMATION_STANDARD_IDLE] );
	Graphics::GetInstance()->LoadAnimationAsset( "../Content/Assets/ForestAssets/Enemies/Raptor/Animations/", "raptorRun.PaMan",				mAnimations[ENEMY_ANIMATION_STANDARD_RUN] );
	Graphics::GetInstance()->LoadAnimationAsset( "../Content/Assets/ForestAssets/Enemies/Raptor/Animations/", "raptorRunDamaged.PaMan",			mAnimations[ENEMY_ANIMATION_STANDARD_RUN_DAMAGED] );
	Graphics::GetInstance()->LoadAnimationAsset( "../Content/Assets/ForestAssets/Enemies/Raptor/Animations/", "raptorAttack1.PaMan",			mAnimations[ENEMY_ANIMATION_STANDARD_ATTACK] );
	Graphics::GetInstance()->LoadAnimationAsset( "../Content/Assets/ForestAssets/Enemies/Raptor/Animations/", "raptorDeath2(fix(fix)).PaMan",	mAnimations[ENEMY_ANIMATION_STANDARD_DEATH] );

	// -------------------
	// Load Ranged Animations
	Graphics::GetInstance()->LoadAnimationAsset( "../Content/Assets/ForestAssets/Enemies/Goblin/Animations/", "goblinIdle.PaMan",			mAnimations[ENEMY_ANIMATION_RANGED_IDLE] );
	Graphics::GetInstance()->LoadAnimationAsset( "../Content/Assets/ForestAssets/Enemies/Goblin/Animations/", "goblinWalk.PaMan",			mAnimations[ENEMY_ANIMATION_RANGED_RUN] );
	Graphics::GetInstance()->LoadAnimationAsset( "../Content/Assets/ForestAssets/Enemies/Goblin/Animations/", "goblinWalkDamaged.PaMan",	mAnimations[ENEMY_ANIMATION_RANGED_RUN_DAMAGED] );
	Graphics::GetInstance()->LoadAnimationAsset( "../Content/Assets/ForestAssets/Enemies/Goblin/Animations/", "goblinAttack.PaMan",			mAnimations[ENEMY_ANIMATION_RANGED_ATTACK] );
	Graphics::GetInstance()->LoadAnimationAsset( "../Content/Assets/ForestAssets/Enemies/Goblin/Animations/", "goblinDeath.PaMan",			mAnimations[ENEMY_ANIMATION_RANGED_DEATH] );

	// -------------------
	// Load Boomer Animations
	Graphics::GetInstance()->LoadAnimationAsset( "../Content/Assets/ForestAssets/Enemies/Blowuposaur/Animations/", "blowuposaurIdle.PaMan",			mAnimations[ENEMY_ANIMATION_BOOMER_IDLE] );
	Graphics::GetInstance()->LoadAnimationAsset( "../Content/Assets/ForestAssets/Enemies/Blowuposaur/Animations/", "blowuposaurWalkDamaged.PaMan",	mAnimations[ENEMY_ANIMATION_BOOMER_RUN] );
	Graphics::GetInstance()->LoadAnimationAsset( "../Content/Assets/ForestAssets/Enemies/Blowuposaur/Animations/", "blowuposaurWalk.PaMan",			mAnimations[ENEMY_ANIMATION_BOOMER_RUN_DAMAGED] );
	Graphics::GetInstance()->LoadAnimationAsset( "../Content/Assets/ForestAssets/Enemies/Blowuposaur/Animations/", "blowuposaurAttack.PaMan",		mAnimations[ENEMY_ANIMATION_BOOMER_ATTACK] );
	Graphics::GetInstance()->LoadAnimationAsset( "../Content/Assets/ForestAssets/Enemies/Blowuposaur/Animations/", "blowuposaurDeath.PaMan",		mAnimations[ENEMY_ANIMATION_BOOMER_DEATH] );

	// -------------------
	// Load Tank Animations
	// Pate Waiting
	Graphics::GetInstance()->LoadAnimationAsset( "../Content/Assets/ForestAssets/Enemies/Tank/Animations/", "tankIdle.PaMan",		mAnimations[ENEMY_ANIMATION_TANK_IDLE] );
	Graphics::GetInstance()->LoadAnimationAsset( "../Content/Assets/ForestAssets/Enemies/Tank/Animations/", "tankWalk.PaMan",		mAnimations[ENEMY_ANIMATION_TANK_RUN] );
	Graphics::GetInstance()->LoadAnimationAsset( "../Content/Assets/ForestAssets/Enemies/Tank/Animations/", "tankAttack.PaMan",		mAnimations[ENEMY_ANIMATION_TANK_ATTACK] );
	Graphics::GetInstance()->LoadAnimationAsset( "../Content/Assets/ForestAssets/Enemies/Tank/Animations/", "tankDeath.PaMan",		mAnimations[ENEMY_ANIMATION_TANK_DEATH] );
	return S_OK;
}

void EnemyAnimationManager::Release()
{
}

EnemyAnimationManager::EnemyAnimationManager()
{
	mModelStandard	= 0;
	mModelRanged	= 0;
	mModelBoomer	= 0;
	mModelTank		= 0;
}

EnemyAnimationManager::~EnemyAnimationManager()
{
}