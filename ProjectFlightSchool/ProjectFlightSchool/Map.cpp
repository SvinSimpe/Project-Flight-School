#include "Map.h"
#include "MapNodeManager.h"
#include "MapNodePlacer.h"

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

	for( auto& it : mapNodes )
	{
		it->Render( deltaTime );
	}


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

		MapNodeManager::GetInstance()->LoadLevel( dataPtr->FilePath() );

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

bool Map::PlayerVsMap( XMFLOAT3 position, XMFLOAT3 &normal )
{
	int x = ( ( (int)GetMapHalfWidth() * NODE_DIM ) + (int)floorf( position.x ) ) / NODE_DIM;
	int z = ( ( (int)GetMapHalfHeight() * NODE_DIM ) + (int)floorf( position.z ) ) / NODE_DIM;

	bool localReturn = false;

	for( int a = x - 1; a < x + 2; a++ )
	{
		for( int b = z - 1; b < z + 2; b++ )
		{
			MapNodeInstance* temp = MapNodePlacer::GetInstance()->GetNodeInstance( a, b );

			if( temp )
			{
				MapNode* node = temp->GetMapNode();

				for( UINT i = 0; i < node->mStaticAssetCount; i++ )
				{
					OctTree* tree	= Graphics::GetInstance()->GetOctTreeFromStatic3DAsset( node->mStaticAssets[i].GetAssetID() );

					XMMATRIX objMat	= XMLoadFloat4x4( &node->mStaticAssets[i].mWorld );

					XMVECTOR translate, scale, rotation;
					XMMatrixDecompose( &scale, &rotation, &translate, objMat );

					translate	= XMLoadFloat3( &XMFLOAT3( XMVectorGetX( translate ), XMVectorGetY( translate ), -XMVectorGetZ( translate ) ) );
					rotation	= XMLoadFloat4( &XMFLOAT4( -XMVectorGetX( rotation ), -XMVectorGetY( rotation ), XMVectorGetZ( rotation ), XMVectorGetW( rotation ) ) );

					objMat		= XMMatrixAffineTransformation( scale, XMVectorZero(), rotation, translate );
					objMat		= objMat * XMMatrixTranslationFromVector( XMLoadFloat3( &MapNodePlacer::GetInstance()->GetNodeInstance( a, b )->GetPos() ) );
					XMMATRIX objInv = XMMatrixInverse( nullptr, objMat );

					/////////////// Player to objspace
					XMFLOAT3 playerPosInObjSpace;
					XMStoreFloat3( &playerPosInObjSpace, XMVector3TransformCoord( XMLoadFloat3( &position ), objInv ) );

					float xScale = 1.0f / XMVectorGetX( scale );
					float yScale = 1.0f / XMVectorGetY( scale );
					float zScale = 1.0f / XMVectorGetZ( scale );

					XMFLOAT3 playerMin = XMFLOAT3(	playerPosInObjSpace.x - 0.4f * xScale,
													playerPosInObjSpace.y,
													playerPosInObjSpace.z - 0.4f * zScale );

					XMFLOAT3 playerMax = XMFLOAT3(	playerPosInObjSpace.x + 0.4f * xScale,
													playerPosInObjSpace.y + 2.0f * yScale, 
													playerPosInObjSpace.z + 0.4f * zScale );


					//////// collision check
					XMFLOAT3 collisionNormal;
					bool collision = tree->AABBvsAABB( playerMin, playerMax, 5, collisionNormal );

					XMStoreFloat3( &collisionNormal, XMVector3TransformNormal( XMLoadFloat3( &collisionNormal ), objMat ) );
					collisionNormal.y	= 0.0f;
					XMVECTOR loaded		= XMLoadFloat3( &collisionNormal );
					XMStoreFloat3( &collisionNormal, XMVector3Normalize( loaded ) );

					///////////////////////////////////////////
					// Draw objstuff
					XMFLOAT4X4 store;
					XMStoreFloat4x4( &store, XMMatrixTranspose( objMat ) );

					if( collision )
					{
						localReturn = true;
						normal		= collisionNormal;

						RenderManager::GetInstance()->AddBoxToList( playerMin, playerMax, store );
						XMFLOAT3 playerStart;
						XMFLOAT3 playerEnd;
						XMStoreFloat3( &playerStart, XMLoadFloat3( &position ) + XMLoadFloat3( &XMFLOAT3( 0.0f, 3.0f, 0.0f ) ) );
						XMStoreFloat3( &playerEnd, XMLoadFloat3( &playerStart ) + XMLoadFloat3( &collisionNormal ) * 2.0f );
						RenderManager::GetInstance()->AddLineToList( playerStart, playerEnd );

						for( int i = 0; i < 8; i++ )
						{
							if( tree->childrenCollides[i] )
							{
								for( int j = 0; j < 8; j++ )
								{
									if( tree->children[i]->childrenCollides[j] )
									{
										for( int k = 0; k < 8; k++ )
										{
											if( tree->children[i]->children[j]->childrenCollides[k] )
											{
												for( int l = 0; l < 8; l++ )
												{
													if( tree->children[i]->children[j]->children[k]->childrenCollides[l] )
													{
														RenderManager::GetInstance()->AddBoxToList( tree->children[i]->children[j]->children[k]->children[l]->boundingBox.min,
																									tree->children[i]->children[j]->children[k]->children[l]->boundingBox.max, store );
													}
												}
											}
										}
									}
								}
							}
						}
					}
				}
			}
		}
	}

	return localReturn;
}

NavTriangle* Map::IsOnNavMesh( XMFLOAT3 pos )
{
	XMFLOAT2 tempPos = XMFLOAT2(pos.x, pos.z);
	XMFLOAT2 p0, p1, p2;

	int unitPosX = (int)pos.x;
	int unitPosZ = (int)pos.z;

	int playerX = ( ( (int)GetMapHalfWidth() * NODE_DIM ) + unitPosX ) / NODE_DIM;
	int playerZ = ( ( (int)GetMapHalfHeight() * NODE_DIM ) + unitPosZ ) / NODE_DIM;

	MapNodeInstance* temp = MapNodePlacer::GetInstance()->GetNodeInstance( playerX, playerZ );

	if( temp )
		return temp->IsOnNavMesh( pos );
	else
		return nullptr;
	////for( auto& it : mNavData )
	////{
	////	p0 = XMFLOAT2( it.triPoints[0].x, it.triPoints[0].z );
	////	p1 = XMFLOAT2( it.triPoints[1].x, it.triPoints[1].z ); 
	////	p2 = XMFLOAT2( it.triPoints[2].x, it.triPoints[2].z ); 

	////	if( HelperFunctions::Inside2DTriangle( tempPos, p0, p1, p2 ) )
	////	{
	////		OutputDebugStringA("Player is inside bounds.\n");
	////		return &it;
	////	}
	////}
	//OutputDebugStringA("Player is outside of map!\n");
	//return nullptr;


	////p0 = XMFLOAT2( -10, -10 );
	////p1 = XMFLOAT2( -10, 10 ); 
	////p2 = XMFLOAT2( 10, 0 ); 

	////if( Inside2DTriangle( tempPos, p0, p1, p2 ) )
	////{
	////	OutputDebugStringA("Player is inside bounds.\n");
	////	return true;
	////}
	////OutputDebugStringA("Player is outside of map!\n");
	////return false;
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