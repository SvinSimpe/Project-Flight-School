#ifndef SERVERSHIP_H
#define SERVERSHIP_H

#include "BoundingGeometry.h"
#include "Events.h"
#include "EnergyCell.h"

class ServerShip
{
	private:
		friend class Server;
		BoundingCircle* mBuffCircle;	// Don't forget to add an event for turning on/off player buffing
		float			mBuffMod;		// Also declared in Player.h... look there to see what this does

	protected:
		UINT			mID;
		UINT			mTeamID;
		XMFLOAT3		mPos;
		XMFLOAT3		mDir;
		float			mMaxHP;
		float			mCurrentHP;
		UINT			mNrOfEnergyCells;
		UINT			mEnergyCells[MAX_ENERGY_CELLS];

	public:

	private:
		void ClientTurretLevel( IEventPtr eventPtr );
		void ClientHullLevel( IEventPtr eventPtr );
		void ClientBuffLevel( IEventPtr eventPtr );
		void ClientDamageShip( IEventPtr eventPtr );

	protected:
	public:
		void			AddEnergyCell( UINT energyCellID );

		UINT			GetID() const;
		UINT			GetTeamID() const;

		bool			TakeDamage( float damage );
		virtual bool	Intersect( BoundingCircle* entity ); // Will check for intersects with buffable players
		virtual void	Reset( UINT id, UINT teamID, XMFLOAT3 pos, XMFLOAT3 dir );
		virtual void	Update( float deltaTime );
		virtual void	Initialize( UINT id, UINT teamID, XMFLOAT3 pos, XMFLOAT3 dir );
		virtual void	Release();
						ServerShip();
		virtual			~ServerShip();
};
#endif