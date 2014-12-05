#include "AssetManager.h"

using namespace std;

#pragma region Private functions

bool AssetManager::AssetExist( char* fileName, AssetID &assetId )
{
	for( UINT i = 0; i < mAssetIdCounter; i++ )
	{
		if( 0 == strcmp( mAssetContainer[i]->GetFileName().c_str(), fileName ) )
		{
			assetId = i;
			return true;
		}
	}
	return false;
}

void AssetManager::AssignAssetId( AssetID &assetId )
{
	assetId = mAssetIdCounter;
	mAssetIdCounter++;
}

HRESULT	AssetManager::PlaceholderAssets( ID3D11Device* device )
{
	HRESULT hr = S_OK;

	//////////////////////////
	// Plane placeholder
	//////////////////////////
	AssetBase* plane;
	plane				= new Static3dAsset;
	plane->mAssetId		= 0;
	plane->mFileName	= "PLANE"; //ADD CORRECT FILENAME HERE
	plane->mVertexCount	= 6;

	float planeSize = 100.0f;

	Vertex planePlaceholder[6] = {
			-0.5f * planeSize, 0.0f, -0.5f * planeSize	,	0.0f, 1.0f, 0.0f,      0.0f, 0.0f, 0.0f,   0.0f, 1.0f,
			-0.5f * planeSize, 0.0f,  0.5f * planeSize	,	0.0f, 1.0f, 0.0f,      0.0f, 0.0f, 0.0f,   0.0f, 0.0f,
			 0.5f * planeSize, 0.0f,  0.5f * planeSize	,	0.0f, 1.0f, 0.0f,      0.0f, 0.0f, 0.0f,   1.0f, 0.0f,

			-0.5f * planeSize, 0.0f, -0.5f * planeSize	,	0.0f, 1.0f, 0.0f,      0.0f, 0.0f, 0.0f,   0.0f, 1.0f,
			 0.5f * planeSize, 0.0f,  0.5f * planeSize	,	0.0f, 1.0f, 0.0f,      0.0f, 0.0f, 0.0f,   1.0f, 0.0f,
			 0.5f * planeSize, 0.0f, -0.5f * planeSize	,	0.0f, 1.0f, 0.0f,      0.0f, 0.0f, 0.0f,   1.0f, 1.0f
	};

	D3D11_BUFFER_DESC bufferDesc;
	ZeroMemory( &bufferDesc, sizeof( bufferDesc ) );
	bufferDesc.BindFlags		= D3D11_BIND_VERTEX_BUFFER;
	bufferDesc.ByteWidth		= sizeof( planePlaceholder );
	bufferDesc.Usage			= D3D11_USAGE_DEFAULT;
	
	D3D11_SUBRESOURCE_DATA subData;
	subData.pSysMem = planePlaceholder;
	
	hr = device->CreateBuffer( &bufferDesc, &subData, &plane->mVertexBuffer );
	if(FAILED((hr)))
	{
		//Failed to create vertex buffer for plane placeholder
		return hr;
	}

	mAssetContainer[0] = plane;


	//////////////////////////
	// Cube placeholder
	//////////////////////////
	AssetBase* cube;
	cube = new Static3dAsset;
	cube->mAssetId		= 1;
	cube->mFileName		= "CUBE"; //ADD CORRECT FILENAME HERE
	cube->mVertexCount	= 36;

	float cubeSize = 1.0f;

	Vertex cubePlaceholder[36] = {
		// Bottom
			-0.5f * cubeSize, 0.0f, -0.5f * cubeSize	,	0.0f, -1.0f, 0.0f,      0.0f, 0.0f, 0.0f,   0.0f, 0.0f,
			 0.5f * cubeSize, 0.0f,  0.5f * cubeSize	,	0.0f, -1.0f, 0.0f,      0.0f, 0.0f, 0.0f,   0.0f, 0.0f,
			-0.5f * cubeSize, 0.0f,  0.5f * cubeSize	,	0.0f, -1.0f, 0.0f,      0.0f, 0.0f, 0.0f,   0.0f, 0.0f,

			-0.5f * cubeSize, 0.0f, -0.5f * cubeSize	,	0.0f, -1.0f, 0.0f,      0.0f, 0.0f, 0.0f,   0.0f, 0.0f,
			 0.5f * cubeSize, 0.0f, -0.5f * cubeSize	,	0.0f, -1.0f, 0.0f,      0.0f, 0.0f, 0.0f,   0.0f, 0.0f,
			 0.5f * cubeSize, 0.0f,  0.5f * cubeSize	,	0.0f, -1.0f, 0.0f,      0.0f, 0.0f, 0.0f,   0.0f, 0.0f,
		
		// Left
			-0.5f * cubeSize, 0.0f, -0.5f * cubeSize	,	-1.0f, 0.0f, 0.0f,      0.0f, 0.0f, 0.0f,   0.0f, 0.0f,
			-0.5f * cubeSize, 0.0f,  0.5f * cubeSize	,	-1.0f, 0.0f, 0.0f,      0.0f, 0.0f, 0.0f,   0.0f, 0.0f,
			-0.5f * cubeSize, 1.0f,  0.5f * cubeSize	,	-1.0f, 0.0f, 0.0f,      0.0f, 0.0f, 0.0f,   0.0f, 0.0f,

			-0.5f * cubeSize, 0.0f, -0.5f * cubeSize	,	-1.0f, 0.0f, 0.0f,      0.0f, 0.0f, 0.0f,   0.0f, 0.0f,
			-0.5f * cubeSize, 1.0f,  0.5f * cubeSize	,	-1.0f, 0.0f, 0.0f,      0.0f, 0.0f, 0.0f,   0.0f, 0.0f,
			-0.5f * cubeSize, 1.0f, -0.5f * cubeSize	,	-1.0f, 0.0f, 0.0f,      0.0f, 0.0f, 0.0f,   0.0f, 0.0f,
		
		// Back
			-0.5f * cubeSize, 0.0f,  0.5f * cubeSize	,	0.0f, 0.0f, 1.0f,     0.0f, 0.0f, 0.0f,   0.0f, 0.0f,
			 0.5f * cubeSize, 1.0f,  0.5f * cubeSize	,	0.0f, 0.0f, 1.0f,     0.0f, 0.0f, 0.0f,   0.0f, 0.0f,
			-0.5f * cubeSize, 1.0f,  0.5f * cubeSize	,	0.0f, 0.0f, 1.0f,     0.0f, 0.0f, 0.0f,   0.0f, 0.0f,

			-0.5f * cubeSize, 0.0f,  0.5f * cubeSize	,	0.0f, 0.0f, 1.0f,     0.0f, 0.0f, 0.0f,   0.0f, 0.0f,
			 0.5f * cubeSize, 0.0f,  0.5f * cubeSize	,	0.0f, 0.0f, 1.0f,     0.0f, 0.0f, 0.0f,   0.0f, 0.0f,
			 0.5f * cubeSize, 1.0f,  0.5f * cubeSize	,	0.0f, 0.0f, 1.0f,     0.0f, 0.0f, 0.0f,   0.0f, 0.0f,

		// Right
			 0.5f * cubeSize, 0.0f,  0.5f * cubeSize	,	1.0f, 0.0f, 0.0f,     0.0f, 0.0f, 0.0f,   0.0f, 0.0f,
			 0.5f * cubeSize, 1.0f, -0.5f * cubeSize	,	1.0f, 0.0f, 0.0f,     0.0f, 0.0f, 0.0f,   0.0f, 0.0f,
			 0.5f * cubeSize, 1.0f,  0.5f * cubeSize	,	1.0f, 0.0f, 0.0f,     0.0f, 0.0f, 0.0f,   0.0f, 0.0f,

			 0.5f * cubeSize, 0.0f,  0.5f * cubeSize	,	1.0f, 0.0f, 0.0f,     0.0f, 0.0f, 0.0f,   0.0f, 0.0f,
			 0.5f * cubeSize, 0.0f, -0.5f * cubeSize	,	1.0f, 0.0f, 0.0f,     0.0f, 0.0f, 0.0f,   0.0f, 0.0f,
			 0.5f * cubeSize, 1.0f, -0.5f * cubeSize	,	1.0f, 0.0f, 0.0f,     0.0f, 0.0f, 0.0f,   0.0f, 0.0f,

		 // front
			-0.5f * cubeSize, 0.0f, -0.5f * cubeSize	,	0.0f, 0.0f, -1.0f,     0.0f, 0.0f, 0.0f,   0.0f, 0.0f,
			-0.5f * cubeSize, 1.0f, -0.5f * cubeSize	,	0.0f, 0.0f, -1.0f,     0.0f, 0.0f, 0.0f,   0.0f, 0.0f,
			 0.5f * cubeSize, 0.0f, -0.5f * cubeSize	,	0.0f, 0.0f, -1.0f,     0.0f, 0.0f, 0.0f,   0.0f, 0.0f,

			 0.5f * cubeSize, 0.0f, -0.5f * cubeSize	,	0.0f, 0.0f, -1.0f,     0.0f, 0.0f, 0.0f,   0.0f, 0.0f,
			-0.5f * cubeSize, 1.0f, -0.5f * cubeSize	,	0.0f, 0.0f, -1.0f,     0.0f, 0.0f, 0.0f,   0.0f, 0.0f,
			 0.5f * cubeSize, 1.0f, -0.5f * cubeSize	,	0.0f, 0.0f, -1.0f,     0.0f, 0.0f, 0.0f,   0.0f, 0.0f,

		 // top
			-0.5f * cubeSize, 1.0f, -0.5f * cubeSize	,	0.0f, 1.0f, 0.0f,     0.0f, 0.0f, 0.0f,   0.0f, 0.0f,
			-0.5f * cubeSize, 1.0f,  0.5f * cubeSize	,	0.0f, 1.0f, 0.0f,     0.0f, 0.0f, 0.0f,   0.0f, 0.0f,
			 0.5f * cubeSize, 1.0f,  0.5f * cubeSize	,	0.0f, 1.0f, 0.0f,     0.0f, 0.0f, 0.0f,   0.0f, 0.0f,

			-0.5f * cubeSize, 1.0f, -0.5f * cubeSize	,	0.0f, 1.0f, 0.0f,     0.0f, 0.0f, 0.0f,   0.0f, 0.0f,
			 0.5f * cubeSize, 1.0f,  0.5f * cubeSize	,	0.0f, 1.0f, 0.0f,     0.0f, 0.0f, 0.0f,   0.0f, 0.0f,
			 0.5f * cubeSize, 1.0f, -0.5f * cubeSize	,	0.0f, 1.0f, 0.0f,     0.0f, 0.0f, 0.0f,   0.0f, 0.0f
	};

	subData.pSysMem = cubePlaceholder;

	bufferDesc.ByteWidth		= sizeof( cubePlaceholder );

	hr = device->CreateBuffer( &bufferDesc, &subData, &cube->mVertexBuffer );
	if( FAILED( ( hr ) ) )
	{
		//Failed to create vertex buffer for cube placeholder
		return hr;
	}

	mAssetContainer[1] = cube;

	return hr;
}

