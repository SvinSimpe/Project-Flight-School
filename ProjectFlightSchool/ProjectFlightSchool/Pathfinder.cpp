#include "Pathfinder.h"
#include "Map.h"

Pathfinder* Pathfinder::instance = nullptr;

HRESULT Pathfinder::Initialize( Map* map )
{
	mMap = map;

	mMapWidth = mMap->GetMapWidth();
	mMapHeight = mMap->GetMapHeight();

	mNavmeshMap = new Navmesh*[mMapWidth * mMapHeight];
	for( int i = 0; i < mMapWidth; i++ )
	{
		for( int j = 0; j < mMapHeight; j++ )
		{
			MapNodeInstance* current = mMap->GetNodeInstance( i, j );
			if( current )
				mNavmeshMap[( i * mMapHeight ) + j] = current->GetNavMesh();
		}
	}
	return S_OK;
}

Path* Pathfinder::RequestPath( DirectX::XMFLOAT3 start, DirectX::XMFLOAT3 end )
{	
	DirectX::XMFLOAT3 currStart = start;
	DirectX::XMFLOAT3 currEnd	= end;

	int startX = (int)( (mMapWidth * NODE_DIM )  + start.x ) / NODE_DIM;
	int startZ = (int)( (mMapHeight * NODE_DIM )  + start.z ) / NODE_DIM;

	int goalX = (int)( (mMapWidth * NODE_DIM )  + end.x ) / NODE_DIM;
	int goalZ = (int)( (mMapHeight * NODE_DIM )  + end.z ) / NODE_DIM;

	int currentX = startX;
	int currentZ = startZ;

	int currIndex	= ( currentX * mMapWidth ) + currentZ;
	int goalIndex	= ( goalX * mMapHeight ) + goalZ;

	Path* temp = &mPaths[mNrOfPaths++];


	while( goalIndex != currIndex )
	{
		if( currIndex < ( mMapWidth * mMapHeight ) && goalIndex < ( mMapWidth * mMapHeight ) )
		{
			currEnd = mNavmeshMap[currIndex]->GetClosestEdgePoint( currStart, end );
			temp->AddSubPath( currStart, currEnd, currIndex );

			currentX = (int)( (mMapWidth * NODE_DIM )  + currEnd.x ) / NODE_DIM;
			currentZ = (int)( (mMapHeight * NODE_DIM )  + currEnd.z ) / NODE_DIM;

			currIndex	= ( currentX * mMapWidth ) + currentZ;

			currStart = currEnd;
		}
		else
		{
			break;
		}
	}

	return temp;
}

bool Pathfinder::CalculateSubPath( Path& path, int nrOfSteps )
{
	if( nrOfSteps == 0 )
		nrOfSteps = path.mNrOfSubPaths;
	else
		path.mCurrentSubPath + nrOfSteps;

	for( int i = path.mCurrentSubPath; i < path.mNrOfSubPaths; i++ )
	{
		SubPath* temp = &path.mSubPaths[i];
		if(! temp->mIsCalculated )
		{
			temp->mPoints = mNavmeshMap[path.mSubPaths[i].mNavMeshIndex]->FindPath( temp->mStart, temp->mEnd );
			path.mCurrentSubPath = i;
		}
	}
	return true;
}

Pathfinder* Pathfinder::GetInstance()
{
	if( instance == nullptr )
	{
		instance = new Pathfinder();
	}
	return instance;
}

Pathfinder::Pathfinder()
{
		mNavmeshMap = nullptr;
		mMap		= nullptr;

		mNrOfPaths = 0;
}

void Pathfinder::Release()
{
	if( instance )
		delete instance;
}

Pathfinder::~Pathfinder()
{
}
