#include "Animated3dAsset.h"


		UINT				mVertexCount;
		ID3D11Buffer*		mVertexBuffer;
		AssetID				mSkeleton;
		AssetID				mAnimation;

		DirectX::XMFLOAT4X4	mCurrentBoneTransforms[16];
	
		int					mCurrentFrame;
		float				mRealValue;

void Animated3dAsset::ResetAnimation()
{

}

void Animated3dAsset::UpdateAnimation( float deltaTime )
{

}

HRESULT Animated3dAsset::Initialize()
{
	return S_OK;
}

void Animated3dAsset::Release()
{

}

Animated3dAsset::Animated3dAsset()
{

}

Animated3dAsset::~Animated3dAsset()
{

}