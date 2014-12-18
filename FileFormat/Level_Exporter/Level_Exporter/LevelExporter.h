#pragma once
#include "data_structures.h"
class LevelExporter
{
private:
	string g_FilePath;
	string g_AssetPath;

	UINT GRIDCOUNT;
	UINT vertexCount;
	char waitingForInput;

	vector<Matrix> matrices;
	
	GridData gridData;

public:
	LevelExporter();
	~LevelExporter();

	void RunExporter();

private:

	bool InitializeMaya();
	void ReleaseMaya();

	bool GetMayaFilenamesInDirectory(vector<string> &filenameList);
	void SceneManager(const char* fileName);
	void ReleaseDataTypes();

	bool ExtractCurrentSceneRawData();
	Matrix ExtractAndConvertMatrix(MFnMesh &mesh, int fauling);
	void ExtractAndConvertGridData(MFnMesh &mesh);
	bool ExtractGridData(MFnMesh &mesh);
	void ConvertGridData(MFnMesh &mesh, MFloatPointArray &points, MFloatVectorArray &normals);
	void GetDimensions(MFnMesh &mesh, UINT* dimensions);

	string CreateExportFile(string fileName, string fileEnding);
	void WriteStaticMeshToFileBinary(const char* fileName);

	void WriteFileToBinary(const char* fileName); 

};
