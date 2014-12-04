#pragma once

#include "data_structures.h"

class Exporter
{
private:
	string g_FilePath;

	vector<MeshInfo_Maya> g_meshInfoMaya;
	vector<MeshData_Export_Static> g_meshDataExportStatic;
	vector<MeshData_Export_Anim> g_meshDataExportAnim;
	vector<Skeleton> g_skeletonData;
	
	char waitingForInput;

public:
	Exporter();
	~Exporter();

	void RunExporter();

private:

	bool InitializeMaya();
	void ReleaseMaya();

	bool GetMayaFilenamesInDirectory(vector<string> &filenameList);
	void GetNodePath(MFnMesh &mesh, MeshInfo_Maya &meshData);
	void GetTexture(MPlugArray &shaderConnections, MeshInfo_Maya &meshData);
	void GetNormalMap(MPlugArray &shaderConnections, MeshInfo_Maya &meshData);
	void GetSpecularMap(MPlugArray &shaderConnections, MeshInfo_Maya &meshData);
	void SceneManager(const char* fileName);
	void ReleaseDataTypes();

	void ConvertAndBuildMeshInfoExport();
	void ConvertAndBuildStaticMesh(MeshInfo_Maya* meshIter);
	void ConvertAndBuildAnimMesh(MeshInfo_Maya* meshIter);

	bool ExtractCurrentSceneRawData();
	bool ExtractMeshData(MFnMesh &mesh);

	void GetWeightAndJointsPerSkinCluster();
	void GetJointsPerSkinCluster(MFnSkinCluster &skinCluster);

	string CreateExportFile(string fileName, string fileEnding);
	void WriteStaticMeshToFileBinary(const char* fileName);
	void WriteAnimMeshToFileBinary(const char* fileName);
	void WriteSkelToFileBinary();

};

