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
	Static3dAsset* plane;
	plane				= new Static3dAsset;
	plane->mAssetId		= 0;
	plane->mFileName	= "PLANE"; //ADD CORRECT FILENAME HERE
	plane->mVertexCount	= 6;

	float planeSize = 100.0f;

	StaticVertex planePlaceholder[6] = {
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
	Static3dAsset* cube;
	cube = new Static3dAsset;
	cube->mAssetId		= 1;
	cube->mFileName		= "CUBE"; //ADD CORRECT FILENAME HERE
	cube->mVertexCount	= 36;

	float cubeSize = 1.0f;

	StaticVertex cubePlaceholder[36] = {
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
		MeshInfo		meshInfo;
		StaticVertex*	vertices	= nullptr;
		UINT			nrOfMeshes	= 0;
		UINT			vertexSize	= sizeof( StaticVertex );

		std::ifstream myFile( fileName, std::ios::binary );

		if( !myFile )
		{
			assetId = 1;
			return S_FALSE;
		}

		//Read fileheader. Holds information about number of meshes in scene
		myFile.read( (char*)&nrOfMeshes, sizeof( UINT ) );
		
		float* rawData = nullptr;

		for( UINT i = 0; i < nrOfMeshes; i++ )
		{
			//Read actual data
			myFile.read( (char*)&meshInfo, sizeof(meshInfo) );
	
			//Memory alloc + reading vertices
			vertices	= new StaticVertex[meshInfo.nrOfVertices];

			myFile.read( (char*)vertices, vertexSize * meshInfo.nrOfVertices );
		}

		myFile.close();

		AssignAssetId( assetId );
		Static3dAsset* temp;
		temp				= new Static3dAsset();
		temp->mAssetId		= assetId;
		temp->mFileName		= fileName;
		temp->mVertexCount	= meshInfo.nrOfVertices;

		D3D11_BUFFER_DESC bufferDesc;
		ZeroMemory( &bufferDesc, sizeof( bufferDesc ) );
		bufferDesc.BindFlags		= D3D11_BIND_VERTEX_BUFFER;
		bufferDesc.ByteWidth		= sizeof( StaticVertex ) * meshInfo.nrOfVertices;
		bufferDesc.Usage			= D3D11_USAGE_DEFAULT;
	
		D3D11_SUBRESOURCE_DATA subData;
		subData.pSysMem = vertices;
	
		hr = device->CreateBuffer( &bufferDesc, &subData, &temp->mVertexBuffer );
		if( FAILED( ( hr ) ) )
		{
			//Failed to create vertex buffer
			return hr;
		}

		mAssetContainer.push_back( temp );

		delete [] vertices;

		return hr;
	}
}

HRESULT	AssetManager::LoadAnimated3dAsset( ID3D11Device* device, char* fileName, AssetID skeletonId, AssetID &assetId )
{
	HRESULT hr = S_OK;

	//If true return to caller because the asset already exist.
	if( AssetExist( fileName, assetId ) )
	{
		return hr;
	}
	else
	{
		//MeshData	meshData;
		MeshInfo		meshInfo;
		AnimatedVertex*	vertices	= nullptr;
		UINT			vertexSize	= sizeof( AnimatedVertex );

		std::ifstream myFile( fileName, std::ios::binary );

		if( !myFile )
		{
			assetId = 1;
			return S_FALSE;
		}

		//Read actual data
		myFile.read( (char*)&meshInfo, sizeof(meshInfo) );
	
		//Memory alloc + reading vertices
		vertices	= new AnimatedVertex[meshInfo.nrOfVertices];

		myFile.read( (char*)vertices, vertexSize * meshInfo.nrOfVertices );

		AnimatedVertex vert[36];
		for( int i = 0; i < 36; i++ )
		{
			vert[i] = vertices[i];
		}

		myFile.close();

		AssignAssetId( assetId );
		Animated3dAsset* temp;
		temp				= new Animated3dAsset();
		temp->mAssetId		= assetId;
		temp->mSkeletonId	= skeletonId;
		temp->mFileName		= fileName;
		temp->mVertexCount	= meshInfo.nrOfVertices;

		D3D11_BUFFER_DESC bufferDesc;
		ZeroMemory( &bufferDesc, sizeof( bufferDesc ) );
		bufferDesc.BindFlags	= D3D11_BIND_VERTEX_BUFFER;
		bufferDesc.ByteWidth	= vertexSize * meshInfo.nrOfVertices;
		bufferDesc.Usage		= D3D11_USAGE_DEFAULT;
	
		D3D11_SUBRESOURCE_DATA subData;
		subData.pSysMem = vertices;
	
		hr = device->CreateBuffer( &bufferDesc, &subData, &temp->mVertexBuffer );
		if( FAILED( ( hr ) ) )
		{
			//Failed to create vertex buffer
			return hr;
		}

		mAssetContainer.push_back( temp );

		delete [] vertices;

		return hr;
	}
}

