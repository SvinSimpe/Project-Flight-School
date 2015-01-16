
#include "Exporter.h"

Exporter::Exporter()
{
	g_FilePath = "../../Asset/";
	g_ExportPath = "";
	g_currentExportMap = "";

	ReadExporterDirectoryNames();
}


Exporter::~Exporter()
{
	ReleaseMaya();
}

void Exporter::ReadExporterDirectoryNames()
{
	ifstream fileIn;

	fileIn.open(g_FilePath + "folderNames.txt");
	if (!fileIn)
	{
		g_ErrorMessages.push_back("folderNames.txt couldnt be opened");
		return;
	}
		

	while(!fileIn.eof())
	{
		string name;
		fileIn >> name;

		if(name.find("#") == -1)
			g_ExporterDirectoryNameList.push_back(name);
	}
}

bool Exporter::InitializeMaya()
{
	cout << "Initializing Maya..." << endl;
	cout << "_________________________________________" << endl << endl;

	if (!MLibrary::initialize("Exporter", false))
	{
		g_ErrorMessages.push_back("<Error> Mlibrary::initialize()");
		return false;
	}

	return true;
}

void Exporter::ReleaseMaya()
{
	MLibrary::cleanup(0, false);
}

bool Exporter::GetDirectories(vector<string> &directorynameList)
{
	WIN32_FIND_DATA fdata;
	HANDLE dhandle;

	char buf[MAX_PATH];
	sprintf_s(buf, sizeof(buf), "%s%s/*", g_FilePath.c_str(), "FilesToExport");

	if ((dhandle = FindFirstFile(buf, &fdata)) == INVALID_HANDLE_VALUE)
		return false;

	while (true)
	{
		if (FindNextFile(dhandle, &fdata))
		{
			string fileName = fdata.cFileName;

			//adds all directories to list
			if (fileName.find(".") == -1)
				directorynameList.push_back(fdata.cFileName);
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

bool Exporter::GetMayaFilenamesInDirectory(const char* directoryname, vector<string> &filenameList)
{
	WIN32_FIND_DATA fdata;
	HANDLE dhandle;

	char buf[MAX_PATH];
	sprintf_s(buf, sizeof(buf), "%s%s/%s/%s/*", g_FilePath.c_str(), "FilesToExport", directoryname, "scenes");

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

bool Exporter::CreateExportPath(string fileName)
{
	for (int i = 0; i < g_ExporterDirectoryNameList.size(); i+=2)
	{
		//To make sure empty names not gets checked
		if (g_ExporterDirectoryNameList[i] != "")
		{
			if (fileName.find(g_ExporterDirectoryNameList[i]) != -1)
			{
				//Directory name is located +1 of the searchpath
				g_ExportPath = "../../Asset/ExportedFiles" + g_ExporterDirectoryNameList[i + 1];
				if (CreateDirectory(g_ExportPath.c_str(), NULL) != ERROR_PATH_NOT_FOUND)
					return true;

				if (CreateDirectory(g_ExportPath.c_str(), NULL) == ERROR_PATH_NOT_FOUND)
				{
					g_ErrorMessages.push_back("<Error> Couldnt create export directory: " + g_ExporterDirectoryNameList[i]);
					g_ExportPath = "";
					return false;
				}
			}
		}
	}

	//If there was no match
	g_ExportPath = "../../Asset/ExportedFiles/UNKNOWN_TYPES/";
	if (CreateDirectory(g_ExportPath.c_str(), NULL) != ERROR_PATH_NOT_FOUND)
		return true;
	else
	{
		g_ErrorMessages.push_back("<Error> Couldnt create export directory: UNKNOWN_TYPES");
		g_ExportPath = "";
		return false;
	}
		
	
}

void Exporter::RunExporter()
{
	vector<string> directorynameList;
	
	if (!InitializeMaya())
	{
		g_ErrorMessages.push_back("<Error> Maya initilization failed.");
		return;
	}

	//Getting scene directories
	if (!GetDirectories(directorynameList))
	{
		g_ErrorMessages.push_back("<Error> Maya directories not found.");
		return;
	}
		
	//Looping through every directory in the list
	for (auto map = directorynameList.begin(); map != directorynameList.end(); map++)
	{
		vector<string> fileNameList;
		//Getting scene directories
		GetMayaFilenamesInDirectory(map->c_str(), fileNameList);

		if(fileNameList.size() == 0)
			g_ErrorMessages.push_back("No files found in directory: " + string(map->c_str()));


		for (auto file = fileNameList.begin(); file != fileNameList.end(); file++)
		{
			SceneManager(map->c_str(), file->c_str());
		}
	}

	cout << "\n##### ERRORS #####" << endl << endl;
	
	if(g_ErrorMessages.size() == 0)
		cout << "No errors" << endl;

	else if(g_ErrorMessages.size() > 0)
	{
		for (int i = 0; i < g_ErrorMessages.size(); i++)
		{
			cout << g_ErrorMessages[i] << endl;
		}
	}

	cout << "\n###############" << endl << endl;
	cin.get();
	return;
}

void Exporter::SceneManager(const char* mapName, const char* fileName)
{
	MStatus status = MS::kSuccess;

	//Releases all data from global structures before using
	ReleaseDataTypes();

	g_currentExportMap = mapName;

	// Set everything back to a new file state
	status = MFileIO::newFile(true);
	if (!status)
	{
		g_ErrorMessages.push_back(string(fileName) + " - <Error> MFileIO::NewFile()");
		return;
	}

	// Creates the fullpath of the file. (directoryPath + fileName)
	char fullPath[MAX_PATH];
	sprintf_s(fullPath, sizeof(fullPath), "%s%s/%s/%s/%s", g_FilePath.c_str(), "FilesToExport" , mapName, "scenes", fileName);
	
	cout << "Open file: " << fileName;
	//Open current maya file
	status = MFileIO::open(fullPath);
	if (!status)
	{
		g_ErrorMessages.push_back(string(fileName) + "<Error> MFileIO::Open()");
		return;
	}

	if(!CreateExportPath(fileName))
	{
		g_ErrorMessages.push_back(string(fileName) + "<Error> Couldnt create export path. ");
		return;
	}

	//Extracting and saving all the data to the meshinfo_maya buffer
	if (!ExtractCurrentSceneRawData())
	{
		g_ErrorMessages.push_back(string(fileName) + "<Error> ExtractCurrentSceneRawData()");
		return;
	}

	//Converting the data to fit the exporter meshinfo struct and building it
	ConvertAndBuildMeshInfoExport();

	//Writing the data to file in binary
	
	if (g_meshDataExportStatic.size() != 0)
		WriteStaticMeshToFileBinary(fileName);
	
	if (g_meshDataExportAnim.size() != 0)
		WriteAnimMeshToFileBinary(fileName);
	
	if (g_skeletonData.size() != 0)
		WriteSkelToFileBinary();

	cout << "    ...Done!" << endl;

	return;
}

void Exporter::ReleaseDataTypes()
{
	if (g_meshInfoMaya.size() > 0)
		g_meshInfoMaya.clear();

	if (g_meshDataExportStatic.size() > 0)
		g_meshDataExportStatic.clear();

	if (g_meshDataExportAnim.size() > 0)
		g_meshDataExportAnim.clear();

	if (g_skeletonData.size() > 0)
		g_skeletonData.clear();

	g_ExportPath = "";
	g_currentExportMap = "";
}

void Exporter::GetWeightAndJointsPerSkinCluster()
{
	MItDependencyNodes iter(MFn::kInvalid);
	MStatus stat;

	for (; !iter.isDone(); iter.next()) {

		MObject object = iter.item();
		if (object.apiType() == MFn::kSkinClusterFilter)
		{
			MFnSkinCluster skinCluster(object);
			MDagPath skinPath;
			
			//Getting all joints from the skin
			GetJointsPerSkinCluster(skinCluster);

			stat = skinCluster.getPathAtIndex(0, skinPath);
			for (int i = 0; i < g_meshInfoMaya.size(); i++)
			{
				//Find the associated mesh
				if (g_meshInfoMaya[i].meshName == skinPath.partialPathName())
				{
					MItGeometry gIter(skinPath);
					for (; !gIter.isDone(); gIter.next())
					{
						int jointCounter = 0;

						//Getting current vertex
						MObject comp = gIter.currentItem(&stat);

						//Get the weights for current vertex (one per joint)
						MDoubleArray wts;
						unsigned int infCount;
						stat = skinCluster.getWeights(skinPath, comp, wts, infCount);

						//Going through every vertex
						for (int j = 0; j < infCount; j++)
						{
							//Only saves the relevant weights and jointIndices
							if (wts[j] != 0)
							{
								g_meshInfoMaya[i].weights.append(wts[j]);	//Weight
								g_meshInfoMaya[i].jointIndex.append(j);		//JointIndex

								jointCounter++;
							}
						}

						while(jointCounter != 4)
						{
							g_meshInfoMaya[i].weights.append(0);
							g_meshInfoMaya[i].jointIndex.append(0);
							
							jointCounter++;
						}
					}
				}
			}
		}
	}
}

void Exporter::GetJointsPerSkinCluster(MFnSkinCluster &skinCluster)
{
	MStatus stat;
	MDagPathArray influences;
	Skeleton tempSkel;

	//Gets skinCluster name (will be used as fileName)
	tempSkel.fileName = skinCluster.name().asChar();

	//Gets number of joints in skin. Allocates memory with that number
	tempSkel.jointCount = skinCluster.influenceObjects(influences, &stat);
	tempSkel.joints = new Joint[tempSkel.jointCount];

	for (int i = 0; i < tempSkel.jointCount; i++)
	{
		Joint tempJoint;

		//Creates the current joint and the parent to joint
		MFnIkJoint joint(influences[i].node());
		MFnIkJoint pJoint(joint.parent(0));
		
		//Stores jointname for current joint and his parent
		sprintf_s(tempJoint.jointName, sizeof(tempJoint.jointName), "%s", joint.name().asChar());
		sprintf_s(tempJoint.parentName, sizeof(tempJoint.parentName), "%s", pJoint.name().asChar());

		//Gets the joint matrix
		MMatrix matrix = joint.transformation().asMatrix();

		//Converts matrix data
		for (int i = 0; i < 4; i++)
		{
			for (int j = 0; j < 4; j++)
			{
				tempJoint.matrix[i][j] = matrix[i][j];
			}
		}

		//Adding joint to skel
		tempSkel.joints[i] = tempJoint;
	}

	g_skeletonData.push_back(tempSkel);
}

bool Exporter::ExtractCurrentSceneRawData()
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
				ExtractMeshData(mesh);
			}
		}
		dagIter.next();
	}
	
	GetWeightAndJointsPerSkinCluster();

	return true;
}

