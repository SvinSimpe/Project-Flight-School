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
	mNavMesh = new Navmesh();

	UINT navVertexCount = mNode->GetNavVertexCount();
	XMFLOAT3* navPoints	= mNode->GetNavData();

	//Deallocation by navmesh
	XMFLOAT3* transformedMesh = new XMFLOAT3[navVertexCount];
	XMMATRIX world	= DirectX::XMLoadFloat4x4( &mWorld );

	for( UINT i = 0; i < navVertexCount; i++ )
	{
		DirectX::XMFLOAT3 tri1;
		DirectX::XMStoreFloat3( &tri1, DirectX::XMVector3TransformCoord( XMLoadFloat3( &navPoints[i] ), world ) );

		transformedMesh[i] = tri1;
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
}
MapNodeInstance::~MapNodeInstance()
{
}