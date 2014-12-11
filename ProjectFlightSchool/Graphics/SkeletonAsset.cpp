#include "SkeletonAsset.h"	

HRESULT	SkeletonAsset::Initialize()
{
	return S_OK;
}

void SkeletonAsset::Release()
{
	mSkeleton.joints.clear();
}

SkeletonAsset::SkeletonAsset()
{

}

SkeletonAsset::~SkeletonAsset()
{
}