#include "GameObject.h"
#include "MapNode.h"
#include "RenderManager.h"

void GameObject::Update(float deltaTime)
{
}

void GameObject::Render(float deltaTime, DirectX::XMFLOAT4X4 parentWorld )
{
	DirectX::XMMATRIX parent		= DirectX::XMLoadFloat4x4( &parentWorld );

	DirectX::XMMATRIX transformation = DirectX::XMMatrixAffineTransformation( DirectX::XMLoadFloat3( &mScale ), DirectX::XMVectorZero(), DirectX::XMLoadFloat4( &mRot ), DirectX::XMLoadFloat3( &mPos ) );

	DirectX::XMMATRIX world = transformation * parent;
	DirectX::XMFLOAT4X4 worldFinished;
	DirectX::XMStoreFloat4x4( &worldFinished, world );
	
	RenderManager::GetInstance()->AddObject3dToList( mAssetID, worldFinished );
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
	return mRot;
}
void GameObject::SetRotation(DirectX::XMFLOAT4 rotation)
{
	mRot = rotation;
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

void GameObject::Initialize( XMFLOAT3 pos, XMFLOAT4 rot, XMFLOAT3 scale, AssetID assetID )
{
	DirectX::XMMATRIX scalemat		= XMMatrixScaling( scale.x, scale.y, scale.z );
	DirectX::XMMATRIX rotation		= XMMatrixRotationRollPitchYaw( rot.x, rot.y, rot.z );
	DirectX::XMMATRIX translation	= XMMatrixTranslation( pos.x, pos.y, pos.z );

	GameObjectInfo goi;
	DirectX::XMStoreFloat4x4( &goi.transformation, scalemat * rotation * translation ); // think about the order here, might be reversed

	Initialize( goi, assetID );
}

void GameObject::Initialize( GameObjectInfo gameObjectInfo, AssetID assetID )
{
	DirectX::XMVECTOR scale;
	DirectX::XMVECTOR rotation;
	DirectX::XMVECTOR translation;

	DirectX::XMMatrixDecompose( &scale, &rotation, &translation, XMLoadFloat4x4( &gameObjectInfo.transformation ) );

	XMStoreFloat3( &mScale, scale );
	XMStoreFloat4( &mRot, rotation );
	XMStoreFloat3( &mPos, translation );

	mRot.x = -mRot.x;
	mRot.y = -mRot.y;

	mPos.z = -mPos.z;

	mAssetID = assetID;
}
void GameObject::Release()
{
}
GameObject::GameObject()
{
	mPos	= DirectX::XMFLOAT3( 0, 0 ,0 );
	mRot	= DirectX::XMFLOAT4( 0, 0 ,0 ,0 );
	mScale	= DirectX::XMFLOAT3( 0, 0 ,0 );

	mAssetID = 0;
}
GameObject::~GameObject()
{
}
