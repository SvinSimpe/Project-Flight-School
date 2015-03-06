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
    //std::ofstream log_file(
    //    "mapNodeLog.jocke", std::ios_base::out | std::ios_base::app );
    //log_file << text << std::endl;
}

MapNode* MapNodeManager::CreateNode( const char* fileName, std::unordered_map<AssetID, AssetInfo>& assetMap )
{
	//char* contentDir	= "../Content/Assets/Nodes/";
	UINT vertexSize		= sizeof( StaticVertex );
	std::vector<GameObject> staticObjects;
	std::vector<PointLight>	pointLights;

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
	UINT nrOfObjects	= 0;
	UINT nrOfLights		= 0;


	//--------------------------Read gridData-------------------------------------------
	char blendFile[64];
	inFile.read( (char*)&blendFile, sizeof( char ) * 64 );
	inFile.read( (char*)&initInfo.gridWidth, sizeof( UINT ) );
	inFile.read( (char*)&initInfo.gridHeight, sizeof( UINT ) );
	inFile.read( (char*)&initInfo.vertexCount, sizeof( UINT ) );

	//This is copied into the grid itself, using regular vertex struct.
	initInfo.grid = new Vertex24[initInfo.vertexCount];

	inFile.read( (char*)initInfo.grid, sizeof( Vertex24 ) * initInfo.vertexCount ) ;

	//--------------------------Read gridData-------------------------------------------

	//-------------------------------Load blendMap--------------------------------------

	std::string folder	= "../Content/Assets/Nodes/BlendMaps/";
	Graphics::GetInstance()->LoadStatic2dAsset( folder + blendFile, initInfo.blendMap ); 

	//-------------------------------Load blendMap-------------------------------------

	//--------------------------Read actual gridObject ---------------------------------

	JMatrix gridMat;
	
	//inFile.read( (char*)&gridMat, sizeof(gridMat) );

	//Maya magic, writes out doubles, this converts them into floats

	ConvertToFloat( initInfo.anchor, gridMat.transformation );

	//--------------------------Read actual gridObject ---------------------------------

	//--------------------------Read navigation Data -----------------------------------

	UINT navVertexCount = 0;
	
	inFile.read( (char*)&navVertexCount, sizeof( UINT ) );

	//Deallocated by the node
	XMFLOAT3* navVertices = new XMFLOAT3[navVertexCount];

	inFile.read( (char*)navVertices, sizeof( DirectX::XMFLOAT3 ) * navVertexCount );

	initInfo.navVertexCount = navVertexCount;
	initInfo.navData		= navVertices;

	//--------------------------Read navigation Data -----------------------------------

	//--------------------------Read light Data ---------------------------------------

	inFile.read( (char*)&nrOfLights, sizeof( UINT ) );

	for( int i = 0; i < (int)nrOfLights; i++ )
	{
		char lightName[32];
		inFile.read( lightName, sizeof( lightName ) );

		float lightData[8];
		inFile.read( (char*)lightData, sizeof( lightData ) );
		PointLight pl;
		pl.positionAndIntensity	= XMFLOAT4( lightData[0], lightData[1], lightData[2], lightData[6] );
		pl.colorAndRadius		= XMFLOAT4( lightData[3], lightData[4], lightData[5], lightData[7] );
		pointLights.push_back( pl );
	}

	//--------------------------Read light Data ---------------------------------------

	//--------------------------Read object Data ---------------------------------------

	inFile.read( (char*)&nrOfObjects, sizeof( UINT ) );

	for( int i = 0; i < (int)nrOfObjects; i++ )
	{
		GameObject ob;
		GameObjectInfo obInfo;
		AssetID assetID;
		inFile.read( (char*)&gridMat, sizeof(JMatrix) );

		ConvertToFloat( obInfo.transformation, gridMat.transformation );
		
		
		Graphics::GetInstance()->LoadStatic3dAsset( "", gridMat.name, assetID );
		if( assetID == CUBE_PLACEHOLDER )
		{
			OutputDebugStringA( "Missing model: " );
			OutputDebugStringA( gridMat.name );
			OutputDebugStringA( " on the tile " );
			OutputDebugStringA( fileName );
			OutputDebugStringA( ".\n" );
		}

		obInfo.collision = assetMap[assetID].collisionType;
		obInfo.renderType = assetMap[assetID].renderType;

		ob.Initialize( obInfo, assetID );
		staticObjects.push_back( ob );
		#ifdef _DEBUG
				writeToLog( "\n -----------------------------------------------------\n" );
				writeToLog( fileName );
				char log[400];
				sprintf_s(log,"Timestamp: %s\nCount: %d\nGameObject allocated with:\nPos: (%f,%f,%f)\nRotation:  (%f,%f,%f)\nScale:  (%f,%f,%f)\nAssetID: %d\nName: %s\n\n\n",__TIME__, i,
				ob.GetPos().x,ob.GetPos().y, ob.GetPos().z,
				ob.GetRotation().x,ob.GetRotation().y, ob.GetRotation().z,
				ob.GetScale().x, ob.GetScale().y, ob.GetScale().z,
				ob.GetAssetID(), gridMat.name);
			writeToLog(log);
		#endif
	}

	//--------------------------Read object Data ---------------------------------------

	inFile.close();

	//--------------------------Copy light Data ---------------------------------------
	initInfo.pointLightCount	= pointLights.size();
	initInfo.pointLights		= new PointLight[initInfo.pointLightCount];

	for( UINT i = 0; i < initInfo.pointLightCount; i++ )
	{
		initInfo.pointLights[i] = pointLights[i];
	}

	//--------------------------Copy light Data ---------------------------------------

	//--------------------------Copy object Data ---------------------------------------

	//Mapnode handles deallocation of this object
	initInfo.staticAssetCount = staticObjects.size();
	initInfo.staticAssets = new GameObject[initInfo.staticAssetCount];

	for( UINT i = 0; i < initInfo.staticAssetCount; i++ )
	{
		initInfo.staticAssets[i] = staticObjects[i];
	}

	MapNode* temp = new MapNode;
	initInfo.name = fileName;
	temp->Initialize( initInfo );

	//--------------------------Copy object Data ---------------------------------------

	delete [] initInfo.grid;

	return temp;
}

