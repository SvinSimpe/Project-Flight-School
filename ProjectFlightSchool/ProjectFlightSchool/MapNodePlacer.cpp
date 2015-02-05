#include "MapNodePlacer.h"
#include "MapNodeManager.h"
#include "NodeTypes.h"

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

NODE_RETURN_CODE MapNodePlacer::CanPlace( int pX, int pY, MapNodeInstance* newNode )
{
	int nodeWith   = (int)( ( newNode->GetMapNode()->GetGridWidth() / ( NODE_DIM ) ) );
	int nodeHeight = (int)( ( newNode->GetMapNode()->GetGridHeight() / ( NODE_DIM ) ) );

	if( (int)mMap->GetMapWidth() < ( pX + nodeWith ) || (int)mMap->GetMapWidth() < ( pY + nodeHeight ) )
	{
		return NOFIT;
	}


	for( int x = pX; x < (int)( pX + nodeWith ); x++ )
	{
		for( int y = pY; y < (int)( pY +  nodeHeight ); y++ )
		{
			if( mBuildMap[x][y] != nullptr )
			{
				return OCCUPIED;
			}
		}
	}

	for( int x = pX; x < (int)( pX + nodeWith ); x++ )
	{
		for( int y = pY; y < (int)( pY +  nodeHeight ); y++ )
		{
			mBuildMap[x][y] = newNode;
		}
	}
	XMFLOAT3 newPos = XMFLOAT3( ((float)pX * NODE_DIM ) - ( mMap->GetMapHalfWidth() * NODE_DIM ), 0, ( (float)pY * NODE_DIM ) - ( mMap->GetMapHalfHeight() * NODE_DIM ) );

	newNode->SetPos( newPos );

	return PLACED;
}
void MapNodePlacer::BuildMap( MapNodeInstance** map )
{
	NodeMap nodeMap = MapNodeManager::GetInstance()->GetNodes();
	int count = 0;

	for( int x = 0; x < (int)mMap->GetMapDim(); x++ )
	{
		for( int y = 0; y < (int)mMap->GetMapDim(); y++ )
		{
			if(MAX_NODES < mNrOfNodes)
			{
				return;
			}
			bool doLoop = true;
			//Check "Tier", lastNode, chances of boss arena and/or energy position.

			while( doLoop )
			{
				int randomNode = rand() % (int)nodeMap[NodeTypes::AREA_NODE].size();
				MapNodeInstance* newNode = nodeMap[NodeTypes::AREA_NODE][randomNode]->GetMapNodeInstance();
				switch( CanPlace( x, y, newNode ) )
				{
					case OCCUPIED:
						doLoop = false;
						newNode->ReleaseInstance();
						break;
					case NOFIT:
						newNode->ReleaseInstance();
						break;
					case PLACED:
						map[mNrOfNodes++] = newNode;
						count++;
						doLoop = false;
						break;
				}
			}
		}
	}
	return;
}
UINT MapNodePlacer::GetNrOfNodes() const
{
	return (UINT)mNrOfNodes;
}
HRESULT	MapNodePlacer::Initialize( Map* map )
{
	mMap = map;
	mBuildMap = new MapNodeInstance**[mMap->GetMapDim()];
	for( int i = 0; i < (int)mMap->GetMapDim(); i++ )
	{
		mBuildMap[i] = new MapNodeInstance*[mMap->GetMapDim()];
		for( int j = 0; j < (int)mMap->GetMapDim(); j++ )
		{
			mBuildMap[i][j] = nullptr;
		}
	}
	return S_OK;
}
void MapNodePlacer::Release()
{
	if ( instance != nullptr )
	{
		for( int i = 0; i < (int)mMap->GetMapDim(); i++ )
		{
			delete[] mBuildMap[i];
		}
		delete[] mBuildMap;

		delete instance;
	}
}
void MapNodePlacer::Reset()
{
	for( int i = 0; i < (int)mMap->GetMapDim(); i++ )
	{
		for( int j = 0; j < (int)mMap->GetMapDim(); j++ )
		{
			mBuildMap[i][j] = nullptr;
		}
	}
	mNrOfNodes = 0;
}
MapNodePlacer::MapNodePlacer()
{
	mNrOfNodes = 0;
}
MapNodePlacer::~MapNodePlacer()
{
}