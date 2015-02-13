#include "Navmesh.h"
#include "HelperFunctions.h"
#include "RenderManager.h"

HRESULT Navmesh::Render()
{
	for(UINT i = 0; i < ( mNavTriangleCount * 3 ) - 1; i++ )
	{
		int portal1 = mPortals[i].adjTri;
		if( portal1 != -1)
		{
			DirectX::XMFLOAT3 c1 = HelperFunctions::GetCenter( mMesh[portal1], mMesh[portal1 + 1], mMesh[portal1 + 2] );
			DirectX::XMFLOAT3 c2 = DirectX::XMFLOAT3( ( mMesh[mPortals[i].points[0]].x + mMesh[mPortals[i].points[1]].x ) / 2.0f, ( mMesh[mPortals[i].points[0]].y + mMesh[mPortals[i].points[1]].y ) / 2.0f , ( mMesh[mPortals[i].points[0]].z + mMesh[mPortals[i].points[1]].z ) / 2.0f );

			//DirectX::XMVECTOR vecDir = DirectX::XMVectorSet( c2.x - c1.x, c2.y - c1.y, c2.z - c1.z, 1.0f );

			//vecDir = DirectX::XMVector3Normalize( vecDir );

			//DirectX::XMVECTOR startVec = DirectX::XMLoadFloat3( &c1 );

			//startVec += -0.5f * vecDir;

			//DirectX::XMVECTOR endVec = startVec + ( 1.0f * vecDir );

			DirectX::XMFLOAT3 start, end;

			//DirectX::XMStoreFloat3( &start, startVec );
			//DirectX::XMStoreFloat3( &end, endVec );

			start	= DirectX::XMFLOAT3( c2.x - 1.0f, -1.0f, c2.z - 1.0f );
			end		= DirectX::XMFLOAT3( c2.x + 1.0f, 1.0f, c2.z + 1.0f );

			RenderManager::GetInstance()->AddLineToList( c1, c2 );
		}

		//int p1 = mPortals[i].points[0];
		//int p2 = mPortals[i].points[1];

		//DirectX::XMFLOAT3 pos1 = DirectX::XMFLOAT3( ( mMesh[mPortals[i].points[0]].x + mMesh[mPortals[i].points[1]].x ) / 2.0f, ( mMesh[mPortals[i].points[0]].y + mMesh[mPortals[i].points[1]].y ) / 2.0f , ( mMesh[mPortals[i].points[0]].z + mMesh[mPortals[i].points[1]].z ) / 2.0f );
		//DirectX::XMFLOAT3 pos2 = DirectX::XMFLOAT3( ( mMesh[mPortals[i + 1].points[0]].x + mMesh[mPortals[i + 1].points[1]].x ) / 2.0f, ( mMesh[mPortals[i + 1].points[0]].y + mMesh[mPortals[i + 1].points[1]].y ) / 2.0f , ( mMesh[mPortals[i + 1].points[0]].z + mMesh[mPortals[i + 1].points[1]].z ) / 2.0f );

		//DirectX::XMVECTOR dirVec = DirectX::XMVectorSet( pos1.z, pos1.y, pos1.x, 1.0f );
		//RenderManager::GetInstance()->AddLineToList( pos1, pos2 );
	}
	RenderManager::GetInstance()->AddBoxToList(XMFLOAT3(2.0f, 0.0f, 1.0f), XMFLOAT3( 3.0f,1.0f,2.0f) );
	RenderManager::GetInstance()->AddBoxToList(XMFLOAT3( 22.0f, 0.0f, 21.0f), XMFLOAT3( 23.0f,1.0f,22.0f) );
	return S_OK;
}
HRESULT Navmesh::Initialize( DirectX::XMFLOAT3* meshData, UINT vertexCount )
{
	mNavTriangleCount = vertexCount / 3;

	mMesh = meshData;
	UINT count = 0;
	DirectX::XMFLOAT3 pos;
	mPortals = new Portal[vertexCount];

	for(UINT i = 0; i < vertexCount; i += 3 )
	{
		pos = mMesh[i];
		pos = mMesh[i + 1];
		pos = mMesh[i + 2];
		mPortals[i] = Portal( i, i + 1 );
		mPortals[i + 1] = Portal( i + 1, i + 2 );
		mPortals[i + 2] = Portal( i + 2, i );
	}

	BuildAdjacencyInfo();

	return S_OK;
}
std::list<DirectX::XMFLOAT3> Navmesh::FindPath( DirectX::XMFLOAT3 start, DirectX::XMFLOAT3 end )
{
	std::list<DirectX::XMFLOAT3> finishedPath;

	int startIndex = -1;
	int endIndex	= -1;
	int currentIndex = startIndex;
	int newIndex	= -1;
	int popIndex	= -1;

	DirectX::XMFLOAT2 startPos, endPos, p1, p2, p3;

	startPos = DirectX::XMFLOAT2( start.x, start.z );
	endPos = DirectX::XMFLOAT2( end.x, end.z );

	DirectX::XMVECTOR startVec, endVec, newVec, vecDist;

	startVec	= DirectX::XMLoadFloat2( &startPos );
	endVec		= DirectX::XMLoadFloat2( &endPos );


	for( int i = 0; i < (int)mNavTriangleCount; i++ )
	{
		int k = i * 3;
		p1 = DirectX::XMFLOAT2( mMesh[k].x, mMesh[k].z );
		p2 = DirectX::XMFLOAT2( mMesh[k + 1].x, mMesh[k + 1].z );
		p3 = DirectX::XMFLOAT2( mMesh[k + 2].x, mMesh[k + 2].z );

		if( HelperFunctions::Inside2DTriangle( startPos, p1, p2, p3 ) )
		{
			startIndex = i;
		}

		if( HelperFunctions::Inside2DTriangle( endPos, p1, p2, p3 ) )
		{
			endIndex = i;
		}

		if( startIndex != -1 && endIndex != -1 )
		{
			break;
		}
	}

	currentIndex = startIndex;
	while( endIndex != currentIndex )
	{
		float minF  = 1000000.0f;


		int searchIndex = currentIndex * 3;
		
		for( int i = 0; i < 3; i++ )
		{
			if( mPortals[currentIndex + i].adjTri != -1 )
			{
				std::list<int>::iterator findIt, findIt2;
				findIt = std::find( mOpenList.begin(), mOpenList.end(), currentIndex + i );
				findIt2 = std::find( mClosedList.begin(), mClosedList.end(), currentIndex +i );
				if( findIt == mOpenList.end() && findIt2 == mClosedList.end() )
				{
					newVec = DirectX::XMLoadFloat2( &DirectX::XMFLOAT2 ( ( mMesh[mPortals[currentIndex + i].points[0]].x + mMesh[mPortals[currentIndex + i].points[1]].x ) / 2.0f,
						( mMesh[mPortals[currentIndex + i].points[0]].z + mMesh[mPortals[currentIndex + i].points[1]].z ) / 2.0f ) );

					vecDist = newVec - startVec;

					mPortals[currentIndex + i].g = DirectX::XMVectorGetX( DirectX::XMVector2LengthEst( vecDist ) );

					vecDist = endVec - newVec;

					mPortals[currentIndex + i].h = DirectX::XMVectorGetX( DirectX::XMVector2LengthEst( vecDist ) );

					//Calculate f for portal
					mOpenList.push_back( currentIndex + i );
				}
			}
		}

		for( auto it : mOpenList )
		{
			float newF = mPortals[it].g + mPortals[it].h;

			if( newF < minF )
			{
				minF = newF;
				newIndex = it;
			}
		}
		mOpenList.remove( newIndex );
		mClosedList.push_back( newIndex );

		currentIndex = mPortals[newIndex].adjTri;
	}
	return finishedPath;
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
