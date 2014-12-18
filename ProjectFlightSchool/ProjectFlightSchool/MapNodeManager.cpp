#include "MapNodeManager.h"
HRESULT MapNodeManager::createNodes( char* fileName, int nrOfNodes )
{
	//char* contentDir	= "../Content/Assets/Nodes/";
	mNrOfNodes			= nrOfNodes;
	UINT vertexSize		= sizeof( StaticVertex );
	std::vector<GameObject> staticObjects;

	for( int i = 0; i < nrOfNodes; i++ )
	{
		//char localFileName[50];
		//sprintf_s( localFileName, "%s%d", fileName, i );
		UINT dimX;
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

		inFile.read( (char*)&initInfo.gridDim, sizeof( UINT ) );
		inFile.read( (char*)&dimZ, sizeof( UINT ) );
		inFile.read( (char*)&initInfo.vertexCount, sizeof( UINT ) );
		initInfo.grid = new Vertex24[initInfo.vertexCount];

		inFile.read( (char*)initInfo.grid, sizeof( Vertex24 ) * initInfo.vertexCount ) ;

		for(int i = 0; i < initInfo.vertexCount; i++ )
		{
			printf("Gridpos: %f | %f | %f \n", initInfo.grid[i].position[0],initInfo.grid[i].position[1],initInfo.grid[i].position[2] );
		}

		JMatrix gridMat;

		inFile.read( (char*)&gridMat, sizeof(JMatrix) );

		initInfo.anchor = gridMat.pos;

		while( !inFile.eof() )
		{
			GameObject ob;
			GameObjectInfo obInfo;
			UINT assetID;
			inFile.read( (char*)&gridMat, sizeof(JMatrix) );
			obInfo.pos		= gridMat.pos;
			obInfo.rotation = gridMat.rot;
			obInfo.scale	= gridMat.scale;
			assetID = Graphics::GetInstance()->LoadStatic3dAsset( gridMat.name, assetID );
			ob.Initialize( obInfo, assetID );
			staticObjects.push_back( ob );
		}
		inFile.close();

		initInfo.staticAssets = new GameObject[staticObjects.size()];

		memcpy(initInfo.staticAssets, &staticObjects[0], sizeof(GameObject) * staticObjects.size() );

	}
	
	return S_OK;
}
HRESULT MapNodeManager::Render( float deltaTime )
{
	XMMATRIX world = XMMatrixTranslationFromVector(XMVectorSet( 0, 0, 0, 1 ) );

	//XMVECTOR worldPos = XMVector3TransformCoord( XMLoadFloat3(mNodes[0].))
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
}
MapNodeManager::MapNodeManager()
{
}
MapNodeManager::~MapNodeManager()
{
}
