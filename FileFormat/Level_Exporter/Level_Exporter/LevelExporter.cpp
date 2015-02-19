#include "LevelExporter.h"


LevelExporter::LevelExporter()
{
	g_FilePath = "../../Asset/";
	g_AssetPath = "../../Asset/";
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
		errorMsg.push_back("<Error> Mlibrary::initialize()");
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
		errorMsg.push_back("<Error> Maya initilization failed.");
		return;
	}

	//Getting filelist
	if (!GetMayaFilenamesInDirectory(fileNameList))
	{
		errorMsg.push_back("<Error> Files not found in directory.");
		return;
	}

	//Looping through every file in the list
	for (auto file = fileNameList.begin(); file != fileNameList.end(); file++)
	{
		errorMsg.push_back("\n#### " + string(file->c_str()) + " ####");
		int nrOfErrors = errorMsg.size();

		SceneManager(file->c_str());

		if(nrOfErrors == errorMsg.size())
			errorMsg.pop_back();
	}

	cout << "\n##### ERRORS #####" << endl << endl;

	if (errorMsg.size() == 0)
		cout << "No errors" << endl;

	else if (errorMsg.size() > 0)
	{
		for (int i = 0; i < errorMsg.size(); i++)
		{
			cout << errorMsg[i] << endl;
		}
	}

	cout << "\n##################" << endl << endl;
	cout << "Press any key to exit." << endl;
	cin.get();
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
		errorMsg.push_back("<Error> MFileIO::NewFile()");

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
		errorMsg.push_back("<Error> MFileIO::Open()");
		return;
	}

	cout << "\n####\n Extracting data..." << endl;
	//Extracting and saving all the data to the meshinfo_maya buffer
	if (!ExtractCurrentSceneRawData())
	{
		errorMsg.push_back("<Error> ExtractCurrentSceneRawData()");
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

	if (navvertexCount > 0)
	{
		delete[] navData.vertices;
		navvertexCount = 0;
	}

	if (matrices.size() > 0)
		matrices.clear();

	if (lights.size() > 0)
		lights.clear();

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

				if (mesh.name() == "GridShape")
				{
					if (!ExtractGridData(mesh))
					{
						errorMsg.push_back("Error GridData" + string(mesh.name().asChar()));
						return false;
					}
				}

				if (mesh.name() == "NavMeshShape")
				{
					if (!ExtractNavMesh(mesh))
					{

						errorMsg.push_back("Error NavMeshData" + string(mesh.name().asChar()));
						return false;
					}
				}

				else if (mesh.name() != "GridShape" || mesh.name() != "NavMeshShape")
				{
					ExtractAndConvertMatrix(mesh, 0);
				}
			}
		}
		dagIter.next();
	}

	ExtractLightData();

	return true;
}

void LevelExporter::GetDimensions(MFnMesh &mesh, UINT* dimensions)
{
	MItDependencyGraph dgIt(mesh.object(), MFn::kPolyMesh, MItDependencyGraph::kUpstream, MItDependencyGraph::kBreadthFirst, MItDependencyGraph::kNodeLevel);
	MObject polyMeshNode = dgIt.currentItem();
	MFnDependencyNode polyFn(polyMeshNode);

	float tempDimension[2];
	polyFn.findPlug("width").getValue(tempDimension[0]);
	polyFn.findPlug("height").getValue(tempDimension[1]);

	dimensions[0] = tempDimension[0];
	dimensions[1] = tempDimension[1];
}

