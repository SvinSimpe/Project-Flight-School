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

	protected:
	public:
		void	Reset( UINT id, UINT teamID, XMFLOAT3 pos, XMFLOAT3 dir );
		void	Update( float deltaTime );
		void	Render();
		void	Initialize( UINT id, UINT teamID, XMFLOAT3 pos, XMFLOAT3 dir );
		void	Release();
				ClientShip();
		virtual	~ClientShip();
};
#endif