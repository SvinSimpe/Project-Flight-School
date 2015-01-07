#include "MapNode.h"
StaticVertex* MapNode::GetGrid() const
{
	return mGrid;
}
UINT MapNode::GetVertexCount() const
{
	return mVertexCount;
}
UINT MapNode::GetGridDim() const
{
	return mGridDim;
}
XMFLOAT3 MapNode::GetAnchor() const
{
	return mAnchor;
}
UINT MapNode::GetNodeDim() const
{
	return mNodeDim;
}
GameObject* MapNode::GetStaticAssets() const
{
	return mStaticAssets;
}
HRESULT	MapNode::Update(float deltaTime)
{
	return S_OK;
}
HRESULT	MapNode::Render(float deltaTime)
{
	for(int i = 0; i < mStaticAssetCount; i++ )
	{
		mStaticAssets[i].Render( deltaTime );
	}

	return S_OK;
}
HRESULT	MapNode::Initialize( MapNodeInfo initInfo )
{
	mGrid				= new StaticVertex[initInfo.vertexCount];
	mAnchor				= initInfo.anchor;
	mGridDim			= initInfo.gridDim;
	mStaticAssets		= initInfo.staticAssets;
	mStaticAssetCount	= initInfo.staticAssetCount;
	mVertexCount		= initInfo.vertexCount;

	for( int i = 0; i < mVertexCount; i++ )
	{
		mGrid[i].position[0]	= initInfo.grid[i].position[0];
		mGrid[i].position[1]	= initInfo.grid[i].position[1];
		mGrid[i].position[2]	= initInfo.grid[i].position[2];

		mGrid[i].normal[0]	= initInfo.grid[i].normal[0];
		mGrid[i].normal[1]	= initInfo.grid[i].normal[1];
		mGrid[i].normal[2]	= initInfo.grid[i].normal[2];

		mGrid[i].tangent[0]	= 1.0f;
		mGrid[i].tangent[1]	= 1.0f;
		mGrid[i].tangent[2]	= 1.0f;

		mGrid[i].uv[0] = 1.0f;
		mGrid[i].uv[1] = 1.0f;
	}
	return S_OK;
}
void MapNode::Release()
{
	delete[] mGrid;
	delete[] mStaticAssets;
}
MapNode::MapNode()
{
	mGrid			= nullptr;
	mVertexCount	= 0;
	mGridDim		= 0;
	mNodeDim		= 0;
	mAnchor			= XMFLOAT3( 0.0f, 0.0f, 0.0f );
	mStaticAssets	= nullptr;
}
MapNode::~MapNode()
{
}
