#ifndef SHIP_H
#define SHIP_H

#include "Turret.h"

class Ship
{
	private:
		BodyPart*		mModel;
		BoundingCircle* mHitBox;
		BoundingCircle* mBuff;
		int				mTeamID;
		Turret*			mTurret;
		float			mMaxHull;
		float			mCurrentHull;

	protected:
	public:

	private:
	protected:
	public:
		void			BuffPlayer( Player* player );
		XMFLOAT3		GetPosition() const;
		BoundingCircle* GetHitBox() const;
		void			TakeDamage( float damage );
		void			PickTurretTarget( std::vector<RemotePlayer*> targets );
		void			Update( float deltaTime );
		void			Render();
		int				GetTeamID() const;
		void			Initialize( int team, XMFLOAT3 pos, XMFLOAT3 dir );
		void			Release();
						Ship();
		virtual			~Ship();
};
#endif