HRESULT	AssetManager::LoadSkeletonAsset( string filePath, string fileName, AssetID &assetId )
{
	HRESULT hr = S_OK;

	if( AssetExist( (char*)(filePath + fileName).c_str(), assetId ) )
	{
		return hr;
	}
	else
	{
		SkeletonAsset* tempSkel = nullptr;

		streampos	size;
		char*		memblock;
		ifstream	file;

		//this is how the final code should look!
		file.open( filePath + fileName, ios::in | ios::binary | ios::ate );

		if( file.is_open() )
		{
			size = file.tellg();
			memblock = new char[(unsigned int)size];
			file.seekg( 0, ios::beg );
			file.read( memblock, size );
			file.close();

			//std::cout << "File read" << std::endl;

			AssignAssetId( assetId );
			tempSkel			= new SkeletonAsset();
			tempSkel->mAssetId	= assetId;
			tempSkel->mFileName	= filePath + fileName;

			int lastindex	= fileName.find_last_of(".");
			string rawName	= fileName.substr(0, lastindex);

			tempSkel->mSkeleton.skeletonName = rawName;

			int padding = 0;
			tempSkel->mSkeleton.nrOfJoints = memblock[padding];
			//memblock should contain nr of joints
			for( int j = 0; j < tempSkel->mSkeleton.nrOfJoints; j++ )
			{
				Joint tempJoint;

				if( j == 0 )
					padding++;

				//following handles jointName
				string tempName;
				int childFor = memblock[padding];
				padding++;
				for( int i = 0; i < childFor; i++ )
				{
					tempName.push_back( memblock[padding] );
					padding++;
				}
				tempJoint.jointName = tempName;

				//following handles parentName
				string tempParentName;
				int parentFor = memblock[padding];
				padding++;
				for( int i = 0; i < parentFor; i++ )
				{
					tempParentName.push_back( memblock[padding] );
					padding++;
				}
				tempJoint.parentName = tempParentName;
				tempJoint.parentIndex = 0;

				float values[16];

				for( int m = 0; m < 16; m++ )
				{
					int tempCounterValue = (int)memblock[padding];
					string tempDouble;
					for( int w = 0; w < tempCounterValue; w++ )
					{
						if( w == 0 )
						{
							padding++;
						}
						tempDouble.push_back( memblock[padding] );
						padding++;

					}
					values[m] = stof( tempDouble );
				}
				tempJoint.originalMatrix =  DirectX::XMFLOAT4X4(	values[0], values[1], values[2], values[3],
																	values[4], values[5], values[6], values[7],
																	values[8], values[9], values[10], values[11],
																	values[12], values[13], values[14], values[15] );
				tempSkel->mSkeleton.joints.push_back( tempJoint );
			}
			delete[] memblock;

			// Fix index for parents
			for (int i = 0; i < (int)tempSkel->mSkeleton.joints.size(); i++)
			{
				if( strcmp( (char*)tempSkel->mSkeleton.joints.at(i).parentName.c_str() , "root" ) == 0 )
				{
					tempSkel->mSkeleton.joints.at(i).parentIndex = -1;
				}
				else
				{
					for( int j = 0; j < (int)tempSkel->mSkeleton.joints.size(); j++ )
					{
						if( strcmp( (char*)tempSkel->mSkeleton.joints.at(i).parentName.c_str(), (char*)tempSkel->mSkeleton.joints.at(j).jointName.c_str() ) == 0 )
						{
							tempSkel->mSkeleton.joints.at(i).parentIndex = j;
							break;
						}
					}
				}
			}

			mAssetContainer.push_back( tempSkel );
		}
		else std::cout << "Error opening file!" << std::endl;
	}
	return hr;
}