Matrix LevelExporter::ExtractAndConvertMatrix(MFnMesh &mesh, int fauling)
{
	Matrix matrix;


	MStatus status = MS::kSuccess;

	MFnTransform mayaTransform(mesh.parent(0), &status);


	string id;

	for (int i = 0; i < mayaTransform.name().length(); i++)
	{
		if (mayaTransform.name().asChar()[i] != '_')
		{
			id += mayaTransform.name().asChar()[i];
		}
		else
		{
			break;
		}
	}

	//Gets parent name and saves it to "name"
	if (fauling == 0)
		sprintf_s(matrix.name, sizeof(matrix.name), "%s%s", id.c_str(), ".pfs");

	if (fauling == 1)
		sprintf_s(matrix.name, sizeof(matrix.name), "%s", mayaTransform.name().asChar());

	MVector translate;
	MQuaternion rotate;
	double scale[3];


	matrix.derp = mayaTransform.transformation().asMatrix();


	translate = mayaTransform.getTranslation(MSpace::kTransform);
	mayaTransform.getRotation(rotate);
	mayaTransform.getScale(scale);


	//matrix.translate[0] = translate.x;
	//matrix.translate[1] = translate.y;
	//matrix.translate[2] = translate.z * -1.0f;

	//matrix.rotate[0] = rotate.x;
	//matrix.rotate[1] = rotate.y;
	//matrix.rotate[2] = rotate.z;
	//matrix.rotate[3] = rotate.w;

	//matrix.scale[0] = scale[0];
	//matrix.scale[1] = scale[1];
	//matrix.scale[2] = scale[2];


	if (fauling == 0)
		matrices.push_back(matrix);

	if (fauling == 1)
		return matrix;
}

void LevelExporter::ExtractLightData()
{
	VolumeLight vLight;
	MItDag itDag(MItDag::kDepthFirst, MFn::kLight);
	MDagPath dPath;
	MSpace::Space wSpace = MSpace::kWorld;

	while (!itDag.isDone())
	{
		itDag.getPath(dPath);
		MFnVolumeLight vl(dPath);
		MObject temp = vl.parent(0);
		MDagPath p = MDagPath::getAPathTo(temp);
		MFnTransform lt(p);
		double scale[3];
		lt.getScale(scale);
		MPoint pos = lt.getTranslation(wSpace);

		if (dPath.apiType() == MFn::kVolumeLight)
		{
			
			sprintf_s(vLight.name, sizeof(vLight.name), "%s", lt.name().asChar());
			vLight.color[0] = vl.color().r;
			vLight.color[1] = vl.color().g;
			vLight.color[2] = vl.color().b;

			vLight.position[0] = pos.x;
			vLight.position[1] = pos.y;
			vLight.position[2] = -pos.z;

			vLight.range = scale[0];
			vLight.intensity = vl.intensity();
		

		}
		lights.push_back(vLight);
		itDag.next();
	}
	
}

bool LevelExporter::ExtractGridData(MFnMesh &mesh)
{
	MFloatPointArray points;
	MFloatVectorArray normals;

	MString command = "polyTriangulate -ch 1 " + mesh.name();
	if (!MGlobal::executeCommand(command))
	{
		errorMsg.push_back("Couldn't triangulate mesh: " + string(mesh.name().asChar()));
		return false;
	}

	//Extractic mesh raw data
	if (!mesh.getPoints(points))
	{
		errorMsg.push_back("Couldn't get points for mesh: " + string(mesh.name().asChar()));
		return false;
	}

	if (!mesh.getNormals(normals))
	{
		errorMsg.push_back("Couldn't get normals for mesh: " + string(mesh.name().asChar()));
		return false;
	}

	GetNodePath(mesh);
	ConvertGridData(mesh, points, normals);

}

void LevelExporter::GetNodePath(MFnMesh &mesh)
{
	MObjectArray nodes;
	MIntArray nodeIndex;

	mesh.getConnectedShaders(0, nodes, nodeIndex);
	MFnDependencyNode findNode(nodes[0]);
	MPlugArray shaderConnections;

	MPlug surfaceShader = findNode.findPlug("surfaceShader");
	surfaceShader.connectedTo(shaderConnections, TRUE, TRUE);

	// Get texture, normalmap and specularmap for a mesh
	GetTexture(shaderConnections);
}

