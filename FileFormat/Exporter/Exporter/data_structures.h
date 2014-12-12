#pragma once

#include "stdafx.h"

struct vec2
{
	float x, y;
};

struct vec3
{
	float x, y, z;
};

struct vec4
{
	float x, y, z, w;
};

struct Vertex_Static
{
	vec3 position;
	vec3 normal;
	vec3 tangent;
	vec2 uv;
};

struct Vertex_Anim
{
	vec3 position;
	vec3 normal;
	vec3 tangent;
	vec2 uv;

	vec4 weights;
	int jointIndex[4];
};


struct MeshInfo_Maya
{
	MString meshName;
	MDagPath meshPath;

	MString diffuseMapName;
	MString normalMapName;
	MString specularMapName;

	MFloatPointArray points;
	MFloatVectorArray normals;
	MFloatVectorArray tangents;
	MFloatArray uvs_x;
	MFloatArray uvs_y;

	MFloatArray weights;
	MIntArray jointIndex;
};

struct MeshInfo
{
	UINT vertexCount;

	char meshName[30];
	char diffuseMapName[30];
	char normalMapName[30];
	char specularMapName[30];

	char skeletonName[30];
};

struct MeshData_Export_Static //MeshData for static objects
{
	MeshInfo meshInfo;
	Vertex_Static* vertices;
};

struct MeshData_Export_Anim // MeshData for animated objects
{
	MeshInfo meshInfo;
	Vertex_Anim* vertices;
};

struct Joint
{
	char jointName[30];
	char parentName[30];

	float matrix[4][4];
};

struct Skeleton
{
	const char* fileName;
	UINT jointCount;

	Joint* joints;
};