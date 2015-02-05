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

struct NavVertex
{
	float Position[3];
};

struct GridData
{
	UINT dimensions[2];
	Vertex* vertices;
	Matrix matrix;
};

struct NavMeshData
{
	NavVertex* vertices;

};

