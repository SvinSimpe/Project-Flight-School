#include "MapNodeManager.h"
#include <fstream>
#include "tinyxml.h"

MapNodeManager* MapNodeManager::instance = nullptr;

 

void MapNodeManager::ConvertToFloat( XMFLOAT4X4& dest, double* values )
{
	XMFLOAT4X4 temp = {		(float)values[0], (float)values[1], (float)values[2], (float)values[3], 
							(float)values[4], (float)values[5], (float)values[6], (float)values[7],
							(float)values[8], (float)values[9], (float)values[10], (float)values[11], 
							(float)values[12], (float)values[13], (float)values[14], (float)values[15] };

	dest = temp;
}
void MapNodeManager::writeToLog( const std::string &text )
{
    std::ofstream log_file(
        "mapNodeLog.jocke", std::ios_base::out | std::ios_base::app );
    log_file << text << std::endl;
}
void MapNodeManager::LoadLevel( std::string filePath ) 
{
	std::vector<std::string> assetPaths;
	TiXmlDocument doc;
	if( !doc.LoadFile( filePath.c_str() ) )
	{
		OutputDebugStringA( "Cannot open file: " );
		OutputDebugStringA( filePath.c_str() );
		OutputDebugStringA( "\n" );
		return;
	}

	TiXmlElement* rootElement = doc.RootElement();
	TiXmlNode* topElement;
	TiXmlElement* nodeElement;

	//Load all assetpaths
	topElement = rootElement->FirstChild("Assets");
	for( nodeElement = topElement->FirstChildElement();
			 nodeElement;
			 nodeElement = nodeElement->NextSiblingElement() )
		{
			assetPaths.push_back( nodeElement->Attribute("path") );
		}
	//Load the assets into the engine
	for( auto it = assetPaths.begin(); it != assetPaths.end(); it++ )
	{
		AssetID id = 0;
		int found = it->find_last_of('/');
		if( it->substr( found + 1 ) == "mushroom1.pfs")
		{
			int d = 0;
		}
		Graphics::GetInstance()->LoadStatic3dAsset( it->substr( 0, found + 1 ), it->substr( found + 1 ), id );
		if( id == CUBE_PLACEHOLDER )
		{
			OutputDebugStringA( "Model not found! Maybe path is wrong? \n");
		}
	}


	topElement = rootElement->FirstChild("Nodes");
	//Load nodes into the engine
	for( nodeElement = topElement->FirstChildElement();
			 nodeElement;
			 nodeElement = nodeElement->NextSiblingElement() )
		{
			MapNode* temp;
			if( temp = CreateNode( nodeElement->Attribute("path") ) )
			{
				mNodeMap[nodeElement->Attribute("type")].push_back( temp );
			}
		}
}
MapNode* MapNodeManager::CreateNode( const char* fileName )
{
	//char* contentDir	= "../Content/Assets/Nodes/";
	UINT vertexSize		= sizeof( StaticVertex );
	std::vector<GameObject> staticObjects;

	//char localFileName[50];
	//sprintf_s( localFileName, "%s%d", fileName, i );

	std::ifstream inFile( fileName, std::ios::binary );

	if( !inFile )
	{
		OutputDebugStringA("Could not open file ");
		OutputDebugStringA(fileName);
		OutputDebugStringA("\n");
		return nullptr;
	}
	MapNodeInfo initInfo;
	UINT nrOfObjects = 0;


	//First read dimension, TODO: remove dimZ, needed atm, read vertexCount
	inFile.read( (char*)&initInfo.gridWidth, sizeof( UINT ) );
	inFile.read( (char*)&initInfo.gridHeight, sizeof( UINT ) );
	inFile.read( (char*)&initInfo.vertexCount, sizeof( UINT ) );
	initInfo.grid = new Vertex24[initInfo.vertexCount];
	inFile.read( (char*)initInfo.grid, sizeof( Vertex24 ) * initInfo.vertexCount ) ;

	JMatrix gridMat;
	//inFile.read( (char*)derpSOMFAN, 50 );
	//inFile.read( (char*)derpface, 128 );
	inFile.read( (char*)&gridMat, sizeof(gridMat) );

	//initInfo.anchor = gridMat.pos;
	//initInfo.anchor = gridMat.transformation;
	ConvertToFloat( initInfo.anchor, gridMat.transformation );

	UINT navVertexCount = 0;
	
	inFile.read( (char*)&navVertexCount, sizeof( UINT ) );

	XMFLOAT3* navVertices = new XMFLOAT3[navVertexCount];

	inFile.read( (char*)navVertices, sizeof( DirectX::XMFLOAT3 ) * navVertexCount );

	inFile.read( (char*)&nrOfObjects, sizeof( UINT ) );
	//Read all the objects associated with the node
	writeToLog( "\n -----------------------------------------------------\n" );
	writeToLog( fileName );
	for( int i = 0; i < (int)nrOfObjects; i++ )
	{
		GameObject ob;
		GameObjectInfo obInfo;
		AssetID assetID;
		inFile.read( (char*)&gridMat, sizeof(JMatrix) );

		ConvertToFloat( obInfo.transformation, gridMat.transformation );
		
		//obInfo.pos		= gridMat.pos;

		//obInfo.rotation = gridMat.rot;
		//obInfo.scale	= gridMat.scale;

		//obInfo.rotation.x = -obInfo.rotation.x;
		//obInfo.rotation.y = -obInfo.rotation.y;
		//obInfo.rotation.z = -obInfo.rotation.z;
		
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

	MapNode* temp = new MapNode;
	temp->Initialize( initInfo );

	delete [] initInfo.grid;
	delete [] navVertices;

	return temp;
}

NodeMap MapNodeManager::GetNodes()
{
	return mNodeMap;
}
MapNodeManager* MapNodeManager::GetInstance()
{
	if( instance == nullptr )
	{
		instance = new MapNodeManager();
	}
	return instance;
}
HRESULT MapNodeManager::Initialize()
{
	return S_OK;
}
void MapNodeManager::Release()
{
	for( auto& it : mNodeMap )
	{
		for( auto& it2 : it.second )
		{
			it2->Release();
			delete it2;
		}
		it.second.clear();
	}

	mNodeMap.clear();
	SAFE_DELETE( instance );
}
MapNodeManager::MapNodeManager()
{
}
MapNodeManager::~MapNodeManager()
{
}

