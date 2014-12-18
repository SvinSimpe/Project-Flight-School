#include "Animated3dAsset.h"


		UINT				mVertexCount;
		ID3D11Buffer*		mVertexBuffer;
		AssetID				mSkeleton;
		AssetID				mAnimation;

		DirectX::XMFLOAT4X4	boneOffsets[16];
	
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
	SAFE_RELEASE( mVertexBuffer );
}

Animated3dAsset::Animated3dAsset()
{
	mVertexCount	= 0;
	mVertexBuffer	= nullptr;
	mSkeleton		= 0;
	mAnimation		= 0;

	for( int i = 0; i < NUM_SUPPORTED_JOINTS; i++ )
		mBoneOffsets[i] = DirectX::XMFLOAT4X4(	0.0f, 0.0f, 0.0f, 0.0f,
												0.0f, 0.0f, 0.0f, 0.0f,
												0.0f, 0.0f, 0.0f, 0.0f,
												0.0f, 0.0f, 0.0f, 0.0f );
	
	mCurrentFrame	= 0;
	mRealValue		= 0.0f;
}

Animated3dAsset::~Animated3dAsset()
{

}