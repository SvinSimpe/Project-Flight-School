#ifndef STATIC2DASSET_H
#define STATIC2DASSET_H

#include "AssetBase.h"

class Static2dAsset : public AssetBase
{

	private:
	protected:
	public:
		ID3D11ShaderResourceView* mSRV;
		
	private:
	protected:
	public:
		HRESULT		Initialize();
		void		Release();
					Static2dAsset();
		virtual		~Static2dAsset();
};

#endif