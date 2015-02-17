#ifndef SERVERTURRET_H
#define SERVERTURRET_H

#include "Events.h"
#include "WeaponInfo.h"
#include "GameObject.h"
#include <vector>

class ServerTurret : public GameObject
{
	private:
		friend class Server;
	protected:
		UINT			mID;
		UINT			mTeamID;

	public:

	private:
	protected:
	public:
		virtual void	Reset( UINT id, UINT team, XMFLOAT3 pos, XMFLOAT4 rot, XMFLOAT3 scale );
		virtual void	Initialize( UINT id, UINT team, XMFLOAT3 pos, XMFLOAT4 rot, XMFLOAT3 scale, AssetID assetID = CUBE_PLACEHOLDER );
		virtual void	Initialize( UINT id, UINT team, GameObjectInfo gameObjectInfo, AssetID assetID = CUBE_PLACEHOLDER );
		virtual void	Release();
						ServerTurret();
		virtual			~ServerTurret();
};
#endif