void Exporter::GetNodePath(MFnMesh &mesh, MeshInfo_Maya &meshData)
{
	MObjectArray nodes;
	MIntArray nodeIndex;

	mesh.getConnectedShaders(0, nodes, nodeIndex);
	MFnDependencyNode findNode(nodes[0]);
	MPlugArray shaderConnections;

	MPlug surfaceShader = findNode.findPlug("surfaceShader");
	surfaceShader.connectedTo(shaderConnections, TRUE, TRUE);

	// Get texture, normalmap and specularmap for a mesh
	GetTexture(shaderConnections, meshData);
	GetNormalMap(shaderConnections, meshData);     
	GetSpecularMap(shaderConnections, meshData);

}

void Exporter::CopyTextureToDirectory(string texturePath, string fileName)
{
	if(texturePath.find("C:/") == -1)
	{
		string dest = g_ExportPath + fileName;

		if(CopyFile(texturePath.c_str(), dest.c_str(), FALSE) == 0)
			g_ErrorMessages.push_back(texturePath + " not copied");
	}

	else if (texturePath.find("C:/") != -1)
	{
		unsigned findMayaMapPath = texturePath.find(g_currentExportMap);
		string subName = texturePath.substr(findMayaMapPath);

		string fixedSrc = g_FilePath + "FilesToExport/" + subName;
		string dest = g_ExportPath + fileName;

		if(CopyFile(fixedSrc.c_str(), dest.c_str(), FALSE) == 0)
			g_ErrorMessages.push_back(subName + " not copied");
	}
}


