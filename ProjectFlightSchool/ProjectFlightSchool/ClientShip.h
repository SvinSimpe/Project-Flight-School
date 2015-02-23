#ifndef CLIENTSHIP_H
#define CLIENTSHIP_H

#include "RenderManager.h"
#include "ServerShip.h"
#include "ClientTurret.h"

class ClientShip : public ServerShip
{
	private:
		BoundingCircle* mHitCircle;
		ClientTurret* mClientTurret;


	protected:
	public:

	private:
		void	RemoteUpdateShip( IEventPtr eventPtr );
		void	RemoteChangeShipLevels( IEventPtr eventPtr );


	protected:
	public:
		UINT	GetID() const;
		UINT	GetTeamID() const;
		UINT	GetNrOfEnergyCells() const;

		void	CalculatePlayerRespawnPosition( IEventPtr eventPtr );
		void	Reset( UINT id, UINT teamID, XMFLOAT3 pos, XMFLOAT4 rot, XMFLOAT3 scale );
		bool	Intersect( BoundingCircle* entity ); // Will check for intersects with damaging stuff
		void	Update( float deltaTime );
		void	Render( float deltaTime, DirectX::XMFLOAT4X4 parentWorld );
		void	Initialize( UINT id, UINT teamID, XMFLOAT3 pos, XMFLOAT4 rot, XMFLOAT3 scale );
		void	Release();
				ClientShip();
		virtual	~ClientShip();
};
#endif