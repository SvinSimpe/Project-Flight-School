#include "Map.h"


HRESULT Map::Render( float deltaTime )
{
	for( UINT i = 0; i < mMapDim * mMapDim; i++ )
	{
		mMapSections[i].Render( deltaTime );
	}
	return S_OK;
}
HRESULT Map::Initialize( float vertexSpacing, UINT mapDim )
{
	mVertexSpacing = vertexSpacing;
	mMapDim = mapDim;
	
	MapSection::SetUpIndices();

	mMapSections = new MapSection[mMapDim * mMapDim];

	for( UINT i = 0; i < mMapDim * mMapDim; i++ )
	{
		
		mMapSections[i].Initialize( i, mVertexSpacing, mMapDim );
	}
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