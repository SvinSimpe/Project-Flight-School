#include "MapNodePlacer.h"
#include "MapNodeManager.h"
#include "NodeTypes.h"

MapNodePlacer* MapNodePlacer::instance = nullptr;

NODE_RETURN_CODE MapNodePlacer::CanPlace( int pX, int pY, MapNodeInstance* newNode, MapNodeInstance*** buildMap )
{
	int nodeWith   = (int)( ( newNode->GetMapNode()->GetGridWidth() / ( NODE_DIM ) ) );
	int nodeHeight = (int)( ( newNode->GetMapNode()->GetGridHeight() / ( NODE_DIM ) ) );

	if( (int)mMap->GetMapWidth() < ( pX + nodeWith ) || (int)mMap->GetMapHeight() < ( pY + nodeHeight ) )
	{
		return NOFIT;
	}

	for( int x = pX; x < (int)( pX + nodeWith ); x++ )
	{
		for( int y = pY; y < (int)( pY +  nodeHeight ); y++ )
		{
			if( buildMap[x][y] != nullptr )
			{
				return OCCUPIED;
			}
		}
	}

	for( int x = pX; x < (int)( pX + nodeWith ); x++ )
	{
		for( int y = pY; y < (int)( pY +  nodeHeight ); y++ )
		{
			buildMap[x][y] = newNode;
			//mNodes.push_back( newNode );
		}
	}
	XMFLOAT3 newPos = XMFLOAT3( ((float)pX * NODE_DIM ) - ( mMap->GetMapHalfWidth() * NODE_DIM ), 0, ( (float)pY * NODE_DIM ) - ( mMap->GetMapHalfHeight() * NODE_DIM ) );

	newNode->SetPos( newPos );
	newNode->Initialize();
	newNode->SetNodeID( mNrOfNodes++ );

	return PLACED;
}

MapNodePlacer* MapNodePlacer::GetInstance()
{
	if ( instance == nullptr )
	{
		instance  = new MapNodePlacer();
		return instance;
	}
	return instance;
}

void MapNodePlacer::BuildMap( MapNodeInstance***& buildMap )
{
	buildMap = new MapNodeInstance**[mMap->GetMapDim()];
	for( int i = 0; i < (int)mMap->GetMapDim(); i++ )
	{
		buildMap[i] = new MapNodeInstance*[mMap->GetMapDim()];
		for( int j = 0; j < (int)mMap->GetMapDim(); j++ )
		{
			buildMap[i][j] = nullptr;
		}
	}
	NodeMap nodeMap = MapNodeManager::GetInstance()->GetNodes();

	int count	= 0;
	int nodeID	= 0;

	int nodeMapSize = (int)nodeMap[NodeTypes::AREA_NODE].size();
	int randomNode = 0;


	//Hardcoding Ships
	CanPlace( mMap->GetMapHalfWidth() - 2, mMap->GetMapHalfHeight(), nodeMap[NodeTypes::SHIP_NODE][randomNode]->GetMapNodeInstance(), buildMap );
	//map[mNrOfNodes++] = nodeMap[NodeTypes::SHIP_NODE][randomNode]->GetMapNodeInstance();
	//////////////////
	////Hardcoding Ships
	CanPlace( mMap->GetMapHalfWidth() + 1, mMap->GetMapHalfHeight(), nodeMap[NodeTypes::SHIP_NODE][randomNode]->GetMapNodeInstance(), buildMap );
	//map[mNrOfNodes++] = nodeMap[NodeTypes::SHIP_NODE][randomNode]->GetMapNodeInstance();
	////////////////

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
			
			randomNode = (randomNode + 1) % nodeMapSize;
			while( doLoop )
			{
				MapNodeInstance* newNode = nodeMap[NodeTypes::AREA_NODE][randomNode]->GetMapNodeInstance();
				switch( CanPlace( x, y, newNode, buildMap ) )
				{
					case OCCUPIED:
						newNode->ReleaseInstance();
						if( count < nodeMapSize )
						{
							count++;
							randomNode = (count + randomNode) % nodeMapSize;
						}
						else
						{
							doLoop = false;
						}
						break;
					case NOFIT:
						newNode->ReleaseInstance();
						if( count < nodeMapSize )
						{
							count++;
							randomNode = (count + randomNode) % nodeMapSize;
						}
						else
						{
							doLoop = false;
							OutputDebugStringA("Couldnt find a node that fits.\n");
						}
						break;
					case PLACED:
						//map[mNrOfNodes++] = newNode;
						count = 0;

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

void MapNodePlacer::Reset()
{
	mNrOfNodes = 0;
}

HRESULT	MapNodePlacer::Initialize( Map* map )
{
	mMap = map;
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
	mNrOfNodes = 0;
}

MapNodePlacer::~MapNodePlacer()
{
}