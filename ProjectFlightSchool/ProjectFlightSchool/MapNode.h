#ifndef MAPNODE_H
#define MAPNODE_H
#include "GameObject.h"
#include "MapNodeInstance.h"

using namespace DirectX;

struct ExitPoints
{
	int x;
	int y;
	MapNodeInstance* neighbour;
};

struct MapNodeInfo
{
		Vertex24*		grid;
		UINT			vertexCount;
		UINT			gridDim;
		XMFLOAT3		anchor;
		UINT			nodeDim;
		GameObject*		staticAssets;
		UINT			staticAssetCount;
};
class MapNode
{
	private:
		StaticVertex*	mGrid;
		UINT			mVertexCount;
		UINT			mGridDim;

		UINT			mGridWidth;
		UINT			mGridHeight;

		XMFLOAT3		mAnchor;
		UINT			mNodeDim;
		GameObject*		mStaticAssets;
		UINT			mStaticAssetCount;



		//////////////////////////////////////////////////////
		ExitPoints exits[4];
		//////////////////////////////////////////////////////

	protected:
	public:

	private:
	protected:
	public:
		HRESULT			Update( float deltaTime );
		HRESULT			Render( float deltaTime );
		HRESULT			Render( float deltaTime, XMFLOAT4X4 parentWorld );

		StaticVertex*	GetGrid() const;

		UINT			GetVertexCount() const;

		UINT			GetGridDim() const;

		UINT			GetGridWidth() const;
		UINT			GetGridHeight() const;

		XMFLOAT3		GetAnchor() const;

		UINT			GetNodeDim() const;

		GameObject*		GetStaticAssets() const;
		HRESULT			SetUpExits();
		ExitPoints*		GetExits();

		MapNodeInstance	GetMapNodeInstance();
		HRESULT			Initialize( MapNodeInfo initInfo );
		void			Release();
						MapNode();
		virtual			~MapNode();
};
#endif