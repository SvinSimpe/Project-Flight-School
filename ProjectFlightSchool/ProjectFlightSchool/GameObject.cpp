#include "GameObject.h"
#include "MapNode.h"
#include "RenderManager.h"

HRESULT	GameObject::Update(float deltaTime)
{
	return S_OK;
}
HRESULT	GameObject::Render(float deltaTime )
{
	//Temporarycode
	DirectX::XMMATRIX scaling		= DirectX::XMMatrixScaling( mScale.x, mScale.y, mScale.z );
	DirectX::XMMATRIX rotation		= DirectX::XMMatrixRotationRollPitchYaw( mRotation.x, mRotation.y, mRotation.z );
	DirectX::XMMATRIX translation	= DirectX::XMMatrixTranslation( mPos.x, mPos.y, mPos.z );

	DirectX::XMMATRIX world = translation * rotation * scaling;

	DirectX::XMFLOAT4X4 worldFinished;
	DirectX::XMStoreFloat4x4( &worldFinished, world );
	

	Graphics::GetInstance()->RenderStatic3dAsset( mAssetID, &worldFinished );
	return S_OK;
}
HRESULT	GameObject::Render(float deltaTime, DirectX::XMFLOAT4X4 parentWorld )
{
	//Temporarycode
	DirectX::XMMATRIX parent		= DirectX::XMLoadFloat4x4( &parentWorld );
	DirectX::XMMATRIX scaling		= DirectX::XMMatrixScaling( mScale.x, mScale.y, mScale.z );
	DirectX::XMMATRIX rotation		= DirectX::XMMatrixRotationRollPitchYaw( mRotation.x, mRotation.y, mRotation.z );
	DirectX::XMMATRIX translation	= DirectX::XMMatrixTranslation( mPos.x, mPos.y, mPos.z );

	DirectX::XMMATRIX world = parent * ( translation * rotation * scaling);

	DirectX::XMFLOAT4X4 worldFinished;
	DirectX::XMStoreFloat4x4( &worldFinished, world );
	

	RenderManager::GetInstance()->AddObject3dToList( mAssetID, worldFinished );
	return S_OK;
}

DirectX::XMFLOAT3 GameObject::GetPos() const
{
	return mPos;
}
void GameObject::SetPos(DirectX::XMFLOAT3 pos)
{
	mPos = pos;
}

DirectX::XMFLOAT3 GameObject::GetRotation() const
{
	return mRotation;
}
void GameObject::SetRotation(DirectX::XMFLOAT3 rotation)
{
	mRotation = rotation;
}

DirectX::XMFLOAT3 GameObject::GetScale() const
{
	return mScale;
}
void GameObject::SetScale(DirectX::XMFLOAT3 scale)
{
	mScale = scale;
}
AssetID	GameObject::GetAssetID() const
{
	return mAssetID;
}
void GameObject::SetAssetID(AssetID assetID)
{
	mAssetID = assetID;
}
HRESULT	GameObject::Initialize( GameObjectInfo gameObjectInfo, AssetID assetID )
{
	mPos		= gameObjectInfo.pos;
	mRotation	= gameObjectInfo.rotation;
	mScale		= gameObjectInfo.scale;
	mAssetID	= assetID;

	return S_OK;
}
void GameObject::Release()
{
}
GameObject::GameObject()
{
	mPos		= DirectX::XMFLOAT3( 0, 0 ,0 );
	mRotation	= DirectX::XMFLOAT3( 0, 0 ,0 );
	mScale		= DirectX::XMFLOAT3( 0, 0 ,0 );

	mAssetID	= 0;
}
GameObject::~GameObject()
{
}
