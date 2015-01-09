#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H
#include "Graphics.h"


struct GameObjectInfo
{
		DirectX::XMFLOAT3 pos;
		DirectX::XMFLOAT3 rotation;
		DirectX::XMFLOAT3 scale;
};
class GameObject
{
	private:
		DirectX::XMFLOAT3 mPos;
		DirectX::XMFLOAT3 mRotation;
		DirectX::XMFLOAT3 mScale;

		AssetID mAssetID;

	protected:
	public:

	private:
	protected:
	public:
		HRESULT		Update( float deltaTime );
		HRESULT		Render( float deltaTime );
		HRESULT		Render( float deltaTime, DirectX::XMFLOAT4X4 parentWorld );
		
		DirectX::XMFLOAT3	GetPos() const;
		void		SetPos( DirectX::XMFLOAT3 pos );

		DirectX::XMFLOAT3	GetRotation() const;
		void		SetRotation( DirectX::XMFLOAT3 rotation );

		DirectX::XMFLOAT3	GetScale() const;
		void		SetScale( DirectX::XMFLOAT3 scale );

		AssetID		GetAssetID() const;
		void		SetAssetID( AssetID assetID );

		HRESULT		Initialize( GameObjectInfo gameObjectInfo, AssetID assetID );
		void		Release();
					GameObject();
					~GameObject();
};
#endif	