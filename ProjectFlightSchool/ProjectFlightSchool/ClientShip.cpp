#include "ClientShip.h"

void ClientShip::RemoteTurretLevel( IEventPtr eventPtr )
{
	// Code for handling upgrading/downgrading of turret here
}

void ClientShip::RemoteHullLevel( IEventPtr eventPtr )
{
	// Code for handling upgrading/downgrading of hull here
}

void ClientShip::RemoteBuffLevel( IEventPtr eventPtr )
{
	// Code for handling upgrading/downgrading of 
}

void ClientShip::RemoteDamageShip( IEventPtr eventPtr )
{
	// Code for handling damaging the ship
}

void ClientShip::Reset( UINT id, UINT teamID, XMFLOAT3 pos, XMFLOAT3 dir )
{
	Release();
	Initialize( id, teamID, pos, dir );
}

void ClientShip::Update( float deltaTime )
{
}

void ClientShip::Initialize( UINT id, UINT teamID, XMFLOAT3 pos, XMFLOAT3 dir )
{
	ServerShip::Initialize( id, teamID, pos, dir );
	Graphics::GetInstance()->LoadStatic3dAsset( "../Content/Assets/PermanentAssets/Ship/", "ship.pfs", mAssetID );
	mHitCircle = new BoundingCircle( mPos, 5.0f );
}

void ClientShip::Release()
{
	ServerShip::Release();
	SAFE_DELETE( mHitCircle );
}

ClientShip::ClientShip() : ServerShip()
{
	mAssetID	= CUBE_PLACEHOLDER;
	mHitCircle	= nullptr;
}

ClientShip::~ClientShip()
{
}