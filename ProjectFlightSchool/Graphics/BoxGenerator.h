#ifndef BOXGENERATOR_H
#define BOXGENERATOR_H

#include "AssetBase.h"
#include "Vertex.h"
#include <DirectXCollision.h>
using namespace std;
using namespace DirectX;

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
	AABB		boundingBox;
	bool		childrenCollides[8];
	bool		collides;
	bool		lastLevel;
	OctTree*	children[8];
	OctTree()
	{
		for(UINT i = 0; i < 8 ; i++)
		{
			children[i] = nullptr;
		}
	}
	bool AABBvsAABB( DirectX::XMFLOAT3 min, DirectX::XMFLOAT3 max, UINT maxDepth, DirectX::XMFLOAT3 &normal )
	{
		if( maxDepth > 0 )
		{
			if( min.x > boundingBox.max.x || boundingBox.min.x > max.x ) return false;
			if( min.y > boundingBox.max.y || boundingBox.min.y > max.y ) return false;
			if( min.z > boundingBox.max.z || boundingBox.min.z > max.z ) return false;
			
			if( maxDepth == 1 )
			{
				XMStoreFloat3( &normal, ( XMLoadFloat3( &boundingBox.min ) + XMLoadFloat3( &boundingBox.max ) ) * 0.5f - XMLoadFloat3( &normal ) );
				normal.y = 0.0f;
				XMVECTOR loaded = XMLoadFloat3( &normal );
				XMStoreFloat3( &normal, XMVector3Normalize( loaded ) );
				return true;
			}
		
			bool childCollision = false;
			for( int i = 0; i < 8; i++ )
				if( childrenCollides[i] )
					if( children[i]->AABBvsAABB( min, max, maxDepth - 1, normal ) ) childCollision = true;

			return childCollision;
		}
		return false;
	}
};

class BoxGenerator
{
	private:
	protected:
	public:
		

	private:
		AABB		Subdivide( AABB* originalBox, int witchBox );
	protected:
	public:
		HRESULT		Initialize();
		AABB		CreateAABBFromVerts( vector<StaticVertex>* vertices );
		void		GenerateOctTree( vector<StaticVertex>* vertices, AABB* boundingAABB, int maxLevel, int currentLevel, OctTree* inOct );
		bool		CheckIntersectionTriangleVSAABB( vector<StaticVertex>* vertices, AABB* collisionBox );
		void		Release( OctTree* killMe );
					BoxGenerator();
		virtual		~BoxGenerator();
};

#endif