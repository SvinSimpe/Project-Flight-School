#include "Map.h"
#include "MapNodeManager.h"
#include "MapNodePlacer.h"
#include "NodeGraph.h"
#include "HelperFunctions.h"

HRESULT Map::Render( float deltaTime, Player* player )
{
	mEnvironmentTimer += deltaTime;
	if( mEnvironmentTimer > 6.283f )
		mEnvironmentTimer-= 6.283f;

	std::vector<MapNodeInstance*> mapNodes;
	int playerPosX = (int)player->GetPosition().x;
	int playerPosY = (int)player->GetPosition().z;

	int playerX = ( ( (int)GetMapHalfWidth() * NODE_DIM ) + playerPosX ) / NODE_DIM;
	int playerZ = ( ( (int)GetMapHalfHeight() * NODE_DIM ) + playerPosY ) / NODE_DIM;

	for( int x = playerX - 1; x < playerX + 2; x++ )
	{
		for( int z = playerZ - 1; z < playerZ + 2; z++ )
		{
			MapNodeInstance* temp = GetNodeInstance( x, z );
			if( temp && std::find( mapNodes.begin(), mapNodes.end(), temp ) == mapNodes.end() )
			{
				mapNodes.push_back( temp );
			}
		}
	}

	for( int x = 0; x < (int)mMapDim; x++ )
	{
		for( int z = 0; z < (int)mMapDim; z++ )
		{
			MapNodeInstance* temp = GetNodeInstance( x, z );
			if( temp && std::find( mapNodes.begin(), mapNodes.end(), temp ) == mapNodes.end() )
			{
				temp->ResetLights();
			}
		}
	}

	for( auto& it : mapNodes )
	{
		it->Render( mEnvironmentTimer );
	}
	return S_OK;
}

void Map::OnLoadLevel( IEventPtr pEvent )
{
	if( Event_Load_Level::GUID == pEvent->GetEventType() )
	{
		std::shared_ptr<Event_Load_Level> dataPtr = std::static_pointer_cast<Event_Load_Level>( pEvent );

		MapNodeManager::GetInstance()->LoadLevel( dataPtr->FilePath() );

		MapNodePlacer::GetInstance()->Reset();

		MapNodePlacer::GetInstance()->BuildMap( mBuildMap );

		mNrOfNodes = MapNodePlacer::GetInstance()->GetNrOfNodes();
	}
}

