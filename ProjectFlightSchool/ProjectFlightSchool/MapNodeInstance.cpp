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
void MapNodeInstance::GetNavigationData()
{
	float edgeMinX = ( ( -(float)mNode->GetGridWidth() * 0.5f ) + 0.5f );
	float edgeMaxX = ( ( (float)mNode->GetGridWidth() * 0.5f ) - 0.5f );

	float edgeMinZ = ( ( -(float)mNode->GetGridHeight() * 0.5f ) + 0.5f );
	float edgeMaxZ = ( ( (float)mNode->GetGridHeight() * 0.5f ) - 0.5f );

	mNavMesh = new Navmesh();

	UINT navVertexCount = mNode->GetNavVertexCount();
	XMFLOAT3* navPoints	= mNode->GetNavData();

	int nrOfExits = 0;

	//Deallocation by navmesh
	XMFLOAT3* transformedMesh = new XMFLOAT3[navVertexCount];
	XMMATRIX world	= DirectX::XMLoadFloat4x4( &mWorld );

	for( UINT i = 0; i < navVertexCount; i++ )
	{
		DirectX::XMFLOAT3 tri1 =  navPoints[i];
		DirectX::XMStoreFloat3( &tri1, DirectX::XMVector3TransformCoord( XMLoadFloat3( &tri1 ), world ) );

		transformedMesh[i] = tri1;
		if( ( navPoints[i].x < edgeMinX || edgeMaxX < navPoints[i].x )
			|| ( navPoints[i].z < edgeMinZ || edgeMaxZ < navPoints[i].z ) ) 
		{
			mEdgePoints.push_back( &transformedMesh[i] );
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
}
MapNodeInstance::~MapNodeInstance()
{
}