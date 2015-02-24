#pragma once
#ifndef NODEGRAPH_H
#define NODEGRAPH_H

#include <Windows.h>
#include <vector>
#include <DirectXMath.h>


class Map;
class Node;

struct TilePosition
{
	int x;
	int y;
	TilePosition()
	{
		x = -1;
		y = -1;
	}

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
		int mEdgeID;

	private:
	protected:
	public:
		HRESULT Render();
		HRESULT Initialize( Node* A, Node* B );
		Edge();
		~Edge();
		bool operator==( const Edge* origObj ) const;
};

class Node
{
	private:
		


	protected:
	public:
		DirectX::XMFLOAT3 centerPoint;
		int mNodeID;
		TilePosition mNodePos;
		std::vector<Edge*> mEdges;

		float g;
		float h;

	private:
	protected:
	public:
		HRESULT Render();
		HRESULT Initialize( TilePosition nodePos, int nodeID );
		bool AddEdge( Node* To );
		void Release();
		Node();
		~Node();
};

struct NodePath
{
	Node* current;
	NodePath* parent;
};

class NodeGraph
{
	private:
		std::vector<Node*> mNodes;
		std::vector<Node*> mFinishedPath;
		NodePath mPath[1000];

	protected:
	public:

	private:
		void BuildGraph( Map* map );
		int FindNodeByID( int nodeID );

	protected:
	public:
		HRESULT Render();
		HRESULT Initialize( Map* map );

		std::vector<Node*> FindPath( DirectX::XMFLOAT3 startPos, DirectX::XMFLOAT3 endPos, int startNodeID, int endNodeID );
		void Release();
		NodeGraph();
		~NodeGraph();
};
#endif

