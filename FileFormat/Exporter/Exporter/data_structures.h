#pragma once

#include "stdafx.h"

struct MeshInfo
{
	MString meshName;
	
	MFloatPointArray points;
	MFloatVectorArray normals;
	MFloatArray uvs_x;
	MFloatArray uvs_y;
};