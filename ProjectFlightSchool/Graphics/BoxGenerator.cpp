#include "BoxGenerator.h"

HRESULT BoxGenerator::Initialize()
{
	return S_OK;
}

AABB BoxGenerator::CreateAABBFromVerts(vector<StaticVertex>* vertices)
{
	AABB tempAABB;
	vector<StaticVertex>& verticesRef = *vertices;

	tempAABB.max.x =0.0f;
	tempAABB.max.y =0.0f;
	tempAABB.max.z =0.0f;
	tempAABB.min.x =0.0f;
	tempAABB.min.y =0.0f;
	tempAABB.min.z =0.0f;

	
	//Extrude Box
	for(UINT i = 0; i < verticesRef.size(); i++)
	{
		//X max Check
		if(verticesRef.at(i).position[0] > tempAABB.max.x)
			tempAABB.max.x = verticesRef.at(i).position[0];
		
		//Y max Check
		if(verticesRef.at(i).position[1] > tempAABB.max.y)
			tempAABB.max.y = verticesRef.at(i).position[1];

		//Z max Check
		if(verticesRef.at(i).position[2] > tempAABB.max.z)
			tempAABB.max.z = verticesRef.at(i).position[2];

		//X min Check
		if(verticesRef.at(i).position[0] < tempAABB.min.x)
			tempAABB.min.x = verticesRef.at(i).position[0];

		//Y min Check
		if(verticesRef.at(i).position[1] < tempAABB.min.y)
			tempAABB.min.y = verticesRef.at(i).position[1];

		//Z min Check
		if(verticesRef.at(i).position[2] < tempAABB.min.z)
			tempAABB.min.z = verticesRef.at(i).position[2];
	}
	
	return tempAABB;
}
OctTree		BoxGenerator::GenerateOctTree(vector<StaticVertex>* vertices, AABB* boundingAABB, int maxLevel, int currentLevel)
{
	//Divide given AABB into children unless given level is reached
	currentLevel++;
	OctTree						thisLevel;
	thisLevel.boundingBox		= *boundingAABB;
	AABB						testBox;
	for(int i = 0; i < 8; i++)
	{
		thisLevel.childrenCollides[i] = false;
	}
	//find TOP_FRONT_LEFT AABB
	testBox = Subdivide(boundingAABB, TOP_FRONT_LEFT);
	if(CheckIntersectionTriangleVSAABB(vertices, &testBox) && currentLevel < maxLevel)
	{
		//thisLevel.childrenCollides[TOP_FRONT_LEFT] = true;
		thisLevel.children[TOP_FRONT_LEFT] = &GenerateOctTree(vertices, &testBox, maxLevel, currentLevel);
	}
	//find TOP_FRONT_RIGHT AABB
	testBox = Subdivide(boundingAABB, TOP_FRONT_RIGHT);
	if(CheckIntersectionTriangleVSAABB(vertices, &testBox) && currentLevel < maxLevel)
	{
		thisLevel.childrenCollides[TOP_FRONT_RIGHT] = true;
		thisLevel.children[TOP_FRONT_RIGHT] = &GenerateOctTree(vertices, &testBox, maxLevel, currentLevel);
	}
	//find TOP_BACK_LEFT AABB
	testBox = Subdivide(boundingAABB, TOP_BACK_LEFT);
	if(CheckIntersectionTriangleVSAABB(vertices, &testBox) && currentLevel < maxLevel)
	{
		thisLevel.childrenCollides[TOP_BACK_LEFT] = true;
		thisLevel.children[TOP_BACK_LEFT] = &GenerateOctTree(vertices, &testBox, maxLevel, currentLevel);
	}
	//find TOP_BACK_RIGHT AABB
	testBox = Subdivide(boundingAABB, TOP_BACK_RIGHT);
	if(CheckIntersectionTriangleVSAABB(vertices, &testBox) && currentLevel < maxLevel)
	{
		thisLevel.childrenCollides[TOP_BACK_RIGHT] = true;
		thisLevel.children[TOP_BACK_RIGHT] = &GenerateOctTree(vertices, &testBox, maxLevel, currentLevel);
	}
	//find BOTTOM_FRONT_LEFT AABB
	testBox = Subdivide(boundingAABB, BOTTOM_FRONT_LEFT);
	if(CheckIntersectionTriangleVSAABB(vertices, &testBox) && currentLevel < maxLevel)
	{
		thisLevel.childrenCollides[BOTTOM_FRONT_LEFT] = true;
		thisLevel.children[BOTTOM_FRONT_LEFT] = &GenerateOctTree(vertices, &testBox, maxLevel, currentLevel);
	}
	//find BOTTOM_FRONT_RIGHT  AABB
	testBox = Subdivide(boundingAABB, BOTTOM_FRONT_RIGHT);
	if(CheckIntersectionTriangleVSAABB(vertices, &testBox) && currentLevel < maxLevel)
	{
		thisLevel.childrenCollides[BOTTOM_FRONT_RIGHT] = true;
		thisLevel.children[BOTTOM_FRONT_RIGHT] = &GenerateOctTree(vertices, &testBox, maxLevel, currentLevel);
	}
	//find BOTTOM_BACK_LEFT AABB
	testBox = Subdivide(boundingAABB, BOTTOM_BACK_LEFT);
	if(CheckIntersectionTriangleVSAABB(vertices, &testBox) && currentLevel < maxLevel)
	{
		thisLevel.childrenCollides[BOTTOM_BACK_LEFT] = true;
		thisLevel.children[BOTTOM_BACK_LEFT] = &GenerateOctTree(vertices, &testBox, maxLevel, currentLevel);
	}
	//find BOTTOM_BACK_RIGHT AABB
	testBox = Subdivide(boundingAABB, BOTTOM_BACK_RIGHT);
	if(CheckIntersectionTriangleVSAABB(vertices, &testBox) && currentLevel < maxLevel)
	{
		thisLevel.childrenCollides[BOTTOM_BACK_RIGHT] = true;
		thisLevel.children[BOTTOM_BACK_RIGHT] = &GenerateOctTree(vertices, &testBox, maxLevel, currentLevel);
	}

	//we dont look for another level so all children are set to nullpointers
	if(currentLevel > maxLevel)
	{
		for(UINT i = 0; i < 8; i++)
		{
			thisLevel.children[i] = nullptr;
		}
	}
	return thisLevel;
}
bool	BoxGenerator::CheckIntersectionTriangleVSAABB(vector<StaticVertex>* vertices, AABB* collisionBox)
{
	DirectX::XMFLOAT3 center;
	center.x = collisionBox->max.x - ((collisionBox->max.x - collisionBox->min.x) * 0.5);
	center.y = collisionBox->max.y - ((collisionBox->max.y - collisionBox->min.y) * 0.5);
	center.z = collisionBox->max.z - ((collisionBox->max.z - collisionBox->min.z) * 0.5);
	DirectX::XMFLOAT3 extents;
	extents.x = (collisionBox->max.x - collisionBox->min.x);
	extents.y = (collisionBox->max.y - collisionBox->min.y);
	extents.z = (collisionBox->max.z - collisionBox->min.z);

	DirectX::BoundingBox colissionBox(center, extents);
	for(UINT i = 0; i < vertices->size(); i += 3)
	{
		DirectX::XMFLOAT3 floatToVert1(vertices->at(i).position[0],vertices->at(i).position[1], vertices->at(i).position[2]);
		DirectX::FXMVECTOR vert1 = XMLoadFloat3(&floatToVert1);

		DirectX::XMFLOAT3 floatToVert2(vertices->at(i + 1).position[0],vertices->at(i + 1).position[1], vertices->at(i + 1).position[2]);
		DirectX::FXMVECTOR vert2 = XMLoadFloat3(&floatToVert2);

		DirectX::XMFLOAT3 floatToVert3(vertices->at(i + 2).position[0],vertices->at(i + 2).position[1], vertices->at(i + 2).position[2]);
		DirectX::FXMVECTOR vert3 = XMLoadFloat3(&floatToVert3);

		bool result = colissionBox.Intersects(vert1, vert2, vert3);
		if(result)
		{
			return true;
		}
	}
    return false;
}
AABB	BoxGenerator::Subdivide(AABB* originalBox, int witchBox)
{
	//Return specific part of given AABB
	AABB outBox;
	switch(witchBox)
	{
	case TOP_FRONT_LEFT:
		{
			//min
			outBox.min.x = originalBox->min.x;
			outBox.min.y = originalBox->max.y - ((originalBox->max.y - originalBox->min.y) * 0.5);
			outBox.min.z = originalBox->max.z - ((originalBox->max.z - originalBox->min.z) * 0.5);
			//max
			outBox.max.x = originalBox->max.x - ((originalBox->max.x - originalBox->min.x) * 0.5);
			outBox.max.y = originalBox->max.y;
			outBox.max.z = originalBox->max.z;
		}break;
	case TOP_FRONT_RIGHT:
		{
		//min
			outBox.min.x = originalBox->max.x - ((originalBox->max.x - originalBox->min.x) * 0.5);
			outBox.min.y = originalBox->max.y - ((originalBox->max.y - originalBox->min.y) * 0.5);
			outBox.min.z = originalBox->max.z - ((originalBox->max.z - originalBox->min.z) * 0.5);
			//max
			outBox.max.x = originalBox->max.x;
			outBox.max.y = originalBox->max.y;
			outBox.max.z = originalBox->max.z;
		}break;
	case TOP_BACK_LEFT:	
		{
			//min
			outBox.min.x = originalBox->min.x;
			outBox.min.y = originalBox->max.y - ((originalBox->max.y - originalBox->min.y) * 0.5);
			outBox.min.z = originalBox->min.z;
			//max
			outBox.max.x = originalBox->max.x - ((originalBox->max.x - originalBox->min.x) * 0.5);
			outBox.max.y = originalBox->max.y;
			outBox.max.z = originalBox->max.z - ((originalBox->max.z - originalBox->min.z) * 0.5);
		}break;	
	case TOP_BACK_RIGHT:
		{
			//min
			outBox.min.x = originalBox->max.x - ((originalBox->max.x - originalBox->min.x) * 0.5);
			outBox.min.y = originalBox->max.y - ((originalBox->max.y - originalBox->min.y) * 0.5);
			outBox.min.z = originalBox->min.z;
			//max
			outBox.max.x = originalBox->max.x;
			outBox.max.y = originalBox->max.y;
			outBox.max.z = originalBox->max.z - ((originalBox->max.z - originalBox->min.z) * 0.5);
		}break;	
	case BOTTOM_FRONT_LEFT:	
		{
			//min
			outBox.min.x = originalBox->min.x;
			outBox.min.y = originalBox->min.y;
			outBox.min.z = originalBox->max.z - ((originalBox->max.z - originalBox->min.z) * 0.5);
			//max
			outBox.max.x = originalBox->max.x - ((originalBox->max.x - originalBox->min.x) * 0.5);
			outBox.max.y = originalBox->max.y - ((originalBox->max.y - originalBox->min.y) * 0.5);
			outBox.max.z = originalBox->max.z;
		}break;
	case BOTTOM_FRONT_RIGHT:
		{
			//min
			outBox.min.x = originalBox->max.x - ((originalBox->max.x - originalBox->min.x) * 0.5);
			outBox.min.y = originalBox->min.y;
			outBox.min.z = originalBox->max.z - ((originalBox->max.z - originalBox->min.z) * 0.5);
			//max
			outBox.max.x = originalBox->max.x;
			outBox.max.y = originalBox->max.y - ((originalBox->max.y - originalBox->min.y) * 0.5);
			outBox.max.z = originalBox->max.z;
		}break;
	case BOTTOM_BACK_LEFT:	
		{
			//min
			outBox.min.x = originalBox->min.x;
			outBox.min.y = originalBox->min.y;
			outBox.min.z = originalBox->min.z;
			//max
			outBox.max.x = originalBox->max.x - ((originalBox->max.x - originalBox->min.x) * 0.5);
			outBox.max.y = originalBox->max.y - ((originalBox->max.y - originalBox->min.y) * 0.5);
			outBox.max.z = originalBox->max.z - ((originalBox->max.z - originalBox->min.z) * 0.5);
		}break;
	case BOTTOM_BACK_RIGHT:	
		{
			//min
			outBox.min.x = originalBox->min.x;
			outBox.min.y = originalBox->min.y;
			outBox.min.z = originalBox->max.z - ((originalBox->max.z - originalBox->min.z) * 0.5);
			//max
			outBox.max.x = originalBox->max.x - ((originalBox->max.x - originalBox->min.x) * 0.5);
			outBox.max.y = originalBox->max.y - ((originalBox->max.y - originalBox->min.y) * 0.5);
			outBox.max.z = originalBox->max.z;
		}break;
	}
	return outBox;
}


BoxGenerator::BoxGenerator()
{
}

BoxGenerator::~BoxGenerator()
{
}