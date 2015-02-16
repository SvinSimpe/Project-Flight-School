#include "ServerTurret.h"

void ServerTurret::Reset( UINT id, UINT team, XMFLOAT3 pos, XMFLOAT4 rot, XMFLOAT3 scale )
{
	//XMFLOAT3 turretPos = pos;
	//turretPos.x += -1.8f;
	//turretPos.y += 2.3f;
	//turretPos.z += 1.0f;
	//XMFLOAT3 turretDir = dir;
	//turretDir.y += 24.761f;

	mID		= id + 10;
	mTeamID	= team;
	mPos	= pos;
	mRot	= rot;
	mScale	= scale;
}

void ServerTurret::Initialize( UINT id, UINT team, XMFLOAT3 pos, XMFLOAT4 rot, XMFLOAT3 scale, AssetID assetID )
{
	GameObject::Initialize( pos, rot, scale, assetID );
	XMFLOAT3 turretPos = mPos;
	//turretPos.x += -1.8f;
	//turretPos.y += 2.3f;
	//turretPos.z += 1.0f;
	XMFLOAT4 turretRot = mRot;
	//turretRot.y += 24.761f;

	mID		= id + 10;
	mTeamID = team;
	mPos	= turretPos;
	mRot	= turretRot;
}

void ServerTurret::Initialize( UINT id, UINT team, GameObjectInfo gameObjectInfo, AssetID assetID )
{
	GameObject::Initialize( gameObjectInfo, assetID );

	XMFLOAT3 turretPos = mPos;
	//turretPos.x += -1.8f;
	//turretPos.y += 2.3f;
	//turretPos.z += 1.0f;
	XMFLOAT4 turretRot = mRot;
	//turretRot.y += 24.761f;

	mID		= id + 10;
	mTeamID	= team;
	mPos	= turretPos;
	mRot	= turretRot;
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