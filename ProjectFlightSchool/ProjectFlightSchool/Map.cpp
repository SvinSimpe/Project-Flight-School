#include "Map.h"
#include "MapNodeManager.h"
#include "MapNodePlacer.h"

HRESULT Map::Render( float deltaTime, Player* player )
{
	
	MapSection* mapNodes[10];
	int count = 0;
	BoundingCircle* c = player->GetBoundingCircle();
	printf( "Center point is: %f | %f | %f\n", c->center.x, c->center.y, c->center.z );
	mMapSection->GetSectionContainingUnit( mapNodes, count, player->GetBoundingBox() );

	for( int i = 0; i < count; i++ )
	{
		mapNodes[i]->Render( deltaTime );
	}

	return S_OK;
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
HRESULT Map::Initialize( UINT mapDim )
{
	//Map size is mapDim* mapDim
	mMapDim = mapDim;
	MapNodeManager::GetInstance()->Initialize( "../Content/Assets/Nodes/gridtest2.lp"  );

	MapNodePlacer::GetInstance()->Initialize( this );
	nodes = MapNodePlacer::GetInstance()->BuildMap();
	mNrOfNodes = MapNodePlacer::GetInstance()->GetNrOfNodes();
	
	//MapSection::SetUpIndices();

	mMapSection = new MapSection();

	mMapSection->Initialize( this, nullptr, nodes, 0 );

	for( int i = 0; i < mNrOfNodes; i++ )
	{
		mMapSection->AddNodeToSection( nodes[i] );
	}
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