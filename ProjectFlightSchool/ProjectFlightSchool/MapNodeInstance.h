#ifndef MAPNODEINSTANCE_H
#define MAPNODEINSTANCE_H
#include <DirectXMath.h>
#include <Windows.h>


static enum Rotation
{
	D0		= 0,
	D90		= 1,
	D180	= 2,
	D270	= 3
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

//Forward declaration for the compiler
class MapNode;

class MapNodeInstance
{
	private:
		MapNode* mNode;

		DirectX::XMFLOAT3 mPos;
		DirectX::XMFLOAT3 mOrigin;
		DirectX::XMFLOAT4X4 mWorld;

		Rotation mNodeRotation;
		int mRotation;

		Corners mCorners;
		ExitPoints* mExits;

	protected:
	public:
	private:
		void				GetExits();
	protected:
	public:
		HRESULT				Update( float deltaTime );
		HRESULT				Render( float deltaTime );

		DirectX::XMFLOAT3	GetPos()const;
		void				SetPos( DirectX::XMFLOAT3 pos );

		DirectX::XMFLOAT3	GetOrigin()const;
		void				SetOrigin( DirectX::XMFLOAT3 orign );
		
		MapNode*			GetMapNode()const;
		void				SetMapNode( MapNode* mapNode );

		Corners				GetCorners()const;
		void				SetCorners( int left, int top );

		HRESULT				Initialize( MapNode* mapNode );
		void				Release();

		bool				AddNeighbour( MapNodeInstance* mapNodeInstance );
							MapNodeInstance();
		virtual				~MapNodeInstance();
};
#endif