#include "Animated3dAsset.h"

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
	mSkeletonId		= 0;

	for( int i = 0; i < NUM_SUPPORTED_JOINTS; i++ )
		mBoneOffsets[i] = DirectX::XMFLOAT4X4(	0.0f, 0.0f, 0.0f, 0.0f,
												0.0f, 0.0f, 0.0f, 0.0f,
												0.0f, 0.0f, 0.0f, 0.0f,
												0.0f, 0.0f, 0.0f, 0.0f );
}

Animated3dAsset::~Animated3dAsset()
{

}