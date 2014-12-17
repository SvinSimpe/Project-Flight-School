#include "Static3dAsset.h"	

HRESULT Static3dAsset::Initialize()
{
	return S_OK;
}

void Static3dAsset::Release()
{
	SAFE_RELEASE( mVertexBuffer );

	for( UINT i = 0; i < TEXTURES_AMOUNT; i++ )
	{
		SAFE_RELEASE( mTextureBuffer[i] );
	}
}

Static3dAsset::Static3dAsset() : AssetBase()
{
	mVertexCount	= 0;
	mVertexBuffer	= nullptr;

	for( UINT i = 0; i < TEXTURES_AMOUNT; i++ )
	{
		mTextureBuffer[i] = nullptr;
	}
}

Static3dAsset::~Static3dAsset()
{
}