#ifndef MAPNODEINSTANCE_H
#define MAPNODEINSTANCE_H
#include <DirectXMath.h>
#include <Windows.h>
#include <vector>
#include "BoundingGeometry.h"
#include "Navmesh.h"


//forward declaration

class MapNode;
class MapNodeInstance
{
public:

	private:
		int mInstanceID;
		int mNodeID;
		int mSizeX;
		int mSizeY;

		MapNode* mNode;

		DirectX::XMFLOAT3 mPos;
		DirectX::XMFLOAT3 mOrigin;
		DirectX::XMFLOAT4X4 mWorld;

		Navmesh* mNavMesh;
		std::vector<DirectX::XMFLOAT3> mEdgePoints;

		UINT		 mNavTriangleCount;

	protected:
	public:

	private:
		void				GetNavigationData();
	protected:
	public:

		HRESULT				Update( float deltaTime );
		HRESULT				Render( float deltaTime );

		Navmesh*			GetNavMesh() const;

		DirectX::XMFLOAT3	GetClosestEdgePoint( DirectX::XMFLOAT3 pos );
		DirectX::XMFLOAT3	GetPos() const;
		void				SetPos( DirectX::XMFLOAT3 pos );

		int					GetNodeSizeX();
		int					GetNodeSizeY();

		void				SetNodeID( int ID );
		int					GetNodeID() const;

		void				SetInstanceID( int ID );
		int					GetInstanceID() const;

		void				ReleaseInstance();

		DirectX::XMFLOAT3	GetOrigin() const;
		void				SetOrigin( DirectX::XMFLOAT3 origin );
		
		MapNode*			GetMapNode() const;
		void				SetMapNode( MapNode* mapNode );

		BoundingRectangle			GetBoundingBox();

		NavTriangle*		IsOnNavMesh( DirectX::XMFLOAT3 pos ) const;			

		HRESULT				Initialize();
		void				Release();
	
							MapNodeInstance();
		virtual				~MapNodeInstance();
};
#endif