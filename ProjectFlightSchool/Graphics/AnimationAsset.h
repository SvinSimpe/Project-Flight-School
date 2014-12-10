#ifndef ANIMATIONASSET_H
#define ANIMATIONASSET_H

#include "AssetBase.h"

struct AnimationAsset : public AssetBase
{
	private:
	protected:
	public:
		AnimationData mAnimationData;

	private:
	protected:
	public:
		HRESULT		Initialize();
		void		Release();
					AnimationAsset();
		virtual		~AnimationAsset();
};

#endif