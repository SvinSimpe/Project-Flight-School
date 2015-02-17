#include "ServerTurret.h"

void ServerTurret::Reset( UINT id, UINT team, XMFLOAT3 pos, XMFLOAT4 rot, XMFLOAT3 scale )
{
	GameObject::Initialize( pos, rot, scale, mAssetID );

	mID		= id + 10;
	mTeamID	= team;
}

void ServerTurret::Initialize( UINT id, UINT team, XMFLOAT3 pos, XMFLOAT4 rot, XMFLOAT3 scale, AssetID assetID )
{
	GameObject::Initialize( pos, rot, scale, assetID );

	mID		= id + 10;
	mTeamID = team;
}

void ServerTurret::Initialize( UINT id, UINT team, GameObjectInfo gameObjectInfo, AssetID assetID )
{
	GameObject::Initialize( gameObjectInfo, assetID );

	mID		= id + 10;
	mTeamID	= team;
}

void ServerTurret::Release()
{
}

ServerTurret::ServerTurret() : GameObject()
{
	mID		= (UINT)-1;
	mTeamID	= (UINT)-1;
}

ServerTurret::~ServerTurret()
{
}