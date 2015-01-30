#include "EnemySpawn.h"

HRESULT EnemySpawn::Update( float deltaTime )
{
	return S_OK;
}

HRESULT EnemySpawn::Render()
{
	RenderManager::GetInstance()->AddObject3dToList( mModel, mPosition );
	return S_OK;
}

unsigned int EnemySpawn::GetID() const
{
	return mID;
}

XMFLOAT3 EnemySpawn::GetPosition() const
{
	return mPosition;
}

void EnemySpawn::SetPosition( XMFLOAT3 posistion )
{
	mPosition = posistion;
	mSpawnPosition = mPosition;
	mSpawnPosition.x -= 1.0f;
	mSpawnPosition.z -= 1.0f;
}

XMFLOAT3 EnemySpawn::GetSpawnPosition() const
{
	return mSpawnPosition;
}

void EnemySpawn::SetSapwnPosition( XMFLOAT3 position )
{
	mSpawnPosition = position;
}

HRESULT EnemySpawn::Initilaize( unsigned int id )
{
	mID			= id;

	HRESULT hr	= Graphics::GetInstance()->LoadStatic3dAsset( "../Content/Assets/Nests/", "nest_2.pfs", mModel );
	
	mSpawnPosition = mPosition;
	mSpawnPosition.x -= 1.0f;
	mSpawnPosition.z -= 1.0f;

	return hr;
}

void EnemySpawn::Release()
{
}

EnemySpawn::EnemySpawn()
{
	mID				= 0;
	mModel			= 0;
	mPosition		= XMFLOAT3( 0.0f, 0.0f, 0.0f );
	mSpawnPosition	= XMFLOAT3( 0.0f, 0.0f, 0.0f );
}

EnemySpawn::~EnemySpawn()
{
}