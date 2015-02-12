#ifndef SERVERSHIP_H
#define SERVERSHIP_H

#include "BoundingGeometry.h"
#include "Events.h"

class ServerShip
{
	private:
		friend class Server;
		BoundingCircle* mBuffCircle;	// Don't forget to add an event for turning on/off player buffing
		float			mBuffMod;		// Also declared in Player.h... look there to see what this does

	protected:
		const UINT MIN_LEVEL = 1;
		const UINT MAX_LEVEL = 3;

		UINT			mID;
		UINT			mTeamID;
		XMFLOAT3		mPos;
		XMFLOAT3		mDir;
		UINT			mTurretLevel;
		UINT			mShieldLevel;
		UINT			mBuffLevel;
		float			mMaxShield;
		float			mCurrentShield;
		float			mMaxHP;
		float			mCurrentHP;

		bool			mWasUpdated;
		bool			mWasChanged;

	public:

	private:
		// Calculates the new level dependant on the change factor sent by the client
		void			ChangeTurretLevel( int change );
		void			ChangeShieldLevel( int change );
		void			ChangeBuffLevel( int change );

		void			ClientUpdateShip( IEventPtr eventPtr );
		void			ClientChangeShipLevels( IEventPtr eventPtr );

	protected:
	public:
		virtual bool	TakeDamage( float damage );
		virtual bool	Intersect( BoundingCircle* entity ); // Will check for intersects with buffable players
		virtual void	Reset( UINT id, UINT teamID, XMFLOAT3 pos, XMFLOAT3 dir );
		virtual void	Update( float deltaTime );
		virtual void	Initialize( UINT id, UINT teamID, XMFLOAT3 pos, XMFLOAT3 dir );
		virtual void	Release();
						ServerShip();
		virtual			~ServerShip();
};
#endif