#ifndef NAVMESH_H
#define NAVMESH_H
#include <Windows.h>
#include <DirectXMath.h>
#include <list>

struct Portal
{
	int points[2];
	int adjTri;
	Portal()
	{
	}
	Portal( int p1, int p2, int pAdjTri = -1 )
	{
		points[0] = p1;
		points[1] = p2;

		adjTri = pAdjTri;
	}
};
struct NavTriangle
{
	DirectX::XMFLOAT3 triPoints[3];
	DirectX::XMFLOAT3 center;

	NavTriangle()
	{ }
	NavTriangle(DirectX::XMFLOAT3 p1, DirectX::XMFLOAT3 p2, DirectX::XMFLOAT3 p3 )
	{
		triPoints[0] = p1;
		triPoints[1] = p2;
		triPoints[2] = p3;
	}
	Portal portals[3];
};
class Navmesh
{
	private:
		Portal* mPortals;
		DirectX::XMFLOAT3* mMesh;		
		UINT mNavTriangleCount;
	protected:
	public:

	private:
		bool IsTriangleAdj( Portal t1, Portal t2 );
		bool BuildAdjacencyInfo();
		void FunnelPath();
	protected:
	public:
		HRESULT Render();

		HRESULT Initialize( DirectX::XMFLOAT3* meshData, UINT vertexCount );
		std::list<DirectX::XMFLOAT3> Path( DirectX::XMFLOAT3 start, DirectX::XMFLOAT3 end );

		Navmesh();
		~Navmesh();
};

#endif