void Exporter::GetTexture(MPlugArray &shaderConnections, MeshInfo_Maya &meshData)
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
			meshData.diffuseMapName = ftn.asString(MDGContext::fsNormal);
			string src = meshData.diffuseMapName.asChar();

			unsigned found = src.find_last_of("\\/");
			string name = src.substr(found + 1);

			CopyTextureToDirectory(src, name);
			
			meshData.diffuseMapName = name.c_str();
		}
	}
}
 
void Exporter::GetNormalMap(MPlugArray &shaderConnections, MeshInfo_Maya &meshData)
{
	MObject materialNode = shaderConnections[0].node();
	MFnDependencyNode ColorPlug(materialNode);
	MPlug materialPlug = ColorPlug.findPlug("normalCamera");
	MPlugArray materialConnection;
	materialPlug.connectedTo(materialConnection, TRUE, TRUE);

	if (materialConnection.length() > 0)
	{
		MObject normal = materialConnection[0].node();
		MFnDependencyNode normalNode(normal);
		MPlug bumpvalue = normalNode.findPlug("bumpValue");
		MPlugArray bumpConnection;
		bumpvalue.connectedTo(bumpConnection, TRUE, TRUE);

		MObject normalFile;
		for (unsigned int i = 0; i < bumpConnection.length(); i++)
		{
			normalFile = bumpConnection[i].node();
			if (normalFile.hasFn(MFn::kFileTexture))
			{
				MFnDependencyNode textureNode(normalFile);
				MPlug ftn = textureNode.findPlug("ftn");
				meshData.normalMapName = ftn.asString();

				string src = meshData.normalMapName.asChar();

				unsigned found = src.find_last_of("\\/");
				string name = src.substr(found + 1);
				
				CopyTextureToDirectory(src, name);

				meshData.normalMapName = name.c_str();

			}
		}
	}
}

