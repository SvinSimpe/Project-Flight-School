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
	//SetUpExits();
	SetCorners();
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
DirectX::XMFLOAT3 MapNodeInstance::GetOrigin() const
{
	return mNode->GetOrigin();
}
void MapNodeInstance::SetOrigin( DirectX::XMFLOAT3 origin )
{
	//obsolete?
	mOrigin	= origin;
	mPos	= DirectX::XMFLOAT3( origin.x - ( mNode->GetOrigin().x * 0.5f ), origin.y - ( mNode->GetOrigin().y * 0.5f ), origin.z - ( mNode->GetOrigin().z * 0.5f ) );
	DirectX::XMStoreFloat4x4( &mWorld, DirectX::XMMatrixTranslationFromVector( XMLoadFloat3( &mPos ) ) );
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
	b.position	= mPos;
	b.width		= mNode->GetGridWidth();
	b.height	= mNode->GetGridHeight();

	return b;
}
Corners	MapNodeInstance::GetCorners() const
{
	return mCorners;
}
void MapNodeInstance::SetCorners()
{
	float halfWidth		= mNode->GetGridWidth() * 0.5f;
	float halfHeight	= mNode->GetGridHeight() * 0.5f;

	//mCorners = Corners( mPos.x - halfWidth, mPos.z - halfHeight, mPos.x + halfWidth, mPos.z + halfHeight );
}
HRESULT	MapNodeInstance::Initialize()
{
	mRotation = 0;
	SetCorners();
	SetUpExits();
	//mNodeRotation	= D0;
	//mNrOfNeighbours = 0;

	//DirectX::XMStoreFloat4x4( &mWorld, DirectX::XMMatrixTranslationFromVector( XMLoadFloat3( &mPos ) ) );

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
ExitPoint* MapNodeInstance::GetExits()
{
	return mExits;
}
void MapNodeInstance::SetUpExits()
{
	ExitPoint* exits = mNode->GetExits();
	for( int i = 0; i < 4; i++ )
	{
		mExits[i].exit		= XMFLOAT3( exits[i].exit.x, exits[i].exit.y, exits[i].exit.z );
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