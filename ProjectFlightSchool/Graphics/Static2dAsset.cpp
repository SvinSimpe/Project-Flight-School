#include "Static2dAsset.h"	

HRESULT Static2dAsset::Initialize()
{
	return S_OK;
}

void Static2dAsset::Release()
{
	SAFE_RELEASE( mSRV );	
}

Static2dAsset::Static2dAsset() : AssetBase()
{
	mSRV = nullptr;
}

Static2dAsset::~Static2dAsset()
{

}