#include "stdafx.h"

struct Matrix
{
	char name[50];
	float translate[3];
	float rotate[3];
	float scale[3];
};

struct Vertex
{
	float Position[3];
	float Normals[3];
};

struct GridData
{;
	UINT dimensions[2];
	Vertex* vertices;
	Matrix matrix;
};

