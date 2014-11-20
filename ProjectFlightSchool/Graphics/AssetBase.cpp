#include "AssetBase.h"

HRESULT	AssetBase::Initialize()
{
	return S_OK;
}

void AssetBase::Release()
{

}

AssetBase::AssetBase()
{
	mAssetId		= 0;

	mVertexBuffer	= nullptr;
	mIndexBUffer	= nullptr;
}

AssetBase::~AssetBase()
{
	SAFE_RELEASE( mVertexBuffer );
	SAFE_RELEASE( mIndexBUffer );
}