#include "Navmesh.h"
#include "HelperFunctions.h"
#include "RenderManager.h"

HRESULT Navmesh::Render()
{
	for(UINT i = 0; i < mNavTriangleCount * 3; i++ )
	{
		int portal1 = mPortals[i].adjTri;
		if( portal1 != -1)
		{
			DirectX::XMFLOAT3 c1 = HelperFunctions::GetCenter( mMesh[portal1], mMesh[portal1 + 1], mMesh[portal1 + 2] );
			DirectX::XMFLOAT3 c2 = DirectX::XMFLOAT3( ( mMesh[mPortals[i].points[0]].x + mMesh[mPortals[i].points[1]].x ) / 2.0f, ( mMesh[mPortals[i].points[0]].x + mMesh[mPortals[i].points[1]].x ) / 2.0f , ( mMesh[mPortals[i].points[0]].x + mMesh[mPortals[i].points[1]].x ) / 2.0f );
		}
	}
}
HRESULT Navmesh::Initialize( DirectX::XMFLOAT3* meshData, UINT vertexCount )
{
	mNavTriangleCount = vertexCount / 3;

	mMesh = meshData;
	UINT count = 0;
	mPortals = new Portal[vertexCount];

	for(UINT i = 0; i < vertexCount; i += 3 )
	{
		mPortals[i] = Portal( i, i + 1 );
		mPortals[i + 1] = Portal( i + 1, i + 2 );
		mPortals[i + 2] = Portal( i + 2, i );
	}

	BuildAdjacencyInfo();

	return S_OK;
}
bool Navmesh::IsTriangleAdj( Portal t1, Portal t2 )
{
	DirectX::XMFLOAT3 p1 = mMesh[t1.points[0]];
	DirectX::XMFLOAT3 p2 = mMesh[t1.points[1]];
	DirectX::XMFLOAT3 c1 = DirectX::XMFLOAT3( ( p1.x + p2.x ) / 2.0f, ( p1.y + p2.y ) / 2.0f, ( p1.z + p2.z ) / 2.0f );

	DirectX::XMFLOAT3 p3 = mMesh[t2.points[0]];
	DirectX::XMFLOAT3 p4 = mMesh[t2.points[1]];
	DirectX::XMFLOAT3 c2 = DirectX::XMFLOAT3( ( p3.x + p4.x ) / 2.0f, ( p3.y + p4.y ) / 2.0f, ( p3.z + p4.z ) / 2.0f );

	float distSquared = HelperFunctions::DistSquared( c1, c2 );

	return ( distSquared < 0.01f );
}

bool Navmesh::BuildAdjacencyInfo()
{
	for( UINT i = 0; i < mNavTriangleCount - 1; i++ )
	{
		for( UINT j = i + 1; j < mNavTriangleCount; j++ )
		{
			for( int k = 0; k < 3; k++ )
			{
				int currTri = i * 3;
				int nextTri = j * 3;

				if( IsTriangleAdj( mPortals[currTri + k], mPortals[nextTri] ) )
				{
					mPortals[currTri + k].adjTri = nextTri;
					mPortals[nextTri].adjTri = currTri;
					break;
				}
				if( IsTriangleAdj( mPortals[currTri + k], mPortals[nextTri + 1] ) )
				{
					mPortals[currTri + k].adjTri = nextTri;
					mPortals[nextTri + 1].adjTri = currTri;
					break;
				}
				if( IsTriangleAdj( mPortals[currTri + k], mPortals[nextTri + 2] ) )
				{
					mPortals[currTri + k].adjTri = nextTri;
					mPortals[nextTri + 2].adjTri = currTri;
					break;
				}
			}
		}
	}

	return true;
}

Navmesh::Navmesh()
{
}

Navmesh::~Navmesh()
{
}