void Exporter::GetSpecularMap(MPlugArray &shaderConnections, MeshInfo_Maya &meshData)
{
	MObject materialNode = shaderConnections[0].node();
	MFnDependencyNode specularNode(materialNode);
	MPlug specplug = specularNode.findPlug("specularColor");
	MPlugArray specConn;
	specplug.connectedTo(specConn, TRUE, TRUE);

	MObject specularFile;
	for (unsigned int i = 0; i < specConn.length(); i++)
	{
		specularFile = specConn[i].node();
		if (specularFile.hasFn(MFn::kFileTexture))
		{
			MFnDependencyNode textureNode(specularFile);
			MPlug ftn = textureNode.findPlug("ftn");
			meshData.specularMapName = ftn.asString();

			string src = meshData.specularMapName.asChar();

			unsigned found = src.find_last_of("\\/");
			string name = src.substr(found + 1);

			CopyTextureToDirectory(src, name);

			meshData.specularMapName = name.c_str();
		}
	}
}

bool Exporter::ExtractMeshData(MFnMesh &mesh)
{
	MeshInfo_Maya meshInfo;

	meshInfo.meshPath = mesh.dagPath();
	meshInfo.meshName = mesh.name();

	//Makes sure the mesh in triangulated
	MString command = "polyTriangulate -ch 1 " + meshInfo.meshName;
	if (!MGlobal::executeCommand(command))
	{
		g_ErrorMessages.push_back(string(mesh.name().asChar()) + " - Couldn't triangulate mesh: ");
		return false;
	}

	//Extractic mesh raw data
	if (!mesh.getPoints(meshInfo.points))
	{
		g_ErrorMessages.push_back(string(mesh.name().asChar()) + " - Couldn't get points for mesh: ");
		return false;
	}

	if (!mesh.getNormals(meshInfo.normals))
	{
		g_ErrorMessages.push_back(string(mesh.name().asChar()) + " - Couldn't get normals for mesh: ");
		return false;
	}

	if (!mesh.getUVs(meshInfo.uvs_x, meshInfo.uvs_y))
	{
		g_ErrorMessages.push_back(string(mesh.name().asChar()) + " - Couldn't get UVs for mesh: ");
		return false;
	}

	if (!mesh.getTangents(meshInfo.tangents))
	{
		g_ErrorMessages.push_back(string(mesh.name().asChar()) + " - Couldn't get tangents for mesh: ");
		return false;
	}

	MStatus res = MS::kSuccess;
	MFnTransform transformation(mesh.parent(0), &res);

	if(res)
		if(!transformation.getScale(meshInfo.scale))
		{
			g_ErrorMessages.push_back(string(mesh.name().asChar()) + " - Couldn't get scale for mesh: ");
			return false;
		}


	GetNodePath(mesh, meshInfo);

	//Pushes mesh to the global meshInfo
	g_meshInfoMaya.push_back(meshInfo);

	return true;
	
}

