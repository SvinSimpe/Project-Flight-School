#ifndef STEERINGBEHAVIORMANAGER_H
#define STEERINGBEHAVIORMANAGER_H

#include "SteeringBehavior.h"

class SteeringBehaviorManager
{
	// Class members
	protected:
		std::vector<SteeringBehavior*>		mBehaviors;
		std::vector<SteeringBehavior*>		mActive;
		std::vector<float>					mActiveForce;
		int									mNumberOfBehaviors;
		Enemy*								mEnemy;
		XMFLOAT3							mTotalSteeringForce;
		float								mMaxSteeringForce;

	// Class functions
	public:
		virtual HRESULT		Update( float deltaTime );
		virtual void		AddBehavior( SteeringBehavior* behavior );
		virtual void		DisableBehavior( int index );
		virtual void		SetUpBehavior( int behaviorIndex, float weight, float probability, bool disable = false );
		virtual bool		CombinedForceWeighted( XMFLOAT3& steeringForce, float weight );
		virtual bool		CombineForcePrioritySum( XMFLOAT3& steeringForce );
		virtual XMFLOAT3	GetFinalSteeringForce() const;

		virtual HRESULT		Initialize( Enemy* enemy = nullptr );
		virtual void		Release();
		virtual void		Reset();
							SteeringBehaviorManager();
						   ~SteeringBehaviorManager();

};

#endif