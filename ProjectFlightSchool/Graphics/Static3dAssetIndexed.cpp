#include "Static3dAssetIndexed.h"

HRESULT Static3dAssetIndexed::Initialize()
{
	return S_OK;
}

void Static3dAssetIndexed::Release()
{
	SAFE_RELEASE( mIndexBuffer );
	SAFE_RELEASE( mVertexBuffer );
}

Static3dAssetIndexed::Static3dAssetIndexed() : Static3dAsset()
{
	mIndexCount	= 0;
	mIndexBuffer	= nullptr;
}

Static3dAssetIndexed::~Static3dAssetIndexed()
{
}