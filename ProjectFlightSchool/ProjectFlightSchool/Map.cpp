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
	for( int i = 0; i < (int)MapNodePlacer::GetInstance()->GetNrOfNodes(); i++ )
	{
		mNodes[i]->Render( deltaTime );
	}

	return S_OK;
}

void Map::OnLoadLevel( IEventPtr pEvent )
{
	if( Event_Load_Level::GUID == pEvent->GetEventType() )
	{
		std::shared_ptr<Event_Load_Level> dataPtr = std::static_pointer_cast<Event_Load_Level>( pEvent );

		MapNodeManager::GetInstance()->LoadLevel( dataPtr->GetFileName() );

		MapNodePlacer::GetInstance()->Reset();

		MapNodePlacer::GetInstance()->BuildMap( mNodes );

		if( mMapSection )
		{
			mMapSection->Release();
			delete mMapSection;
		}
		mNrOfNodes = MapNodePlacer::GetInstance()->GetNrOfNodes();

		mMapSection = new MapSection();

		mMapSection->Initialize( this, nullptr, mNodes, 0 );

		for( int i = 0; i < (int)mNrOfNodes; i++ )
		{
			mMapSection->AddNodeToSection( mNodes[i] );
		}
	}
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
	MapNodeManager::GetInstance()->Initialize();
	MapNodePlacer::GetInstance()->Initialize( this );

	EventManager::GetInstance()->AddListener( &Map::OnLoadLevel, this, Event_Load_Level::GUID );

	//"../Content/Assets/Nodes/test11.lp"
	
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