#ifndef SERVERSHIP_H
#define SERVERSHIP_H

#include "BoundingGeometry.h"
#include "Events.h"
#include "EnergyCell.h"
#include "ServerTurret.h"
#include "GameObject.h"

class ServerShip : public GameObject
{
	private:
		friend class Server;
		const UINT MIN_LEVEL = 1;
		const UINT MAX_LEVEL = 3;
		float			mBuffMod;		// Also declared in Player.h... look there to see what this does

	protected:
		ServerTurret*	mServerTurret;
		BoundingCircle* mBuffCircle;
		BoundingCircle* mHitCircle;
		UINT			mID;
		UINT			mTeamID;
		XMFLOAT4X4		mWorld;
		float			mMaxShield;
		float			mCurrentShield;
		float			mMaxHP;
		float			mCurrentHP;
		UINT			mNrOfEnergyCells;
		UINT			mNrOfAvailableEnergyCells;

		UINT			mTurretLevel;
		UINT			mBuffLevel;
		UINT			mShieldLevel;
		UINT			mEngineLevel;

		bool			mWasUpdated;
		bool			mIsAlive;



	public:
		UINT			EnemiesTargetMe;
		UINT			TanksTargetMe;

	private:
		// Calculates the new level dependent on the change factor sent by the client
		void			ChangeTurretLevel( int change );
		void			ChangeShieldLevel( int change );
		void			ChangeBuffLevel( int change );
		void			ChangeEngineLevel( int change );

		void			CalcTurretLevel();
		void			CalcShieldLevel();
		void			CalcBuffMod();

		void			ClientUpdateShip( IEventPtr eventPtr );

	protected:
	public:
		void			AddEnergyCell( UINT energyCellOwnerID );
		float			PercentShield() const;
		float			PercentHP() const;
		
		void			ClientChangeShipLevels( int changeTurretLevel, int changeShieldLevel, int changeBuffLevel, int changeEngineLevel );

		UINT			GetID() const;
		BoundingCircle*	GetHitCircle() const;
		BoundingCircle*	GetBuffCircle() const;
		bool			IsAlive() const;
		UINT			GetTeamID() const;
		virtual bool	TakeDamage( float damage );
		virtual bool	Intersect( BoundingCircle* entity ); // Will check for intersects with buffable players
		virtual void	Update( float deltaTime );
		void			FindTurretTarget( std::vector<BoundingCircle*> enemies );
		virtual void	Reset( UINT id, UINT teamID, XMFLOAT3 pos, XMFLOAT4 rot, XMFLOAT3 scale, AssetID assetID = CUBE_PLACEHOLDER );
		virtual void	Initialize( UINT id, UINT team, XMFLOAT3 pos, XMFLOAT4 rot, XMFLOAT3 scale, AssetID assetID = CUBE_PLACEHOLDER );
		virtual void	Release();
						ServerShip();
		virtual			~ServerShip();
};
#endif