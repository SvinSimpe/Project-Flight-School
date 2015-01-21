#ifndef SHIP_H
#define SHIP_H

#include "Turret.h"

class Ship
{
private:
	BodyPart*	mModel;
	int			mTeamID;
	Turret*		mTurret;
protected:
public:

private:
protected:
public:
	void	PickTurretTarget( std::vector<RemotePlayer*> targets );
	void	Update( float deltaTime );
	void	Render();
	int		GetTeamID() const;
	void	Initialize( int team, XMFLOAT3 pos, XMFLOAT3 dir );
	void	Release();
			Ship();
	virtual	~Ship();
};
#endif