bool Map::PlayerVsMap( XMFLOAT3 position, XMFLOAT3 &normal )
{
	int playerX = ( ( (int)GetMapHalfWidth() * NODE_DIM ) + (int)floorf( position.x ) ) / NODE_DIM;
	int playerZ = ( ( (int)GetMapHalfHeight() * NODE_DIM ) + (int)floorf( position.z ) ) / NODE_DIM;

	bool localReturn = false;

	for( int x = playerX - 1; x < playerX + 2; x++ )
	{
		for( int z = playerZ - 1; z < playerZ + 2; z++ )
		{
			MapNodeInstance* temp = GetNodeInstance( x, z );

			if( temp )
			{
				MapNode* node = temp->GetMapNode();

				for( UINT i = 0; i < node->mStaticAssetCount; i++ )
				{
					if( node->mStaticAssets[i].GetCollisionType() != NONE_COLLISION )
					{
						OctTree* tree	= Graphics::GetInstance()->GetOctTreeFromStatic3DAsset( node->mStaticAssets[i].GetAssetID() );

						XMMATRIX objMat	= XMLoadFloat4x4( &node->mStaticAssets[i].mWorld );

						XMVECTOR translate, scale, rotation;
						XMMatrixDecompose( &scale, &rotation, &translate, objMat );

						translate		= XMLoadFloat3( &XMFLOAT3( XMVectorGetX( translate ), XMVectorGetY( translate ), -XMVectorGetZ( translate ) ) );
						rotation		= XMLoadFloat4( &XMFLOAT4( -XMVectorGetX( rotation ), -XMVectorGetY( rotation ), XMVectorGetZ( rotation ), XMVectorGetW( rotation ) ) );

						objMat			= XMMatrixAffineTransformation( scale, XMVectorZero(), rotation, translate );
						objMat			= objMat * XMMatrixTranslationFromVector( XMLoadFloat3( &temp->GetOrigin() ) );
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


						//////// Collision check
						XMFLOAT3 collisionNormal = XMFLOAT3( 0.0f, 1.0f, 0.0f );
						bool collision = false;
						if( tree )
							collision = tree->AABBvsAABB( playerMin, playerMax, 4, collisionNormal );

						// normal from objspace to worldspace.
						XMStoreFloat3( &collisionNormal, XMVector3TransformNormal( XMLoadFloat3( &collisionNormal ), objMat ) );
						collisionNormal.y	= 0.0f;
						XMVECTOR loaded		= XMLoadFloat3( &collisionNormal );
						XMStoreFloat3( &collisionNormal, XMVector3Normalize( loaded ) );

						XMFLOAT4X4 store;
						XMStoreFloat4x4( &store, XMMatrixTranspose( objMat ) );

						if( collision )
						{
							localReturn = true;
							normal		= collisionNormal;

							////////////////////////////////////////////
							// DEBUG RENDERING FOLLOWS, PLEASE KEEP FOR NOW!
							//RenderManager::GetInstance()->AddBoxToList( playerMin, playerMax, store );
							//XMFLOAT3 playerStart;
							//XMFLOAT3 playerEnd;
							//XMStoreFloat3( &playerStart, XMLoadFloat3( &position ) + XMLoadFloat3( &XMFLOAT3( 0.0f, 3.0f, 0.0f ) ) );
							//XMStoreFloat3( &playerEnd, XMLoadFloat3( &playerStart ) + XMLoadFloat3( &collisionNormal ) * 2.0f );
							//RenderManager::GetInstance()->AddLineToList( playerStart, playerEnd );

							//for( int i = 0; i < 8; i++ )
							//{
							//	if( tree->childrenCollides[i] )
							//	{
							//		for( int j = 0; j < 8; j++ )
							//		{
							//			if( tree->children[i]->childrenCollides[j] )
							//			{
							//				for( int k = 0; k < 8; k++ )
							//				{
							//					if( tree->children[i]->children[j]->childrenCollides[k] )
							//					{
							//						RenderManager::GetInstance()->AddBoxToList( tree->children[i]->children[j]->children[k]->boundingBox.min,
							//																			tree->children[i]->children[j]->children[k]->boundingBox.max, store );
							//						//for( int l = 0; l < 8; l++ )
							//						//{
							//						//	if( tree->children[i]->children[j]->children[k]->childrenCollides[l] )
							//						//	{
							//						//		RenderManager::GetInstance()->AddBoxToList( tree->children[i]->children[j]->children[k]->children[l]->boundingBox.min,
							//						//													tree->children[i]->children[j]->children[k]->children[l]->boundingBox.max, store );
							//						//	}
							//						//}
							//					}
							//				}
							//			}
							//		}
							//	}
							//}
						}
					}
				}
			}
		}
	}

	return localReturn;
}

