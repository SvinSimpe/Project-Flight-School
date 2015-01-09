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
bool MapNodePlacer::canPlace( int left, int top, MapNode node )
{
	if ( (left % mMap->GetMapWidth() < node.GetGridWidth()) && (top % mMap->GetMapHeight() < node.GetGridHeight()) )
	{
		for ( int i = 0; i < 2; i++ )
		{
			for ( int x = left; x < (int)node.GetGridWidth(); x++ )
			{
				int dX = x + ( i *  + node.GetGridWidth() );
				if ( mBuildMap[x][top] != 0 )
				{
					return false;
				}
			}
			for ( int y = top; y < (int)node.GetGridHeight(); y++ )
			{
				int dY = y + ( i * node.GetGridHeight() );
				if ( mBuildMap[left][y] != 0 )
				{
					return false;
				}
			}
		}
		for (int y = top; y < top + node.GetGridHeight(); y++)
		{
			for (int x = left; x < left + node.GetGridWidth(); x++)
			{
				mBuildMap[x][y] = 1;
			}
		}
		return true;
	}
	return false;
}
std::vector<MapNodeInstance> MapNodePlacer::PlaceNodes()
{
	MapNode* nodes = MapNodeManager::GetInstance()->GetNodes();
	std::vector<MapNodeInstance> placedNodes;

	for (int y = 0; y < (int)mMap->GetMapHeight(); y++)
	{
		for (int x = 0; x < (int)mMap->GetMapWidth(); x++)
		{
			if ( canPlace( x, y, nodes[0] ) )
			{
				MapNodeInstance mapNodeInst = nodes[0].GetMapNodeInstance();
				mapNodeInst.SetCorners( x, y );
				XMFLOAT3 pos;
				pos.x = ((float)x - mMap->GetMapHalfWidth());
				pos.y = 0.0f;
				pos.z = ((float)y - mMap->GetMapHalfHeight());
				mapNodeInst.SetPos( pos );
				placedNodes.push_back(nodes[0].GetMapNodeInstance());
			}
		}
	}
	for (int y = 0; y < (int)mMap->GetMapHeight(); y++)
	{
		
		for (int x = 0; x < (int)mMap->GetMapWidth(); x++)
		{
			printf("[%d]",mBuildMap[x][y]);
		}

		printf("\n");
	}
	return placedNodes;
}
HRESULT	MapNodePlacer::Initialize( Map* map )
{
	mMap = map;
	//mBuildMapSize = mMap->GetMapWidth() * mMap->GetMapHeight();
	mBuildMap = new int*[mMap->GetMapHeight()];
	for (int y = 0; y < (int)mMap->GetMapHeight(); y++)
	{
		for (int x = 0; x < (int)mMap->GetMapWidth(); x++)
		{
			mBuildMap[x] = new int[mMap->GetMapHeight()];
		}
	}
	for (int y = 0; y < (int)mMap->GetMapHeight(); y++)
	{
		for (int x = 0; x < (int)mMap->GetMapWidth(); x++)
		{
			mBuildMap[x][y] = 0;
		}
	}
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