#ifndef NAVMESH_H
#define NAVMESH_H
#include <Windows.h>
#include <DirectXMath.h>
#include <vector>
#include <unordered_map>


struct NavTriangle
{
	DirectX::XMFLOAT3 triPoints[3];
	UINT adjTri[3];
	int mMapNodeIndex;
};

//forward declaration

typedef UINT ClusterID;
typedef UINT TriangleID;
typedef std::vector<NavTriangle> NavData;
typedef std::unordered_map<ClusterID, TriangleID> NavCluster;



class Navmesh
{
	private:
		NavData mMesh;
		NavCluster mCluster;
		
	protected:
	public:

	private:
	protected:
	public:
		HRESULT Initialize();

		HRESULT AddTri( NavTriangle navTri );
		NavData GetClusterData( int clusterID );
		

		Navmesh();
		~Navmesh();
};

#endif