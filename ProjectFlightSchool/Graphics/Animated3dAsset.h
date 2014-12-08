#ifndef ANIMATED3DASSET_H
#define ANIMATED3DASSET_H

#include "AssetBase.h"

struct Animated3dAsset : public AssetBase
{
	private:
	protected:
	public:
		UINT				mVertexCount;
		ID3D11Buffer*		mVertexBuffer;
		AssetID				mSkeleton;
		AssetID				mAnimation;

		DirectX::XMFLOAT4X4	mCurrentBoneTransforms[16];
	
		int					mCurrentFrame;
		float				mRealValue;

	private:
	protected:
	public:
		void		ResetAnimation();
		void		UpdateAnimation( float deltaTime );

		HRESULT		Initialize();
		void		Release();
					Animated3dAsset();
		virtual		~Animated3dAsset();
};

#endif