HRESULT	AssetManager::LoadAnimationAsset( string filePath, string fileName, AssetID &assetId )
{
	HRESULT hr = S_OK;

	if( AssetExist( (char*)(filePath + fileName).c_str(), assetId ) )
	{
		return hr;
	}
	else
	{
		AnimationAsset* tempAnim = nullptr;

		streampos	size;
		char*		memblock;
		ifstream	file;
		int			animLength	= 0;

		//this is how the final code should look!
		file.open( filePath + fileName, ios::in | ios::binary | ios::ate );
		//AnimationData tempAnim;

		if( file.is_open() )
		{
			size		= file.tellg();
			memblock	= new char[(unsigned int)size];
			file.seekg( 0, ios::beg );
			file.read( memblock, size );
			file.close();

			//std::cout << "File read" << std::endl;

			AssignAssetId( assetId );
			tempAnim			= new AnimationAsset();
			tempAnim->mAssetId	= assetId;
			tempAnim->mFileName	= filePath + fileName;

			int lastindex	= fileName.find_last_of( "." );
			string rawName	= fileName.substr(0, lastindex);

			tempAnim->mAnimationData.animationName	= rawName;

			int padding = 0;
			tempAnim->mAnimationData.nrOfJoints = memblock[padding];
		
			//memblock should contain nr of joints
			for( int j = 0; j < tempAnim->mAnimationData.nrOfJoints; j++ )
			{
				JointAnimation tempJoint;

				if( j == 0 )
					padding++;

				//following handles jointName
				string tempName;
				int childFor = memblock[padding];
				padding++;
				for( int i = 0; i < childFor; i++ )
				{
					tempName.push_back( memblock[padding] );
					padding++;
				}
				tempJoint.jointName = tempName;

				//following handles parentName
				string tempParentName;
				int parentFor = memblock[padding];
				padding++;
				for( int i = 0; i < parentFor; i++ )
				{
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
					int tempKeyCounterValue = (int)memblock[padding];
					string tempKeyDouble;
					for ( int w = 0; w < tempKeyCounterValue; w++ )
					{
						if ( w == 0 )
						{
							padding++;
						}
						tempKeyDouble.push_back( memblock[padding] );
						padding++;
					}
					tempJoint.keys.push_back( (int)stof( tempKeyDouble ) );

					for ( int m = 0; m < 16; m++ )
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
						values[m] = stof( tempDouble );
					}

					tempJoint.matricies.push_back( DirectX::XMFLOAT4X4(	values[0], values[1], values[2], values[3],
																		values[4], values[5], values[6], values[7],
																		values[8], values[9], values[10], values[11],
																		values[12], values[13], values[14], values[15] ) );
				}
				if(tempJoint.keys.at(tempJoint.keys.size() - 1) > animLength)
					animLength = tempJoint.keys.at(tempJoint.keys.size() - 1);
				tempAnim->mAnimationData.joints.push_back( tempJoint );
			}
			delete[] memblock;
		}
		else std::cout << "Error opening file!" << std::endl;
		tempAnim->mAnimationData.AnimLength = animLength;
		
		// Fix index for parents
		for (int i = 0; i < (int)tempAnim->mAnimationData.joints.size(); i++)
		{
			if( strcmp( (char*)tempAnim->mAnimationData.joints.at(i).parentName.c_str() , "root" ) == 0 )
			{
				tempAnim->mAnimationData.joints.at(i).parentIndex = -1;
			}
			else
			{
				for( int j = 0; j < (int)tempAnim->mAnimationData.joints.size(); j++ )
				{
					if( strcmp( (char*)tempAnim->mAnimationData.joints.at(i).parentName.c_str(), (char*)tempAnim->mAnimationData.joints.at(j).jointName.c_str() ) == 0 )
					{
						tempAnim->mAnimationData.joints.at(i).parentIndex = j;
						break;
					}
				}
			}
		}

		mAssetContainer.push_back( tempAnim );
	}
	return hr;
}

