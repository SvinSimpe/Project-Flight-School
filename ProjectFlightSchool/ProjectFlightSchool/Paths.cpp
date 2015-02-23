#include "Paths.h"
#include "HelperFunctions.h"
HRESULT SubPath::Initialize( DirectX::XMFLOAT3 start, DirectX::XMFLOAT3 end, int navMeshIndex )
{
	mStart			= start;
	mEnd			= end;
	mNavMeshIndex	= navMeshIndex;

	return S_OK;
}

SubPath::SubPath()
{
	mIsCalculated		= false;
	mNavMeshIndex		= 0;
}

SubPath::~SubPath()
{
}

bool SubPath::operator==( const SubPath& origOb )const
{
	return HelperFunctions::Float3Equal( origOb.mStart, mStart ) && HelperFunctions::Float3Equal( origOb.mEnd, mEnd );
}

///-------------------
bool Path::AddSubPath( DirectX::XMFLOAT3 start, DirectX::XMFLOAT3 end, int navMeshIndex )
{
	SubPath temp;
	temp.mNavMeshIndex = navMeshIndex;
	temp.mStart = start;
	temp.mEnd = end;
	temp.mIsCalculated = false;

	mSubPaths[mNrOfSubPaths++] = temp;

	return true;
}
SubPath* Path::GetNextSubPath()
{
	return &mSubPaths[mCurrentSubPath++];
}

Path::Path()
{
	mNrOfSubPaths = 0;
	mCurrentSubPath = 0;
}


Path::~Path()
{
}
