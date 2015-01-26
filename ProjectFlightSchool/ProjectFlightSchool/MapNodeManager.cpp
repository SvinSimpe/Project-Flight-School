#include "MapNodeManager.h"
#include <fstream>

MapNodeManager* MapNodeManager::instance = nullptr;

 
void MapNodeManager::writeToLog( const std::string &text )
{
    std::ofstream log_file(
        "mapNodeLog.txt", std::ios_base::out | std::ios_base::app );
    log_file << text << std::endl;
}
HRESULT MapNodeManager::createNodes( char* fileName, int nrOfNodes )
{
	//char* contentDir	= "../Content/Assets/Nodes/";
	mNrOfNodes			= nrOfNodes;
	UINT vertexSize		= sizeof( StaticVertex );
	std::vector<GameObject> staticObjects;
	std::vector<MapNode> mapNodes;


	//For every node
	for( int i = 0; i < nrOfNodes; i++ )
	{
		//char localFileName[50];
		//sprintf_s( localFileName, "%s%d", fileName, i );
		UINT dimZ;

		std::ifstream inFile( fileName, std::ios::binary );

		if( !inFile )
		{
			OutputDebugStringA("Could not open file ");
			OutputDebugStringA(fileName);
			OutputDebugStringA("\n");
			return E_FAIL;
		}
		MapNodeInfo initInfo;
		UINT nrOfObjects = 0;


		//First read dimension, TODO: remove dimZ, needed atm, read vertexCount
		inFile.read( (char*)&initInfo.gridDim, sizeof( UINT ) );
		inFile.read( (char*)&dimZ, sizeof( UINT ) );
		inFile.read( (char*)&initInfo.vertexCount, sizeof( UINT ) );
		initInfo.grid = new Vertex24[initInfo.vertexCount];

		inFile.read( (char*)initInfo.grid, sizeof( Vertex24 ) * initInfo.vertexCount ) ;
		


		JMatrix gridMat;

		inFile.read( (char*)&gridMat, sizeof(JMatrix) );

		initInfo.anchor = gridMat.pos;

		inFile.read( (char*)&nrOfObjects,sizeof(UINT));
		//Read all the objects associated with the node
		for( int i = 0; i < (int)nrOfObjects; i++ )
		{
			GameObject ob;
			GameObjectInfo obInfo;
			AssetID assetID;
			inFile.read( (char*)&gridMat, sizeof(JMatrix) );

			obInfo.pos		= gridMat.pos;
			obInfo.rotation = gridMat.rot;
			obInfo.scale	= gridMat.scale;
			Graphics::GetInstance()->LoadStatic3dAsset( "", gridMat.name, assetID );
			ob.Initialize( obInfo, assetID );
			staticObjects.push_back( ob );
			char log[400];
			sprintf_s(log,"Timestamp: %s\nCount: %d\nGameObject allocated with:\nPos: (%f,%f,%f)\nRotation:  (%f,%f,%f)\nScale:  (%f,%f,%f)\nAssetID: %d\nName: %s\n\n\n",__TIME__, i,
				ob.GetPos().x,ob.GetPos().y, ob.GetPos().z,
				ob.GetRotation().x,ob.GetRotation().y, ob.GetRotation().z,
				ob.GetScale().x, ob.GetScale().y, ob.GetScale().z,
				ob.GetAssetID(), gridMat.name);
			writeToLog(log);
		}
		inFile.close();

		//Mapnode handles deallocation of this object
		initInfo.staticAssetCount = staticObjects.size();
		initInfo.staticAssets = new GameObject[initInfo.staticAssetCount];

		for( UINT i = 0; i < initInfo.staticAssetCount; i++ )
		{
			initInfo.staticAssets[i] = staticObjects[i];
		}

		MapNode temp;
		temp.Initialize( initInfo );
		mapNodes.push_back( temp );
		delete initInfo.grid;
	}

	mNrOfNodes = mapNodes.size();
	mNodes = new MapNode[mNrOfNodes];
	

	for( int i = 0; i < mNrOfNodes;i++ )
	{
		mNodes[i] = mapNodes[i];
	}

	return S_OK;
}
//HRESULT MapNodeManager::Render( float deltaTime )
//{
//	for( int i = 0; i < mNrOfNodes; i++ )
//	{
//		mNodes[i].Render( 0.0f );
//	}
//	return S_OK;
//}
//HRESULT	MapNodeManager::Update( float deltaTime )
//{
//	return S_OK;
//}
MapNode* MapNodeManager::GetNodes()
{
	return mNodes;
}
MapNodeManager* MapNodeManager::GetInstance()
{
	if( instance == nullptr )
	{
		instance = new MapNodeManager();
	}
	return instance;
}
HRESULT MapNodeManager::Initialize( char* fileName )
{
	createNodes( fileName, 1 );
	return S_OK;
}
void MapNodeManager::Release()
{
	for( int i = 0; i < mNrOfNodes; i++ )
	{
		mNodes[i].Release();
	}
	delete[] mNodes;
	SAFE_DELETE( instance );
}
MapNodeManager::MapNodeManager()
{
}
MapNodeManager::~MapNodeManager()
{
}
