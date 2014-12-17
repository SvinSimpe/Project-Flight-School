#include "LevelExporter.h"


LevelExporter::LevelExporter()
{
	g_FilePath = "../../../Asset/";
	g_AssetPath = "../../Asset/";
	GRIDCOUNT = (15 * 15);
}


LevelExporter::~LevelExporter()
{
	ReleaseMaya();
}

bool LevelExporter::InitializeMaya()
{
	cout << "Initializing Maya..." << endl;
	cout << "_________________________________________" << endl << endl;

	if (!MLibrary::initialize("Exporter", false))
	{
		cout << "<Error> Mlibrary::initialize()" << endl;
		return false;
	}

	return true;
}
void LevelExporter::ReleaseMaya()
{
	cout << "Closing Maya..." << endl;
	MLibrary::cleanup(0, false);
}
bool LevelExporter::GetMayaFilenamesInDirectory(vector<string> &filenameList)
{
	WIN32_FIND_DATA fdata;
	HANDLE dhandle;

	char buf[MAX_PATH];
	sprintf_s(buf, sizeof(buf), "%s%s/*", g_FilePath.c_str(), "LevelEditor/MayaFiles");

	if ((dhandle = FindFirstFile(buf, &fdata)) == INVALID_HANDLE_VALUE)
		return false;

	while (true)
	{
		if (FindNextFile(dhandle, &fdata))
		{
			//Only adds the .mb files to list
			if (strcmp(&fdata.cFileName[strlen(fdata.cFileName) - 3], ".mb") == 0)
			{
				filenameList.push_back(fdata.cFileName);
			}
		}
		else
		{
			if (GetLastError() == ERROR_NO_MORE_FILES)
				break;

			else
			{
				FindClose(dhandle); 
				return false;
			}
		}
	}

	if (!FindClose(dhandle))
	{
		return false;
	}
	
	return true;
}
void LevelExporter::RunExporter()
{
	vector<string> fileNameList;
	  
	if (!InitializeMaya())
	{
		cout << "<Error> Maya initilization failed." << endl;
		cin >> waitingForInput;
		return;
	}

	//Getting filelist
	if (!GetMayaFilenamesInDirectory(fileNameList))
	{
		cout << "<Error> Files not found in directory." << endl;
		return;
	}
		
	//Looping through every file in the list
	for (auto file = fileNameList.begin(); file != fileNameList.end(); file++)
	{
		SceneManager(file->c_str());
	}

	return;
}
void LevelExporter::SceneManager(const char* fileName)
{
	MStatus status = MS::kSuccess;

	//Releases all data from global structures before using
	ReleaseDataTypes();

	// Set everything back to a new file state
	status = MFileIO::newFile(true);
	if (!status)
	{
		cout << "<Error> MFileIO::NewFile()" << endl;
		cin >> waitingForInput;
		return;
	}

	// Creates the fullpath of the file. (directoryPath + fileName)
	char fullPath[MAX_PATH];
	sprintf_s(fullPath, sizeof(fullPath), "%s%s%s", g_FilePath.c_str(), "LevelEditor/MayaFiles/", fileName);
	
	cout << "Open file: " << fileName << endl << endl;
	//Open current maya file
	status = MFileIO::open(fullPath);
	if (!status)
	{
		cout << "<Error> MFileIO::Open()" << endl;
		cin >> waitingForInput;
		return;
	}

	cout << "\n####\n Extracting data..." << endl;
	//Extracting and saving all the data to the meshinfo_maya buffer
	if (!ExtractCurrentSceneRawData())
	{
		cout << "<Error> ExtractCurrentSceneRawData()" << endl;
		cin >> waitingForInput;
		return;
	}

	cout << "Writing data to file... " << endl;
	//Writing the data to file in binary
	
	
	WriteFileToBinary(fileName);

	return;
}

void LevelExporter::ReleaseDataTypes()
{
	
	if (vertexCount > 0)
	{
		delete[] gridData.vertices;
		vertexCount = 0;
	}

	if(matrices.size() > 0)
		matrices.clear();

}

bool LevelExporter::ExtractCurrentSceneRawData()
{
	MDagPath dagPath;
	MItDag dagIter(MItDag::kBreadthFirst, MFn::kMesh);

	while (!dagIter.isDone())
	{
		if (dagIter.getPath(dagPath))
		{
			MFnDagNode dagNode = dagPath.node();

			//Only get meshes with non-history
			if (!dagNode.isIntermediateObject())
			{
				MFnMesh mesh(dagPath);
				
				if(mesh.name() != "gridShape")
					ExtractAndConvertMatrix(mesh, 0);
				
				else if(mesh.name() == "gridShape")
					if(!ExtractGridData(mesh))
						return false;
			}
		}
		dagIter.next();
	}

	return true;
}

