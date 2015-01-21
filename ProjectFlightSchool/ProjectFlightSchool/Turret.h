#ifndef TURRET_H
#define TURRET_H

#include "Player.h"
#include <math.h>

#define SHOOTCOOLDOWN 0.1f
#define PI 3.14159265f

struct BodyPart
{
	AssetID		model	= CUBE_PLACEHOLDER;
	XMFLOAT3	pos		= XMFLOAT3( 0.0f, 0.0f, 0.0f );
	XMFLOAT3	dir		= XMFLOAT3( 0.0f, 0.0f, 0.0f );
};

class Turret
{
	// classes
	private:
		class ITurretMode
		{
		public:
			virtual void Action( Turret* t ) = 0;
			ITurretMode() {}
			virtual ~ITurretMode() {}
		};

		class IdleTurret : public ITurretMode
		{
		public:
			void Action( Turret* t );
			IdleTurret() : ITurretMode() {}
			~IdleTurret() {}
		};

		class AttackingTurret : public ITurretMode
		{
		public:
			void Action( Turret* t);
			AttackingTurret() : ITurretMode() {}
			~AttackingTurret() {}
		};
	protected:
	public:

	// Variables
	private:
		int					mTeamID;
		BodyPart*			mUpperBody;
		BodyPart*			mMiddleBody;
		BodyPart*			mLowerBody;
		float				mRange;
		LoadOut*			mLoadOut;
		float				mShootTimer;
		BoundingCircle*		mBoundingCircle;
		RemotePlayer*		mTarget;
		ITurretMode*		mCurrentMode;
		float				mDT;
		IdleTurret*			mIdle;
		AttackingTurret*	mAttacking;

	protected:
	public:

	// Functions
	private:
		void				Fire();
		void				SwitchMode( ITurretMode* mode );
		void				CheckTarget( std::vector<RemotePlayer*> targets );
	protected:
	public:
		void				PickTarget( std::vector<RemotePlayer*> targets );
		HRESULT				Update( float deltaTime );
		void				Render();
		void				Initialize( int team );
		void				Release();
							Turret();
		virtual				~Turret();
};
#endif