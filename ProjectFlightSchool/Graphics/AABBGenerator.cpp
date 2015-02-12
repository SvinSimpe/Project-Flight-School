#include "AABBGenerator.h"

HRESULT AABBGenerator::Initialize()
{
	return S_OK;
}

AABB AABBGenerator::CreateAABBFromVerts(vector<StaticVertex>* vertices)
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


AABBGenerator::AABBGenerator()
{
}

AABBGenerator::~AABBGenerator()
{
}