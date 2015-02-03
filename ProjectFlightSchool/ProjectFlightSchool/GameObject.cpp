#include "GameObject.h"
#include "MapNode.h"
#include "RenderManager.h"

HRESULT	GameObject::Update(float deltaTime)
{
	return S_OK;
}
HRESULT	GameObject::Render(float deltaTime, DirectX::XMFLOAT4X4 parentWorld )
{
	DirectX::XMMATRIX parent		= DirectX::XMLoadFloat4x4( &parentWorld );
	//DirectX::XMMATRIX scaling		= DirectX::XMMatrixScaling( mScale.x, mScale.y, mScale.z );
	//DirectX::XMMATRIX rotation		= DirectX::XMMatrixRotationRollPitchYaw( mRotation.x, mRotation.y, mRotation.z );
	//DirectX::XMMATRIX translation	= DirectX::XMMatrixTranslation( mPos.x, mPos.y, mPos.z );

	//DirectX::XMMATRIX scaling		= DirectX::XMMatrixScalingFromVector( DirectX::XMLoadFloat3( &mScale ) );
	//DirectX::XMMATRIX rotation		= DirectX( DirectX::XMLoadFloat3( &mRotation ) );
	//DirectX::XMMATRIX translation	= DirectX::XMMatrixTranslationFromVector( DirectX::XMLoadFloat3( &mPos ) );



	DirectX::XMMATRIX transformation = DirectX::XMMatrixAffineTransformation( DirectX::XMLoadFloat3( &mScale ), DirectX::XMVectorZero(), DirectX::XMLoadFloat4( &mRotation ), DirectX::XMLoadFloat3( &mPos ) );

	//DirectX::XMMATRIX world			= ( scaling * rotation * translation ) * parent;
	DirectX::XMMATRIX world = transformation * parent;
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

DirectX::XMFLOAT4 GameObject::GetRotation() const
{
	return mRotation;
}
void GameObject::SetRotation(DirectX::XMFLOAT4 rotation)
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
	mRotation	= DirectX::XMFLOAT4( 0, 0 ,0 ,0 );
	mScale		= DirectX::XMFLOAT3( 0, 0 ,0 );

	mAssetID	= 0;
}
GameObject::~GameObject()
{
}
