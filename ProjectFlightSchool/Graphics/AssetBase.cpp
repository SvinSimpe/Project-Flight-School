#include "AssetBase.h"
std::string AssetBase::GetFileName()
{
	return mFileName;
}

UINT AssetBase::GetVertexCount()
{
	return mVertexCount;
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
	mVertexCount	= 0;
	mVertexBuffer	= nullptr;
	mIndexBUffer	= nullptr;
}

AssetBase::~AssetBase()
{
	
}