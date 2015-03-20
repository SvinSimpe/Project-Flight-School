#include "NodeGraph.h"
#include "Map.h"
#include "RenderManager.h"
#include "HelperFunctions.h"
//---------------------Edge-----------------------

HRESULT Edge::Render()
{
	//DirectX::XMFLOAT3 start = From->centerPoint;
	//DirectX::XMFLOAT3 end = To->centerPoint;

	//RenderManager::GetInstance()->AddLineToList( start, end );



	return S_OK;
}

HRESULT Edge::Initialize( Node* A, Node* B )
{
	From	= A;
	To		= B;

	return S_OK;
}

Edge::Edge()
{
	From	= nullptr;
	To		= nullptr;
}

Edge::~Edge()
{
}
bool Edge::operator==( const Edge* origObj ) const
{
	return ( mEdgeID == origObj->mEdgeID );
}

//---------------------Node-----------------------
bool Node::AddEdge( Node* To )
{
	for( auto& it: mEdges )
	{
		if( it->To->mNodeID == To->mNodeID )
		{
			return false;
		}
	}

	Edge* newEdge = new Edge();
	newEdge->Initialize( this, To );
	mEdges.push_back( newEdge );

	return true;
}
HRESULT Node::Render()
{
	for( auto& it : mEdges )
	{
		it->Render();
	}
	return S_OK;
}

HRESULT Node::Initialize( TilePosition nodePos, int nodeID )
{
	mNodePos	= nodePos;
	mNodeID		= nodeID;

	return S_OK;
}

void Node::Release()
{
	for( auto& it : mEdges )
	{
		delete it;
	}
}

Node::Node()
{
	mNodeID = -1;

	mNodePos.x = -1;
	mNodePos.y = -1;
}

Node::~Node()
{
}

//-------------------NodeGraph--------------------

void NodeGraph::BuildGraph( Map* map )
{
	for( auto& it : mNodes )
	{
		//Find neighbours to current node
		int x = it->mNodePos.x;
		int y = it->mNodePos.y;
		MapNodeInstance* currentNode = map->GetNodeInstance( x, y );

		//For 4-dir connections
		//MapNodeInstance* top;
		//MapNodeInstance* left;
		//MapNodeInstance* bottom;
		//MapNodeInstance* right;

		MapNodeInstance* current;

		//calculate node size
		int sizeX = currentNode->GetNodeSizeX();
		int sizeY = currentNode->GetNodeSizeY();

		for( int i = 0; i < sizeX; i++ )
		{
			for( int j = 0; j < sizeY; j++ )
			{
				int xP = ( ( x + i ) - 1 );
				int yP = ( ( y + j ) - 1 );
				//Box-filter
				for( int p = 0; p < 3; p++ )
				{
					for( int q = 0; q < 3; q++ )
					{
						current = map->GetNodeInstance( xP + p, yP + q );
						if( current != nullptr )
						{
							int index = FindNodeByID( current->GetNodeID() );
							it->AddEdge( mNodes[index] );
							mNodes[index]->AddEdge( it );		
						}
					}
				}

				//For 4-dir connections
				//South, West, North, East
				//bottom = map->GetNodeInstance( x + i, ( y + j ) + 1 );
				//if( bottom != nullptr )
				//{
				//	int index = FindNodeByID( bottom->GetNodeID() );

				//	it->AddEdge( mNodes[index] );
				//	mNodes[index]->AddEdge( it );		
				//}

				//left = map->GetNodeInstance( ( x + i ) - 1, y + j );
				//if( left != nullptr )
				//{
				//	int index = FindNodeByID( left->GetNodeID() );

				//	it->AddEdge( mNodes[index] );
				//	mNodes[index]->AddEdge( it );		
				//}

				//top = map->GetNodeInstance( x + i, ( y + j ) - 1 );
				//if( top != nullptr )
				//{
				//	int index = FindNodeByID( top->GetNodeID() );

				//	it->AddEdge( mNodes[index] );
				//	mNodes[index]->AddEdge( it );		
				//}

				//right = map->GetNodeInstance( ( x + i ) + 1, y + j );
				//if( right != nullptr )
				//{
				//	int index = FindNodeByID( right->GetNodeID() );

				//	it->AddEdge( mNodes[index] );
				//	mNodes[index]->AddEdge( it );		
				//}
			}
		}
	}
}
int NodeGraph::FindNodeByID( int nodeID )
{
	for( int i = 0; i < (int)mNodes.size(); i++ )
	{
		if( mNodes[i]->mNodeID == nodeID )
			return i;
	}

	return -1;
}
HRESULT NodeGraph::Render()
{
	//for( auto& it : mNodes )
	//{
	//	it->Render();
	//}

	//if( !mFinishedPath.empty() )
	//{
	//	for( int i = 0; i < (int)mFinishedPath.size() - 1; i++ )
	//	{
	//		RenderManager::GetInstance()->AddLineToList( mFinishedPath[i]->centerPoint, mFinishedPath[i+1]->centerPoint );
	//	}
	//}
	return S_OK;
}
HRESULT NodeGraph::Initialize( Map* map )
{
	int width = map->GetMapWidth();
	int height = map->GetMapHeight();

	for( int i = 0; i < width; i++ )
	{
		for( int j = 0; j < height; j++ )
		{
			MapNodeInstance* currentNode = map->GetNodeInstance( i, j );
			std::vector<Node*>::iterator findIt = mNodes.end();
			if( currentNode )
			{
				for( auto it = mNodes.begin(); it != mNodes.end(); it++ )
				{
					if( (*it)->mNodeID == currentNode->GetNodeID() )
					{
						findIt = it;
						break;
					}
				}
				if( findIt == mNodes.end() )
				{
					Node* temp = new Node();
					temp->Initialize( TilePosition( i, j ), currentNode->GetNodeID() );
					temp->centerPoint = currentNode->GetOrigin();
					mNodes.push_back( temp );
				}
			}
		}
	}
	BuildGraph( map );
	return S_OK;
}

