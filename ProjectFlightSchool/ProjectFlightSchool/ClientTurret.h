#ifndef CLIENTTURRET_H
#define CLIENTTURRET_H

#include "ServerTurret.h"
#include "RenderManager.h"
#include <math.h>

class ClientTurret : public ServerTurret
{
	private:
		struct TurretPart
		{
			AssetID model	= CUBE_PLACEHOLDER;
			XMFLOAT3 pos	= XMFLOAT3( 0.0f, 0.0f, 0.0f );
			XMFLOAT4 rot	= XMFLOAT4( 0.0f, 0.0f, 0.0f, 0.0f );
			XMFLOAT3 scale	= XMFLOAT3( 0.0f, 0.0f, 0.0f );
		};

		static const int 
			IDLE_MODE		= 0,
			ATTACKING_MODE	= 1,
			MODE_AMOUNT		= 2;

		class ITurretMode
		{
		public:
			virtual void Action( ClientTurret* t ) = 0;
					ITurretMode() {}
			virtual ~ITurretMode() {}
		};

		class IdleTurret : public ITurretMode
		{
		public:
			void Action( ClientTurret* t );
			IdleTurret() : ITurretMode() {}
			~IdleTurret() {}
		};

		class AttackingTurret : public ITurretMode
		{
		public:
			void Action( ClientTurret* t );
			AttackingTurret() : ITurretMode() {}
			~AttackingTurret() {}
		};

		int				mCurrentMode;
		ITurretMode*	mModes[MODE_AMOUNT];
		BoundingCircle*	mTargetCircle; // Defines the circle of the current target
		BoundingCircle* mScanCircle; // Defines the area where the turret will be looking for targets
		LoadOut*		mLoadOut;
		TurretPart*		mTurretHead;
		float			mDT;

	protected:
	public:

	private:
		void	SwitchMode( int mode );

	protected:
	public:
		void	FindTarget( BoundingCircle* targetCircle );
		void	Render( float deltaTime, DirectX::XMFLOAT4X4 parentWorld );
		void	Update( float deltaTime );
		void	Reset( UINT id, UINT team, XMFLOAT3 pos, XMFLOAT4 rot, XMFLOAT3 scale );
		void	Initialize( UINT id, UINT team, XMFLOAT3 pos, XMFLOAT4 rot, XMFLOAT3 scale );
		void	Release();
				ClientTurret();
		virtual	~ClientTurret();
};
#endif