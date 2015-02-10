#ifndef ANIMATED3DASSET_H
#define ANIMATED3DASSET_H

#include "AssetBase.h"

#define NUM_SUPPORTED_JOINTS 32

#define	TEXTURES_DIFFUSE	0
#define	TEXTURES_NORMAL		1
#define	TEXTURES_SPECULAR	2

//Add new items above this comment
#define	TEXTURES_AMOUNT 3


struct Animated3dAsset : public AssetBase
{
	private:
	protected:
	public:
		UINT				mVertexCount;
		ID3D11Buffer*		mVertexBuffer;
		AssetID				mSkeletonId;
		AssetID				mTextures[TEXTURES_AMOUNT];		
		DirectX::XMFLOAT4X4	mBoneOffsets[NUM_SUPPORTED_JOINTS];

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