bool Map::BulletVsMap( XMFLOAT3 position, XMFLOAT3 &normal )
{
	int playerX = ( ( (int)GetMapHalfWidth() * NODE_DIM ) + (int)floorf( position.x ) ) / NODE_DIM;
	int playerZ = ( ( (int)GetMapHalfHeight() * NODE_DIM ) + (int)floorf( position.z ) ) / NODE_DIM;

	bool localReturn = false;

	MapNodeInstance* temp = GetNodeInstance( playerX, playerZ );

	if( temp )
	{
		MapNode* node = temp->GetMapNode();

		for( UINT i = 0; i < node->mStaticAssetCount; i++ )
		{
			if( node->mStaticAssets[i].GetCollisionType() != NONE_COLLISION  )
			{
				OctTree* tree	= Graphics::GetInstance()->GetOctTreeFromStatic3DAsset( node->mStaticAssets[i].GetAssetID() );

				XMMATRIX objMat	= XMLoadFloat4x4( &node->mStaticAssets[i].mWorld );

				XMVECTOR translate, scale, rotation;
				XMMatrixDecompose( &scale, &rotation, &translate, objMat );

				translate		= XMLoadFloat3( &XMFLOAT3( XMVectorGetX( translate ), XMVectorGetY( translate ), -XMVectorGetZ( translate ) ) );
				rotation		= XMLoadFloat4( &XMFLOAT4( -XMVectorGetX( rotation ), -XMVectorGetY( rotation ), XMVectorGetZ( rotation ), XMVectorGetW( rotation ) ) );

				objMat			= XMMatrixAffineTransformation( scale, XMVectorZero(), rotation, translate );
				objMat			= objMat * XMMatrixTranslationFromVector( XMLoadFloat3( &temp->GetOrigin() ) );
				XMMATRIX objInv = XMMatrixInverse( nullptr, objMat );

				/////////////// Player to objspace
				XMFLOAT3 playerPosInObjSpace;
				XMStoreFloat3( &playerPosInObjSpace, XMVector3TransformCoord( XMLoadFloat3( &position ), objInv ) );

				float xScale = 1.0f / XMVectorGetX( scale );
				float yScale = 1.0f / XMVectorGetY( scale );
				float zScale = 1.0f / XMVectorGetZ( scale );

				XMFLOAT3 playerMin = XMFLOAT3(	playerPosInObjSpace.x - 0.1f * xScale,
												playerPosInObjSpace.y - 0.1f * yScale,
												playerPosInObjSpace.z - 0.1f * zScale );

				XMFLOAT3 playerMax = XMFLOAT3(	playerPosInObjSpace.x + 0.1f * xScale,
												playerPosInObjSpace.y + 0.1f * yScale, 
												playerPosInObjSpace.z + 0.1f * zScale );


				//////// Collision check
				XMFLOAT3 collisionNormal = XMFLOAT3( 0.0f, 1.0f, 0.0f );
				bool collision = false;
				if( tree )
					collision = tree->AABBvsAABB( playerMin, playerMax, 4, collisionNormal );

				// normal from objspace to worldspace.
				XMStoreFloat3( &collisionNormal, XMVector3TransformNormal( XMLoadFloat3( &collisionNormal ), objMat ) );
				collisionNormal.y	= 0.0f;
				XMVECTOR loaded		= XMLoadFloat3( &collisionNormal );
				XMStoreFloat3( &collisionNormal, XMVector3Normalize( loaded ) );

				//XMFLOAT4X4 store;
				//XMStoreFloat4x4( &store, XMMatrixTranspose( objMat ) );

				if( collision )
				{
					localReturn = true;
					normal		= collisionNormal;
				}
			}
		}
	}
	return localReturn;
}

//std::vector<DirectX::XMFLOAT2> Map::GetPath( XMFLOAT3 start, XMFLOAT3 goal )
//{
//	std::vector<Node*> path;
//	std::vector<DirectX::XMFLOAT2> path1;
//	std::vector<DirectX::XMFLOAT2> temp;
//	MapNodeInstance* currentNode;
////	MapNodeInstance* nextNode;
//
//	Navmesh* currentNavMesh;
//
//	DirectX::XMFLOAT3 newStart, newEnd, newCenter;
//
//	int startX = (int)( (GetMapHalfWidth() * NODE_DIM )  + start.x ) / NODE_DIM;
//	int startZ = (int)( (GetMapHalfHeight() * NODE_DIM )  + start.z ) / NODE_DIM;
//
//	int goalX = (int)( (GetMapHalfWidth() * NODE_DIM )  + goal.x ) / NODE_DIM;
//	int goalZ = (int)( (GetMapHalfHeight() * NODE_DIM )  + goal.z ) / NODE_DIM;
//
//	int currX = startX;
//	int currZ = startZ;
//
//	MapNodeInstance* startNode = GetNodeInstance( startX, startZ );
//	MapNodeInstance* goalNode = GetNodeInstance( goalX, goalZ );
//
//	newStart = start;
//	path1.push_back( DirectX::XMFLOAT2( start.x, start.z ) );
//
//	currentNode = GetNodeInstance( startX, startZ );
//
//	if( startNode && goalNode )
//	{
//		path = nodeGraph->FindPath( start, goal, startNode->GetNodeID(), goalNode->GetNodeID() );
//
//		//for( int i = 0; i < (int)path.size() - 1; i++ )
//		//{
//		while( currentNode->GetNodeID() != goalNode->GetNodeID() )
//		{
//			//currentNode = GetNodeInstance( path[i]->mNodePos.x, path[i]->mNodePos.y );
//
//			if( currentNode )
//			{
//				currentNavMesh = currentNode->GetNavMesh();
//
//				//newCenter = HelperFunctions::GetLineCenter( start, goal );
//
//				newEnd = currentNode->GetClosestEdgePoint( newStart, goal );
//
//				temp = currentNavMesh->FindPath( newStart, newEnd );
//
//				for( auto& it : temp )
//				{
//					path1.push_back( it );
//				}
//				newStart = newEnd;
//
//				
//				currX = (int)( (GetMapHalfWidth() * NODE_DIM )  + newStart.x ) / NODE_DIM;
//				currZ = (int)( (GetMapHalfHeight() * NODE_DIM )  + newStart.z ) / NODE_DIM;
//
//				currentNode = GetNodeInstance( currX, currZ );
//			}
//			else
//			{
//				break;
//			}
//		}
//		//}
//
//		//Last subpath
//		currX = (int)( (GetMapHalfWidth() * NODE_DIM )  + newStart.x ) / NODE_DIM;
//		currZ = (int)( (GetMapHalfHeight() * NODE_DIM )  + newStart.z ) / NODE_DIM;
//
//		currentNode = GetNodeInstance( currX, currZ );
//
//		if( currentNode )
//		{
//			currentNavMesh = currentNode->GetNavMesh();
//		
//			temp = currentNavMesh->FindPath( newStart, goal );
//
//			for( auto& it : temp )
//			{
//				path1.push_back( it );
//			}
//		}
//	}
//
//	return path1;
//}
UINT Map::GetMapDim() const
{
	return mMapDim ;
}

