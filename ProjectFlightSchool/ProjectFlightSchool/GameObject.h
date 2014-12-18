#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H
#include "Graphics.h"


struct GameObjectInfo
{
		XMFLOAT3 pos;
		XMFLOAT3 rotation;
		XMFLOAT3 scale;
};
class GameObject
{
	private:
		XMFLOAT3 mPos;
		XMFLOAT3 mRotation;
		XMFLOAT3 mScale;

		AssetID mAssetID;

	protected:
	public:

	private:
	protected:
	public:
		HRESULT		Update( float deltaTime );
		HRESULT		Render( float deltaTime );
		
		XMFLOAT3	GetPos() const;
		void		SetPos( XMFLOAT3 pos );

		XMFLOAT3	GetRotation() const;
		void		SetRotation( XMFLOAT3 rotation );

		AssetID		GetAssetID() const;
		void		SetAssetID( AssetID assetID );

		HRESULT		Initialize( GameObjectInfo gameObjectInfo, AssetID assetID );
		void		Release();
					GameObject();
					~GameObject();
};
#endif	