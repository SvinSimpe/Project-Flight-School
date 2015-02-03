#include "MapNode.h"
#include "MapNodeInstance.h"

HRESULT	MapNode::Render( float deltaTime, XMFLOAT4X4 parentWorld )
{
	for( int i = 0; i < (int)mStaticAssetCount; i++ )
	{
		mStaticAssets[i].Render( deltaTime, parentWorld );
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
UINT MapNode::GetGridWidth() const
{
	return mGridWidth;
}
UINT MapNode::GetGridHeight() const
{
	return mGridHeight;
}
XMFLOAT3 MapNode::GetOrigin() const
{
	return mOrigin;
}
GameObject* MapNode::GetStaticAssets() const
{
	return mStaticAssets;
}
HRESULT	MapNode::Update( float deltaTime )
{
	return S_OK;
}
MapNodeInstance* MapNode::GetMapNodeInstance()
{
	MapNodeInstance* result = nullptr;
	bool instanceFound = false;
	for( int i = 0; i < (int)INSTANCE_COUNT && !instanceFound; i++ )
	{
		if( mInstances[i].GetInstanceID() == -1 )
		{
			result = &mInstances[i];
			result->SetMapNode( this );
			result->SetInstanceID( i );
			result->Initialize();
			printf( "Instance number: %d\n", i);
			instanceFound = true;
		}
	}
	return result;
}
void MapNode::ReleaseInstance( int InstanceID )
{
	for( int i = 0; i < INSTANCE_COUNT; i++ )
	{
		if( mInstances[i].GetInstanceID() == InstanceID )
		{
			mInstances[i].SetInstanceID( -1 );
		}
	}
}
HRESULT	MapNode::Initialize( MapNodeInfo initInfo )
{
	mGrid				= new StaticVertex[initInfo.vertexCount];
	mOrigin				= XMFLOAT3(initInfo.anchor.x, 0, initInfo.anchor.z);
	//BUGHAXX
	//mGridDim			= initInfo.gridDim;
	mGridDim			= initInfo.gridDim;
	//Handles releaseing assets
	mStaticAssets		= initInfo.staticAssets;
	mStaticAssetCount	= initInfo.staticAssetCount;
	mVertexCount		= initInfo.vertexCount;

	mGridWidth			= mGridDim;
	mGridHeight			= mGridDim;

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
	mOrigin			= XMFLOAT3( 0.0f, 0.0f, 0.0f );
	mStaticAssets	= nullptr;
}
MapNode::~MapNode()
{
}
