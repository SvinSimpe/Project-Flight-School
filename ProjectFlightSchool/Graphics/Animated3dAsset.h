#ifndef ANIMATED3DASSET_H
#define ANIMATED3DASSET_H

#include "AssetBase.h"

#define NUM_SUPPORTED_JOINTS 16

struct Animated3dAsset : public AssetBase
{
	private:
	protected:
	public:
		UINT				mVertexCount;
		ID3D11Buffer*		mVertexBuffer;
		AssetID				mSkeletonId;

		DirectX::XMFLOAT4X4	mCurrentBoneTransforms[NUM_SUPPORTED_JOINTS];

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