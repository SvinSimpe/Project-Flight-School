#include "Navmesh.h"
#include "HelperFunctions.h"
#include "RenderManager.h"
#include <algorithm>

HRESULT Navmesh::Render()
{
	return S_OK;
}
HRESULT Navmesh::Initialize( DirectX::XMFLOAT3* meshData, UINT vertexCount, std::vector<DirectX::XMFLOAT3>& edgePoints )
{
	mEdgePoints = edgePoints;
	mNavTriangleCount = vertexCount / 3;
	mMaxPathLength = mNavTriangleCount;

	mPath = new PortalPath[vertexCount * 2];

	mMesh = meshData;
	mPortals = new Portal[vertexCount];

	for(int i = 0; i < (int)vertexCount; i += 3 )
	{
		XMFLOAT3 c = HelperFunctions::GetLineCenter( mMesh[i], mMesh[i + 1] );
		mPortals[i] = Portal( mMesh[i], mMesh[i + 1] );
		mPortals[i].center = c;

		c = HelperFunctions::GetLineCenter( mMesh[i + 1], mMesh[i + 2] );
		mPortals[i + 1] = Portal( mMesh[i + 1], mMesh[i + 2] );
		mPortals[i + 1].center = c;

		c = HelperFunctions::GetLineCenter( mMesh[i + 2], mMesh[i] );
		mPortals[i + 2] = Portal( mMesh[i + 2], mMesh[i] );
		mPortals[i + 2].center = c;
	}

	BuildAdjacencyInfo();

	return S_OK;
}
std::vector<DirectX::XMFLOAT2> Navmesh::FindPath( DirectX::XMFLOAT3 start, DirectX::XMFLOAT3 end )
{
	PathList mOpenList;
	PathList mClosedList;

	Portal startPortal	= Portal( start, start );
	Portal endPortal	= Portal( end, end );

	startPortal.points[0] = start;
	startPortal.points[1] = start;

	endPortal.points[0] = end;
	endPortal.points[1] = end;

	float cost_so_far = 10000.0f;

	std::vector<DirectX::XMFLOAT2> unFinishedPath;

	std::vector<Portal> finishedPath;

	PortalPath* currentPath = nullptr;

	int startTri		= -1;
	int endTri			= -1;
	int currentPortal	= -1;
	int currentTriangle = -1;

	int newIndex		= -1;
	int popIndex		= -1;
	int pathIndex		=  0;

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
			startTri = k;
		}

		if( HelperFunctions::Inside2DTriangle( endPos, p1, p2, p3 ) )
		{
			endTri = k;
		}

		if( startTri != -1 && endTri != -1 )
		{
			break;
		}
	}

	if( startTri == -1 || endTri == -1 )
		return unFinishedPath;

	currentTriangle = startTri;

	while( endTri != currentTriangle )
	{
		float minF  = 1000000.0f;

		currentPortal  = currentTriangle;

		
		
		for( int i = 0; i < 3; i++ )
		{
			Portal* comp = &mPortals[currentPortal + i];

			if( comp->adjTri != -1 )
			{
				bool found = false;

				for( auto it = mOpenList.begin(); it != mOpenList.end(); it++ )
				{
					found = (*it)->portal->portalID == comp->portalID;
				}

				for( auto it = mClosedList.begin(); it != mClosedList.end() && !found; it++ )
				{
					found = (*it)->portal->portalID == comp->portalID;
				}

				if( !found )
				{
					newVec = DirectX::XMLoadFloat2( &DirectX::XMFLOAT2(comp->center.x, comp->center.z) );

					vecDist = newVec - startVec;

					mPortals[currentPortal + i].g = DirectX::XMVectorGetX( DirectX::XMVector2LengthEst( vecDist ) );

					vecDist = endVec - newVec;

					mPortals[currentPortal + i].h = DirectX::XMVectorGetX( DirectX::XMVector2LengthEst( vecDist ) );

					//Calculate f for portal
					PortalPath* temp = &mPath[pathIndex++];
					char buf[20];
					sprintf_s(buf, "PathIndex: %d\n", pathIndex );
					//OutputDebugStringA( buf );

					temp->portal = comp;
					temp->parent = currentPath;

					mOpenList.push_back( temp );
				}
			}
		}

		std::list<PortalPath*>::iterator removeIt = mOpenList.end();
		for( auto it = mOpenList.begin(); it != mOpenList.end(); it++ )
		{
			float newF = (*it)->portal->g + (*it)->portal->h;

			if( newF < minF )
			{
				removeIt = it;
				minF = newF;
			}
		}
		if( removeIt == mOpenList.end() )
		{
			OutputDebugStringA("No path available!\n");
			return unFinishedPath;
		}
		
		currentTriangle = (*removeIt)->portal->adjTri;
		currentPath = (*removeIt);
		mClosedList.push_back( (*removeIt) );
		mOpenList.erase( removeIt );
	}

	finishedPath.push_back( endPortal );

	while( currentPath != nullptr )
	{
		currentPath->portal;

		finishedPath.push_back( *currentPath->portal );

		currentPath = currentPath->parent;
	}
	finishedPath.push_back( startPortal );

	std::reverse( finishedPath.begin(), finishedPath.end() );

	unFinishedPath = FunnelPath( finishedPath );

	return unFinishedPath;
}

