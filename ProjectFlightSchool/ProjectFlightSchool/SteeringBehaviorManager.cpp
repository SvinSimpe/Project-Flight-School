#include "Enemy.h"

HRESULT SteeringBehaviorManager::Update( float deltaTime )
{
	// Don't do anything if you have no states
	if( mBehaviors.size() == 0 )
		return S_OK;

	// Clear out debug logs
	mActive.clear();
	mActiveForce.clear();

	// Reset steering vector
	mTotalSteeringForce = XMFLOAT3( 0.0f, 0.0f, 0.0f );

	// Update all the behaviors
	bool needToClamp = false;
	for ( size_t i = 0; i < mBehaviors.size(); i++ )
	{
		XMFLOAT3 steeringForce = XMFLOAT3( 0.0f, 0.0f, 0.0f );
		
		bool didSomething	= mBehaviors[i]->Update( deltaTime, steeringForce );
		if( didSomething )
		{
			// Keep track of the behaviors that updated this frame
			mActive.push_back( mBehaviors[i] );
			mActiveForce.push_back( XMVectorGetX( XMVector3Length( XMLoadFloat3( &steeringForce ) ) ) );

			// Combine the behaviors into the total steering force
			bool keepGoing	= false;

			// ONLY USE ONE Combine Force function

			// Simple weighted combination function
			//keepGoing		= CombinedForceWeighted( steeringForce, mBehaviors[i]->mWeight );
			// Normalize the result
			//needToClamp		= true;

			// Prioritized Sum funtion
			keepGoing		= CombineForcePrioritySum( steeringForce, mBehaviors[i]->mWeight );

			if( !keepGoing )
				break;
		}

		if( needToClamp )
		{
			// if mTotalSteeringForce > mMaxSteeringForce
			//			mTotalSteeringForce.normalize()
			//			mTotalSteeringForce *= mMaxSteeringForce
			//	else if mTotalSteeringForce < 0.0f 
			//			mTotalSteeringForce = 0.0f
			//	else
			//			mTotalSteeringForce = mTotalSteeringForce
		}
	}

	return S_OK;
}

void SteeringBehaviorManager::AddBehavior( SteeringBehavior* behavior )
{
	mBehaviors.push_back( behavior );
}

void SteeringBehaviorManager::DisableBehavior( int index )
{
	mBehaviors[index]->mDisable	= true;
}

void SteeringBehaviorManager::SetUpBehavior( int behaviorIndex, float weight, float probability, bool disable )
{
	mBehaviors[behaviorIndex]->mWeight		= weight;
	mBehaviors[behaviorIndex]->mProbability	= probability;
	mBehaviors[behaviorIndex]->mDisable		= disable;
}

bool SteeringBehaviorManager::CombinedForceWeighted( XMFLOAT3& steeringForce, float weight )
{
	mTotalSteeringForce.x	+= steeringForce.x * weight;
	mTotalSteeringForce.z	+= steeringForce.z * weight;
	mTotalSteeringForce.y	+= steeringForce.y * weight;
	return true;
}

bool SteeringBehaviorManager::CombineForcePrioritySum( XMFLOAT3& steeringForce, float weight )
{
	bool retVal			= false;

	// totalForce = mTotalSteeringForce.Lenght()
	float totalForce	= XMVectorGetX( XMVector3Length( XMLoadFloat3( &mTotalSteeringForce ) ) );
	
	float forceLeft		= mMaxSteeringForce - totalForce;

	if( forceLeft > 0.0f )
	{
		XMStoreFloat3( &steeringForce, XMLoadFloat3( &steeringForce ) * weight );
		// newForce = steeringForce.Lenght()
		float newForce = XMVectorGetX( XMVector3Length( XMLoadFloat3( &steeringForce ) ) );

		if( newForce < forceLeft )
		{
			// mTotalSteeringForce += steeringForce
			mTotalSteeringForce.x	+= steeringForce.x;
			mTotalSteeringForce.z	+= steeringForce.z;
			mTotalSteeringForce.y	+= steeringForce.y;
		}
		else
		{
			// mTotalSteeringForce += steeringForce.normalize() * forceleft
			XMFLOAT3 temp = mTotalSteeringForce;
			XMStoreFloat3( &temp, XMVector3Normalize( XMLoadFloat3( &mTotalSteeringForce ) ) * forceLeft );
			mTotalSteeringForce.x	+= temp.x;
			mTotalSteeringForce.z	+= temp.z;
			mTotalSteeringForce.y	+= temp.y;
		}

		if( (forceLeft - newForce) > 0 )
			retVal	= true;
	}

	return retVal;
}

XMFLOAT3 SteeringBehaviorManager::GetFinalSteeringForce() const
{
	return mTotalSteeringForce;
}

HRESULT SteeringBehaviorManager::Initialize( Enemy* enemy )
{
	mEnemy				= enemy;
	mMaxSteeringForce	= 400.0f;
	return S_OK;
}

void SteeringBehaviorManager::Release()
{
	for( unsigned int i = 0; i < mBehaviors.size(); i++ )
		SAFE_RELEASE_DELETE( mBehaviors[i] );
	mBehaviors.clear();
}

void SteeringBehaviorManager::Reset()
{
}

SteeringBehaviorManager::SteeringBehaviorManager()
{
	mTotalSteeringForce = XMFLOAT3( 0.0f, 0.0f, 0.0f );
}

SteeringBehaviorManager::~SteeringBehaviorManager()
{
}