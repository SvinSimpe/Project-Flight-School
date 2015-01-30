#include "EnemyAnimationManager.h"

AssetID EnemyAnimationManager::GetAnimation( EnemyType enemy, EnemyState state ) const
{
	switch( enemy )
	{
		case Standard:
		{
			if( state == Idle )			return mAnimations[ENEMY_ANIMATION_STANDARD_IDLE];			
			else if( state == Run )		return mAnimations[ENEMY_ANIMATION_STANDARD_RUN];
			else if( state == Attack )	return mAnimations[ENEMY_ANIMATION_STANDARD_ATTACK];
			else if( state == Death  )	return mAnimations[ENEMY_ANIMATION_STANDARD_DEATH];
			else OutputDebugStringA( "--Error getting Standard animation" );

		}
		break;
		case Ranged:
		{
			if( state == Idle )			return mAnimations[ENEMY_ANIMATION_RANGED_IDLE];			
			else if( state == Run )		return mAnimations[ENEMY_ANIMATION_RANGED_RUN];
			else if( state == Attack )	return mAnimations[ENEMY_ANIMATION_RANGED_ATTACK];
			else if( state == Death  )	return mAnimations[ENEMY_ANIMATION_RANGED_DEATH];
			else OutputDebugStringA( "--Error getting Ranged animation" );
		}
		break;
		case Boomer:
		{
			if( state == Idle )			return mAnimations[ENEMY_ANIMATION_BOOMER_IDLE];			
			else if( state == Run )		return mAnimations[ENEMY_ANIMATION_BOOMER_RUN];
			else if( state == Attack )	return mAnimations[ENEMY_ANIMATION_BOOMER_ATTACK];
			else if( state == Death  )	return mAnimations[ENEMY_ANIMATION_BOOMER_DEATH];
			else OutputDebugStringA( "--Error getting Boomer animation" );
		}
		break;
		case Tank:
		{
			if( state == Idle )			return mAnimations[ENEMY_ANIMATION_TANK_IDLE];			
			else if( state == Run )		return mAnimations[ENEMY_ANIMATION_TANK_RUN];
			else if( state == Attack )	return mAnimations[ENEMY_ANIMATION_TANK_ATTACK];
			else if( state == Death  )	return mAnimations[ENEMY_ANIMATION_TANK_DEATH];
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
	Graphics::GetInstance()->LoadSkeletonAsset( "../Content/Assets/Raptor/Animations/", "raptor.Skel", skeleton );
	// Load animated 3d asset
	Graphics::GetInstance()->LoadAnimated3dAsset( "../Content/Assets/Raptor/", "scaledScene.apfs", skeleton, mModelStandard );

	// -------------------
	// Load Ranged Model
	// Load skeleton
	Graphics::GetInstance()->LoadSkeletonAsset( "../Content/Assets/Goblin/animations/", "goblinSkel.Skel", skeleton );
	// Load animated 3d asset
	Graphics::GetInstance()->LoadAnimated3dAsset( "../Content/Assets/Goblin/", "goblin.apfs", skeleton, mModelRanged );

	// -------------------
	// Load Boomer Model
	// Load skeleton
	Graphics::GetInstance()->LoadSkeletonAsset( "../Content/Assets/Blowuposaur/Animations/", "blowuposaurSkel.Skel", skeleton );
	// Load animated 3d asset
	Graphics::GetInstance()->LoadAnimated3dAsset( "../Content/Assets/Blowuposaur/", "blowuposaur.apfs", skeleton, mModelBoomer );

	// ------------------- ( Pate Waiting )
	// Load Tank Model
	// Load skeleton
	//Graphics::GetInstance()->LoadSkeletonAsset( "../Content/Assets/Raptor/Animations/", "raptor.Skel", skeleton );
	// Load animated 3d asset
	//Graphics::GetInstance()->LoadAnimated3dAsset( "../Content/Assets/Raptor/", "scaledScene.apfs", skeleton, mModelStandard );

	// ----------------
	// LOAD ANMIMATIONS
	// ----------------

	// -------------------
	// Load Standard Animations
	Graphics::GetInstance()->LoadAnimationAsset( "../Content/Assets/Raptor/Animations/", "raptor_idle.PaMan",	mAnimations[ENEMY_ANIMATION_STANDARD_IDLE] );
	Graphics::GetInstance()->LoadAnimationAsset( "../Content/Assets/Raptor/Animations/", "raptor_run.PaMan",	mAnimations[ENEMY_ANIMATION_STANDARD_RUN] );
	Graphics::GetInstance()->LoadAnimationAsset( "../Content/Assets/Raptor/Animations/", "raptor_attack.PaMan", mAnimations[ENEMY_ANIMATION_STANDARD_ATTACK] );
	Graphics::GetInstance()->LoadAnimationAsset( "../Content/Assets/Raptor/Animations/", "raptor_death.PaMan",	mAnimations[ENEMY_ANIMATION_STANDARD_DEATH] );

	// -------------------
	// Load Ranged Animations
	Graphics::GetInstance()->LoadAnimationAsset( "../Content/Assets/Goblin/animations/", "goblinIdle.PaMan",	mAnimations[ENEMY_ANIMATION_RANGED_IDLE] );
	Graphics::GetInstance()->LoadAnimationAsset( "../Content/Assets/Goblin/animations/", "goblinWalk.PaMan",	mAnimations[ENEMY_ANIMATION_RANGED_RUN] );
	Graphics::GetInstance()->LoadAnimationAsset( "../Content/Assets/Goblin/animations/", "goblinAttack.PaMan",	mAnimations[ENEMY_ANIMATION_RANGED_ATTACK] );
	Graphics::GetInstance()->LoadAnimationAsset( "../Content/Assets/Goblin/animations/", "goblinDeath.PaMan",	mAnimations[ENEMY_ANIMATION_RANGED_DEATH] );

	// -------------------
	// Load Boomer Animations
	Graphics::GetInstance()->LoadAnimationAsset( "../Content/Assets/Blowuposaur/Animations/", "blowuposaurIdle2.PaMan",		mAnimations[ENEMY_ANIMATION_BOOMER_IDLE] );
	Graphics::GetInstance()->LoadAnimationAsset( "../Content/Assets/Blowuposaur/Animations/", "blowuposaurrUN.PaMan",		mAnimations[ENEMY_ANIMATION_BOOMER_RUN] );
	Graphics::GetInstance()->LoadAnimationAsset( "../Content/Assets/Blowuposaur/Animations/", "blowuposaurAttack.PaMan",	mAnimations[ENEMY_ANIMATION_BOOMER_ATTACK] );
	Graphics::GetInstance()->LoadAnimationAsset( "../Content/Assets/Blowuposaur/Animations/", "blowuposaurDeath.PaMan",		mAnimations[ENEMY_ANIMATION_BOOMER_DEATH] );

	// -------------------
	// Load Tank Animations
	// Pate Waiting
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