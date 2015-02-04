#ifndef MAPNODE_H
#define MAPNODE_H
#include "GameObject.h"
#include "MapNodeInstance.h"
#define INSTANCE_COUNT 150

using namespace DirectX;


struct MapNodeInfo
{
		Vertex24*		grid;
		UINT			vertexCount;
		UINT			gridDim;
		//XMFLOAT3		anchor;
		XMFLOAT4X4		anchor;
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

		XMFLOAT3		mOrigin;
		XMFLOAT3		mPos;
		UINT			mNodeDim;
		GameObject*		mStaticAssets;
		UINT			mStaticAssetCount;

		MapNodeInstance mInstances[150];

	protected:
	public:

	private:
	protected:
	public:
		HRESULT				Update( float deltaTime );
		HRESULT				Render( float deltaTime, XMFLOAT4X4 parentWorld );

		StaticVertex*		GetGrid() const;

		UINT				GetVertexCount() const;

		UINT				GetGridDim() const;

		UINT				GetGridWidth() const;
		UINT				GetGridHeight() const;

		XMFLOAT3			GetOrigin() const;

		GameObject*			GetStaticAssets() const;

		MapNodeInstance*	GetMapNodeInstance();
		void				ReleaseInstance( int InstanceID );
		HRESULT				Initialize( MapNodeInfo initInfo );
		void				Release();
							MapNode();
		virtual				~MapNode();
};
#endif