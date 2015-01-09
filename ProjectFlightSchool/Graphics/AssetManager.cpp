#include "AssetManager.h"
#include "WICTextureLoader.h"
#include <sstream>

using namespace std;

#pragma region Private functions

bool AssetManager::AssetExist( std::string fileName, AssetID &assetId )
{
	for( UINT i = 0; i < mAssetIdCounter; i++ )
	{
		if( 0 == strcmp( mAssetContainer[i]->GetFileName().c_str(), fileName.c_str() ) )
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

HRESULT	AssetManager::PlaceholderAssets( ID3D11Device* device, ID3D11DeviceContext* dc )
{
	HRESULT hr = S_OK;

	//////////////////////////
	// Plane placeholder
	//////////////////////////
	Static3dAsset* plane;
	plane				= new Static3dAsset;
	plane->mAssetId		= 0;
	plane->mFileName	= "NO PATHPLANE"; //ADD CORRECT FILENAME HERE

	Mesh planeMesh;
	planeMesh.mVertexCount	= 6;

	for( int i = 0; i < TEXTURES_AMOUNT; i++ )
	{
		planeMesh.mTextures[i] = 2;
	}

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
	
	hr = device->CreateBuffer( &bufferDesc, &subData, &planeMesh.mVertexBuffer );
	if(FAILED((hr)))
	{
		//Failed to create vertex buffer for plane placeholder
		return hr;
	}

	plane->mMeshes.push_back( planeMesh );

	mAssetContainer[0] = plane;


	//////////////////////////
	// Cube placeholder
	//////////////////////////
	Static3dAsset* cube;
	cube = new Static3dAsset;
	cube->mAssetId		= 1;
	cube->mFileName		= "NO PATHCUBE"; //ADD CORRECT FILENAME HERE

	Mesh cubeMesh;
	cubeMesh.mVertexCount	= 36;

	for( int i = 0; i < TEXTURES_AMOUNT; i++ )
	{
		cubeMesh.mTextures[i] = 2;
	}

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

	hr = device->CreateBuffer( &bufferDesc, &subData, &cubeMesh.mVertexBuffer );
	if( FAILED( ( hr ) ) )
	{
		//Failed to create vertex buffer for cube placeholder
		return hr;
	}
	cube->mMeshes.push_back( cubeMesh );

	mAssetContainer[1] = cube;



	ID3D11ShaderResourceView* srv = nullptr;
	ID3D10Texture2D* tex = nullptr;

	hr = LoadTextureFromFile( device, dc, StringToWstring( "../Content/Assets/Textures/gladgubbe.png" ).c_str(), (ID3D11Resource**)tex, &srv, NULL );
	if( FAILED( hr ) )
	{	
		//Failed to create the placeholder SRV
		return hr;
	}

	Static2dAsset* texture;
	texture				= new Static2dAsset;
	texture->mAssetId	= 2;
	texture->mFileName	= "PLACEHOLDER TEXTURE";
	texture->mSRV		= srv;

	mAssetContainer[2] = texture;

	return hr;
}

//Loads a texture from file, the filename can be expressed as a string put with L prefix e.g L"Hello World", texture and SRV are both optional, size = Maximum size of buffer.
HRESULT AssetManager::LoadTextureFromFile ( ID3D11Device* device, ID3D11DeviceContext* dc, const wchar_t* fileName, ID3D11Resource** texture, ID3D11ShaderResourceView** srv, size_t size )
{
	HRESULT hr = S_OK;
	hr = CreateWICTextureFromFile( device, dc, fileName, texture, srv, size );
	return hr;
}

std::wstring AssetManager::StringToWstring( std::string fileName )
{
	return std::wstring( fileName.begin(), fileName.end() );
}

HRESULT AssetManager::CreateSRVAssets( ID3D11Device* device, ID3D11DeviceContext* dc, std::string filePath, MeshInfo &meshInfo, AssetID &assetId )
{
	//Creating SRVs from the different texture maps in the mesh info.
	std::string tempStr;
	HRESULT hr = S_OK;
	//Diffuse map
	if( string( meshInfo.diffuseMapName ) == "N/A" )
	{
		if( typeid( *mAssetContainer[assetId] ) == typeid( Animated3dAsset ) )
		{
			( (Animated3dAsset*)mAssetContainer[assetId] )->mTextures[TEXTURES_DIFFUSE] = 2;
		}
	}
	else
	{
		tempStr = filePath + string( meshInfo.diffuseMapName );
		if( typeid( *mAssetContainer[assetId] ) == typeid( Animated3dAsset ) )
		{
			hr = LoadStatic2dAsset( device, dc, (char*)tempStr.c_str(), ( (Animated3dAsset*)mAssetContainer[assetId] )->mTextures[TEXTURES_DIFFUSE] );
			if( FAILED( hr ) ) return hr;
		}

	}

	//Normal map
	if( string( meshInfo.normalMapName ) == "N/A" )
	{
		if( typeid( *mAssetContainer[assetId] ) == typeid( Animated3dAsset ) )
		{
			( (Animated3dAsset*)mAssetContainer[assetId] )->mTextures[TEXTURES_NORMAL] = 2;
		}
	}
	else
	{
		tempStr = filePath + string( meshInfo.normalMapName );
		if( typeid( *mAssetContainer[assetId] ) == typeid( Animated3dAsset ) )
		{
			hr = LoadStatic2dAsset( device, dc, (char*)tempStr.c_str(), ( (Animated3dAsset*)mAssetContainer[assetId] )->mTextures[TEXTURES_NORMAL] );
			if( FAILED( hr ) ) return hr;
		}
	}

	//Specular map
	if( string( meshInfo.specularMapName ) == "N/A" )
	{
		if( typeid( *mAssetContainer[assetId] ) == typeid( Animated3dAsset ) )
		{
			( (Animated3dAsset*)mAssetContainer[assetId] )->mTextures[TEXTURES_SPECULAR] = 2;
		}
	}
	else
	{
		tempStr = filePath + string( meshInfo.specularMapName );
		if( typeid( *mAssetContainer[assetId] ) == typeid( Animated3dAsset ) )
		{
			hr = LoadStatic2dAsset( device, dc, (char*)tempStr.c_str(), ( (Animated3dAsset*)mAssetContainer[assetId] )->mTextures[TEXTURES_SPECULAR] );
			if( FAILED( hr ) ) return hr;
		}
	}

	return hr;
}

HRESULT AssetManager::CreateSRVAssetsMesh( ID3D11Device* device, ID3D11DeviceContext* dc, std::string filePath, MeshInfo &meshInfo, Mesh &mesh )
{
	//Creating SRVs from the different texture maps in the mesh info.
	std::string tempStr;
	HRESULT hr = S_OK;
	//Diffuse map
	if( string( meshInfo.diffuseMapName ) == "N/A" )
		mesh.mTextures[TEXTURES_DIFFUSE] = 2;
	else
	{
		tempStr = filePath + string( meshInfo.diffuseMapName );

		hr = LoadStatic2dAsset( device, dc, (char*)tempStr.c_str(), mesh.mTextures[TEXTURES_DIFFUSE] );
		if( FAILED( hr ) ) return hr;
	}

	//Normal map
	if( string( meshInfo.normalMapName ) == "N/A" )
			mesh.mTextures[TEXTURES_NORMAL] = 2;
	else
	{
		tempStr = filePath + string( meshInfo.normalMapName );

		hr = LoadStatic2dAsset( device, dc, (char*)tempStr.c_str(), mesh.mTextures[TEXTURES_NORMAL] );
		if( FAILED( hr ) ) return hr;
	}

	//Specular map
	if( string( meshInfo.specularMapName ) == "N/A" )
		mesh.mTextures[TEXTURES_SPECULAR] = 2;
	else
	{
		tempStr = filePath + string( meshInfo.specularMapName );

		hr = LoadStatic2dAsset( device, dc, (char*)tempStr.c_str(), mesh.mTextures[TEXTURES_SPECULAR] );
		if( FAILED( hr ) ) return hr;
	}

	return hr;
}

#pragma endregion Helper functions for the class

#pragma region Public functions
HRESULT AssetManager::LoadStatic2dAsset( ID3D11Device* device, ID3D11DeviceContext* dc, std::string fileName, AssetID &assetId )
{
	HRESULT hr = S_OK;

	//If true return to caller because the asset already exist.
	if( AssetExist( fileName, assetId ) )
	{
		return hr;
	}
	else
	{	 
		ID3D11ShaderResourceView* srv = nullptr;
		ID3D11Texture2D* texture = nullptr;
		AssignAssetId( assetId );

		hr = LoadTextureFromFile( device, dc, StringToWstring( fileName ).c_str(), (ID3D11Resource**)texture, &srv, NULL );
		if(FAILED ( hr ) ) return hr;
		
		Static2dAsset* temp;
		temp				= new Static2dAsset();
		temp->mAssetId		= assetId;
		temp->mSRV			= srv;
		temp->mFileName		= fileName;

		mAssetContainer.push_back( temp );

		return hr;
	}
}

HRESULT	AssetManager::LoadStatic3dAsset( ID3D11Device* device, ID3D11DeviceContext* dc, std::string filePath, std::string fileName, AssetID &assetId )
{
	HRESULT hr = S_OK;

	//If true return to caller because the asset already exist.
	if( AssetExist( ( filePath + fileName ), assetId ) )
	{
		return hr;
	}
	else
	{	 
		Mesh*			meshes		= nullptr;
		MeshInfo*		meshInfo	= nullptr;
		StaticVertex**	vertices	= nullptr;
		UINT			nrOfMeshes	= 0;
		UINT			vertexSize	= sizeof( StaticVertex );

		std::ifstream myFile( ( filePath + fileName ), std::ios::binary );

		if( !myFile )
		{
			assetId = 1;
			return S_FALSE;
		}

		//Read fileheader. Holds information about number of meshes in scene
		myFile.read( (char*)&nrOfMeshes, sizeof( UINT ) );

		meshInfo	= new MeshInfo[nrOfMeshes];
		meshes		= new Mesh[nrOfMeshes];
		vertices	= new StaticVertex*[nrOfMeshes];
		
		for( UINT i = 0; i < nrOfMeshes; i++ )
		{
			//Read actual data
			myFile.read( (char*)&meshInfo[i], sizeof( MeshInfo ) );
	
			//Memory alloc + reading vertices
			vertices[i]	= new StaticVertex[meshInfo[i].nrOfVertices];

			myFile.read( (char*)vertices[i], vertexSize * meshInfo[i].nrOfVertices );
		}

		myFile.close();

		AssignAssetId( assetId );
		Static3dAsset* temp;
		temp				= new Static3dAsset();
		temp->mAssetId		= assetId;
		temp->mFileName		= fileName;
		
		mAssetContainer.push_back( temp );

		for( UINT i = 0; i < nrOfMeshes; i++ )
		{
			meshes[i].mVertexCount = meshInfo[i].nrOfVertices;

			D3D11_BUFFER_DESC bufferDesc;
			ZeroMemory( &bufferDesc, sizeof( bufferDesc ) );
			bufferDesc.BindFlags		= D3D11_BIND_VERTEX_BUFFER;
			bufferDesc.ByteWidth		= sizeof( StaticVertex ) * meshInfo[i].nrOfVertices;
			bufferDesc.Usage			= D3D11_USAGE_DEFAULT;
	
			D3D11_SUBRESOURCE_DATA subData;
			subData.pSysMem = vertices[i];
	
			hr = device->CreateBuffer( &bufferDesc, &subData, &meshes[i].mVertexBuffer );
			if( FAILED( ( hr ) ) )
			{
				//Failed to create vertex buffer
				return hr;
			}
			
			hr = CreateSRVAssetsMesh( device, dc, filePath, meshInfo[i], meshes[i] );
			if( FAILED( hr ) ) return hr;

			temp->mMeshes.push_back( meshes[i] );
		}

		if( meshInfo )
			delete [] meshInfo;
		if( meshes )
			delete [] meshes;
		for( UINT i = 0; i < nrOfMeshes; i++ )
			delete [] vertices[i];
		delete [] vertices;

		return hr;
	}
}

HRESULT	AssetManager::LoadStatic3dAssetIndexed( ID3D11Device* device, Indexed3DAssetInfo &info, AssetID &assetId )
{
	HRESULT hr = S_OK;
	
//If true return to caller because the asset already exist.
	if( AssetExist( info.assetName, assetId ) )
	{
		return hr;
	}
	else
	{
		UINT vertexSize = sizeof( StaticVertex );

		AssignAssetId( assetId );
		
		D3D11_BUFFER_DESC bufferDesc;
		ZeroMemory( &bufferDesc, sizeof( bufferDesc ) );
		bufferDesc.BindFlags	= D3D11_BIND_VERTEX_BUFFER;
		bufferDesc.ByteWidth	= sizeof( StaticVertex ) * info.vertexCount;
		bufferDesc.Usage		= D3D11_USAGE_DEFAULT;

		D3D11_BUFFER_DESC iBufferDesc;
		ZeroMemory( &iBufferDesc, sizeof( iBufferDesc ) );
		iBufferDesc.BindFlags	= D3D11_BIND_INDEX_BUFFER;
		iBufferDesc.ByteWidth	= sizeof( UINT ) * info.indexCount;
		iBufferDesc.Usage		= D3D11_USAGE_DEFAULT;

		Static3dAssetIndexed *temp = new Static3dAssetIndexed();
		temp->mAssetId		= assetId;
		temp->mFileName		= info.assetName;
		temp->mIndexCount	= info.indexCount;

		Mesh tempMesh;
		tempMesh.mVertexCount = info.vertexCount;
		for( int i = 0; i < 3; i++ )
			tempMesh.mTextures[i] = 2;

		D3D11_SUBRESOURCE_DATA pData;
		pData.pSysMem			= info.vertices;
		
		hr = device->CreateBuffer( &bufferDesc, &pData, &tempMesh.mVertexBuffer );
		if( FAILED( hr ) )
		{
			return hr;
		}

		pData.pSysMem			= info.indices;
		
		hr = device->CreateBuffer( &iBufferDesc, &pData, &temp->mIndexBuffer );
		if( FAILED( hr ) )
		{
			return hr;
		}

		mAssetContainer.push_back( temp );

		temp = nullptr;

		return hr;
	}
}

HRESULT	AssetManager::LoadAnimated3dAsset( ID3D11Device* device, ID3D11DeviceContext* dc, std::string filePath, std::string fileName, AssetID skeletonId, AssetID &assetId )
{
	HRESULT hr = S_OK;

	//If true return to caller because the asset already exist.
	if( AssetExist( ( filePath + fileName ), assetId ) )
	{
		return hr;
	}
	else
	{
		//MeshData	meshData;
		MeshInfo		meshInfo;
		AnimatedVertex*	vertices	= nullptr;
		UINT			vertexSize	= sizeof( AnimatedVertex );

		std::ifstream myFile( ( filePath + fileName ), std::ios::binary );

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

		for( int i = 0; i < (int)meshInfo.nrOfVertices; i++ )
		{
			float normalizer = vertices[i].weights[0] + vertices[i].weights[1] + vertices[i].weights[2] + vertices[i].weights[3];
			for( int j = 0; j < 4; j++ )
			{
				vertices[i].weights[j] /= normalizer;
			}
		}

		AnimatedVertex test[100];
		for( int i = 0; i < 100; i++ )
		{
			test[i] = vertices[i];
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

		//Skeleton boneOffsets
		DirectX::XMMATRIX boneOffsets[NUM_SUPPORTED_JOINTS];
		for( int i = 0; i < NUM_SUPPORTED_JOINTS; i++ )
			boneOffsets[i] = DirectX::XMMatrixIdentity();

		Skeleton* mySkeleton = &( (SkeletonAsset*)mAssetContainer[skeletonId] )->mSkeleton;

		for( int i = 0; i < (int)mySkeleton->joints.size(); i++ )
		{
			if( mySkeleton->joints.at(i).parentIndex == -1 )
			{
				boneOffsets[i] = DirectX::XMLoadFloat4x4( &mySkeleton->joints.at(i).originalMatrix );
			}
			else
			{
				DirectX::XMMATRIX child		= DirectX::XMLoadFloat4x4( &mySkeleton->joints.at(i).originalMatrix );
				DirectX::XMMATRIX parent	= boneOffsets[mySkeleton->joints.at(i).parentIndex];

				boneOffsets[i] = child * parent;
			}
		}
		for( int i = 0; i < NUM_SUPPORTED_JOINTS; i++ )
			DirectX::XMStoreFloat4x4( &temp->mBoneOffsets[i], DirectX::XMMatrixInverse( nullptr, boneOffsets[i] ) );

		mAssetContainer.push_back( temp );

		hr = CreateSRVAssets( device, dc, filePath, meshInfo, assetId );
		if( FAILED( hr ) ) return hr;

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

				DirectX::XMFLOAT4X4 storeMatrix = DirectX::XMFLOAT4X4(	values[0], values[1], values[2], values[3],
																			values[4], values[5], values[6], values[7],
																			values[8], values[9], values[10], values[11],
																			values[12], values[13], values[14], values[15] );

				DirectX::XMVECTOR scale;
				DirectX::XMVECTOR rotation;
				DirectX::XMVECTOR translation;

				DirectX::XMMatrixDecompose( &scale, &rotation, &translation, DirectX::XMLoadFloat4x4( &storeMatrix ) );

				DirectX::XMFLOAT4 unpack;
				DirectX::XMStoreFloat4( &unpack, rotation );
				rotation = DirectX::XMVectorSet( -unpack.x, -unpack.y, unpack.z, unpack.w );

				DirectX::XMStoreFloat4( &unpack, translation );
				translation = DirectX::XMVectorSet( unpack.x, unpack.y, -unpack.z, unpack.w );

				DirectX::XMStoreFloat4x4( &storeMatrix, DirectX::XMMatrixAffineTransformation( scale, DirectX::XMVectorZero(), rotation, translation ) );

				tempJoint.originalMatrix =  DirectX::XMFLOAT4X4( storeMatrix );
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

	if( AssetExist( ( filePath + fileName ), assetId ) )
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

					DirectX::XMFLOAT4X4 storeMatrix = DirectX::XMFLOAT4X4(	values[0], values[1], values[2], values[3],
																			values[4], values[5], values[6], values[7],
																			values[8], values[9], values[10], values[11],
																			values[12], values[13], values[14], values[15] );

					DirectX::XMVECTOR scale;
					DirectX::XMVECTOR rotation;
					DirectX::XMVECTOR translation;

					DirectX::XMMatrixDecompose( &scale, &rotation, &translation, DirectX::XMLoadFloat4x4( &storeMatrix ) );

					DirectX::XMFLOAT4 unpack;
					DirectX::XMStoreFloat4( &unpack, rotation );
					rotation = DirectX::XMVectorSet( -unpack.x, -unpack.y, unpack.z, unpack.w );

					DirectX::XMStoreFloat4( &unpack, translation );
					translation = DirectX::XMVectorSet( unpack.x, unpack.y, -unpack.z, unpack.w );

					DirectX::XMStoreFloat4x4( &storeMatrix, DirectX::XMMatrixAffineTransformation( scale, DirectX::XMVectorZero(), rotation, translation ) );

					tempJoint.matricies.push_back( storeMatrix );
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

HRESULT	AssetManager::Initialize( ID3D11Device* device, ID3D11DeviceContext* dc )
{
	mAssetIdCounter = 3;
	mAssetContainer.resize( mAssetIdCounter );
	PlaceholderAssets( device, dc );
	
	return S_OK;
}

void AssetManager::Release()
{
	for( UINT i = 0; i < mAssetContainer.size(); i++ )
	{
		mAssetContainer[i]->Release();
		//if( typeid( *mAssetContainer[i] ) == typeid( Static3dAsset ) )
		//	( (Static3dAsset*)mAssetContainer[i] )->Release();
		//else if( typeid( *mAssetContainer[i] ) == typeid( Static3dAssetIndexed ) )
		//	( (Static3dAssetIndexed*)mAssetContainer[i] )->Release();
		//else if( typeid( *mAssetContainer[i] ) == typeid( Animated3dAsset ) )
		//	( (Animated3dAsset*)mAssetContainer[i] )->Release();
		//else if( typeid( *mAssetContainer[i] ) == typeid( SkeletonAsset ) )
		//	( (SkeletonAsset*)mAssetContainer[i] )->Release();
		//else if( typeid( *mAssetContainer[i] ) == typeid( AnimationAsset ) )
		//	( (AnimationAsset*)mAssetContainer[i] )->Release();
		//else if( typeid( *mAssetContainer[i] ) == typeid( Static2dAsset ) )
		//	( (Static2dAsset*)mAssetContainer[i] )->Release();
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