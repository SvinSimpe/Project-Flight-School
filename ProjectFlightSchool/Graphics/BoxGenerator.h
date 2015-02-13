#ifndef BOXGENERATOR_H
#define BOXGENERATOR_H

#include "AssetBase.h"
#include "Vertex.h"
#include <DirectXCollision.h>
using namespace std;

enum OctreeParts
{
  TOP_FRONT_LEFT		= 0,
  TOP_FRONT_RIGHT		= 1,
  TOP_BACK_LEFT			= 2,
  TOP_BACK_RIGHT		= 3,
  BOTTOM_FRONT_LEFT		= 4,
  BOTTOM_FRONT_RIGHT	= 5,
  BOTTOM_BACK_LEFT		= 6,
  BOTTOM_BACK_RIGHT		= 7
};

struct AABB
{
	DirectX::XMFLOAT3 max;
	DirectX::XMFLOAT3 min;
};

struct OctTree
{
	//int			levels;
	AABB		boundingBox;
	OctTree*	children[8];
};
//struct OctDivision
//{
//	AABB		boundingBox;
//	bool		collides;
//	OctDivision *children[8];
//};

class BoxGenerator
{
	private:
	protected:
	public:
		

	private:
		AABB		Subdivide(AABB* originalBox, int witchBox);
	protected:
	public:
		HRESULT		Initialize();
		AABB		CreateAABBFromVerts(vector<StaticVertex>* vertices);
		OctTree		GenerateOctTree(vector<StaticVertex>* vertices, AABB* boundingAABB, int maxLevel, int currentLevel);
		bool		CheckIntersectionTriangleVSAABB(vector<StaticVertex>* vertices, AABB* collisionBox);
		void		Release();
					BoxGenerator();
		virtual		~BoxGenerator();
};

#endif