#include "Enemy.h"

////////////////////////////////////////////////////////////////////
//					Steering Behavior Base
////////////////////////////////////////////////////////////////////

bool SteeringBehavior::Update(float deltaTime, XMFLOAT3& totalForce )
{
	return false;
}

void SteeringBehavior::SteerTowards( XMFLOAT3& target, XMFLOAT3& result )
{
	XMFLOAT3 desired;
	desired.x = target.x - mEnemy->GetPosition().x;
	desired.z = target.z - mEnemy->GetPosition().z;
	desired.y = 0.0f;

	// Check if this get the right values
	// targetDistance = desired.length()
	float targetDistance = XMVectorGetX( XMVector3Length( XMLoadFloat3( &desired ) ) ); 

	if( targetDistance > 0.0f )
	{
		// desired = desired.normalize * enemy->speed
		XMStoreFloat3( &desired, XMVector3Normalize( XMLoadFloat3( &desired ) ) * mEnemy->GetSpeed() );
		// result = desired - enemy->velocity
		result.x = desired.x - mEnemy->GetVelocity().x;
		result.z = desired.z - mEnemy->GetVelocity().z;
		result.y = 0.0f;
	}
	else
		XMStoreFloat3( &result, XMVectorZero() );
}

void SteeringBehavior::SteerAway( XMFLOAT3& target, XMFLOAT3& result )
{
	XMFLOAT3 desired;
	desired.x =  mEnemy->GetPosition().x - target.x;
	desired.z =  mEnemy->GetPosition().z - target.z;
	desired.y = 0.0f;

	// Check if this get the right values
	// targetDistance = desired.length()
	float targetDistance = XMVectorGetX( XMVector3Length( XMLoadFloat3( &desired ) ) ); 

	if( targetDistance > 0.0f )
	{
		// desired = desired.normalize * enemy->speed
		XMStoreFloat3( &desired, XMVector3Normalize( XMLoadFloat3( &desired ) ) * mEnemy->GetSpeed() );
		// result = desired - enemy->velocity
		result.x = desired.x - mEnemy->GetVelocity().x;
		result.z = desired.z - mEnemy->GetVelocity().z;
		result.y = 0.0f;
	}
	else
		XMStoreFloat3( &result, XMVectorZero() );
}

void SteeringBehavior::Reset()
{
}

void SteeringBehavior::Release()
{
}

SteeringBehavior::SteeringBehavior( Enemy* enemy )
{
	mEnemy			= enemy;
	mDisable		= false;
	mWeight			= 0.0f;
	mProbability	= 0.0f;
}

SteeringBehavior::~SteeringBehavior()
{

}

////////////////////////////////////////////////////////////////////
//						Steer Approach
////////////////////////////////////////////////////////////////////

bool SteerApproach::Update( float deltaTime, XMFLOAT3& totalForce )
{
	XMFLOAT3 steeringForce = XMFLOAT3( 0.0f, 0.0f, 0.0f );
	SteerTowards( mEnemy->mPlayers[mEnemy->mTargetIndex]->Pos, steeringForce );
	totalForce.x	+= steeringForce.x;
	totalForce.z	+= steeringForce.z;
	totalForce.y	 = 0.0f;

	return true;
}

SteerApproach::SteerApproach( Enemy* enemy ) : SteeringBehavior( enemy ) { }

SteerApproach::~SteerApproach() { }

////////////////////////////////////////////////////////////////////
//						Steer Pursuit
////////////////////////////////////////////////////////////////////

bool SteerPursuit::Update( float deltatime, XMFLOAT3& totalForce )
{
	bool retVal		= false;
	bool found		= FindTarget();

	if( found )
	{
		XMFLOAT3 steeringForce = XMFLOAT3( 0.0f, 0.0f, 0.0f );
		SteerTowards( mEnemy->mPlayers[mEnemy->mTargetID]->Pos, steeringForce );
		totalForce.x	+= steeringForce.x;
		totalForce.z	+= steeringForce.z;
		totalForce.y	 = 0.0f;

		retVal = true;
	}

	return retVal;
}

bool SteerPursuit::FindTarget()
{
	bool retVal		= false;

	// If the player is in the enemies path, just approach, 
	// otherwise try to predict player movement and head him off
	return retVal;
}

////////////////////////////////////////////////////////////////////
//						Steer Evade
////////////////////////////////////////////////////////////////////

bool SteerEvade::Update( float deltaTime, XMFLOAT3& totalForce )
{
	bool adjustment		= false;

	// Move away from the nearest object ( for now just other enemies )
	for ( size_t i = 0; i < MAX_NR_OF_ENEMIES; i++ )
	{
		if( mEnemy->GetAttackCircle()->Intersect( mEnemy->mOtherEnemies[i]->GetAttackCircle() ) )
		{
			XMFLOAT3 steeringForce = XMFLOAT3( 0.0f, 0.0f, 0.0f );
			SteerAway( mEnemy->mPlayers[mEnemy->mTargetIndex]->Pos, steeringForce );
			totalForce.x	+= steeringForce.x;
			totalForce.z	+= steeringForce.z;
			totalForce.y	 = 0.0f;
		}
	}

	return adjustment;
}

SteerEvade::SteerEvade( Enemy* enemy ) : SteeringBehavior( enemy ) {}

SteerEvade::~SteerEvade() { }