#include "MapNodeInstance.h"
#include "MapNode.h"
#include "RenderManager.h"
#include "HelperFunctions.h"

HRESULT MapNodeInstance::Update( float deltaTime )
{
	return S_OK;
}
HRESULT	MapNodeInstance::Render( float deltaTime  )
{
	if( mNode != nullptr )
	{
		mNode->Render( deltaTime, mWorld );
	}
	DirectX::XMFLOAT3 min = DirectX::XMFLOAT3( mPos.x - ( mNode->GetGridWidth() * 0.5f ), 0, mPos.z - ( mNode->GetGridHeight() * 0.5f ) );
	DirectX::XMFLOAT3 max = DirectX::XMFLOAT3( min.x + mNode->GetGridWidth(), 5, min.z + mNode->GetGridHeight() );
	RenderManager::GetInstance()->AddBoxToList( min, max );

	mNavMesh->Render();
	return S_OK;
}

Navmesh* MapNodeInstance::GetNavMesh() const
{ 
	return mNavMesh;
}

DirectX::XMFLOAT3	MapNodeInstance::GetClosestEdgePoint( DirectX::XMFLOAT3 start, DirectX::XMFLOAT3 goal )
{
	DirectX::XMFLOAT3 result;
	float dist = 100000.0f;
	for( auto& it : mEdgePoints )
	{
		float g = HelperFunctions::Dist3Squared( start, it ) * 0.9f;
		float h = HelperFunctions::Dist3Squared( goal, it );

		float f = g + h;
		if( f < dist )
		{
			dist = f;
			result = it;
		}
	}

	return result;
}

void MapNodeInstance::GetNavigationData()
{
	float edgeMinX = ( -(float)mNode->GetGridWidth() * 0.5f ) + 1;
	float edgeMaxX = ( (float)mNode->GetGridWidth() * 0.5f ) - 1;

	float edgeMinZ = ( -(float)mNode->GetGridHeight() * 0.5f ) + 1;
	float edgeMaxZ = ( (float)mNode->GetGridHeight() * 0.5f ) - 1;

	mNavMesh = new Navmesh();

	UINT navVertexCount = mNode->GetNavVertexCount();
	XMFLOAT3* navPoints	= mNode->GetNavData();

	DirectX::XMFLOAT3 p1, p2, p3, c;

	int nrOfExits = 0;

	//Deallocation by navmesh
	XMFLOAT3* transformedMesh = new XMFLOAT3[navVertexCount];
	XMMATRIX world	= DirectX::XMLoadFloat4x4( &mWorld );

	for( UINT i = 0; i < navVertexCount; i += 3 )
	{
		bool edge = false;
		DirectX::XMStoreFloat3( &p1, DirectX::XMVector3TransformCoord( XMLoadFloat3( &navPoints[i] ), world ) );
		DirectX::XMStoreFloat3( &p2, DirectX::XMVector3TransformCoord( XMLoadFloat3( &navPoints[i + 1] ), world ) );
		DirectX::XMStoreFloat3( &p3, DirectX::XMVector3TransformCoord( XMLoadFloat3( &navPoints[i + 2] ), world ) );

		transformedMesh[i] = p1;
		transformedMesh[i + 1] = p2;
		transformedMesh[i + 2] = p3;

		c = HelperFunctions::GetTriCenter( navPoints[i], navPoints[i + 1], navPoints[i + 2] );

		if( ( c.x < edgeMinX || edgeMaxX < c.x )  || ( c.z < edgeMinZ || edgeMaxZ < c.z ) )
		{
			c = HelperFunctions::GetTriCenter( p1, p2, p3 );
			mEdgePoints.push_back( c );
		}
	}

	mNavMesh->Initialize( transformedMesh, navVertexCount );
}
DirectX::XMFLOAT3 MapNodeInstance::GetPos()const
{
	return mPos;
}
void MapNodeInstance::SetPos( DirectX::XMFLOAT3 pos )
{
	mPos = XMFLOAT3( pos.x + ( mNode->GetGridWidth() * 0.5f ), 0, pos.z + ( mNode->GetGridHeight() * 0.5f ) );
	DirectX::XMStoreFloat4x4( &mWorld, DirectX::XMMatrixTranslationFromVector( XMLoadFloat3( &mPos ) ) );
	mOrigin = XMFLOAT3( pos.x + mNode->GetGridWidth() * 0.5f, 0, pos.z + mNode->GetGridHeight() * 0.5f );
}

int	 MapNodeInstance::GetNodeSizeX()
{
	return mSizeX;
}

int	 MapNodeInstance::GetNodeSizeY()
{
	return mSizeY;
}

void MapNodeInstance::SetNodeID( int ID )
{
	mNodeID = ID;
}

int MapNodeInstance::GetNodeID() const
{
	return mNodeID;
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
	return mOrigin;
}

MapNode* MapNodeInstance::GetMapNode() const
{
	return mNode;
}
void MapNodeInstance::SetMapNode( MapNode* mapNode )
{
	mNode = mapNode;
}
BoundingRectangle MapNodeInstance::GetBoundingBox()
{
	BoundingRectangle b;
	b.position	= mPos;
	b.width		= (float)mNode->GetGridWidth();
	b.height	= (float)mNode->GetGridHeight();
	return b;
}

HRESULT	MapNodeInstance::Initialize()
{
	GetNavigationData();
	mSizeX = mNode->GetGridWidth() / 24;
	mSizeY = mNode->GetGridHeight() / 24;
	return S_OK;
}
void MapNodeInstance::Release()
{
	SAFE_RELEASE_DELETE( mNavMesh );
}
MapNodeInstance::MapNodeInstance()
{
	mNavMesh	= nullptr;
	mNode		= nullptr;
	mPos		= XMFLOAT3( 0, 0, 0 );
	mOrigin		= XMFLOAT3( 0, 0, 0 );
	mInstanceID	= -1;
	mNodeID		= -1;
	mSizeX		= 0;
	mSizeY		= 0;
}
MapNodeInstance::~MapNodeInstance()
{
}