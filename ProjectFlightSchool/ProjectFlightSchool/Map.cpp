#include "Map.h"
#include "MapNodeManager.h"
#include "MapNodePlacer.h"
#include "HelperFunctions.h"

HRESULT Map::Render( float deltaTime, Player* player )
{
	std::vector<MapNodeInstance*> mapNodes;
	int playerPosX = (int)player->GetPosition().x;
	int playerPosY = (int)player->GetPosition().z;

	int playerX = ( ( (int)GetMapHalfWidth() * NODE_DIM ) + playerPosX ) / NODE_DIM;
	int playerZ = ( ( (int)GetMapHalfHeight() * NODE_DIM ) + playerPosY ) / NODE_DIM;

	for( int x = playerX - 1; x < playerX + 2; x++ )
	{
		for( int z = playerZ - 1; z < playerZ + 2; z++ )
		{
			MapNodeInstance* temp = MapNodePlacer::GetInstance()->GetNodeInstance( x, z );
			if( temp && std::find( mapNodes.begin(), mapNodes.end(), temp ) == mapNodes.end() )
			{
				mapNodes.push_back( temp );
			}
		}
	}

	//for( auto& it : mapNodes )
	//{
	//	it->Render( deltaTime );
	//}


	//std::vector<MapNodeInstance*> nodes = MapNodePlacer::GetInstance()->GetAllNodes();
	//for( auto& it : nodes )
	//{
	//	it->Render( deltaTime );
	//}

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
	}
}

NavTriangle* Map::IsOnNavMesh( XMFLOAT3 pos )
{
	XMFLOAT2 tempPos = XMFLOAT2(pos.x, pos.z);
	XMFLOAT2 p0, p1, p2;

	//for( auto& it : mNavData )
	//{
	//	p0 = XMFLOAT2( it.triPoints[0].x, it.triPoints[0].z );
	//	p1 = XMFLOAT2( it.triPoints[1].x, it.triPoints[1].z ); 
	//	p2 = XMFLOAT2( it.triPoints[2].x, it.triPoints[2].z ); 

	//	if( HelperFunctions::Inside2DTriangle( tempPos, p0, p1, p2 ) )
	//	{
	//		OutputDebugStringA("Player is inside bounds.\n");
	//		return &it;
	//	}
	//}
	OutputDebugStringA("Player is outside of map!\n");
	return nullptr;


	//p0 = XMFLOAT2( -10, -10 );
	//p1 = XMFLOAT2( -10, 10 ); 
	//p2 = XMFLOAT2( 10, 0 ); 

	//if( Inside2DTriangle( tempPos, p0, p1, p2 ) )
	//{
	//	OutputDebugStringA("Player is inside bounds.\n");
	//	return true;
	//}
	//OutputDebugStringA("Player is outside of map!\n");
	//return false;
}
//bool Map::Inside2DTriangle( XMFLOAT2 p, XMFLOAT2 p0, XMFLOAT2 p1, XMFLOAT2 p2 )
//{
//	float A = (float)1/2 * (-p1.y * p2.x + p0.y * (-p1.x + p2.x) + p0.x * (p1.y - p2.y) + p1.x * p2.y);
//    int sign = A < 0 ? -1 : 1;
//    float s = (p0.y * p2.x - p0.x * p2.y + (p2.y - p0.y) * p.x + (p0.x - p2.x) * p.y) * sign;
//    float t = (p0.x * p1.y - p0.y * p1.x + (p0.y - p1.y) * p.x + (p1.x - p0.x) * p.y) * sign;
//
//	return s > 0 && t > 0 && (s + t) < ( 2 * A * sign );
//}
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
	//MapNodeManager::GetInstance()->CreateNode( "../Content/Assets/Nodes/testTile.lp" );
	
	return S_OK;
}
void Map::Release()
{
	for( UINT i = 0; i < mNrOfNodes; i++ )
	{

	}
	MapNodePlacer::GetInstance()->Release();
	MapNodeManager::GetInstance()->Release();
}
Map::Map()
{
}
Map::~Map()
{
}