#ifndef STATIC3DASSET_H
#define STATIC3DASSET_H

#include "AssetBase.h"

enum TEXTURES
{
	TEXTURES_DIFFUSE,
	TEXTURES_NORMAL,
	TEXTURES_SPECULAR,


	//Add new items above this comment
	TEXTURES_AMOUNT
};

struct Static3dAsset : public AssetBase
{
	private:
	protected:
	public:
		UINT			mVertexCount;
		ID3D11Buffer*	mVertexBuffer;
		AssetID			mTextures[TEXTURES_AMOUNT];						

	private:
	protected:
	public:
		HRESULT		Initialize();
		void		Release();
					Static3dAsset();
		virtual		~Static3dAsset();
};

#endif