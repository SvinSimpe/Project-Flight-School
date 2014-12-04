#ifndef ASSETMANAGER_H
#define ASSETMANAGER_H

#include "AssetBase.h"
#include "Static3dAsset.h"
#include "AnimationAsset.h"
#include "Vertex.h"
//#include "ImporterAnim.h"
//#include "MapPathImportHandler.h"
#include <vector>
#include <fstream>
#include <iostream>

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
		AssetID mAssetIdCounter;
	protected:
	public:
		std::vector<AssetBase*> mAssetContainer;
		AnimationAsset			mTestAnim;

	private:		
		bool		AssetExist( char* fileName, AssetID &assetId );	//Returns true and assigns the correct id to assetId if the asset exist.	
		void		AssignAssetId( AssetID &assetId );					//Assigns the asset an id and increase the mAssetIdCounter.
		HRESULT		PlaceholderAssets( ID3D11Device* device );
	protected:
	public:
		HRESULT			LoadStatic3dAsset( ID3D11Device* device, char* fileName, AssetID &assetId );

		AnimationData	ImportBinaryAnimData( string directoryPath, string fileName );
		Skeleton		ImportBinarySkelData( string directoryPath, string fileName );

		HRESULT		Initialize( ID3D11Device* device );
		void		Release();
					AssetManager();
		virtual		~AssetManager();

};
#endif