#pragma endregion Helper functions for the class

#pragma region Public functions
HRESULT	AssetManager::LoadStatic3dAsset( ID3D11Device* device, char* fileName, AssetID &assetId )
{
	HRESULT hr = S_OK;

	//If true return to caller because the asset already exist.
	if( AssetExist( fileName, assetId ) )
	{
		return hr;
	}
	else
	{
		MeshData	meshData;
		UINT		meshCount	= 0;
		UINT		vertexSize	= sizeof( Vertex );

		std::ifstream myFile( fileName, std::ios::binary );

		if( !myFile )
		{
			assetId = 1;
			return S_FALSE;
		}

		//Read fileheader. Holds information about meshes in scene
		myFile.read( (char*)&meshCount, sizeof( UINT ) );
		
		float* rawData = nullptr;

		for( UINT i = 0; i < meshCount; i++ )
		{
			//Read actual data
			myFile.read( (char*)&meshData.meshInfo, sizeof(meshData.meshInfo) );
	
			//Memory alloc + reading vertices
			meshData.vertices	= new Vertex[meshData.meshInfo.vertexCount];
			//rawData				= new float[11  * meshData.meshInfo.vertexCount];

			myFile.read( (char*)meshData.vertices, vertexSize  * meshData.meshInfo.vertexCount );

			for( UINT j = 0; j < meshData.meshInfo.vertexCount * 11; j += 11 )
			{
				//meshData.vertices[j / 11].position	= DirectX::XMLoadFloat3( &DirectX::XMFLOAT3( rawData[j], rawData[j + 1], rawData[j + 2] ) );
				//meshData.vertices[j / 11].normal	= DirectX::XMLoadFloat3( &DirectX::XMFLOAT3( rawData[j + 3], rawData[j + 4], rawData[j + 5] ) );
			}
			
			//delete [] rawData;
		}

		myFile.close();

		AssignAssetId( assetId );
		AssetBase* temp;
		temp				= new Static3dAsset;
		temp->mAssetId		= assetId;
		temp->mFileName		= fileName;
		temp->mVertexCount	= meshData.meshInfo.vertexCount;

		D3D11_BUFFER_DESC bufferDesc;
		ZeroMemory( &bufferDesc, sizeof( bufferDesc ) );
		bufferDesc.BindFlags		= D3D11_BIND_VERTEX_BUFFER;
		bufferDesc.ByteWidth		= sizeof( Vertex ) * meshData.meshInfo.vertexCount;
		bufferDesc.Usage			= D3D11_USAGE_DEFAULT;
	
		D3D11_SUBRESOURCE_DATA subData;
		subData.pSysMem = meshData.vertices;
	
		hr = device->CreateBuffer( &bufferDesc, &subData, &temp->mVertexBuffer );
		if( FAILED( ( hr ) ) )
		{
			//Failed to create vertex buffer
			return hr;
		}

		mAssetContainer.push_back( temp );

		delete [] meshData.vertices;

		return hr;
	}
}

