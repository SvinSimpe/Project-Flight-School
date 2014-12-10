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
}

AssetBase::AssetBase()
{
	mAssetId		= 0;
	mFileName		= "";
}

AssetBase::~AssetBase()
{
	
}