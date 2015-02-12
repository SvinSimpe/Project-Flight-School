#ifndef STATIC3DASSET_H
#define STATIC3DASSET_H

#include "AssetBase.h"
#include "AABBGenerator.h"

#define	TEXTURES_DIFFUSE 0
#define	TEXTURES_NORMAL 1
#define	TEXTURES_SPECULAR 2

//Add new items above this comment
#define	TEXTURES_AMOUNT 3

struct Mesh
{
	UINT			mVertexCount;
	ID3D11Buffer*	mVertexBuffer;
	AssetID			mTextures[TEXTURES_AMOUNT];		
};

struct Static3dAsset : public AssetBase
{
	private:
	protected:
	public:
		std::vector<Mesh> mMeshes;
		AABB mAssetAABB;

	private:
	protected:
	public:
		HRESULT		Initialize();
		void		Release();
					Static3dAsset();
		virtual		~Static3dAsset();
};

#endif