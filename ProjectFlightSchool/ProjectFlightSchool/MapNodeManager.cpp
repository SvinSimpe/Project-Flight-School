#include "MapNodeManager.h"
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


		//First read dimension, TODO: remove dimZ, needed atm, read vertexCount
		inFile.read( (char*)&initInfo.gridDim, sizeof( UINT ) );
		inFile.read( (char*)&dimZ, sizeof( UINT ) );
		inFile.read( (char*)&initInfo.vertexCount, sizeof( UINT ) );
		initInfo.grid = new Vertex24[initInfo.vertexCount];

		inFile.read( (char*)initInfo.grid, sizeof( Vertex24 ) * initInfo.vertexCount ) ;

		for(UINT i = 0; i < initInfo.vertexCount; i++ )
		{
			printf("Gridpos: %f | %f | %f \n", initInfo.grid[i].position[0],initInfo.grid[i].position[1],initInfo.grid[i].position[2] );
		}

		JMatrix gridMat;

		inFile.read( (char*)&gridMat, sizeof(JMatrix) );

		initInfo.anchor = gridMat.pos;

		//Read all the objects associated with the node
		while( !inFile.eof() )
		{
			GameObject ob;
			GameObjectInfo obInfo;
			AssetID assetID;
			inFile.read( (char*)&gridMat, sizeof(JMatrix) );
			obInfo.pos		= gridMat.pos;
			obInfo.rotation = gridMat.rot;
			obInfo.scale	= gridMat.scale;
			Graphics::GetInstance()->LoadStatic3dAsset( "../Content/Assets/Stones/", "stone_1.pfs", assetID );
			ob.Initialize( obInfo, assetID );
			staticObjects.push_back( ob );
		}
		inFile.close();

		//Mapnode handles deallocation of this object
		initInfo.staticAssetCount = staticObjects.size();
		initInfo.staticAssets = new GameObject[initInfo.staticAssetCount];

		//memcpy(initInfo.staticAssets, &staticObjects[0], sizeof(GameObject) * staticObjects.size() );

		for( int i = 0; i < initInfo.staticAssetCount; i++ )
		{
			initInfo.staticAssets[i] = staticObjects[i];
			printf("GameObject allocated with:\nPos: (%f,%f,%f)\nRotation:  (%f,%f,%f)\nScale:  (%f,%f,%f)\nAssetID: %d\n",
				initInfo.staticAssets[i].GetPos().x,initInfo.staticAssets[i].GetPos().y,initInfo.staticAssets[i].GetPos().z,
				initInfo.staticAssets[i].GetRotation().x,initInfo.staticAssets[i].GetRotation().y,initInfo.staticAssets[i].GetRotation().z,
				initInfo.staticAssets[i].GetScale().x,initInfo.staticAssets[i].GetScale().y,initInfo.staticAssets[i].GetScale().z,
				initInfo.staticAssets[i].GetAssetID());
		}

		MapNode temp;
		temp.Initialize( initInfo );
		mapNodes.push_back( temp );
	}

	mNrOfNodes = mapNodes.size();
	mNodes = new MapNode[mNrOfNodes];
	

	for( int i = 0; i < mNrOfNodes;i++ )
	{
		mNodes[i] = mapNodes[i];
	}

	return S_OK;
}
HRESULT MapNodeManager::Render( float deltaTime )
{
	for( int i = 0; i < mNrOfNodes; i++ )
	{
		mNodes[i].Render( 0.0f );
	}
	return S_OK;
}
HRESULT	MapNodeManager::Update( float deltaTime )
{
	return S_OK;
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
}
MapNodeManager::MapNodeManager()
{
}
MapNodeManager::~MapNodeManager()
{
}