void LevelExporter::GetTexture(MPlugArray &shaderConnections)
{
	MObject materialNode = shaderConnections[0].node();
	MFnDependencyNode ColorPlug(materialNode);
	MPlug materialPlug = ColorPlug.findPlug("color");
	MPlugArray materialConnection;
	materialPlug.connectedTo(materialConnection, TRUE, TRUE);

	MObject textureFile;

	for (unsigned int i = 0; i < materialConnection.length(); i++)
	{
		textureFile = materialConnection[i].node();

		if (textureFile.hasFn(MFn::kFileTexture))
		{
			MFnDependencyNode textureNode(textureFile);
			MPlug ftn = textureNode.findPlug("ftn");
			MString diffuseMapName = ftn.asString(MDGContext::fsNormal);
			string src = diffuseMapName.asChar();

			unsigned found = src.find_last_of("\\/");
			string name = src.substr(found + 1);
			
			sprintf_s(gridData.blendMap, sizeof(gridData.blendMap), "%s", name.c_str());
			int a = 0;
		}
	}
}

bool LevelExporter::ExtractNavMesh(MFnMesh &mesh)
{
	MFloatPointArray points;

	MString command = "polyTriangulate -ch 1 " + mesh.name();
	if (!MGlobal::executeCommand(command))
	{
		errorMsg.push_back("Couldn't triangulate Navmesh: " + string(mesh.name().asChar()));
		return false;
	}

	if (!mesh.getPoints(points))
	{
		errorMsg.push_back("Couldn't get points for NavMesh: " + string(mesh.name().asChar()));
		return false;
	}

	ConvertNavMeshData(mesh, points);
}

void LevelExporter::ConvertNavMeshData(MFnMesh &mesh, MFloatPointArray & points)
{
	MDagPath meshPath = mesh.dagPath();
	MItMeshPolygon polygon_iter(meshPath);

	navvertexCount = polygon_iter.count() * 3;

	navData.vertices = new NavVertex[navvertexCount];
	int nindex = 0;

	while (!polygon_iter.isDone())
	{
		MIntArray indexArray;
		polygon_iter.getVertices(indexArray);

		if (indexArray.length() == 3)
		{
			for (int i = 0; i < 3; i++)
			{
				int vert_index = polygon_iter.vertexIndex(2 - i);

				navData.vertices[nindex + i].Position[0] = points[vert_index].x;
				navData.vertices[nindex + i].Position[1] = points[vert_index].y;
				navData.vertices[nindex + i].Position[2] = -points[vert_index].z;
			}
		}
		nindex += 3;
		polygon_iter.next();

	}
	int a = 0;
}

void LevelExporter::ConvertGridData(MFnMesh &mesh, MFloatPointArray &points, MFloatVectorArray &normals)
{
	MDagPath meshPath = mesh.dagPath();
	MItMeshPolygon polygon_iter(meshPath);

	GetDimensions(mesh, gridData.dimensions);
	gridData.matrix = ExtractAndConvertMatrix(mesh, 1);

	vertexCount = polygon_iter.count() * 3;
	gridData.vertices = new Vertex[vertexCount];

	int index = 0;
	while (!polygon_iter.isDone())
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

	cout << "Exporting node to " << fullPath.c_str() << endl << endl;
	UINT nrOfObjects = matrices.size();
	UINT nrOfLights  = lights.size();

	fileOut.write((char*)gridData.blendMap, sizeof(gridData.blendMap));
	fileOut.write((char*)&gridData.dimensions, sizeof(gridData.dimensions));
	fileOut.write((char*)&vertexCount, sizeof(UINT));
	fileOut.write((char*)gridData.vertices, sizeof(Vertex) * vertexCount);
	fileOut.write((char*)&gridData.matrix, sizeof(Matrix));
	fileOut.write((char*)&navvertexCount, sizeof(UINT));
	fileOut.write((char*)navData.vertices, sizeof(NavVertex) * navvertexCount);

	//fileOut.write((char*)&nrOfLights, sizeof(nrOfLights));
	//for (int i = 0; i < lights.size(); i++)
	//{
	//	fileOut.write((char*)&lights[i], sizeof(VolumeLight));
	//}

	fileOut.write((char*)&nrOfObjects, sizeof(nrOfObjects));
	for (int i = 0; i < matrices.size(); i++)
	{
		//Writing matrix info to file
		fileOut.write((char*)&matrices[i], sizeof(Matrix));
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