AnimationData AssetManager::ImportBinaryAnimData( string directoryPath,string fileName )
{
	streampos size;
	char * memblock;

	ifstream file;

	//this is how the final code should look!
	file.open( directoryPath + fileName, ios::in | ios::binary | ios::ate);

	//string fullPath = "myHeartIsInPieces.PaMan";

	//file.open( fullPath, ios::in | ios::binary | ios::ate );
	AnimationData tempAnim;

	int lastindex = fileName.find_last_of(".");
	string rawName = fileName.substr(0, lastindex);

	tempAnim.animationName = rawName;

	if ( file.is_open() )
	{
		size = file.tellg();
		memblock = new char[(unsigned int)size];
		file.seekg( 0, ios::beg );
		file.read( memblock, size );
		file.close();

		cout << "File read" << endl;

		int padding = 0;
		tempAnim.nrOfJoints = memblock[padding];
		//memblock should contain nr of joints
		for ( int j = 0; j < tempAnim.nrOfJoints; j++ )
		{
			JointAnimation tempJoint;

			if ( j == 0 )
				padding++;

			//following handles jointName
			string tempName;
			int childFor = memblock[padding];
			padding++;
			for ( int i = 0; i < childFor; i++ )
			{
				//if ( i == 0 )
				//{
				//	padding++;
				//}
				tempName.push_back( memblock[padding] );
				padding++;
			}
			tempJoint.jointName = tempName;

			//following handles parentName
			string tempParentName;
			int parentFor = memblock[padding];
			padding++;
			for ( int i = 0; i < parentFor; i++ )
			{
				/*if ( i == 0 )
				{
					padding++;
				}*/
				tempParentName.push_back( memblock[padding] );
				padding++;
			}
			tempJoint.parentName = tempParentName;
			tempJoint.parentIndex = 0;

			int keys = memblock[padding];
			padding++;

			float values[16];

			for ( int k = 0; k < keys; k++ )
			{
				tempJoint.keys.push_back( memblock[padding] );
				padding++;

				for ( int m = 0; m < 4; m++ )
				{
					for ( int n = 0; n < 4; n++ )
					{
						int tempCounterValue = (int)memblock[padding];
						string tempDouble;
						for ( int w = 0; w < tempCounterValue; w++ )
						{
							if ( w == 0 )
							{
								padding++;
							}
							tempDouble.push_back( memblock[padding] );
							padding++;

						}
						values[m * 4 + n] = stof( tempDouble );
					}
				}

				tempJoint.matricies.push_back( DirectX::XMFLOAT4X4(	values[0], values[1], values[2], values[3],
																	values[4], values[5], values[6], values[7],
																	values[8], values[9], values[10], values[11],
																	values[12], values[13], values[14], values[15] ) );
			}
			tempAnim.joints.push_back( tempJoint );
		}
		delete[] memblock;
	}
	else cout << "Error opening file!" << endl;
	return tempAnim;
}

