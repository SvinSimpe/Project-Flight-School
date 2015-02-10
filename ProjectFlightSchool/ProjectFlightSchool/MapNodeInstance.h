#ifndef MAPNODEINSTANCE_H
#define MAPNODEINSTANCE_H
#include <DirectXMath.h>
#include <Windows.h>
#include <vector>
#include "BoundingGeometry.h"

struct NavTriangle
{
	DirectX::XMFLOAT3 triPoints[3];
	int adjTri[3];
};

//forward declaration

class MapNode;
class MapNodeInstance
{
public:

	private:
		int mInstanceID;
		MapNode* mNode;

		DirectX::XMFLOAT3 mPos;
		DirectX::XMFLOAT3 mOrigin;
		DirectX::XMFLOAT4X4 mWorld;

		NavTriangle* mNavMesh;

	protected:
	public:

	private:
		void				GetNavigationData();
	protected:
	public:

		HRESULT				Update( float deltaTime );
		HRESULT				Render( float deltaTime );

		
		DirectX::XMFLOAT3	GetPos() const;
		void				SetPos( DirectX::XMFLOAT3 pos );

		void				SetInstanceID( int ID );
		int					GetInstanceID() const;

		void				ReleaseInstance();

		DirectX::XMFLOAT3	GetOrigin() const;
		void				SetOrigin( DirectX::XMFLOAT3 origin );
		
		MapNode*			GetMapNode() const;
		void				SetMapNode( MapNode* mapNode );

		BoundingBox			GetBoundingBox();

		HRESULT				Initialize();
		void				Release();
		
		

							MapNodeInstance();
		virtual				~MapNodeInstance();
};
#endif