#ifndef NAVMESH_H
#define NAVMESH_H

#include <Windows.h>
#include <DirectXMath.h>
#include <list>
#include <vector>


struct Portal
{
	DirectX::XMFLOAT3 center;
	int portalID;
	DirectX::XMFLOAT3 points[2];
	int adjTri;

	float h;
	float g;

	Portal()
	{
		adjTri		= -1;

		h = 0;
		g = 0;
	}
	Portal( DirectX::XMFLOAT3 p1, DirectX::XMFLOAT3 p2, int pAdjTri = -1 )
	{
		points[0] = p1;
		points[1] = p2;

		adjTri = pAdjTri;
	}
};

struct PortalPath
{
	Portal* portal;
	PortalPath* parent;

	PortalPath()
	{
		portal = nullptr;
		parent = nullptr;
	}

	bool operator==( Portal const& p1 )
	{
		return portal->portalID == p1.portalID;
	}
};

typedef std::list<PortalPath*> PathList;

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
		PortalPath*	mPath;
		Portal* mPortals;
		DirectX::XMFLOAT3* mMesh;	

		UINT mNavTriangleCount;
		UINT mMaxPathLength;

		std::vector<DirectX::XMFLOAT3> mEdgePoints;

	protected:
	public:

	private:
		bool IsTriangleAdj( Portal t1, Portal t2 );
		bool BuildAdjacencyInfo();
		std::vector<DirectX::XMFLOAT2> FunnelPath( std::vector<Portal>& path );
	protected:
	public:
		HRESULT Render();
		
		HRESULT Initialize( DirectX::XMFLOAT3* meshData, UINT vertexCount, std::vector<DirectX::XMFLOAT3>& edgePoints );
		bool IsOnNavMesh( DirectX::XMFLOAT3 pos );
		std::vector<DirectX::XMFLOAT2> FindPath( DirectX::XMFLOAT3 start, DirectX::XMFLOAT3 end );
		DirectX::XMFLOAT3 GetClosestEdgePoint( DirectX::XMFLOAT3 start, DirectX::XMFLOAT3 goal );
		void Release();

		Navmesh();
		~Navmesh();
};

#endif