AnimationData AssetManager::ImportBinaryAnimData( string directoryPath,string fileName )
{
	streampos size;
	char * memblock;

	ifstream file;

	//this is how the final code should look!
	file.open( directoryPath + fileName, ios::in | ios::binary | ios::ate );
	AnimationData tempAnim;

	int lastindex	= fileName.find_last_of( "." );
	string rawName	= fileName.substr(0, lastindex);

	tempAnim.animationName = rawName;
	int animLength = 0;

	if( file.is_open() )
	{
		size		= file.tellg();
		memblock	= new char[(unsigned int)size];
		file.seekg( 0, ios::beg );
		file.read( memblock, size );
		file.close();

		cout << "File read" << endl;

		int padding = 0;
		tempAnim.nrOfJoints = memblock[padding];
		
		//memblock should contain nr of joints
		for( int j = 0; j < tempAnim.nrOfJoints; j++ )
		{
			JointAnimation tempJoint;

			if( j == 0 )
				padding++;

			//following handles jointName
			string tempName;
			int childFor = memblock[padding];
			padding++;
			for( int i = 0; i < childFor; i++ )
			{
				tempName.push_back( memblock[padding] );
				padding++;
			}
			tempJoint.jointName = tempName;

			//following handles parentName
			string tempParentName;
			int parentFor = memblock[padding];
			padding++;
			for( int i = 0; i < parentFor; i++ )
			{
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

				for ( int m = 0; m < 16; m++ )
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
					values[m] = stof( tempDouble );
				}

				tempJoint.matricies.push_back( DirectX::XMFLOAT4X4(	values[0], values[1], values[2], values[3],
																	values[4], values[5], values[6], values[7],
																	values[8], values[9], values[10], values[11],
																	values[12], values[13], values[14], values[15] ) );
			}
			if(tempJoint.keys.at(tempJoint.keys.size() - 1) > animLength)
				animLength = tempJoint.keys.at(tempJoint.keys.size() - 1);
			tempAnim.joints.push_back( tempJoint );
		}
		delete[] memblock;
	}
	else cout << "Error opening file!" << endl;
	tempAnim.AnimLength = animLength;
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

	int lastindex	= fileName.find_last_of(".");
	string rawName	= fileName.substr(0, lastindex);

	tempSkel.skeletonName = rawName;

	if( file.is_open() )
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
		for( int j = 0; j < tempSkel.nrOfJoints; j++ )
		{
			Joint tempJoint;

			if( j == 0 )
				padding++;

			//following handles jointName
			string tempName;
			int childFor = memblock[padding];
			padding++;
			for( int i = 0; i < childFor; i++ )
			{
				tempName.push_back( memblock[padding] );
				padding++;
			}
			tempJoint.jointName = tempName;

			//following handles parentName
			string tempParentName;
			int parentFor = memblock[padding];
			padding++;
			for( int i = 0; i < parentFor; i++ )
			{
				tempParentName.push_back( memblock[padding] );
				padding++;
			}
			tempJoint.parentName = tempParentName;
			tempJoint.parentIndex = 0;

			float values[16];

			for( int m = 0; m < 16; m++ )
			{
				int tempCounterValue = (int)memblock[padding];
				string tempDouble;
				for( int w = 0; w < tempCounterValue; w++ )
				{
					if( w == 0 )
					{
						padding++;
					}
					tempDouble.push_back( memblock[padding] );
					padding++;

				}
				values[m] = stof( tempDouble );
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
	
	return S_OK;
}

void AssetManager::Release()
{
	for( UINT i = 0; i < mAssetContainer.size(); i++ )
	{
		if( typeid( mAssetContainer[i] ) == typeid( Static3dAsset ) )
			( (Static3dAsset*)mAssetContainer[i] )->Release();
		else if( typeid( mAssetContainer[i] ) == typeid( Animated3dAsset ) )
			( (Animated3dAsset*)mAssetContainer[i] )->Release();
		else if( typeid( mAssetContainer[i] ) == typeid( SkeletonAsset ) )
			( (SkeletonAsset*)mAssetContainer[i] )->Release();
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