Skeleton AssetManager::ImportBinarySkelData( string directoryPath, string fileName )
{
	streampos size;
	char * memblock;

	ifstream file;

	//this is how the final code should look!
	file.open( directoryPath + fileName, ios::in | ios::binary | ios::ate );

	Skeleton tempSkel;

	int lastindex = fileName.find_last_of(".");
	string rawName = fileName.substr(0, lastindex);

	tempSkel.skeletonName = rawName;

	if ( file.is_open() )
	{
		size = file.tellg();
		memblock = new char[(unsigned int)size];
		file.seekg( 0, ios::beg );
		file.read( memblock, size );
		file.close();

		cout << "File read" << endl;

		int padding = 0;
		tempSkel.nrOfJoints = memblock[padding];
		//memblock should contain nr of joints
		for ( int j = 0; j < tempSkel.nrOfJoints; j++ )
		{
			Joint tempJoint;

			if ( j == 0 )
				padding++;

			//following handles jointName
			string tempName;
			int childFor = memblock[padding];
			padding++;
			for ( int i = 0; i < childFor; i++ )
			{
				//if ( i == 0 )
				//{
				//	padding++;
				//}
				tempName.push_back( memblock[padding] );
				padding++;
			}
			tempJoint.jointName = tempName;

			//following handles parentName
			string tempParentName;
			int parentFor = memblock[padding];
			padding++;
			for ( int i = 0; i < parentFor; i++ )
			{
				/*if ( i == 0 )
				{
					padding++;
				}*/
				tempParentName.push_back( memblock[padding] );
				padding++;
			}
			tempJoint.parentName = tempParentName;
			tempJoint.parentIndex = 0;

			float values[16];

			for ( int m = 0; m < 4; m++ )
			{
				for ( int n = 0; n < 4; n++ )
				{
					int tempCounterValue = (int)memblock[padding];
					string tempDouble;
					for ( int w = 0; w < tempCounterValue; w++ )
					{
						if ( w == 0 )
						{
							padding++;
						}
						tempDouble.push_back( memblock[padding] );
						padding++;

					}
					values[m * 4 + n] = stof( tempDouble );
				}
			}
			tempJoint.originalMatrix =  DirectX::XMFLOAT4X4(	values[0], values[1], values[2], values[3],
																values[4], values[5], values[6], values[7],
																values[8], values[9], values[10], values[11],
																values[12], values[13], values[14], values[15] );
			tempSkel.joints.push_back( tempJoint );
		}
		delete[] memblock;
	}
	else cout << "Error opening file!" << endl;
	return tempSkel;
}

