#include "Static3dAsset.h"	

HRESULT Static3dAsset::Initialize()
{
	return S_OK;
}

void Static3dAsset::Release()
{
	SAFE_RELEASE( mVertexBuffer );
}

Static3dAsset::Static3dAsset() : AssetBase()
{
	mVertexCount	= 0;
	mVertexBuffer	= nullptr;
}

Static3dAsset::~Static3dAsset()
{
}