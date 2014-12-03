#ifndef ASSETMANAGER_H
#define ASSETMANAGER_H

#include "AssetBase.h"
#include "Static3dAsset.h"
#include "Vertex.h"
#include "ImporterAnim.h"
#include "MapPathImportHandler.h"
#include <vector>
#include <fstream>

struct MeshInfo
{
	UINT vertexCount;

	char meshName[50];
	char diffuseMapName[50];
	char normalMapName[50];
	char specularMapName[50];
};

struct MeshData //MeshData_Export
{
	MeshInfo meshInfo;
	Vertex* vertices;
};

class AssetManager
{
	private:
		UINT mAssetIdCounter;
	protected:
	public:
		std::vector<AssetBase*> mAssetContainer;

	private:		
		bool		AssetExist( char* fileName, UINT &assetId );	//Returns true and assigns the correct id to assetId if the asset exist.	
		void		AssignAssetId( UINT &assetId );					//Assigns the asset an id and increase the mAssetIdCounter.
		HRESULT		PlaceholderAssets( ID3D11Device* device );
	protected:
	public:
		HRESULT		LoadStatic3dAsset( ID3D11Device* device, char* fileName, UINT &assetId );

		HRESULT		Initialize( ID3D11Device* device );
		void		Release();
					AssetManager();
		virtual		~AssetManager();

};
#endif