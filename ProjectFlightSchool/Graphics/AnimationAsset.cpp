#include "AnimationAsset.h"

HRESULT	AnimationAsset::Initialize()
{
	return S_OK;
}

void AnimationAsset::Release()
{

	mAnimationData.joints.clear();
}

AnimationAsset::AnimationAsset()
{

}

AnimationAsset::~AnimationAsset()
{

}