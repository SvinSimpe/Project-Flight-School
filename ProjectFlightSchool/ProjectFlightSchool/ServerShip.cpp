#include "ServerShip.h"

void ServerShip::ClientTurretLevel( IEventPtr eventPtr )
{
	// Code for handling upgrading/downgrading of turret here
}

void ServerShip::ClientHullLevel( IEventPtr eventPtr )
{
	// Code for handling upgrading/downgrading of hull here
}

void ServerShip::ClientBuffLevel( IEventPtr eventPtr )
{
	// Code for handling upgrading/downgrading of 
}

void ServerShip::ClientDamageShip( IEventPtr eventPtr )
{
	// Code for handling damaging the ship
}

bool ServerShip::Intersect( BoundingCircle* entity )
{
	if( mBuffCircle->Intersect( entity ) )
	{
		return true;
	}
	return false;
}

void ServerShip::Reset( UINT id, UINT teamID, XMFLOAT3 pos, XMFLOAT3 dir )
{
	Release();
	Initialize( id, teamID, pos, dir );
}

void ServerShip::Update( float deltaTime )
{
}

void ServerShip::Initialize( UINT id, UINT teamID, XMFLOAT3 pos, XMFLOAT3 dir )
{
	mBuffCircle = new BoundingCircle( pos, 20.0f );
	mID			= id;
	mTeamID		= teamID;
	mPos		= pos;
	mDir		= dir;
	mCurrentHP	= mMaxHP;
	mMaxHP		= 100.0f;
	mCurrentHP	= mMaxHP;
}

void ServerShip::Release()
{
	if( mBuffCircle )
	{
		delete mBuffCircle;
		mBuffCircle = nullptr;
	}
}

ServerShip::ServerShip()
{
	mBuffCircle	= nullptr;
	mID			= (UINT)-1;
	mTeamID		= (UINT)-1;
	mPos		= XMFLOAT3( 0.0f, 0.0f, 0.0f );
	mDir		= XMFLOAT3( 0.0f, 0.0f, 0.0f );
	mMaxHP		= 0.0f;
	mCurrentHP	= 0.0f;
}

ServerShip::~ServerShip()
{
}