UINT Map::GetMapWidth() const
{
	return mMapDim ;
}

UINT Map::GetMapHeight() const
{
	return mMapDim ;
}

UINT Map::GetMapHalfWidth() const
{
	return mMapDim / 2;
}

UINT Map::GetMapHalfHeight() const
{
	return mMapDim / 2;
}

float Map::GetHeight( DirectX::XMFLOAT3 pos )
{
	DirectX::XMFLOAT3 corrPos;
	DirectX::XMFLOAT3 corrNodePos;

	float x = ( pos.x + GetMapHalfWidth() * NODE_DIM );
	float z = ( pos.z + GetMapHalfWidth() * NODE_DIM );

	int pX = (int)floorf( x / NODE_DIM );
	int pZ = (int)floorf( z / NODE_DIM );

	MapNodeInstance* temp = GetNodeInstance( pX, pZ );
	if( temp )
	{
		corrNodePos.x = temp->GetPos().x + ( GetMapHalfWidth() * NODE_DIM );
		corrNodePos.z = temp->GetPos().z + ( GetMapHalfHeight() * NODE_DIM );

		corrPos.x = x - corrNodePos.x;
		corrPos.z = z - corrNodePos.z;

		return temp->GetMapNode()->GetHeight( corrPos );
	}

	return 0.0f;
}

UINT Map::GetNrOfNodes() const
{
	return mNrOfNodes;
}

MapNodeInstance*** Map::GetNodeMap() const
{
	return mBuildMap;
}

MapNodeInstance* Map::GetNodeInstance( int x, int z )
{
	MapNodeInstance* result = nullptr;
	if( ( x < (int)mMapDim && 0 <= x ) &&
		( z < (int)mMapDim && 0 <= z ) )
	{
		result = mBuildMap[x][z];
	}
	return result;
}

HRESULT Map::Initialize( UINT mapDim )
{
	//Map size is mapDim* mapDim

	mMapDim = mapDim;
	MapNodeManager::GetInstance()->Initialize();
	MapNodePlacer::GetInstance()->Initialize( this );

	EventManager::GetInstance()->AddListener( &Map::OnLoadLevel, this, Event_Load_Level::GUID );
	
	return S_OK;
}

void Map::Release()
{
	
	if( mBuildMap )
	{
		for( int i = 0; i < (int)mMapDim; i++ )
		{
			if( mBuildMap[i] )
				delete[] mBuildMap[i];
		}
		delete[] mBuildMap;
	}

	MapNodePlacer::GetInstance()->Release();
	MapNodeManager::GetInstance()->Release();
}

Map::Map()
{
	mEnvironmentTimer	= 0.0f;
}

Map::~Map()
{
}