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
	plane->mVertexCount	= 4;

	Vertex planePlaceholder[4];
	planePlaceholder[0].position = DirectX::XMLoadFloat3( &DirectX::XMFLOAT3( -0.75f, -0.75f, 0.0f ) );
	planePlaceholder[1].position = DirectX::XMLoadFloat3( &DirectX::XMFLOAT3( -0.75f, 0.75f, 0.0f ) );
	planePlaceholder[2].position = DirectX::XMLoadFloat3( &DirectX::XMFLOAT3( 0.75f, -0.75f, 0.0f ) );
	planePlaceholder[3].position = DirectX::XMLoadFloat3( &DirectX::XMFLOAT3( 0.75f, 0.75f, 0.0f ) );


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
	cube->mVertexCount	= 8;

	Vertex cubePlaceholder[8];
	cubePlaceholder[0].position = DirectX::XMLoadFloat3( &DirectX::XMFLOAT3( -0.25f, -0.25f, 0.0f ) );
	cubePlaceholder[1].position = DirectX::XMLoadFloat3( &DirectX::XMFLOAT3( -0.25f, 0.25f, 0.0f ) );
	cubePlaceholder[2].position = DirectX::XMLoadFloat3( &DirectX::XMFLOAT3( 0.25f, -0.25f, 0.0f ) );
	cubePlaceholder[3].position = DirectX::XMLoadFloat3( &DirectX::XMFLOAT3( 0.25f, 0.25f, 0.0f ) );
	
	cubePlaceholder[4].position = DirectX::XMLoadFloat3( &DirectX::XMFLOAT3( -0.25f, -0.25f, -0.5f ) );
	cubePlaceholder[5].position = DirectX::XMLoadFloat3( &DirectX::XMFLOAT3( -0.25f, 0.25f, -0.5f ) );
	cubePlaceholder[6].position = DirectX::XMLoadFloat3( &DirectX::XMFLOAT3( 0.25f, -0.25f, -0.5f ) );
	cubePlaceholder[7].position = DirectX::XMLoadFloat3( &DirectX::XMFLOAT3( 0.25f, 0.25f, -0.5f ) );
	
	subData.pSysMem = cubePlaceholder;

	bufferDesc.ByteWidth		= sizeof( cubePlaceholder );

	hr = device->CreateBuffer( &bufferDesc, &subData, &cube->mVertexBuffer );
	if(FAILED((hr)))
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
		AssignAssetId( assetId );
		AssetBase* temp;
		temp			= new Static3dAsset;
		temp->mAssetId	= assetId;
		temp->mFileName = fileName;
	
		////////////////////////////////
		//		TESTU-BUFFERU
		//////////////////////////////

		Vertex derpface[4];
		derpface[0].position = DirectX::XMLoadFloat3( &DirectX::XMFLOAT3( -0.5f, -0.5f, 0.0f ) );
		derpface[1].position = DirectX::XMLoadFloat3( &DirectX::XMFLOAT3( -0.5f, 0.5f, 0.0f ) );
		derpface[2].position = DirectX::XMLoadFloat3( &DirectX::XMFLOAT3( 0.5f, -0.5f, 0.0f ) );
		derpface[3].position = DirectX::XMLoadFloat3( &DirectX::XMFLOAT3( 0.5f, 0.5f, 0.0f ) );

		D3D11_BUFFER_DESC bufferDesc;
		ZeroMemory( &bufferDesc, sizeof( bufferDesc ) );
		bufferDesc.BindFlags		= D3D11_BIND_VERTEX_BUFFER;
		bufferDesc.ByteWidth		= sizeof( derpface );
		bufferDesc.Usage			= D3D11_USAGE_DEFAULT;
	
		D3D11_SUBRESOURCE_DATA subData;
		subData.pSysMem = derpface;
	
		hr = device->CreateBuffer( &bufferDesc, &subData, &temp->mVertexBuffer );
		if(FAILED((hr)))
		{
			//Failed to create vertex buffer
			return hr;
		}

		mAssetContainer.push_back( temp );

		return hr;
	}
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