void Exporter::ConvertAndBuildMeshInfoExport()
{
	for (auto meshIter = g_meshInfoMaya.begin(); meshIter != g_meshInfoMaya.end(); meshIter++)
	{
		if (meshIter->weights.length() == 0)
			ConvertAndBuildStaticMesh(meshIter._Ptr);
		else
			ConvertAndBuildAnimMesh(meshIter._Ptr);
	}
}

void Exporter::ConvertAndBuildStaticMesh(MeshInfo_Maya* meshIter)
{
	MeshData_Export_Static tempMeshData;
	MItMeshPolygon polygon_iter(meshIter->meshPath);
	int vertex_index = 0, normal_index = 0, uv_index = 0, tangent_index = 0, index = 0;

	//MeshName
	sprintf_s(tempMeshData.meshInfo.meshName, sizeof(tempMeshData.meshInfo.meshName), "%s", meshIter->meshName.asChar());

	//VertexCount 
	tempMeshData.meshInfo.vertexCount = polygon_iter.count() * 3;

	//Diffusemap name
	sprintf_s(tempMeshData.meshInfo.diffuseMapName, sizeof(tempMeshData.meshInfo.diffuseMapName), "%s", meshIter->diffuseMapName.asChar());

	//Normalmap name
	if (meshIter->normalMapName.length() != 0)
		sprintf_s(tempMeshData.meshInfo.normalMapName, sizeof(tempMeshData.meshInfo.normalMapName), "%s", meshIter->normalMapName.asChar());
	else
		sprintf_s(tempMeshData.meshInfo.normalMapName, sizeof(tempMeshData.meshInfo.normalMapName), "%s", "N/A");

	//Specularmap name
	if (meshIter->specularMapName.length() != 0)
		sprintf_s(tempMeshData.meshInfo.specularMapName, sizeof(tempMeshData.meshInfo.specularMapName), "%s", meshIter->specularMapName.asChar());
	else
		sprintf_s(tempMeshData.meshInfo.specularMapName, sizeof(tempMeshData.meshInfo.specularMapName), "%s", "N/A");

	//Skeleton
	sprintf_s(tempMeshData.meshInfo.skeletonName, sizeof(tempMeshData.meshInfo.skeletonName), "%s", "N/A");

	//Allocate memory for temp vertex
	tempMeshData.vertices = new Vertex_Static[tempMeshData.meshInfo.vertexCount];

	//Adding vertices information
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
				vertex_index = polygon_iter.vertexIndex(2 - i);
				normal_index = polygon_iter.normalIndex(2 - i);
				polygon_iter.getUVIndex(2 - i, uv_index);
				tangent_index = polygon_iter.tangentIndex(2 - i);

				tempMeshData.vertices[index + i].position.x = meshIter->points[vertex_index].x * meshIter->scale[0];
				tempMeshData.vertices[index + i].position.y = meshIter->points[vertex_index].y * meshIter->scale[1];
				tempMeshData.vertices[index + i].position.z = -meshIter->points[vertex_index].z * meshIter->scale[2];

				tempMeshData.vertices[index + i].normal.x = meshIter->normals[normal_index].x;
				tempMeshData.vertices[index + i].normal.y = meshIter->normals[normal_index].y;
				tempMeshData.vertices[index + i].normal.z = -meshIter->normals[normal_index].z;

				tempMeshData.vertices[index + i].uv.x = meshIter->uvs_x[uv_index];
				tempMeshData.vertices[index + i].uv.y = 1 - meshIter->uvs_y[uv_index];

				tempMeshData.vertices[index + i].tangent.x = meshIter->tangents[tangent_index].x;
				tempMeshData.vertices[index + i].tangent.y = meshIter->tangents[tangent_index].y;
				tempMeshData.vertices[index + i].tangent.z = -meshIter->tangents[tangent_index].z;
			}
		}
		else
		{
			g_ErrorMessages.push_back(string(meshIter->meshName.asChar()) + " - Error: non-triangular polygon detected. DO IT RIGHT NEXT TIME OR DIE." "Attempts to continue export with missing polygon...");
		}
		polygon_iter.next();
		index += 3;
	}
	//Push mesh to the global static mesh exporter
	g_meshDataExportStatic.push_back(tempMeshData);
}

