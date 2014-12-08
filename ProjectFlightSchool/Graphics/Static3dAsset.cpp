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

}

Static3dAsset::~Static3dAsset()
{
	mVertexCount	= 0;
	mVertexBuffer	= nullptr;
}