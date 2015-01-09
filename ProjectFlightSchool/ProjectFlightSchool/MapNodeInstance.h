#ifndef MAPNODEINSTANCE_H
#define MAPNODEINSTANCE_H
#include <DirectXMath.h>
#include <Windows.h>


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
		int mRotation;
		Corners mCorners;
		DirectX::XMFLOAT4X4 mWorld;

	protected:
	public:
	private:
	protected:
	public:
		HRESULT				Update( float deltaTime );
		HRESULT				Render( float deltaTime );

		DirectX::XMFLOAT3	GetPos()const;
		void				SetPos( DirectX::XMFLOAT3 pos );
		
		MapNode*			GetMapNode()const;
		void				SetMapNode( MapNode* mapNode );

		Corners				GetCorners()const;
		void				SetCorners( int left, int top );

		HRESULT				Initialize( MapNode* mapNode );
		void				Release();
							MapNodeInstance();
		virtual				~MapNodeInstance();
};
#endif