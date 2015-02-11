#ifndef SERVERSHIP_H
#define SERVERSHIP_H

#include "BoundingGeometry.h"
#include "Events.h"

class ServerShip
{
	private:
	protected:
		UINT			mID;
		UINT			mTeamID;
		XMFLOAT3		mPos;
		XMFLOAT3		mDir;
		float			mMaxHP;
		float			mCurrentHP;
		BoundingCircle* mBuffCircle;

	public:

	private:
		void ClientTurretLevel( IEventPtr eventPtr );
		void ClientHullLevel( IEventPtr eventPtr );
		void ClientBuffLevel( IEventPtr eventPtr );
		void ClientDamageShip( IEventPtr eventPtr );

	protected:
	public:
		virtual void	Reset( UINT id, UINT teamID, XMFLOAT3 pos, XMFLOAT3 dir );
		virtual void	Update( float deltaTime );
		virtual void	Initialize( UINT id, UINT teamID, XMFLOAT3 pos, XMFLOAT3 dir );
		virtual void	Release();
						ServerShip();
		virtual			~ServerShip();
};
#endif