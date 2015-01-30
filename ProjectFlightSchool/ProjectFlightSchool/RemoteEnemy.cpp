#include "RemoteEnemy.h"

HRESULT RemoteEnemy::Update( float deltaTime )
{
	RenderManager::GetInstance()->AnimationUpdate( mAnimationTrack, deltaTime );
	return S_OK;
}

HRESULT RemoteEnemy::Render()
{
	RenderManager::GetInstance()->AddAnim3dToList( mAnimationTrack, ANIMATION_PLAY_LOOPED, mPosition );
	return S_OK;
}

void RemoteEnemy::SetID( unsigned int id )
{
	mID = id;
}

void RemoteEnemy::SetEnemyType( EnemyType type )
{
	mEnemyType	= type;
}

void RemoteEnemy::SetModelID( AssetID model, AssetID defaultAnimation )
{
	RenderManager::GetInstance()->AnimationInitialize( mAnimationTrack, model, defaultAnimation );
}

void RemoteEnemy::SetAnimation( AssetID animation )
{
	if( mAnimationTrack.mNextAnimation != animation )
		RenderManager::GetInstance()->AnimationStartNew( mAnimationTrack, animation );
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

HRESULT RemoteEnemy::Initialize( int id, AssetID model, AssetID animation )
{
	mID				= id;
	mEnemyType		= Standard;		// Default enemy
	mPosition		= XMFLOAT3( 0.0f, 0.0f, 0.0f );
	mDirection		= XMFLOAT3( 0.0f, 0.0f, 0.0f );
	mIsSynced		= false;

	// Load animationTrack with model and standard animation
	RenderManager::GetInstance()->AnimationInitialize( mAnimationTrack, model, animation );

	return S_OK;
}

void RemoteEnemy::Release()
{
}

RemoteEnemy::RemoteEnemy()
{
	mID				= 0;
	mPosition		= XMFLOAT3( 0.0f, 0.0f, 0.0f );
	mDirection		= XMFLOAT3( 0.0f, 0.0f, 0.0f );
	mIsSynced		= false;
}

RemoteEnemy::~RemoteEnemy()
{
}