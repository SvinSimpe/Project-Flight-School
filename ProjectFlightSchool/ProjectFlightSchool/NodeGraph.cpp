#include "NodeGraph.h"
#include "Map.h"
#include "RenderManager.h"

//---------------------Edge-----------------------

HRESULT Edge::Render()
{
	DirectX::XMFLOAT3 start = From->centerPoint;
	DirectX::XMFLOAT3 end = To->centerPoint;

	RenderManager::GetInstance()->AddLineToList( start, end );

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

		MapNodeInstance* top;
		MapNodeInstance* left;
		MapNodeInstance* bottom;
		MapNodeInstance* right;

		top = map->GetNodeInstance( x, y + 1 );
		if( top != nullptr )
		{
			int index = FindNodeByID( top->GetNodeID() );

			it->AddEdge( mNodes[index] );
			mNodes[index]->AddEdge( it );		
		}

		left = map->GetNodeInstance( x - 1, y );
		if( left != nullptr )
		{
			int index = FindNodeByID( left->GetNodeID() );

			it->AddEdge( mNodes[index] );
			mNodes[index]->AddEdge( it );		
		}

		bottom = map->GetNodeInstance( x, y - 1);
		if( bottom != nullptr )
		{
			int index = FindNodeByID( bottom->GetNodeID() );

			it->AddEdge( mNodes[index] );
			mNodes[index]->AddEdge( it );		
		}

		right = map->GetNodeInstance( x + 1, y );
		if( right != nullptr )
		{
			int index = FindNodeByID( right->GetNodeID() );

			it->AddEdge( mNodes[index] );
			mNodes[index]->AddEdge( it );		
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
	for( auto& it : mNodes )
	{
		it->Render();
	}
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
