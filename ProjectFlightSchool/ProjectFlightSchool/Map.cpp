#include "Map.h"
#include "MapNodeManager.h"
#include "MapNodePlacer.h"

HRESULT Map::Render( float deltaTime, Player* player )
{
	
	//MapSection* mapNodes[20];
	//int count = 0;
	//mMapSection->GetSectionContainingUnit( mapNodes, count, player->GetBoundingBox() );

	//for( int i = 0; i < count; i++ )
	//{
	//	mapNodes[i]->Render( deltaTime );
	//}
	for( int i = 0; i < MapNodePlacer::GetInstance()->GetNrOfNodes(); i++ )
	{
		mNodes[i]->Render( deltaTime );
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
	MapNodeManager::GetInstance()->Initialize( "../Content/Assets/Nodes/test11.lp"  );

	MapNodePlacer::GetInstance()->Initialize( this );
	MapNodePlacer::GetInstance()->BuildMap( mNodes );
	mNrOfNodes = MapNodePlacer::GetInstance()->GetNrOfNodes();
	
	//MapSection::SetUpIndices();

	mMapSection = new MapSection();

	mMapSection->Initialize( this, nullptr, mNodes, 0 );

	for( int i = 0; i < mNrOfNodes; i++ )
	{
		mMapSection->AddNodeToSection( mNodes[i] );
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
	mMapSection->Release();
	SAFE_DELETE( mMapSection );

	MapNodePlacer::GetInstance()->Release();
	MapNodeManager::GetInstance()->Release();

}
Map::Map()
{
}
Map::~Map()
{
}