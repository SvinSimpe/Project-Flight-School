#include "MapNodeInstance.h"
#include "MapNode.h"
#include "BoundingGeometry.h"

HRESULT MapNodeInstance::Update( float deltaTime )
{
	return S_OK;
}
HRESULT	MapNodeInstance::Render( float deltaTime )
{
	if( mNode != nullptr )
	{
		mNode->Render( deltaTime, mWorld );
	}

	return S_OK;
}
DirectX::XMFLOAT3 MapNodeInstance::GetPos()const
{
	return mPos;
}
void MapNodeInstance::SetPos( DirectX::XMFLOAT3 pos )
{
	mPos = pos;
	mOrigin = DirectX::XMFLOAT3( mPos.x + ( mNode->GetGridWidth() * 0.5f ), 0, mPos.z + ( mNode->GetGridHeight() * 0.5f ) );
	DirectX::XMStoreFloat4x4( &mWorld, DirectX::XMMatrixTranslationFromVector( XMLoadFloat3( &mPos ) ) );
}

void MapNodeInstance::SetInstanceID( int ID )
{
	mInstanceID = ID;
}
int MapNodeInstance::GetInstanceID() const
{
	return mInstanceID;
}
void MapNodeInstance::ReleaseInstance()
{
	mNode->ReleaseInstance( mInstanceID );
}
MapNode* MapNodeInstance::GetMapNode() const
{
	return mNode;
}
void MapNodeInstance::SetMapNode( MapNode* mapNode )
{
	mNode = mapNode;
}
BoundingBox MapNodeInstance::GetBoundingBox()
{
	BoundingBox b;
	b.position	= XMFLOAT3( mPos.x - ( mNode->GetGridWidth() * 0.5f ), 0, mPos.z - ( mNode->GetGridWidth() * 0.5f ) );
	b.width		= (float)mNode->GetGridWidth();
	b.height	= (float)mNode->GetGridHeight();

	return b;
}
HRESULT	MapNodeInstance::Initialize()
{
	mRotation = 0;
	return S_OK;
}
ExitPoint MapNodeInstance::GetExit( int exitSlot )
{
	return mExits[exitSlot];
}
void MapNodeInstance::SetUpExits()
{
	ExitPoint* exits = mNode->GetExits();
	for( int i = 0; i < 4; i++ )
	{
		mExits[i].exit		= XMFLOAT3( mPos.x + exits[i].exit.x, mPos.y + exits[i].exit.y, mPos.z + exits[i].exit.z );
		mExits[i].valid		= exits[i].valid;
		mExits[i].neighbour	= nullptr;
	}
}
int MapNodeInstance::HasExit()
{
	int result = -1;
	bool exitFound = false;

	for( int i = 0; i < 4 && !exitFound; i++ )
	{
		if( mExits[i].valid && mExits[i].neighbour == nullptr )
		{
			result = i;
			exitFound = true;
		}
	}

	return result;
}
void MapNodeInstance::AddNeighbour( int exitSlot, MapNodeInstance* neighbour )
{
	mExits[exitSlot].neighbour = neighbour;
}
void MapNodeInstance::Release()
{
}
MapNodeInstance::MapNodeInstance()
{
	mNode		= nullptr;
	mPos		= XMFLOAT3( 0, 0, 0 );
	mOrigin		= XMFLOAT3( 0, 0, 0 );
	mInstanceID	= -1;
	mRotation	= 0;
}
MapNodeInstance::~MapNodeInstance()
{
}