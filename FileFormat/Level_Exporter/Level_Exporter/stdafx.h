#pragma once

/***********************/
/*** GLOBAL INCLUDES ***/
/***********************/

#include <string>
#include <iostream>
#include <fstream>
#include <vector>

using namespace std;


/*********************/
/*** MAYA INCLUDES ***/
/*********************/

#define NT_APP // Extern Maya-application

#include <maya\MLibrary.h>
#include <maya\MFileIO.h>
#include <maya\MGlobal.h>

#include <maya\MDagPath.h>
#include <maya\MFloatPointArray.h>
#include <maya\MFnPointLight.h>
#include <maya\MFnVolumeLight.h>

#include <maya\MFnIkJoint.h>
#include <maya\MFnSkinCluster.h>
#include <maya/MDagPathArray.h>
#include <maya/MItGeometry.h>

#include <maya\MFnMesh.h>
#include <maya\MFnCamera.h>
#include <maya\MFnDependencyNode.h>
#include <maya\MItDependencyNodes.h>
#include <maya\MItDependencyGraph.h>
#include <maya\MPlug.h>

#include <maya\MFnTransform.h>
#include <maya\MEulerRotation.h>
#include <maya\MQuaternion.h>
#include <maya\MMatrix.h>

#include <maya\MItDag.h>
#include <maya\MItMeshPolygon.h>

#pragma comment(lib, "Foundation.lib")
#pragma comment(lib, "OpenMaya.lib")
#pragma comment(lib, "OpenMayaAnim.lib")