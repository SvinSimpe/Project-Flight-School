#ifndef MAPNODE_H
#define MAPNODE_H
#include <Graphics.h>
#include "GameObject.h"
using namespace DirectX;

struct MapNodeInfo
{
		Vertex24*		grid;
		UINT			vertexCount;
		UINT			gridDim;
		XMFLOAT3		anchor;
		UINT			nodeDim;
		GameObject*		staticAssets;
};

class MapNode
{
	private:
		StaticVertex*	mGrid;
		UINT			mVertexCount;
		UINT			mGridDim;
		XMFLOAT3		mAnchor;
		UINT			mNodeDim;
		GameObject*		mStaticAssets;

	protected:
	public:

	private:
	protected:
		StaticVertex*	GetGrid() const;
		UINT			GetVertexCount() const;
		UINT			GetGridDim() const;
		XMFLOAT3		GetAnchor() const;
		UINT			GetNodeDim() const;
		GameObject*		GetStaticAssets() const;
	public:
		HRESULT		Update( float deltaTime );
		HRESULT		Render( float deltaTime );
		HRESULT		Initialize( MapNodeInfo initInfo );
		void		Release();
					MapNode();
		virtual		~MapNode();
};
#endif