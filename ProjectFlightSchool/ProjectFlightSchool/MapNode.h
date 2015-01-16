#ifndef MAPNODE_H
#define MAPNODE_H
#include "GameObject.h"
#include "MapNodeInstance.h"

using namespace DirectX;




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

//Forward declaration for the compiler

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
		UINT			mInstanceCount;

		//////////////////////////////////////////////////////
		ExitPoint exits[4];
		//////////////////////////////////////////////////////

	protected:
	public:

	private:
	protected:
	public:
		HRESULT				Update( float deltaTime );
		HRESULT				Render( float deltaTime );
		HRESULT				Render( float deltaTime, XMFLOAT4X4 parentWorld );

		StaticVertex*		GetGrid() const;

		UINT				GetVertexCount() const;

		UINT				GetGridDim() const;

		UINT				GetGridWidth() const;
		UINT				GetGridHeight() const;

		XMFLOAT3			GetOrigin() const;

		UINT				GetNodeDim() const;

		GameObject*			GetStaticAssets() const;
		HRESULT				SetUpExits();
		/*int					GetNrOfExits() const;*/
		ExitPoint*			GetExits();

		MapNodeInstance*	GetMapNodeInstance();
		void				ReleaseInstance( int InstanceID );
		HRESULT				Initialize( MapNodeInfo initInfo );
		void				Release();
							MapNode();
		virtual				~MapNode();
};
#endif