#include "AssetManager.h"

#pragma region Private functions

bool AssetManager::AssetExist( char* fileName, UINT &assetId )
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

void AssetManager::AssignAssetId( UINT &assetId )
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

	float planeSize = 10.0f;

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
HRESULT	AssetManager::LoadStatic3dAsset( ID3D11Device* device, char* fileName, UINT &assetId )
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

HRESULT	AssetManager::Initialize( ID3D11Device* device )
{
	mAssetIdCounter = 2;
	mAssetContainer.resize( mAssetIdCounter );
	PlaceholderAssets( device );

	////////////////////////////////////////////////////////////////////////////////

	ImporterAnim testAnimation;
	testAnimation.ImportBinaryAnimData( "bajs" );

	////////////////////////////////////////////////////////////////////////////////
	
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