#include "stdafx.h"

struct Matrix
{
	char name[64];
	MMatrix derp;
};

struct Vertex
{
	float Position[3];
	float Normals[3];
};

struct VolumeLight
{
	char name[32];
	float position[3];
	float color[3];
	float intensity;
	float range;
};

struct NavVertex
{
	float Position[3];
};

struct GridData
{
	char blendMap[64];
	UINT dimensions[2];
	Vertex* vertices;
};

struct NavMeshData
{
	NavVertex* vertices;

};
