#ifndef CLIENTSHIP_H
#define CLIENTSHIP_H

#include "RenderManager.h"
#include "ServerShip.h"

class ClientShip : public ServerShip
{
	private:
		// Turret? Buffcircle?
		AssetID mAssetID;
		BoundingCircle* mHitCircle;


	protected:
	public:

	private:
		void RemoteTurretLevel( IEventPtr eventPtr );
		void RemoteHullLevel( IEventPtr eventPtr );
		void RemoteBuffLevel( IEventPtr eventPtr );
		void RemoteDamageShip( IEventPtr eventPtr );
		void ServerResetShip( IEventPtr eventPtr ); // An event will be sent from the server when a reset is required
		void Reset( UINT id, UINT teamID, XMFLOAT3 pos, XMFLOAT3 dir );

	protected:
	public:
		void	AddEnergyCell( UINT energyCellID );
		bool	Intersect( BoundingCircle* entity ); // Will check for intersects with damaging stuff
		void	Update( float deltaTime );
		void	Render();
		void	Initialize( UINT id, UINT teamID, XMFLOAT3 pos, XMFLOAT3 dir );
		void	Release();
				ClientShip();
		virtual	~ClientShip();
};
#endif