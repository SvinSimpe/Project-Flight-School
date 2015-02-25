#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H
#include "Graphics.h"

static enum CollisionType
{
	Full = 0,
	Half = 1,
	None = 2
};

static enum RenderType
{
	Static = 0,
	Dynamic = 1,
	Invisible = 2
};

struct GameObjectInfo
{
	DirectX::XMFLOAT4X4 transformation;
	CollisionType collision;
	RenderType renderType;
};

class GameObject
{
	private:
	protected:
		DirectX::XMFLOAT3 mPos;
		DirectX::XMFLOAT4 mRot;
		DirectX::XMFLOAT3 mScale;

		CollisionType mCollisionType;
		RenderType mRenderType;

		AssetID mAssetID;
	public:

	private:
	protected:
	public:
		virtual void	Update( float deltaTime );
		virtual void	Render( float deltaTime, DirectX::XMFLOAT4X4 parentWorld );
		
		DirectX::XMFLOAT3	GetPos() const;
		void			SetPos( DirectX::XMFLOAT3 pos );

		DirectX::XMFLOAT4	GetRotation() const;
		void			SetRotation( DirectX::XMFLOAT4 rotation );

		DirectX::XMFLOAT3	GetScale() const;
		void			SetScale( DirectX::XMFLOAT3 scale );

		AssetID			GetAssetID() const;
		void			SetAssetID( AssetID assetID );

		RenderType		GetRenderType() const;
		void			SetRenderType( RenderType type );

		CollisionType	GetCollisionType() const;
		void			SetCollisionType( CollisionType type );
		
		virtual void	Initialize( DirectX::XMFLOAT3 pos, DirectX::XMFLOAT4 rot, DirectX::XMFLOAT3 scale, AssetID assetID );
		virtual void	Initialize( GameObjectInfo gameObjectInfo, AssetID assetID );
		virtual void	Release();
						GameObject();
		virtual			~GameObject();
};
#endif	