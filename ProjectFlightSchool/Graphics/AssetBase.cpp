#include "AssetBase.h"
std::string AssetBase::GetFileName()
{
	return mFileName;
}

HRESULT	AssetBase::Initialize()
{
	return S_OK;
}

void AssetBase::Release()
{
	SAFE_RELEASE( mVertexBuffer );
	SAFE_RELEASE( mIndexBUffer );
}

AssetBase::AssetBase()
{
	mAssetId		= 0;
	mFileName		= "";
	mVertexBuffer	= nullptr;
	mIndexBUffer	= nullptr;
}

AssetBase::~AssetBase()
{
	
}