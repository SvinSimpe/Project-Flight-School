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

		static const UINT 
			IDLE_MODE		= 0,
			ATTACK_MODE		= 1,
			MODE_AMOUNT		= 2;

		class ITurretMode
		{
		public:
			virtual void Action( ServerTurret* t, float dt ) = 0;
					ITurretMode() {}
			virtual ~ITurretMode() {}
		};

		class IdleTurret : public ITurretMode
		{
		public:
			void Action( ServerTurret* t, float dt );
			IdleTurret() : ITurretMode() {}
			~IdleTurret() {}
		};

		class AttackingTurret : public ITurretMode
		{
		public:
			void Action( ServerTurret* t, float dt );
			AttackingTurret() : ITurretMode() {}
			~AttackingTurret() {}
		};

		float			mWeaponCooldown;
		UINT			mCurrentMode;
		ITurretMode*	mModes[MODE_AMOUNT];
		BoundingCircle*	mTarget;				// Defines the circle of the current target, is never initialized on its own
		BoundingCircle* mScanCircle;			// Defines the area where the turret will be looking for targets

	protected:
		struct TurretHead
		{
			AssetID model	= CUBE_PLACEHOLDER;
			XMFLOAT3 pos	= XMFLOAT3( 0.0f, 0.0f, 0.0f );
			XMFLOAT4 rot	= XMFLOAT4( 0.0f, 0.0f, 0.0f, 0.0f );
			XMFLOAT3 scale	= XMFLOAT3( 0.0f, 0.0f, 0.0f );
		};

		UINT			mID;
		UINT			mTeamID;
		TurretHead*		mTurretHead;
		LoadOut*		mLoadOut;
		float			mTurretTurnSpeed;

	public:

	private:
		void			SwitchMode( UINT mode );
		void			Fire();

	protected:
	public:
		void			ChangeLevel( UINT level );
		void			ClearTarget();
		UINT			CheckMode() const;
		void			FindTarget( BoundingCircle* enemy );
		virtual void	Reset( UINT id, UINT team, XMFLOAT3 pos, XMFLOAT4 rot, XMFLOAT3 scale );
		void			Update( float deltaTime );
		virtual void	Initialize( UINT id, UINT team, XMFLOAT3 pos, XMFLOAT4 rot, XMFLOAT3 scale, AssetID assetID = CUBE_PLACEHOLDER );
		virtual void	Release();
						ServerTurret();
		virtual			~ServerTurret();
};
#endif