#ifndef CLIENTTURRET_H
#define CLIENTTURRET_H

#include "ServerTurret.h"
#include "RenderManager.h"
#include <math.h>

class ClientTurret : public ServerTurret
{
	private:
	protected:
	public:

	private:
		void	ServerUpdateTurret( IEventPtr eventPtr );

	protected:
	public:
		void	Render( float deltaTime, DirectX::XMFLOAT4X4 parentWorld );
		void	Update( float dt );
		void	Reset( UINT id, UINT team, XMFLOAT3 pos, XMFLOAT4 rot, XMFLOAT3 scale );
		void	Initialize( UINT id, UINT team, XMFLOAT3 pos, XMFLOAT4 rot, XMFLOAT3 scale, AssetID assetID = CUBE_PLACEHOLDER );
		void	Release();
				ClientTurret();
		virtual	~ClientTurret();
};
#endif