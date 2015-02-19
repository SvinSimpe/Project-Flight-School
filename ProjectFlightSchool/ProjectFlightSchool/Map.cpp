#include "Map.h"
#include "MapNodeManager.h"
#include "MapNodePlacer.h"
#include "NodeGraph.h"

HRESULT Map::Render( float deltaTime, Player* player )
{
	std::vector<MapNodeInstance*> mapNodes;
	int playerPosX = (int)player->GetPosition().x;
	int playerPosY = (int)player->GetPosition().z;

	int playerX = ( ( (int)GetMapHalfWidth() * NODE_DIM ) + playerPosX ) / NODE_DIM;
	int playerZ = ( ( (int)GetMapHalfHeight() * NODE_DIM ) + playerPosY ) / NODE_DIM;

	for( int x = playerX - 5; x < playerX + 6; x++ )
	{
		for( int z = playerZ - 5; z < playerZ + 6; z++ )
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

	//nodeGraph->Render();

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

		//nodeGraph = new NodeGraph();
		//nodeGraph->Initialize( this );
	}
}

std::vector<DirectX::XMFLOAT2> Map::GetPath( XMFLOAT3 start, XMFLOAT3 goal )
{
	std::vector<Node*> path;
	std::vector<DirectX::XMFLOAT2> path1;
	std::vector<DirectX::XMFLOAT2> temp;
	MapNodeInstance* currentNode;
	MapNodeInstance* nextNode;
	Navmesh* currentNavMesh;

	DirectX::XMFLOAT3 newStart, newEnd;

	int startX = (int)( (GetMapHalfWidth() * NODE_DIM )  + start.x ) / NODE_DIM;
	int startZ = (int)( (GetMapHalfHeight() * NODE_DIM )  + start.z ) / NODE_DIM;

	int goalX = (int)( (GetMapHalfWidth() * NODE_DIM )  + goal.x ) / NODE_DIM;
	int goalZ = (int)( (GetMapHalfHeight() * NODE_DIM )  + goal.z ) / NODE_DIM;

	MapNodeInstance* startNode = GetNodeInstance( startX, startZ );
	MapNodeInstance* goalNode = GetNodeInstance( goalX, goalZ );

	newStart = start;
	path1.push_back( DirectX::XMFLOAT2( start.x, start.z ) );

	//if( startNode && goalNode )
	//{

	//	path = nodeGraph->FindPath( startNode->GetNodeID(), goalNode->GetNodeID() );

	//	for( int i = 0; i < (int)path.size() - 1; i++ )
	//	{
	//		currentNode = GetNodeInstance( path[i]->mNodePos.x, path[i]->mNodePos.y );
	//		
	//		currentNavMesh = currentNode->GetNavMesh();

	//		newEnd = currentNode->GetClosestEdgePoint( path[i + 1]->centerPoint );

	//		temp = currentNavMesh->FindPath( newStart, newEnd );

	//		for( auto& it : temp )
	//		{
	//			path1.push_back( it );
	//		}
	//		newStart = newEnd;
	//	}

	//	//Last subpath
	//	currentNode = GetNodeInstance( path.back()->mNodePos.x, path.back()->mNodePos.y );
	//	currentNavMesh = currentNode->GetNavMesh();
	//	
	//	temp = currentNavMesh->FindPath( newStart, goal );

	//	for( auto& it : temp )
	//	{
	//		path1.push_back( it );
	//	}

	//
	//}

	return path1;
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