#include "RemoteEnemy.h"

HRESULT RemoteEnemy::Update( float deltaTime )
{
	mAnimationTime += deltaTime;
	return S_OK;
}

HRESULT RemoteEnemy::Render()
{
	RenderManager::GetInstance()->AddAnim3dToList( mModel, mAnimations[ENEMY_ANIMATION_IDLE], &mAnimationTime, ANIMATION_PLAY_LOOPED, mPosition );
	return S_OK;
}

void RemoteEnemy::SetID( unsigned int id )
{
	mID = id;
}

void RemoteEnemy::SetModelID( AssetID model )
{
	mModel	= model;
}

void RemoteEnemy::SetAnimation( AssetID animation )
{
	mAnimations[ENEMY_ANIMATION_IDLE] = animation;
}

void RemoteEnemy::SetPosition( XMFLOAT3 position )
{
	mPosition	= position;
}

void RemoteEnemy::SetDirection( XMFLOAT3 direction )
{
	mDirection = direction;
}

HRESULT RemoteEnemy::Initialize( int id )
{
	mID				= id;
	mModel			= 0;
	mPosition		= XMFLOAT3( 0.0f, 0.0f, 0.0f );
	mDirection		= XMFLOAT3( 0.0f, 0.0f, 0.0f );
	mAnimationTime	= 1.0f;

	return S_OK;
}

void RemoteEnemy::Release()
{
}

RemoteEnemy::RemoteEnemy()
{
	mID				= 0;
	mModel			= 0;
	mPosition		= XMFLOAT3( 0.0f, 0.0f, 0.0f );
	mDirection		= XMFLOAT3( 0.0f, 0.0f, 0.0f );
	mAnimationTime	= 0.0f;
}

RemoteEnemy::~RemoteEnemy()
{
}