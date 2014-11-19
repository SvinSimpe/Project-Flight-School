#ifndef ASSETMANAGER_H
#define ASSETMANAGER_H

#include "AssetBase.h"
#include "Static3dAsset.h"
#include "Vertex.h"

class AssetManager
{
	private:
		AssetBase*	mTestAsset;

	protected:
	public:

	private:
	protected:
	public:
		HRESULT		LoadStatic3dAsset( ID3D11Device* device, char* fileName, UINT &assetId );

		HRESULT		Initialize();
		void		Release();
					AssetManager();
		virtual		~AssetManager();

};
#endif