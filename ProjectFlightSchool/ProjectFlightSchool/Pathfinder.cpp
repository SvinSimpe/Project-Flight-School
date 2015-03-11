#include "Pathfinder.h"
#include "Map.h"

Pathfinder* Pathfinder::instance = nullptr;

HRESULT Pathfinder::Initialize( Map* map )
{
	mMap = map;

	mMapWidth = mMap->GetMapWidth();
	mMapHeight = mMap->GetMapHeight();

	mNodeGraph = new NodeGraph();
	mNodeGraph->Initialize( map );

	mNavmeshMap = new Navmesh*[mMapWidth * mMapHeight];
	for( UINT i = 0; i < mMapWidth; i++ )
	{
		for( UINT j = 0; j < mMapHeight; j++ )
		{
			MapNodeInstance* current = mMap->GetNodeInstance( i, j );
			if( current )
				mNavmeshMap[( i * mMapHeight ) + j] = current->GetNavMesh();
			else
				mNavmeshMap[( i * mMapHeight ) + j] = nullptr;
		}
	}
	return S_OK;
}

bool Pathfinder::IsOnNavMesh( DirectX::XMFLOAT3 pos )
{
	XMFLOAT2 tempPos = XMFLOAT2(pos.x, pos.z);
	XMFLOAT2 p0, p1, p2;

	int unitPosX = (int)pos.x;
	int unitPosZ = (int)pos.z;

	int playerX = ( ( (int)mMap->GetMapHalfWidth() * NODE_DIM ) + unitPosX ) / NODE_DIM;
	int playerZ = ( ( (int)mMap->GetMapHalfHeight() * NODE_DIM ) + unitPosZ ) / NODE_DIM;

	UINT index = ( playerX * mMapWidth ) + playerZ;

	if( index < mMapWidth * mMapHeight )
	{
		Navmesh* temp = mNavmeshMap[index];
		if( temp )
			return temp->IsOnNavMesh( pos );
	}
	return false;
}

DirectX::XMFLOAT3 Pathfinder::GetRandomTriInMesh( DirectX::XMFLOAT3 pos )
{
	XMFLOAT2 tempPos = XMFLOAT2(pos.x, pos.z);
	XMFLOAT2 p0, p1, p2;

	int unitPosX = (int)pos.x;
	int unitPosZ = (int)pos.z;

	int playerX = ( ( (int)mMap->GetMapHalfWidth() * NODE_DIM ) + unitPosX ) / NODE_DIM;
	int playerZ = ( ( (int)mMap->GetMapHalfHeight() * NODE_DIM ) + unitPosZ ) / NODE_DIM;

	UINT index = ( playerX * mMapWidth ) + playerZ;

	if( index < mMapWidth * mMapHeight )
	{
		Navmesh* temp = mNavmeshMap[index];
		if( temp )
			return temp->GetRandomTriCenter();
	}
	return DirectX::XMFLOAT3( 0, 0, 0 );
}

void Pathfinder::RequestPath( Path* path, DirectX::XMFLOAT3 start, DirectX::XMFLOAT3 end )
{	
	DirectX::XMFLOAT3 currStart = start;
	DirectX::XMFLOAT3 currEnd	= end;
	std::vector<Node*> nodePath;

	path->mNrOfSubPaths		= 0;
	path->mCurrentSubPath	= 0;

	int startX = (int)( ( mMap->GetMapHalfWidth() * NODE_DIM )  + floorf( start.x ) ) / NODE_DIM;
	int startZ = (int)( ( mMap->GetMapHalfHeight() * NODE_DIM )  + floorf( start.z ) ) / NODE_DIM;

	int goalX = (int)( ( mMap->GetMapHalfWidth() * NODE_DIM )  + floorf( end.x ) ) / NODE_DIM;
	int goalZ = (int)( ( mMap->GetMapHalfHeight() * NODE_DIM )  + floorf( end.z ) ) / NODE_DIM;

	int currentX, currentZ, currIndex;


	MapNodeInstance* startNode	= mMap->GetNodeInstance( startX, startZ );
	MapNodeInstance* endNode	= mMap->GetNodeInstance( goalX, goalZ );

	if( startNode && endNode )
	{
		nodePath = mNodeGraph->FindPath( start, end, startNode->GetNodeID(), endNode->GetNodeID() );

		currentX = nodePath.front()->mNodePos.x;
		currentZ = nodePath.front()->mNodePos.y;

		currIndex = ( currentX * mMapWidth ) + currentZ;

			for( int i = 0; i < (int)nodePath.size() - 1; i++ )
			{
				currentX = nodePath[i]->mNodePos.x;
				currentZ = nodePath[i]->mNodePos.y;

				currIndex	= ( currentX * mMapWidth ) + currentZ;

				currEnd = mNavmeshMap[currIndex]->GetClosestEdgePoint( currStart, end );

				path->AddSubPath( currStart, currEnd, currIndex );

				currStart = currEnd;
		}
		currentX = nodePath.back()->mNodePos.x;
		currentZ = nodePath.back()->mNodePos.y;

		currIndex = ( currentX * mMapWidth ) + currentZ;

		path->AddSubPath( currStart, end, currIndex );

	}

	//path->AddSubPath( currStart, end, currIndex );
}

bool Pathfinder::CalculateSubPath( Path* path, int nrOfSteps )
{
	if( nrOfSteps == 0 )
		nrOfSteps = path->mNrOfSubPaths;
	else
		path->mCurrentSubPath += nrOfSteps;

	for( int i = path->mCurrentSubPath; i < path->mNrOfSubPaths; i++ )
	{
		SubPath* temp = &path->mSubPaths[i];
		if(! temp->mIsCalculated )
		{
			temp->mPoints = mNavmeshMap[path->mSubPaths[i].mNavMeshIndex]->FindPath( temp->mStart, temp->mEnd );
			temp->mIsCalculated = true;
			path->mCurrentSubPath = i;
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
	if( mNodeGraph )
		mNodeGraph->Release();
		delete mNodeGraph;

	delete[] mNavmeshMap;

	if( instance )
		delete instance;
}

Pathfinder::~Pathfinder()
{
}
