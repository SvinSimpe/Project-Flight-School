#ifndef STEERINGBEHAVIOR_H
#define STEERINGBEHAVIOR_H

#include "Enemy.h"

class SteeringBehavior
{
	// Class members
	private:
	public:
		Enemy*		mEnemy;
		float		mWeight;
		float		mProbability;
		bool		mDisable;

	// Class functions
	public:
		virtual bool	Update( float deltaTime, XMFLOAT3& totalForce );
		virtual void	SteerTowards( XMFLOAT3& target, XMFLOAT3& result );
		virtual void	SteerAway( XMFLOAT3& target, XMFLOAT3& result );
		virtual HRESULT Initialize( Enemy* enemy );
		virtual void	Reset();
		virtual void	Release();
						SteeringBehavior();
					   ~SteeringBehavior();

};

#endif