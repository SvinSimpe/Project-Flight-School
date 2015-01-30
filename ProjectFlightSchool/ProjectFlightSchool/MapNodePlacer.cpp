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

bool MapNodePlacer::CanPlace( int pX, int pY, MapNodeInstance* newNode )
{
	int debug = (int)( ( newNode->GetMapNode()->GetGridDim() / ( NODE_DIM ) ) );
	for( int x = pX; x < (int)( pX + ( newNode->GetMapNode()->GetGridDim() ) / ( NODE_DIM ) ); x++ )
	{
		for( int y = pY; y < (int)( pY +  ( newNode->GetMapNode()->GetGridDim() ) / ( NODE_DIM ) ); y++ )
		{
			if( mBuildMap[x][y] != nullptr )
			{
				return false;
			}
		}
	}

	for( int x = pX; x < (int)( pX + ( newNode->GetMapNode()->GetGridDim() ) / ( NODE_DIM ) ); x++ )
	{
		for( int y = pY; y < (int)( pY + ( newNode->GetMapNode()->GetGridDim() ) / ( NODE_DIM ) ); y++ )
		{
			mBuildMap[x][y] = newNode;
		}
	}
	XMFLOAT3 newPos = XMFLOAT3( ((float)pX * NODE_DIM ) - ( mMap->GetMapHalfWidth() * NODE_DIM ), 0, ( (float)pY * NODE_DIM ) - ( mMap->GetMapHalfHeight() * NODE_DIM ) );

	newNode->SetPos( newPos );

	return true;
}
void MapNodePlacer::BuildMap( MapNodeInstance** map )
{
	for( int x = 0; x < (int)mMap->GetMapDim(); x++ )
	{
		for( int y = 0; y < (int)mMap->GetMapDim(); y++ )
		{
			//Check "Tier", lastNode, chances of boss arena and/or energy position.
			MapNodeInstance* newNode = MapNodeManager::GetInstance()->GetNodes()[NodeTypes::BOSS_NODE][0]->GetMapNodeInstance();
			if(MAX_NODES < mNrOfNodes)
			{
				return;
			}
			if( CanPlace( x, y, newNode ) )
			{
				map[mNrOfNodes++] = newNode;
			}
			else
			{
				newNode->ReleaseInstance();
			}
		}
	}
	for( int y = 0; y < (int)mMap->GetMapDim(); y++ )
	{
		for( int x = 0; x < (int)mMap->GetMapDim(); x++ )
		{
			if(mBuildMap[x][y] != nullptr )
			{
				printf(" NODE ");
			}
			else
			{
				printf(" NULL ");
			}
		}
		printf("\n");
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