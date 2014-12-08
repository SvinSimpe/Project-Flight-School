#ifndef STATIC3DASSET_H
#define STATIC3DASSET_H

#include "AssetBase.h"

struct Static3dAsset : public AssetBase
{
	private:
	protected:
	public:
		UINT				mVertexCount;
		ID3D11Buffer*		mVertexBuffer;

	private:
	protected:
	public:
		HRESULT		Initialize();
		void		Release();
					Static3dAsset();
		virtual		~Static3dAsset();
};

#endif