void Exporter::ConvertAndBuildAnimMesh(MeshInfo_Maya* meshIter)
{
	MeshData_Export_Anim tempMeshData;
	MItMeshPolygon polygon_iter(meshIter->meshPath);
	int vertex_index = 0, normal_index = 0, uv_index = 0, tangent_index = 0, index = 0;

	//MeshName
	sprintf_s(tempMeshData.meshInfo.meshName, sizeof(tempMeshData.meshInfo.meshName), "%s", meshIter->meshName.asChar());

	//VertexCount
	tempMeshData.meshInfo.vertexCount = polygon_iter.count() * 3;

	//Diffusemap name
	sprintf_s(tempMeshData.meshInfo.diffuseMapName, sizeof(tempMeshData.meshInfo.diffuseMapName), "%s", meshIter->diffuseMapName.asChar());

	//Normalmap name
	if (meshIter->normalMapName.length() != 0)
		sprintf_s(tempMeshData.meshInfo.normalMapName, sizeof(tempMeshData.meshInfo.normalMapName), "%s", meshIter->normalMapName.asChar());
	else
		sprintf_s(tempMeshData.meshInfo.normalMapName, sizeof(tempMeshData.meshInfo.normalMapName), "%s", "N/A");

	//Specularmap name
	if (meshIter->specularMapName.length() != 0)
		sprintf_s(tempMeshData.meshInfo.specularMapName, sizeof(tempMeshData.meshInfo.specularMapName), "%s", meshIter->specularMapName.asChar());
	else
		sprintf_s(tempMeshData.meshInfo.specularMapName, sizeof(tempMeshData.meshInfo.specularMapName), "%s", "N/A");

	//Skeleton
	sprintf_s(tempMeshData.meshInfo.skeletonName, sizeof(tempMeshData.meshInfo.skeletonName), "%s", g_skeletonData[0].fileName);
	
	//Allocate memory for temp vertex
	tempMeshData.vertices = new Vertex_Anim[tempMeshData.meshInfo.vertexCount];

	//Adding vertices information
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
				vertex_index = polygon_iter.vertexIndex(2 - i);
				normal_index = polygon_iter.normalIndex(2 - i);
				polygon_iter.getUVIndex(2 - i, uv_index);
				tangent_index = polygon_iter.tangentIndex(2 - i);

				tempMeshData.vertices[index + i].position.x = meshIter->points[vertex_index].x * meshIter->scale[0];
				tempMeshData.vertices[index + i].position.y = meshIter->points[vertex_index].y * meshIter->scale[1];
				tempMeshData.vertices[index + i].position.z = -meshIter->points[vertex_index].z * meshIter->scale[2];

				tempMeshData.vertices[index + i].normal.x = meshIter->normals[normal_index].x;
				tempMeshData.vertices[index + i].normal.y = meshIter->normals[normal_index].y;
				tempMeshData.vertices[index + i].normal.z = -meshIter->normals[normal_index].z;

				tempMeshData.vertices[index + i].uv.x = meshIter->uvs_x[uv_index];
				tempMeshData.vertices[index + i].uv.y = 1 - meshIter->uvs_y[uv_index];

				tempMeshData.vertices[index + i].tangent.x = meshIter->tangents[tangent_index].x;
				tempMeshData.vertices[index + i].tangent.y = meshIter->tangents[tangent_index].y;
				tempMeshData.vertices[index + i].tangent.z = -meshIter->tangents[tangent_index].z;

				//Weight and joint data is stored in single arrays. Weights for 1st vertex is at {0 1 2}, the second vertex at {3 4 5} and so on. The same goes for joints.
				tempMeshData.vertices[index + i].weights.x = meshIter->weights[vertex_index * 4];
				tempMeshData.vertices[index + i].weights.y = meshIter->weights[vertex_index * 4 + 1];
				tempMeshData.vertices[index + i].weights.z = meshIter->weights[vertex_index * 4 + 2];
				tempMeshData.vertices[index + i].weights.w = meshIter->weights[vertex_index * 4 + 3];

				tempMeshData.vertices[index + i].jointIndex[0] = meshIter->jointIndex[vertex_index * 4];
				tempMeshData.vertices[index + i].jointIndex[1] = meshIter->jointIndex[vertex_index * 4 + 1];
				tempMeshData.vertices[index + i].jointIndex[2] = meshIter->jointIndex[vertex_index * 4 + 2];
				tempMeshData.vertices[index + i].jointIndex[3] = meshIter->jointIndex[vertex_index * 4 + 3];
			}
		}
		else
		{
			g_ErrorMessages.push_back(string(meshIter->meshName.asChar()) + " - Error: non-triangular polygon detected. DO IT RIGHT NEXT TIME OR DIE." "Attempts to continue export with missing polygon...");
		}
		polygon_iter.next();
		index += 3;
	}
	//Push mesh to the global animated mesh exporter
	g_meshDataExportAnim.push_back(tempMeshData);
}

