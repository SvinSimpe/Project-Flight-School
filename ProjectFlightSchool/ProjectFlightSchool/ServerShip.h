#ifndef SERVERSHIP_H
#define SERVERSHIP_H

#include "BoundingGeometry.h"
#include "Events.h"

class ServerShip
{
	private:
		friend class Server;
		BoundingCircle* mBuffCircle;	// Don't forget to add an event for turning on/off player buffing

	protected:
		UINT			mID;
		UINT			mTeamID;
		XMFLOAT3		mPos;
		XMFLOAT3		mDir;
		float			mMaxHP;
		float			mCurrentHP;

	public:

	private:
		void ClientTurretLevel( IEventPtr eventPtr );
		void ClientHullLevel( IEventPtr eventPtr );
		void ClientBuffLevel( IEventPtr eventPtr );
		void ClientDamageShip( IEventPtr eventPtr );

	protected:
	public:
		virtual bool	Intersect( BoundingCircle* entity ); // Will check for intersects with buffable players
		virtual void	Reset( UINT id, UINT teamID, XMFLOAT3 pos, XMFLOAT3 dir );
		virtual void	Update( float deltaTime );
		virtual void	Initialize( UINT id, UINT teamID, XMFLOAT3 pos, XMFLOAT3 dir );
		virtual void	Release();
						ServerShip();
		virtual			~ServerShip();
};
#endif