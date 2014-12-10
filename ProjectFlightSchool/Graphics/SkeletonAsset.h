#ifndef SKELETONASSET_H
#define SKELETONASSET_H

#include "AssetBase.h"

struct SkeletonAsset : public AssetBase
{
	private:
	protected:
	public:
		Skeleton	mSkeleton;

	private:
	protected:
	public:
		HRESULT		Initialize();
		void		Release();
					SkeletonAsset();
		virtual		~SkeletonAsset();
};
#endif