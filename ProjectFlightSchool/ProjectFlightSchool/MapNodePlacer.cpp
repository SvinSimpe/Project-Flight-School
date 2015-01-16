#include "MapNodePlacer.h"
#include "MapNodeManager.h"

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
//bool MapNodePlacer::canPlace( int left, int top, MapNode current )
//{
//	//if ( ( node.GetGridWidth() <= ( mMap->GetMapWidth() - left ) ) && ( node.GetGridHeight() <= ( mMap->GetMapHeight() - top ) ) )
//	//{
//	//	for ( int i = 0; i < 2; i++ )
//	//	{
//	//		for ( int x = left; x < left + (int)node.GetGridWidth(); x++ )
//	//		{
//	//			int dY = top + ( i * ( node.GetGridWidth() - 1 ) );
//	//			if ( mBuildMap[x][dY] != 0 )
//	//			{
//	//				return false;
//	//			}
//	//		}
//	//		for ( int y = top; y < top + (int)node.GetGridHeight(); y++ )
//	//		{
//	//			int dX = left + ( i * ( node.GetGridHeight() - 1 ) );
//	//			if ( mBuildMap[dX][y] != 0 )
//	//			{
//	//				return false;
//	//			}
//	//		}
//	//	}
//	//	for (int y = top; y < top + (int)node.GetGridHeight(); y++)
//	//	{
//	//		for (int x = left; x < left + (int)node.GetGridWidth(); x++)
//	//		{
//	//			mBuildMap[x][y] = 1;
//	//		}
//	//	}
//	//	return true;
//	//}
//	//else
//	//{
//	//	int debug = mMap->GetMapWidth() - left;
//	//	int debug2 = mMap->GetMapHeight() - top;
//	//}
//	//return false;
//
//
//
//	return false;
//}