Matrix LevelExporter::ExtractAndConvertMatrix(MFnMesh &mesh, int fauling)
{
	Matrix matrix;

	MStatus status = MS::kSuccess;
	
	MFnTransform mayaTransform(mesh.parent(0), &status);
	
	//Gets parent name and saves it to "name"
	if(fauling == 0)
		sprintf_s(matrix.name, sizeof(matrix.name), "%s%s%s%s", g_AssetPath.c_str(), "bin/", mayaTransform.name().asChar(), ".pfs");
	
	if(fauling == 1)
		sprintf_s(matrix.name, sizeof(matrix.name), "%s", mayaTransform.name().asChar());

	MMatrix tempMatrix;
	tempMatrix = mayaTransform.transformation().asMatrix();
	
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			if(j != 2)
				matrix.transform[i][j] = tempMatrix[i][j];
			else
				matrix.transform[i][j] = tempMatrix[i][j] * - 1;
		}
	}

	if(fauling == 0)
		matrices.push_back(matrix);

	if(fauling == 1)
		return matrix;
}


bool LevelExporter::ExtractGridData(MFnMesh &mesh)
{
	MFloatPointArray points;
	MFloatVectorArray normals;

	//Check if gridsize is correct

	MString command = "polyTriangulate -ch 1 " + mesh.name();
	MGlobal::executeCommand(command);

	//Extractic mesh raw data
	if (!mesh.getPoints(points))
	{
		cout << "Couldn't get points for mesh: " << mesh.name() << endl;
		cin >> waitingForInput;
		return false;
	}

	if (!mesh.getNormals(normals))
	{
		cout << "Couldn't get normals for mesh: " << mesh.name() << endl;
		cin >> waitingForInput;
		return false;
	}

	ConvertGridData(mesh, points, normals);

}

void LevelExporter::ConvertGridData(MFnMesh &mesh, MFloatPointArray points, MFloatVectorArray normals)
{
	MDagPath meshPath = mesh.dagPath();
	MItMeshPolygon polygon_iter(meshPath);

	int index = 0;

	gridData.matrix = ExtractAndConvertMatrix(mesh, 1);

	vertexCount = polygon_iter.count() * 3;
	gridData.vertices = new Vertex[vertexCount];

	while(!polygon_iter.isDone())
	{
		MIntArray indexArray;
		polygon_iter.getVertices(indexArray);

		//Mesh need to be triangulated
		if (indexArray.length() == 3)
		{
			for (int i = 0; i < 3; i++)
			{
				//Getting indices inverted to fit righthanded
				int vertex_index = polygon_iter.vertexIndex(2 - i);
				int normal_index = polygon_iter.normalIndex(2 - i);

				gridData.vertices[index + i].Position[0] = points[vertex_index].x;
				gridData.vertices[index + i].Position[1] = points[vertex_index].y;
				gridData.vertices[index + i].Position[2] = points[vertex_index].z * -1;

				gridData.vertices[index + i].Normals[0] = normals[normal_index].x;
				gridData.vertices[index + i].Normals[1] = normals[normal_index].y;
				gridData.vertices[index + i].Normals[2] = normals[normal_index].z * -1;
			}
		}
		index += 3;
		polygon_iter.next();
	}
}
void LevelExporter::WriteFileToBinary(const char* fileName)
{
	string fullPath = CreateExportFile(fileName, ".lp");

	//Open/Creates file
	ofstream fileOut;
	fileOut.open(fullPath, ios_base::binary);
	
	if (!fileOut)
		return;

	cout << "Exporting level grid to " << fullPath.c_str() << endl << endl;

	fileOut.write((char*)&vertexCount, sizeof(UINT)); 
	fileOut.write((char*)gridData.vertices, sizeof(Vertex) * vertexCount); 
	fileOut.write((char*)&gridData.matrix, sizeof(Matrix));

	for (int i = 0; i < matrices.size(); i++)
	{
		//Writing matrix info to file
		fileOut.write((char*)&matrices, sizeof(Matrix));
	}

	fileOut.close();
}

string LevelExporter::CreateExportFile(string fileName, string fileEnding)
{
	 //remove .mb from file
	int sub_string_length = (int)fileName.find_last_of(".", fileName.size() - 1);

	// fileName with right filetype
	string exportFileName = fileName.substr(0, sub_string_length) + fileEnding.c_str();

	char fullPath[MAX_PATH];
	sprintf_s(fullPath, sizeof(fullPath), "%s%s%s", g_FilePath.c_str(), "LevelEditor/", exportFileName.c_str());

	//Returning full path
	return fullPath;
}