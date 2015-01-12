#include "MapNode.h"

HRESULT	MapNode::Render(float deltaTime, XMFLOAT4X4 parentWorld)
{
	for(int i = 0; i < (int)mStaticAssetCount; i++ )
	{
		mStaticAssets[i].Render( deltaTime, parentWorld );
	}

	return S_OK;
}
HRESULT	MapNode::Render(float deltaTime)
{
	for(int i = 0; i < (int)mStaticAssetCount; i++ )
	{
		mStaticAssets[i].Render( deltaTime );
	}

	return S_OK;
}
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
UINT MapNode::GetGridWidth() const
{
	return mGridWidth;
}
UINT MapNode::GetGridHeight() const
{
	return mGridHeight;
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
MapNodeInstance MapNode::GetMapNodeInstance()
{
	MapNodeInstance result;
	result.Initialize( this );

	return result;
}
HRESULT	MapNode::Initialize( MapNodeInfo initInfo )
{
	mGrid				= new StaticVertex[initInfo.vertexCount];
	mAnchor				= initInfo.anchor;
	mGridDim			= initInfo.gridDim;
	mStaticAssets		= initInfo.staticAssets;
	mStaticAssetCount	= initInfo.staticAssetCount;
	mVertexCount		= initInfo.vertexCount;

	mGridWidth			= initInfo.gridDim;
	mGridHeight			= initInfo.gridDim;

	for( int i = 0; i < (int)mVertexCount; i++ )
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
////////////////////////////////////////////////////
	SetUpExits();
////////////////////////////////////////////////////
	return S_OK;
}
HRESULT MapNode::SetUpExits()
{
	exits[0].x = 11;
	exits[0].y = 0;
	exits[0].neighbour = nullptr;

	exits[1].x = 0;
	exits[1].y = 11;
	exits[2].neighbour = nullptr;

	exits[2].x = 11;
	exits[2].y = 11;
	exits[2].neighbour = nullptr;

	exits[3].x = 11;
	exits[3].y = 11;
	exits[3].neighbour = nullptr;

	return S_OK;
}
ExitPoints* MapNode::GetExits()
{
	return exits;
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
