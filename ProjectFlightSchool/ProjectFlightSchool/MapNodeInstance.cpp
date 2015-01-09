#include "MapNodeInstance.h"
#include "MapNode.h"

HRESULT MapNodeInstance::Update(float deltaTime)
{
	return S_OK;
}
HRESULT	MapNodeInstance::Render(float deltaTime)
{
	if( mNode != nullptr )
	{
		mNode->Render( deltaTime, mWorld );
	}

	return S_OK;
}
DirectX::XMFLOAT3	MapNodeInstance::GetPos()const
{
	return mPos;
}
void MapNodeInstance::SetPos(DirectX::XMFLOAT3 pos)
{
	mPos = pos;
	DirectX::XMStoreFloat4x4( &mWorld, DirectX::XMMatrixTranslationFromVector( XMLoadFloat3( &mPos ) ) );
}
MapNode* MapNodeInstance::GetMapNode()const
{
	return mNode;
}
void MapNodeInstance::SetMapNode(MapNode* mapNode)
{
	mNode = mapNode;
}
Corners	MapNodeInstance::GetCorners()const
{
	return mCorners;
}
void MapNodeInstance::SetCorners(int left, int top)
{
	mCorners = Corners( left, top, left + mNode->GetGridWidth(), top + mNode->GetGridHeight() ); 
}
HRESULT	MapNodeInstance::Initialize( MapNode* mapNode )
{
	mNode = mapNode;
	if( mNode != nullptr )
	{
		mCorners = Corners( 0 , 0, mNode->GetGridDim(), mNode->GetGridDim() );
	}
	mRotation = 0;
	mPos = DirectX::XMFLOAT3(0, 0, 0);
	DirectX::XMStoreFloat4x4( &mWorld, DirectX::XMMatrixTranslationFromVector( XMLoadFloat3( &mPos ) ) );

	return S_OK;
}
void MapNodeInstance::Release()
{
}
MapNodeInstance::MapNodeInstance()
{
}
MapNodeInstance::~MapNodeInstance()
{
}