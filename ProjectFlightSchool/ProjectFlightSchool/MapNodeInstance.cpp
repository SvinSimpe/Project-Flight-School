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
	mOrigin = DirectX::XMFLOAT3( mPos.x + (float)( mCorners.right * 0.5f ), 0, mPos.z + (float)( mCorners.bottom * 0.5f ) );
	DirectX::XMStoreFloat4x4( &mWorld, DirectX::XMMatrixTranslationFromVector( XMLoadFloat3( &mPos ) ) );
}
DirectX::XMFLOAT3 MapNodeInstance::GetOrigin()const
{
	return mOrigin;
}
void MapNodeInstance::SetOrigin( DirectX::XMFLOAT3 origin )
{
	mOrigin = origin;
	mPos	= DirectX::XMFLOAT3( mOrigin.x - (float)( mCorners.right * 0.5f ), 0, mOrigin.z - (float)( mCorners.bottom * 0.5f ) );
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
	mOrigin			= DirectX::XMFLOAT3( 0, 0, 0 );
	mRotation		= 0;
	mPos			= DirectX::XMFLOAT3( 0, 0, 0 );
	mNodeRotation	= D0;
	//mNrOfNeighbours = 0;
	GetExits();

	DirectX::XMStoreFloat4x4( &mWorld, DirectX::XMMatrixTranslationFromVector( XMLoadFloat3( &mPos ) ) );

	return S_OK;
}
//bool MapNodeInstance::AddNeighbour( MapNodeInstance* mapNodeInstance )
//{
//	bool result = false;
//	if( mNrOfNeighbours < 4 )
//	{
//		mNeighbours[mNrOfNeighbours++] = mapNodeInstance;
//		result = true;
//	}
//	return result;
//}
void MapNodeInstance::GetExits()
{
	ExitPoints* exits = mNode->GetExits();
	for( int i = 0; i < 4; i++ )
	{
		mExits[i] = exits[i];
	}
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