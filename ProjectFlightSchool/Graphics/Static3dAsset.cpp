#include "Static3dAsset.h"	

HRESULT Static3dAsset::Initialize()
{
	return S_OK;
}

void Static3dAsset::Release()
{
	BoxGenerator killer;
	killer.Release( mOctTree );
	for( int i = 0; i < (int)mMeshes.size(); i++ )
		SAFE_RELEASE( mMeshes[i].mVertexBuffer );
	mMeshes.clear();
}

Static3dAsset::Static3dAsset() : AssetBase()
{
}

Static3dAsset::~Static3dAsset()
{
}