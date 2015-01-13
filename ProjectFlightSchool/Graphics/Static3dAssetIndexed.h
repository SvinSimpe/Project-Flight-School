#ifndef STATIC3DASSETINDEXED_H
#define STATIC3DASSETINDEXED_H

#include "Static3dAsset.h"

struct Static3dAssetIndexed : public Static3dAsset
{
	private:
	protected:
	public:
		UINT				mIndexCount;
		ID3D11Buffer*		mIndexBuffer;

	private:
	protected:
	public:
		HRESULT		Initialize();
		void		Release();
					Static3dAssetIndexed();
		virtual		~Static3dAssetIndexed();
};

#endif