void Exporter::WriteStaticMeshToFileBinary(const char* fileName)
{
	UINT meshCount;

	string fullPath = CreateExportFile(fileName, ".pfs");

	//Open/Creates file
	ofstream fileOut;
	fileOut.open(fullPath, ios_base::binary);
	
	if (!fileOut)
	{
		g_ErrorMessages.push_back(string(fileName) + " - coudlnt be created/opened");
		return;
	}

	//Set and write fileheader to file
	meshCount = g_meshDataExportStatic.size();
	fileOut.write((char*)&meshCount, sizeof(UINT));

	for (int i = 0; i < g_meshDataExportStatic.size(); i++)
	{
		//Writing meshinfo to file
		fileOut.write((char*)&g_meshDataExportStatic[i].meshInfo, sizeof(MeshInfo));
		fileOut.write((char*)g_meshDataExportStatic[i].vertices, sizeof(Vertex_Static) * g_meshDataExportStatic[i].meshInfo.vertexCount);
	}

	fileOut.close();
}

void Exporter::WriteAnimMeshToFileBinary(const char* fileName)
{
	string fullPath = CreateExportFile(fileName, ".apfs");

	//Open/Creates file
	ofstream fileOut;
	fileOut.open(fullPath, ios_base::binary);

	if (!fileOut)
	{
		g_ErrorMessages.push_back(string(fileName) + " - coudlnt be created/opened");
		return;
	}

	for (int i = 0; i < g_meshDataExportAnim.size(); i++)
	{
		//Writing meshinfo to file
		fileOut.write((char*)&g_meshDataExportAnim[i].meshInfo, sizeof(MeshInfo));
		fileOut.write((char*)g_meshDataExportAnim[i].vertices, sizeof(Vertex_Anim) * g_meshDataExportAnim[i].meshInfo.vertexCount);
	}

	fileOut.close();
}

void Exporter::WriteSkelToFileBinary()
{
	for (int i = 0; i < g_skeletonData.size(); i++)
	{
		string fullPath = CreateExportFile(g_skeletonData[i].fileName, ".skel");

		//Open/Creates file
		ofstream fileOut;
		fileOut.open(fullPath, ios_base::binary);

		if (!fileOut)
		{
			g_ErrorMessages.push_back(string(g_skeletonData[i].fileName) + " - coudlnt be created/opened");
			return;
		}

		//Writing jointInfo to file
		fileOut.write((char*)g_skeletonData[i].joints, sizeof(Joint) * g_skeletonData[i].jointCount);

		fileOut.close();
	}
}

string Exporter::CreateExportFile(string fileName, string fileEnding)
{
	 //remove .mb from file
	int sub_string_length = (int)fileName.find_last_of(".", fileName.size() - 1);

	// fileName with right filetype
	string exportFileName = fileName.substr(0, sub_string_length) + fileEnding.c_str();

	char fullPath[MAX_PATH];
	sprintf_s(fullPath, sizeof(fullPath), "%s%s", g_ExportPath.c_str(), exportFileName.c_str());

	if(fileEnding == ".skel")
	{
		string animationPath = g_ExportPath + "Animation";
		if(CreateDirectory(animationPath.c_str(), NULL) == 0)
			g_ErrorMessages.push_back(string(animationPath.c_str()) + " - Couldnt be created");
	}

	//Returning full path
	return fullPath;
}