HRESULT	AssetManager::Initialize( ID3D11Device* device )
{
	mAssetIdCounter = 2;
	mAssetContainer.resize( mAssetIdCounter );
	PlaceholderAssets( device );

	////////////////////////////////////////////////////////////////////////////////////////////////////

	/*MapPathImportHandler testHandler;
	testHandler.HandlePaManPath("C:\\Users\\KungTrulls\\Desktop\\testMap\\");
	testHandler.HandleSkelPath("C:\\Users\\KungTrulls\\Desktop\\testMap\\");*/

	mTestAnim.mAnimationData	= ImportBinaryAnimData( "../Content/Assets/Animations/", "walk.PaMan" );
	mTestAnim.mSkeleton			= ImportBinarySkelData( "../Content/Assets/Animations/", "walk.Skel" );

	mTestAnim.ParentIndexer();
	mTestAnim.ResetAnimation();

	////////////////////////////////////////////////////////////////////////////////////////////////////
	
	return S_OK;
}

void AssetManager::Release()
{
	for( UINT i = 0; i < mAssetContainer.size(); i++ )
	{
		mAssetContainer[i]->Release();
		SAFE_DELETE( mAssetContainer[i] );
	}
	mAssetContainer.clear();
}

AssetManager::AssetManager()
{

}

AssetManager::~AssetManager()
{

}

#pragma endregion 