DirectX::XMFLOAT3 Navmesh::GetClosestEdgePoint( DirectX::XMFLOAT3 start, DirectX::XMFLOAT3 goal )
{
	DirectX::XMFLOAT3 result;
	float dist = 100000.0f;

	for( auto& it : mEdgePoints )
	{
		float g = HelperFunctions::Dist3Squared( start, it );
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

bool Navmesh::IsTriangleAdj( Portal t1, Portal t2 )
{
	return HelperFunctions::Float3Equal( t1.center, t2.center );
}

bool Navmesh::BuildAdjacencyInfo()
{
	int portalID = 0;
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
					mPortals[currTri + k].adjTri	= nextTri;
					mPortals[currTri + k].portalID	= portalID;

					mPortals[nextTri].adjTri	= currTri;
					mPortals[nextTri].portalID	= portalID;

					portalID++;

					break;
				}
				if( IsTriangleAdj( mPortals[currTri + k], mPortals[nextTri + 1] ) )
				{
					mPortals[currTri + k].adjTri	= nextTri;
					mPortals[currTri + k].portalID	= portalID;

					mPortals[nextTri + 1].adjTri	= currTri;
					mPortals[nextTri + 1].portalID	= portalID;
					portalID++;

					break;
				}
				if( IsTriangleAdj( mPortals[currTri + k], mPortals[nextTri + 2] ) )
				{
					mPortals[currTri + k].adjTri	= nextTri;
					mPortals[currTri + k].portalID	= portalID;

					mPortals[nextTri + 2].adjTri	= currTri;
					mPortals[nextTri + 2].portalID	= portalID;
					portalID++;

					break;
				}
			}
		}
	}

	return true;
}

std::vector<XMFLOAT2> Navmesh::FunnelPath( std::vector<Portal>& path )
{
	std::vector<XMFLOAT2> points;
	int npts = 0;
	int maxPts = 100;
	int apexIndex = 0, leftIndex = 0, rightIndex = 0;

	XMFLOAT2 portalApex, portalLeft, portalRight;

	portalApex	= XMFLOAT2( path[0].points[0].x, path[0].points[0].z );
	portalLeft	= portalApex;
	portalRight = portalApex;

	points.push_back( portalApex );
	npts++;

	for( int i = 1; i < (int)path.size() && npts < maxPts; i++ )
	{
		XMFLOAT2 left	= XMFLOAT2( path[i].points[0].x, path[i].points[0].z );
		XMFLOAT2 right	= XMFLOAT2( path[i].points[1].x, path[i].points[1].z );

		if( HelperFunctions::TriArea2( portalApex, portalRight, right ) <= 0.0f )
		{
			if ( HelperFunctions::Float2Equal( portalApex, portalRight) || HelperFunctions::TriArea2( portalApex, portalLeft, right ) > 0.0f )
			{
				portalRight = right;
				rightIndex = i;
			}
			else
			{
				points.push_back( portalLeft );
				npts++;

				portalApex = portalLeft;
				apexIndex = leftIndex;

				portalLeft = portalApex;
				portalRight = portalApex;

				leftIndex = apexIndex;
				rightIndex = apexIndex;

				i = apexIndex;
				continue;
			}

		}
		if (HelperFunctions::TriArea2(portalApex, portalLeft, left) >= 0.0f)
		{
			 if (HelperFunctions::Float2Equal(portalApex, portalLeft) || HelperFunctions::TriArea2(portalApex, portalRight, left) < 0.0f)
			 {
					 // Tighten the funnel.
				 portalLeft = left;
				 leftIndex = i;
			 }
			 else
			 {
				  // Left over right, insert right to path and restart scan from portal right point.
				 points.push_back( portalRight );
				 npts++;
					 // Make current right the new apex.
				 portalApex = portalRight;
				 apexIndex = rightIndex;
					 // Reset portal
				 portalLeft = portalApex;
				 portalRight = portalApex;
				 leftIndex = apexIndex;
				 rightIndex = apexIndex;
					 // Restart scan
				 i = apexIndex;
				 continue;
			 }
		 }
	 }
     // Append last point to path.
 if (npts < maxPts)
 {
	 XMFLOAT2 pos = XMFLOAT2( path.back().points[0].x, path.back().points[0].z );
     points.push_back( pos );
     npts++;
 }

 return points;
}

void Navmesh::Release()
{
	if( mPortals )
		delete[] mPortals;
	if( mMesh )
		delete[] mMesh;
	if( mPath )
		delete[] mPath;
}

Navmesh::Navmesh()
{
	mPortals	= nullptr;
	mMesh		= nullptr;
	mPath		= nullptr;
}	

Navmesh::~Navmesh()
{
}