NodeMap MapNodeManager::GetNodes()
{
	return mNodeMap;
}

void MapNodeManager::LoadLevel( std::string filePath ) 
{
	HRESULT hr = S_OK;
	std::vector<AssetInfo> assets;
	std::unordered_map<AssetID, AssetInfo> assetMap;
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
			AssetInfo temp;

			temp.assetPath = nodeElement->Attribute("path");
			nodeElement->Attribute("collisionType", (int*)&temp.collisionType );
			nodeElement->Attribute("renderType", (int*)&temp.renderType );

			assets.push_back( temp );
		}
	//Load the assets into the engine
	for( auto it = assets.begin(); it != assets.end(); it++ )
	{
		AssetID id = 0;
		int found = (*it).assetPath.find_last_of('/');
		if( (*it).assetPath.substr( found + 1 ) == "mushroom1.pfs")
		{
			int d = 0;
		}
		hr = Graphics::GetInstance()->LoadStatic3dAsset( (*it).assetPath.substr( 0, found + 1 ), (*it).assetPath.substr( found + 1 ), id );

		assetMap[id] = (*it);

		if( id == CUBE_PLACEHOLDER )
		{
			OutputDebugStringA( "Model not found:" );
			OutputDebugStringA( (*it).assetPath.substr( found + 1 ).c_str() );
			OutputDebugStringA( " Maybe path is wrong? \n" );
		}
	}


	topElement = rootElement->FirstChild("Nodes");
	//Load nodes into the engine
	for( nodeElement = topElement->FirstChildElement();
			 nodeElement;
			 nodeElement = nodeElement->NextSiblingElement() )
		{
			MapNode* temp;
			if( temp = CreateNode( nodeElement->Attribute("path"), assetMap ) )
			{
				mNodeMap[nodeElement->Attribute("type")].push_back( temp );
			}
		}
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

