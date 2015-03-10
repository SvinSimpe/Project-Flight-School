#include "RemoteEnemy.h"

XMFLOAT3 RemoteEnemy::GetPosition() const
{
	return mPosition;
}

HRESULT RemoteEnemy::Update( float deltaTime )
{
	mBoundingCircle->center = mPosition;
	RenderManager::GetInstance()->AnimationUpdate( mAnimationTrack, deltaTime );
	return S_OK;
}

HRESULT RemoteEnemy::Render()
{
	float radians = atan2f( mDirection.z, mDirection.x );
	//RenderManager::GetInstance()->AddCircleToList( mBoundingCircle->center, XMFLOAT3( 1.0f, 0.0f, 0.0f ), mBoundingCircle->radius );
	if( mLoopAnimation)
		RenderManager::GetInstance()->AddAnim3dToList( mAnimationTrack, ANIMATION_PLAY_LOOPED, mPosition, XMFLOAT3( 0.0f, -radians, 0.0f ) );
	else
		RenderManager::GetInstance()->AddAnim3dToList( mAnimationTrack, ANIMATION_PLAY_ONCE, mPosition, XMFLOAT3( 0.0f, -radians, 0.0f ) );

	return S_OK;
}

void RemoteEnemy::SetID( unsigned int id )
{
	mID = id;
}

unsigned int RemoteEnemy::GetID() const
{
	return mID;
}

void RemoteEnemy::SetHP( float hp )
{
	mCurrentHP	= hp;
}

float RemoteEnemy::GetHP() const
{
	return mCurrentHP;
}

void RemoteEnemy::SetIsAlive( bool isAlive )
{
	mIsAlive	= isAlive;
}

bool RemoteEnemy::IsAlive() const
{
	return mIsAlive;
}

void RemoteEnemy::SetLoopAnimation( bool loop )
{
	mLoopAnimation	= loop;
}

void RemoteEnemy::SetEnemyType( EnemyType type )
{
	mEnemyType	= type;
}

void RemoteEnemy::SetModelID( AssetID model, AssetID defaultAnimation )
{
	RenderManager::GetInstance()->AnimationInitialize( mAnimationTrack, model, defaultAnimation );
}

void RemoteEnemy::SetAnimation( AssetID animation, bool blendWithCurrent )
{
	if( mAnimationTrack.mNextAnimation != animation )
		RenderManager::GetInstance()->AnimationStartNew( mAnimationTrack, animation, blendWithCurrent );
}

void RemoteEnemy::SetPosition( XMFLOAT3 position )
{
	mPosition	= position;
}

void RemoteEnemy::SetDirection( XMFLOAT3 direction )
{
	mDirection = direction;
}

void RemoteEnemy::SetSynced( bool isSynced )
{
	mIsSynced	= isSynced;
}

bool RemoteEnemy::IsSynced() const
{
	return mIsSynced;
}

EnemyType RemoteEnemy::GetEnemyType() const
{
	return mEnemyType;
}

BoundingCircle* RemoteEnemy::GetBoundingCircle() const
{
	return mBoundingCircle;
}

HRESULT RemoteEnemy::Initialize( int id, AssetID model, AssetID animation )
{
	mID				= id;
	mEnemyType		= Standard;		// Default enemy
	mPosition		= XMFLOAT3( 0.0f, 0.0f, 0.0f );
	mDirection		= XMFLOAT3( 0.0f, 0.0f, 0.0f );
	mIsSynced		= false;
	mLoopAnimation	= true;

	// Load animationTrack with model and standard animation
	//RenderManager::GetInstance()->AnimationInitialize( mAnimationTrack, model, animation );

	mBoundingCircle		= new BoundingCircle( 0.5f );

	return S_OK;
}

void RemoteEnemy::Release()
{
	SAFE_DELETE( mBoundingCircle );
}

RemoteEnemy::RemoteEnemy()
{
	mID				= 0;
	mPosition		= XMFLOAT3( 0.0f, 0.0f, 0.0f );
	mDirection		= XMFLOAT3( 0.0f, 0.0f, 0.0f );
	mIsSynced		= false;
	mIsAlive		= false;
	mLoopAnimation	= false;
	mBoundingCircle	= nullptr;
}

RemoteEnemy::~RemoteEnemy()
{
}