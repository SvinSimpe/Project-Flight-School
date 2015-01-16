#ifndef MAPNODEINSTANCE_H
#define MAPNODEINSTANCE_H
#include <DirectXMath.h>
#include <Windows.h>
#include "BoundingGeometry.h"
static enum ExitDir
{
	WEST,
	NORTH,
	EAST,
	SOUTH
};
struct Corners
{
	int left;
	int top;
	int right;
	int bottom;

	Corners()
	{
		left	= 0;
		top		= 0;
		right	= 0;
		bottom	= 0;
	}
	Corners(int pLeft, int pTop, int pRight, int pBottom)
	{
		left	= pLeft;
		top		= pTop;
		right	= pRight;
		bottom	= pBottom;
	};
};



//forward declaration

class MapNode;
class MapNodeInstance;

struct ExitPoint
{
	DirectX::XMFLOAT3 exit;
	bool valid;
	MapNodeInstance* neighbour;
};

class MapNodeInstance
{
public:

	private:
		int mInstanceID;
		MapNode* mNode;

		DirectX::XMFLOAT3 mPos;
		DirectX::XMFLOAT3 mOrigin;
		DirectX::XMFLOAT4X4 mWorld;


		int mRotation;
		ExitPoint	mExits[4];
		Corners		mCorners;

	protected:
	public:
	private:
		void				SetUpExits();
		void				SetCorners();
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

		ExitPoint			GetExit( int exitSlot );
		void				SetExit( ExitPoint exit );
		XMFLOAT3			GetExitPos( int exitSlot );
		int					HasExit();

		BoundingBox			GetBoundingBox();
		
		void				AddNeighbour( int exitSlot, MapNodeInstance* neighbour );

		HRESULT				Initialize();
		void				Release();
		
		

							MapNodeInstance();
		virtual				~MapNodeInstance();
};
#endif