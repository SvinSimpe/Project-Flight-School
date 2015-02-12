#ifndef SERVERSHIP_H
#define SERVERSHIP_H

#include "BoundingGeometry.h"
#include "Events.h"

class ServerShip
{
	private:
		friend class Server;
		const UINT MIN_LEVEL = 1;
		const UINT MAX_LEVEL = 3;
		// Since buff and turret are handled by the server, these modifiers and such are calculated here
		UINT			mTurretLevel;
		UINT			mBuffLevel;
		UINT			mShieldLevel;

		float			mBuffMod;		// Also declared in Player.h... look there to see what this does
		BoundingCircle* mBuffCircle;	// Don't forget to add an event for turning on/off player buffing

	protected:
		UINT			mID;
		UINT			mTeamID;
		XMFLOAT3		mPos;
		XMFLOAT3		mDir;
		float			mMaxShield;
		float			mCurrentShield;
		float			mMaxHP;
		float			mCurrentHP;

		bool			mWasUpdated;

	public:

	private:
		// Calculates the new level dependant on the change factor sent by the client
		void			ChangeTurretLevel( int change );
		void			ChangeShieldLevel( int change );
		void			ChangeBuffLevel( int change );

		void			CalcTurretLevel();
		void			CalcShieldLevel();
		void			CalcBuffMod();

		void			ClientUpdateShip( IEventPtr eventPtr );
		void			ClientChangeShipLevels( IEventPtr eventPtr );

	protected:
	public:
		float			PercentShield() const;
		float			PercentHP() const;

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