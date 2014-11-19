#include "AssetManager.h"

HRESULT	AssetManager::LoadStatic3dAsset( ID3D11Device* device, char* fileName, UINT &assetId )
{
	assetId					= 1;
	mTestAsset->mAssetId	= assetId;

	mTestAsset	= new Static3dAsset();
	
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

	return device->CreateBuffer( &bufferDesc, &subData, &mTestAsset->mVertexBuffer );
}

HRESULT	AssetManager::Initialize()
{
	mTestAsset->Initialize();
	return S_OK;
}

void AssetManager::Release()
{

}

AssetManager::AssetManager()
{

}

AssetManager::~AssetManager()
{

}