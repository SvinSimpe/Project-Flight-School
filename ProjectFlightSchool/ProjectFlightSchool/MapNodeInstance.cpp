#include "MapNodeInstance.h"
#include "MapNode.h"
#include "RenderManager.h"

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
	DirectX::XMFLOAT3 min = DirectX::XMFLOAT3( mPos.x - ( mNode->GetGridWidth() * 0.5f ), 0, mPos.z - ( mNode->GetGridHeight() * 0.5f ) );
	DirectX::XMFLOAT3 max = DirectX::XMFLOAT3( min.x + mNode->GetGridWidth(), 5, min.z + mNode->GetGridHeight() );
	RenderManager::GetInstance()->AddBoxToList( min, max );
	return S_OK;
}
void MapNodeInstance::GetNavigationData()
{
	UINT navVertexCount = mNode->GetNavVertexCount();
	XMFLOAT3* navPoints	= mNode->GetNavData();

	mNavMesh = new NavTriangle[navVertexCount / 3];
	UINT count = 0;
	XMMATRIX world	= DirectX::XMLoadFloat4x4( &mWorld );

	for( UINT i = 0; i < navVertexCount; i += 3 )
	{

		DirectX::XMFLOAT3 tri1, tri2, tri3;
		DirectX::XMStoreFloat3( &tri1, DirectX::XMVector3TransformCoord( XMLoadFloat3( &navPoints[i] ), world ) );
		DirectX::XMStoreFloat3( &tri2, DirectX::XMVector3TransformCoord( XMLoadFloat3( &navPoints[i + 1] ), world ) );
		DirectX::XMStoreFloat3( &tri3, DirectX::XMVector3TransformCoord( XMLoadFloat3( &navPoints[i + 2] ), world ) );


		NavTriangle temp;
		temp.triPoints[0] = tri1;
		temp.triPoints[1] = tri2;
		temp.triPoints[2] = tri3;

		temp.adjTri[0] = -1;
		temp.adjTri[1] = -1;
		temp.adjTri[2] = -1;

		mNavMesh[count++] = temp;
	}
	//Connect triangles
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
BoundingBox MapNodeInstance::GetBoundingBox()
{
	BoundingBox b;
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
	if( mNavMesh )
		delete[] mNavMesh;
}
MapNodeInstance::MapNodeInstance()
{
	mNode		= nullptr;
	mPos		= XMFLOAT3( 0, 0, 0 );
	mOrigin		= XMFLOAT3( 0, 0, 0 );
	mInstanceID	= -1;
}
MapNodeInstance::~MapNodeInstance()
{
}