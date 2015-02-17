#include "NodeGraph.h"
#include "Map.h"

//---------------------Edge-----------------------
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
bool Node::AddEdge( Node* A, Node* B )
{
	for( auto& it: mEdges )
	{
		if( it->From->mNodeID == A->mNodeID && it->To->mNodeID == B->mNodeID )
		{
			return false;
		}
	}

	Edge* newEdge = new Edge();
	newEdge->Initialize( A, B );
	mEdges.push_back( newEdge );

	return true;
}

HRESULT Node::Initialize( TilePosition nodePos, int nodeID )
{
	mNodePos	= nodePos;
	mNodeID		= nodeID;

	return S_OK;
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
HRESULT NodeGraph::Initialize( Map* map )
{
	MapNodeInstance*** nodeMap = map->GetNodeMap();
	int width = map->GetMapWidth();
	int height = map->GetMapHeight();
	std::vector<Node*>::iterator findIt = mNodes.end();

	for( int i = 0; i < width; i++ )
	{
		for( int j = 0; j < height; j++ )
		{
			
			for( auto it = mNodes.begin(); it != mNodes.end(); it++ )
			{
				if( (*it)->mNodeID == nodeMap[i][j]->GetNodeID() )
				{
					findIt = it;
					break;
				}
			}
			if( findIt == mNodes.end() )
			{
				Node* temp = new Node();
				temp->Initialize( TilePosition( i, j ), nodeMap[i][j]->GetNodeID() );
				mNodes.push_back( temp );
			}
		}
	}

	MapNodeInstance* adjNodes[4];
	for( auto& it : mNodes )
	{
	}
	return S_OK;
}

NodeGraph::NodeGraph()
{
}


NodeGraph::~NodeGraph()
{
}
