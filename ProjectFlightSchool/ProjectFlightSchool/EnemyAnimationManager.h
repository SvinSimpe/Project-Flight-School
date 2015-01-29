#ifndef ENEMYANIMATIONMANAGER_H
#define ENEMYANIMATIONMANAGER_H

#include "Graphics.h"
#include "RenderManager.h"
#include "Enemy.h"

class EnemyAnimationManager
{
	// Member variables
	private:
		AssetID				mModelStandard;
		AssetID				mModelRanged;
		AssetID				mModelBoomer;
		AssetID				mModelTank;
		AssetID				mAnimations[ENEMY_ANIMATION_COUNT];

	protected:
	public:

	// Member functions
	private:
	protected:
	public:
};

#endif