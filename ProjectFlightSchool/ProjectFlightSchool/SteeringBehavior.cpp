#include "SteeringBehavior.h"

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

HRESULT SteeringBehavior::Initialize( Enemy* enemy )
{
	mEnemy			= enemy;
	mDisable		= false;
	mWeight			= 0.0f;
	mProbability	= 0.0f;

	return S_OK;
}

void SteeringBehavior::Reset()
{
}

void SteeringBehavior::Release()
{
}

SteeringBehavior::SteeringBehavior()
{
	mEnemy			= nullptr;
	mDisable		= false;
	mWeight			= 0.0f;
	mProbability	= 0.0f;
}

SteeringBehavior::~SteeringBehavior()
{

}