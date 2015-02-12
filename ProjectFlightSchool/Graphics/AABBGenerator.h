#ifndef AABBGENERATOR_H
#define AABBGENERATOR_H

#include "AssetBase.h"
#include "Vertex.h"
#include <vector>
#include <DirectXMath.h>
using namespace DirectX;
using namespace std;

struct AABB
{
	XMFLOAT3 max;
	XMFLOAT3 min;
};

class AABBGenerator
{
	private:
	protected:
	public:
		

	private:
	protected:
	public:
		HRESULT		Initialize();
		AABB		CreateAABBFromVerts(vector<StaticVertex>* vertices);
		void		Release();
					AABBGenerator();
		virtual		~AABBGenerator();
};

#endif