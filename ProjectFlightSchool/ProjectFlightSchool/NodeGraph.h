#pragma once
#ifndef NODEGRAPH_H
#define NODEGRAPH_H

#include <Windows.h>
#include <vector>


class Node;
class MapNodeInstance;

struct TilePosition
{
	int x;
	int y;
	TilePosition( int pX, int pY )
	{
		x = pX;
		y = pY;
	}
};

class Edge
{
	private:
	protected:
	public:
		Node* From;
		Node* To;

	private:
	protected:
	public:
		HRESULT Initialize( Node* A, Node* B );
		Edge();
		~Edge();
		bool operator==( const Edge& origObj ) const;
};

class Node
{
	private:
		


	protected:
	public:
		int mNodeID;
		TilePosition mNodePos;
		float g;
		float h;
		std::vector<Edge*> mEdges;

	private:
	protected:
	public:
		HRESULT Initialize( TilePosition nodePos, int nodeID );
		bool AddEdge( Node* A, Node* B );
		Node();
		~Node();
};

class NodeGraph
{
	private:
		std::vector<Node*> mNodes;
	protected:
	public:

	private:
	protected:
	public:
		HRESULT Initialize( Map* map );

		void FindPath( TilePosition start, TilePosition end );
		NodeGraph();
		~NodeGraph();
};
#endif

