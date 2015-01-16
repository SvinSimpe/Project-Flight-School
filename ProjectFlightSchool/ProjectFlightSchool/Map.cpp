#include "Map.h"
#include "MapNodeManager.h"
#include "MapNodePlacer.h"

HRESULT Map::Render( float deltaTime )
{
	for( int i = 0; i < 100; i++ )
	{
		nodes[i]->Render( deltaTime );
	}
	return S_OK;
}
UINT Map::GetMapDim() const
{
	return mMapDim * SECTION_DIM;
}
UINT Map::GetMapWidth() const
{
	return mMapDim * SECTION_DIM;
}
UINT Map::GetMapHeight() const
{
	return mMapDim * SECTION_DIM;
}
float Map::GetMapHalfWidth() const
{
	return ( (float)mMapDim * SECTION_DIM) * 0.5f;
}
float Map::GetMapHalfHeight() const
{
	return ( (float)mMapDim * SECTION_DIM) * 0.5f;
}
HRESULT Map::Initialize( float vertexSpacing, UINT mapDim )
{
	mVertexSpacing = vertexSpacing;
	mMapDim = mapDim;
	MapNodeManager::GetInstance()->Initialize( "../Content/Assets/Nodes/gridtest2.lp"  );

	MapNodePlacer::GetInstance()->Initialize( this );
	nodes = MapNodePlacer::GetInstance()->PlaceNodes();
	
	//MapSection::SetUpIndices();

	//mMapSections = new MapSection[mMapDim * mMapDim];

	//for( UINT i = 0; i < mMapDim * mMapDim; i++ )
	//{
	//	
	//	mMapSections[i].Initialize( i, mVertexSpacing, mMapDim );
	//}

	return S_OK;
}
void Map::Release()
{
	for( int i = 0; i < SECTION_DIM * SECTION_DIM; i++ )
	{
		//mMapSections[i].Release();
	}
	//delete[] mMapSections;
}
Map::Map()
{
}
Map::~Map()
{
}