//Return codes: -1 no exit found; 0 exit found and match was made; 1 exit found but no match
int MapNodePlacer::CanPlace(MapNodeInstance* currentNode, MapNodeInstance* newNode)
{
	ExitPoint*	currExits	= currentNode->GetExits();
	ExitPoint*	newExits	= newNode->GetExits();

	//Corners currCorners		= currentNode->GetCorners();



	////Check for match, if match place, else rotate
	int exit = currentNode->HasExit();

	int result = -1;

	//Check if exit exists
	if( exit != -1 )
	{
		//check for opposite side exit on the new node
		int oppositeSide = ( exit + 2 ) % 4;

		//Match found
		if( newExits[oppositeSide].valid )
		{
			//Place node!

			//currentNode->AddNeighbour( exit, newNode );
			//newNode->AddNeighbour( oppositeSide, currentNode );

			XMFLOAT3 newPos = XMFLOAT3( ( currentNode->GetPos().x + ( currExits[exit].exit.x - newExits[oppositeSide].exit.x ) ), 
										( currentNode->GetPos().y + ( currExits[exit].exit.y - newExits[oppositeSide].exit.y ) ),
										( newNode->GetPos().z + ( currExits[exit].exit.z - newExits[oppositeSide].exit.z ) ) );
			newNode->SetPos( newPos );

			for( int i = 0; i < mNrOfNodes )

			result = 0;
		}
		else
		{
			result = 1;
		}
	}
	//for( int i = 0; i < newNode.GetMapNode()->GetNrOfExits(); i++ )
	//{
	//	Corners newCorners = newNode.GetCorners();
	//	for( int j = 0; j < currentNode->GetMapNode()->GetNrOfExits; j++ )
	//	{
	//		//no neighbour
	//		if( currExits[j].neighbour == nullptr )
	//		{
	//			//Check to place node on any exitpoint
	//			//Lefthand exit or righthand exit
	//			if( currCorners.top < currExits[j].exit.z && currExits[j].exit.z < currCorners.bottom )
	//			{
	//				//Righthand exit
	//				if( currentNode->GetPos().x < currExits[j].exit.x )
	//				{
	//				}
	//				//Lefthand exit
	//				else
	//				{
	//				}
	//			}
	//			//Top exit or bottom exit
	//			else if( currCorners.left < currExits[j].exit.x && currExits[j].exit.x < currCorners.right )
	//			{
	//				//Bottom exit
	//				if( currentNode->GetPos().z < currExits[j].exit.z )
	//				{
	//				}
	//				//Top exit
	//				else
	//				{
	//				}
	//			}
	//		}
	//	}
	//}
	return result;
}
MapNodeInstance** MapNodePlacer::PlaceNodes()
{
	int maxNodes	= 100;
	mNrOfNodes	= 0;
	bool nodeFull	= false;
	MapNode* nodes = MapNodeManager::GetInstance()->GetNodes();
	MapNodeInstance* startNode = nodes[0].GetMapNodeInstance();
	int currentNode = 0;
	startNode->SetPos( XMFLOAT3( 0, 0, 0 ) );

	mPlacedNodes[0] = startNode;
	mNrOfNodes++;

	while( mNrOfNodes < maxNodes )
	{
		nodeFull	= false;
		printf("Loop counter: %d\n", mNrOfNodes);
		while( !nodeFull )
		{
			//Idea: Inject currentNode-Type into NodeManager, nodeManager returns potential nodes according to rules


			//Aquire instance
			MapNodeInstance* newNode = nodes[0].GetMapNodeInstance();

			switch( CanPlace( mPlacedNodes[currentNode], newNode ) )
			{
			case -1:
				//Go to next node in the placednodes array
				newNode->ReleaseInstance();
				currentNode++;
				nodeFull = true;
				break;
			case 0:
				//newNode got placed

				mPlacedNodes[mNrOfNodes++] = newNode;
				break;
			case 1:
				//newNode didnt fit currentnode, either rotate or newnode
				//No nodes atm, so this is bork.
				//break the loop
				newNode->ReleaseInstance();
				mNrOfNodes = 100;
				break;
			case 2:
				//Node couldnt get placed, neighbour existed
				break;
			}
		}
	}
	//mBuildMap = new int*[mMap->GetMapHeight()];
	//for (int y = 0; y < (int)mMap->GetMapHeight(); y++)
	//{
	//	for (int x = 0; x < (int)mMap->GetMapWidth(); x++)
	//	{
	//		mBuildMap[x] = new int[mMap->GetMapHeight()];
	//	}
	//}
	//for (int y = 0; y < (int)mMap->GetMapHeight(); y++)
	//{
	//	for (int x = 0; x < (int)mMap->GetMapWidth(); x++)
	//	{
	//		mBuildMap[x][y] = 0;
	//	}
	//}

	//for (int y = 0; y < (int)mMap->GetMapHeight(); y++)
	//{
	//	for (int x = 0; x < (int)mMap->GetMapWidth(); x++)
	//	{
	//		if ( canPlace( x, y, nodes[0] ) )
	//		{
	//			MapNodeInstance mapNodeInst = nodes[0].GetMapNodeInstance();
	//			mapNodeInst.SetCorners( x, y );
	//			XMFLOAT3 origin;
	//			origin.x = ((float)x - mMap->GetMapHalfWidth());
	//			origin.y = 0.0f;
	//			origin.z = ((float)y - mMap->GetMapHalfHeight());
	//			mapNodeInst.SetOrigin( origin );
	//			placedNodes.push_back( mapNodeInst );
	//		}
	//	}
	//}

	//for (int y = 0; y < (int)mMap->GetMapHeight(); y++)
	//{
	//	for (int x = 0; x < (int)mMap->GetMapWidth(); x++)
	//	{
	//		printf("[%d]", mBuildMap[x][y]);
	//	}
	//	printf("\n");
	//}

	//while( nrOfNodes < maxNodes )
	//{
	//	for (int i = 0; i < 4; i++)
	//	{

	//	}
	//}
	//for (int i = 0; i < 4; i++)
	//{
	//	currentNode;
	//}
	for( int i = 0; i < 100; i++ )
	{
		printf(" Node position is: %f | %f | %f \n", placedNodes[i]->GetPos().x, placedNodes[i]->GetPos().y, placedNodes[i]->GetPos().z );
	}
	return placedNodes;
}
HRESULT	MapNodePlacer::Initialize( Map* map )
{
	mMap = map;
	mPlacedNodes = new MapNodeInstance*[100];
	//mBuildMapSize = mMap->GetMapWidth() * mMap->GetMapHeight();
	//for (int y = 0; y < (int)mMap->GetMapHeight(); y++)
	//{
	//	for (int x = 0; x < (int)mMap->GetMapWidth(); x++)
	//	{
	//		mBuildMap[x] = new int[mMap->GetMapHeight()];
	//	}
	//}
	//for (int y = 0; y < (int)mMap->GetMapHeight(); y++)
	//{
	//	for (int x = 0; x < (int)mMap->GetMapWidth(); x++)
	//	{
	//		mBuildMap[x][y] = 0;
	//	}
	//}
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
}
MapNodePlacer::~MapNodePlacer()
{
}