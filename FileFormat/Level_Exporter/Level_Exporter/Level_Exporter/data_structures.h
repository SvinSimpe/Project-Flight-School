#include "stdafx.h"

struct Matrix
{
	char name[50];
	double transform[4][4];
};

struct Vertex
{
	float Position[3];
	float Normals[3];
};

struct GridData
{;
	Vertex* vertices;
	Matrix matrix;
};

