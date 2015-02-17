#include "Map.h"
#include "MapNodeManager.h"
#include "MapNodePlacer.h"

HRESULT Map::Render( float deltaTime, Player* player )
{
	std::vector<MapNodeInstance*> mapNodes;
	int playerPosX = (int)player->GetPosition().x;
	int playerPosY = (int)player->GetPosition().z;

	int playerX = ( ( (int)GetMapHalfWidth() * NODE_DIM ) + playerPosX ) / NODE_DIM;
	int playerZ = ( ( (int)GetMapHalfHeight() * NODE_DIM ) + playerPosY ) / NODE_DIM;

	for( int x = playerX - 1; x < playerX + 2; x++ )
	{
		for( int z = playerZ - 1; z < playerZ + 2; z++ )
		{
			MapNodeInstance* temp = GetNodeInstance( x, z );
			if( temp && std::find( mapNodes.begin(), mapNodes.end(), temp ) == mapNodes.end() )
			{
				mapNodes.push_back( temp );
			}
		}
	}

	for( auto& it : mapNodes )
	{
		it->Render( deltaTime );
	}


	//std::vector<MapNodeInstance*> nodes = MapNodePlacer::GetInstance()->GetAllNodes();
	//for( auto& it : nodes )
	//{
	//	it->Render( deltaTime );
	//}

	return S_OK;
}
void Map::OnLoadLevel( IEventPtr pEvent )
{
	if( Event_Load_Level::GUID == pEvent->GetEventType() )
	{
		std::shared_ptr<Event_Load_Level> dataPtr = std::static_pointer_cast<Event_Load_Level>( pEvent );

		MapNodeManager::GetInstance()->LoadLevel( dataPtr->FilePath() );

		MapNodePlacer::GetInstance()->Reset();

		MapNodePlacer::GetInstance()->BuildMap( mBuildMap );

		if( mMapSection )
		{
			mMapSection->Release();
			delete mMapSection;
		}
		mNrOfNodes = MapNodePlacer::GetInstance()->GetNrOfNodes();
	}
}

std::vector<DirectX::XMFLOAT2> Map::GetPath( XMFLOAT3 start, XMFLOAT3 goal )
{
	std::vector<DirectX::XMFLOAT2> path;

	int startX = (int)( (GetMapHalfWidth() * NODE_DIM )  + start.x ) / NODE_DIM;
	int startZ = (int)( (GetMapHalfHeight() * NODE_DIM )  + start.z ) / NODE_DIM;

	MapNodeInstance* temp = GetNodeInstance( startX, startZ );

	if( temp )
	{
		Navmesh* navMesh = temp->GetNavMesh();

		path = navMesh->FindPath( start, goal );
	}

	return path;
}

UINT Map::GetMapDim() const
{
	return mMapDim ;//* SECTION_DIM;
}
UINT Map::GetMapWidth() const
{
	return mMapDim ;//* SECTION_DIM;
}
UINT Map::GetMapHeight() const
{
	return mMapDim ;//* SECTION_DIM;
}
UINT Map::GetMapHalfWidth() const
{
	return mMapDim / 2;
}
UINT Map::GetMapHalfHeight() const
{
	return mMapDim / 2;
}
UINT Map::GetNrOfNodes() const
{
	return mNrOfNodes;
}

MapNodeInstance*** Map::GetNodeMap() const
{
	return mBuildMap;
}

MapNodeInstance* Map::GetNodeInstance( int x, int z )
{
	MapNodeInstance* result = nullptr;
	if( ( x < (int)mMapDim && 0 <= x ) &&
		( z < (int)mMapDim && 0 <= z ) )
	{
		result = mBuildMap[x][z];
	}
	return result;
}

HRESULT Map::Initialize( UINT mapDim )
{
	//Map size is mapDim* mapDim
	mMapDim = mapDim;
	MapNodeManager::GetInstance()->Initialize();
	MapNodePlacer::GetInstance()->Initialize( this );

	EventManager::GetInstance()->AddListener( &Map::OnLoadLevel, this, Event_Load_Level::GUID );
	
	return S_OK;
}
void Map::Release()
{
	MapNodePlacer::GetInstance()->Release();
	MapNodeManager::GetInstance()->Release();
}
Map::Map()
{
}
Map::~Map()
{
}