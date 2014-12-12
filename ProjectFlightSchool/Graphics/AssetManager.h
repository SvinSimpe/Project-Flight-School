#ifndef ASSETMANAGER_H
#define ASSETMANAGER_H

#include "Static3dAsset.h"
#include "Static3dAssetIndexed.h"
#include "Animated3dAsset.h"
#include "SkeletonAsset.h"
#include "AnimationAsset.h"
#include "Vertex.h"
//#include "ImporterAnim.h"
//#include "MapPathImportHandler.h"
#include <vector>
#include <fstream>
#include <iostream>
//#include <string>

struct MeshInfo
{
	UINT nrOfVertices;

	char meshName[50];
	char diffuseMapName[50];
	char normalMapName[50];
	char specularMapName[50];
};
struct Indexed3DAssetInfo
{
	UINT vertexCount;
	UINT indexCount;

	StaticVertex* vertices;
	UINT* indices;
	char* assetName;
};
class AssetManager
{
	private:
		AssetID mAssetIdCounter;
	protected:
	public:
		std::vector<AssetBase*> mAssetContainer;

	private:		
		bool		AssetExist( char* fileName, AssetID &assetId );	//Returns true and assigns the correct id to assetId if the asset exist.	
		void		AssignAssetId( AssetID &assetId );					//Assigns the asset an id and increase the mAssetIdCounter.
		HRESULT		PlaceholderAssets( ID3D11Device* device );
	protected:
	public:
		HRESULT			LoadStatic3dAsset( ID3D11Device* device, char* fileName, AssetID &assetId );
		HRESULT			LoadStatic3dAssetIndexed( ID3D11Device* device, Indexed3DAssetInfo &info, AssetID &assetId );
		HRESULT			LoadAnimated3dAsset( ID3D11Device* device, char* fileName, AssetID skeletonId, AssetID &assetId );
		HRESULT			LoadSkeletonAsset( std::string filePath, std::string fileName, AssetID &assetId );
		HRESULT			LoadAnimationAsset( std::string filePath, std::string fileName, AssetID &assetId );

		AnimationData	ImportBinaryAnimData( std::string directoryPath, std::string fileName );
		Skeleton		ImportBinarySkelData( std::string directoryPath, std::string fileName );

		HRESULT		Initialize( ID3D11Device* device );
		void		Release();
					AssetManager();
		virtual		~AssetManager();

};
#endif