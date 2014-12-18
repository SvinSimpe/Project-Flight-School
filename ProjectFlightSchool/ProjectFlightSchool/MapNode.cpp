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
	return S_OK;
}
HRESULT	MapNode::Initialize( MapNodeInfo initInfo )
{
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
