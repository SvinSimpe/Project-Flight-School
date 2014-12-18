#include "GameObject.h"

HRESULT	GameObject::Update(float deltaTime)
{
	return S_OK;
}
HRESULT	GameObject::Render(float deltaTime)
{
	//Temporarycode
	Graphics::GetInstance()->RenderStatic3dAsset( mAssetID, mPos, mRotation );
	return S_OK;
}

XMFLOAT3 GameObject::GetPos() const
{
	return mPos;
}
void GameObject::SetPos(XMFLOAT3 pos)
{
	mPos = pos;
}
XMFLOAT3 GameObject::GetRotation() const
{
	return mRotation;
}
void GameObject::SetRotation(XMFLOAT3 rotation)
{
	mRotation = rotation;
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
	mPos		= XMFLOAT3( 0, 0 ,0 );
	mRotation	= XMFLOAT3( 0, 0 ,0 );
	mScale		= XMFLOAT3( 0, 0 ,0 );

	mAssetID	= 0;
}
GameObject::~GameObject()
{
}
