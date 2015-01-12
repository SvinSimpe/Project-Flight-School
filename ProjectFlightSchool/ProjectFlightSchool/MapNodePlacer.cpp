#include "MapNodePlacer.h"
#include "MapNodeManager.h"

MapNodePlacer* MapNodePlacer::instance = nullptr;

MapNodePlacer* MapNodePlacer::GetInstance()
{
	if ( instance == nullptr )
	{
		instance  = new MapNodePlacer();
		return instance;
	}
	return instance;
}
bool MapNodePlacer::canPlace( int left, int top, MapNode current )
{
	//if ( ( node.GetGridWidth() <= ( mMap->GetMapWidth() - left ) ) && ( node.GetGridHeight() <= ( mMap->GetMapHeight() - top ) ) )
	//{
	//	for ( int i = 0; i < 2; i++ )
	//	{
	//		for ( int x = left; x < left + (int)node.GetGridWidth(); x++ )
	//		{
	//			int dY = top + ( i * ( node.GetGridWidth() - 1 ) );
	//			if ( mBuildMap[x][dY] != 0 )
	//			{
	//				return false;
	//			}
	//		}
	//		for ( int y = top; y < top + (int)node.GetGridHeight(); y++ )
	//		{
	//			int dX = left + ( i * ( node.GetGridHeight() - 1 ) );
	//			if ( mBuildMap[dX][y] != 0 )
	//			{
	//				return false;
	//			}
	//		}
	//	}
	//	for (int y = top; y < top + (int)node.GetGridHeight(); y++)
	//	{
	//		for (int x = left; x < left + (int)node.GetGridWidth(); x++)
	//		{
	//			mBuildMap[x][y] = 1;
	//		}
	//	}
	//	return true;
	//}
	//else
	//{
	//	int debug = mMap->GetMapWidth() - left;
	//	int debug2 = mMap->GetMapHeight() - top;
	//}
	//return false;



	return false;
}
std::vector<MapNodeInstance> MapNodePlacer::PlaceNodes()
{
	int maxNodes	= 100;
	int nrOfNodes	= 0;
	MapNode* nodes = MapNodeManager::GetInstance()->GetNodes();
	std::vector<MapNodeInstance> placedNodes;

	MapNodeInstance startNode = nodes[0].GetMapNodeInstance();
	MapNodeInstance currentNode = startNode;

	startNode.SetOrigin( XMFLOAT3(0, 0 ,0) );

	placedNodes.push_back( startNode );
	nrOfNodes++;

	//mBuildMap = new int*[mMap->GetMapHeight()];
	//for (int y = 0; y < (int)mMap->GetMapHeight(); y++)
	//{
	//	for (int x = 0; x < (int)mMap->GetMapWidth(); x++)
	//	{
	//		mBuildMap[x] = new int[mMap->GetMapHeight()];
	//	}
	//}
	//for (int y = 0; y < (int)mMap->GetMapHeight(); y++)
	//{
	//	for (int x = 0; x < (int)mMap->GetMapWidth(); x++)
	//	{
	//		mBuildMap[x][y] = 0;
	//	}
	//}

	//for (int y = 0; y < (int)mMap->GetMapHeight(); y++)
	//{
	//	for (int x = 0; x < (int)mMap->GetMapWidth(); x++)
	//	{
	//		if ( canPlace( x, y, nodes[0] ) )
	//		{
	//			MapNodeInstance mapNodeInst = nodes[0].GetMapNodeInstance();
	//			mapNodeInst.SetCorners( x, y );
	//			XMFLOAT3 origin;
	//			origin.x = ((float)x - mMap->GetMapHalfWidth());
	//			origin.y = 0.0f;
	//			origin.z = ((float)y - mMap->GetMapHalfHeight());
	//			mapNodeInst.SetOrigin( origin );
	//			placedNodes.push_back( mapNodeInst );
	//		}
	//	}
	//}

	//for (int y = 0; y < (int)mMap->GetMapHeight(); y++)
	//{
	//	for (int x = 0; x < (int)mMap->GetMapWidth(); x++)
	//	{
	//		printf("[%d]", mBuildMap[x][y]);
	//	}
	//	printf("\n");
	//}

	while( nrOfNodes < maxNodes )
	{
		for(int i = 0; i < 4; i++)
		{

		}
	}

	return placedNodes;
}
HRESULT	MapNodePlacer::Initialize( Map* map )
{
	mMap = map;
	//mBuildMapSize = mMap->GetMapWidth() * mMap->GetMapHeight();
	//for (int y = 0; y < (int)mMap->GetMapHeight(); y++)
	//{
	//	for (int x = 0; x < (int)mMap->GetMapWidth(); x++)
	//	{
	//		mBuildMap[x] = new int[mMap->GetMapHeight()];
	//	}
	//}
	//for (int y = 0; y < (int)mMap->GetMapHeight(); y++)
	//{
	//	for (int x = 0; x < (int)mMap->GetMapWidth(); x++)
	//	{
	//		mBuildMap[x][y] = 0;
	//	}
	//}
	return S_OK;
}
void MapNodePlacer::Release()
{
	if ( instance != nullptr )
	{
		delete instance;
	}
}
MapNodePlacer::MapNodePlacer()
{
}
MapNodePlacer::~MapNodePlacer()
{
}