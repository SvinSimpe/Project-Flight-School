#ifndef MAPNODE_H
#define MAPNODE_H
#include "GameObject.h"
#include "MapNodeInstance.h"
#include "Text.h"
#define INSTANCE_COUNT 100

using namespace DirectX;


struct MapNodeInfo
{
		Vertex24*		grid;
		UINT			vertexCount;
		XMFLOAT3*		navData;
		UINT			navVertexCount;
		UINT			gridWidth;
		UINT			gridHeight;
		XMFLOAT4X4		anchor;
		UINT			nodeDim;
		GameObject*		staticAssets;
		UINT			staticAssetCount;
		std::string		name;
};

class MapNode
{
	private:
		std::string		mName;
		StaticVertex*	mGrid;
		XMFLOAT3*		mNavData;

		UINT			mNavVertexCount;
		UINT			mVertexCount;
		UINT			mGridDim;

		UINT			mGridWidth;
		UINT			mGridHeight;

		XMFLOAT3		mOrigin;
		XMFLOAT3		mPos;
		UINT			mNodeDim;
		GameObject*		mStaticAssets;
		UINT			mStaticAssetCount;
		AssetID			blendMap;

		MapNodeInstance mInstances[INSTANCE_COUNT];

	protected:
	public:

	private:
	protected:
	public:
		HRESULT				Update( float deltaTime );
		HRESULT				Render( float deltaTime, XMFLOAT4X4 parentWorld );

		StaticVertex*		GetGrid() const;

		UINT				GetVertexCount() const;
		UINT				GetNavVertexCount() const;

		UINT				GetGridDim() const;

		UINT				GetGridWidth() const;
		UINT				GetGridHeight() const;

		XMFLOAT3			GetOrigin() const;
		XMFLOAT3*			GetNavData() const;

		GameObject*			GetStaticAssets() const;

		MapNodeInstance*	GetMapNodeInstance();
		void				ReleaseInstance( int InstanceID );
		HRESULT				Initialize( MapNodeInfo initInfo );
		void				Release();
							MapNode();
		virtual				~MapNode();
};
#endif