std::vector<Node*> NodeGraph::FindPath( DirectX::XMFLOAT3 startPos, DirectX::XMFLOAT3 endPos, int startNodeID, int endNodeID )
{
	mFinishedPath.clear();
	std::list<NodePath*> mOpenList;
	std::list<NodePath*> mClosedList;

	Node* startNode	= mNodes[FindNodeByID( startNodeID )];
	Node* endNode	= mNodes[FindNodeByID( endNodeID )];

	Node* currentNode = startNode;
	Node* newNode = nullptr;

	int count = 0;

	NodePath* currentPath = &mPath[count++];

	currentPath->current = startNode;
	currentPath->parent	= nullptr;

	TilePosition start	= startNode->mNodePos;
	TilePosition end	= endNode->mNodePos;
	TilePosition next;

	DirectX::XMVECTOR newVec, startVec, endVec, vecDist;

	startVec	= DirectX::XMLoadFloat3( &startPos );
	endVec		= DirectX::XMLoadFloat3( &endPos );


	std::list<NodePath*>::iterator findIt;
	float minF = 100000.0f;


	while( currentNode->mNodeID != endNode->mNodeID )
	{
		minF = 100000.0f;
		for( int i = 0; i < (int)currentNode->mEdges.size(); i++ )
		{
			newNode = currentNode->mEdges[i]->To;
			findIt = mOpenList.end();

			for( auto it = mOpenList.begin(); it != mOpenList.end(); it++ )
			{
				if( (*it)->current->mNodeID == newNode->mNodeID )
				{
					findIt = it;
					break;
				}
			}
			if( findIt == mOpenList.end() )
			{
				findIt = mClosedList.end();
				for( auto it = mClosedList.begin(); it != mClosedList.end(); it++ )
				{
					if( (*it)->current->mNodeID == newNode->mNodeID )
					{
						findIt = it;
						break;
					}
				}
				if( findIt == mClosedList.end() )
				{
					//next = newNode->mNodePos;
					newVec = DirectX::XMLoadFloat3( &newNode->centerPoint );

					vecDist = newVec - startVec;

					newNode->g = DirectX::XMVectorGetX( DirectX::XMVector3LengthEst( vecDist ) );

					vecDist = endVec - newVec;

					newNode->h = DirectX::XMVectorGetX( DirectX::XMVector3LengthEst( vecDist ) );

					NodePath* temp = &mPath[count++];

					temp->current	= newNode;
					temp->parent	= currentPath;

					mOpenList.push_back( temp );
				}
			}
		}

		findIt = mOpenList.end();

		for( auto it = mOpenList.begin(); it != mOpenList.end(); it++ )
		{
			float newF = (*it)->current->g + (*it)->current->h;
			if( newF < minF )
			{
				findIt = it;
				minF = newF;
			}
		}

		if( findIt == mOpenList.end() )
		{
			OutputDebugStringA("No path available!\n");
		}
		
		currentNode = (*findIt)->current;
		currentPath = (*findIt);
		mClosedList.push_back( (*findIt) );
		mOpenList.erase( findIt );
	}

	while( currentPath != nullptr )
	{
		mFinishedPath.push_back( currentPath->current );
		currentPath = currentPath->parent;
	}

	std::reverse( mFinishedPath.begin(), mFinishedPath.end() );

	return mFinishedPath;
}

void NodeGraph::Release()
{
	for( auto& it : mNodes )
	{
		it->Release();
		delete it;
	}
}

NodeGraph::NodeGraph()
{
}


NodeGraph::~NodeGraph()
{
}
